#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#else
#include <unistd.h>
  #include <sys/mman.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <sys/wait.h>
  #include <errno.h>
  #include <pthread.h>
  #include <mach-o/dyld.h> // для macOS
#endif


// Конфигурация имен для shared memory

#ifdef _WIN32
static const char* SHM_NAME = "MySharedMemory_HW3";
#else
static const char* SHM_NAME = "/my_shm_hw3";        // shm_open
#endif


// Структура для разделяемых данных

struct SharedData {
    // Общий счётчик
    int counter;

    // Флаги занятости копий по одному на копию
    bool copy1Active;
    bool copy2Active;
};

// Глобальный указатель на разделяемую структуру и флаг

static SharedData* g_shared = nullptr;
static bool g_isMaster = false;

// Получаем PID

int getPID() {
#ifdef _WIN32
    return (int)GetCurrentProcessId();
#else
    return (int)getpid();
#endif
}


// Получаем текущее время в формате "YYYY-MM-DD HH:MM:SS.mmm"

std::string currentTimeString() {
    char buf[128];
#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                  (int)st.wYear, (int)st.wMonth, (int)st.wDay,
                  (int)st.wHour, (int)st.wMinute, (int)st.wSecond,
                  (int)st.wMilliseconds);
#else
    // POSIX
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);  // поддерживается на Linux/macOS
    struct tm tmNow;
    localtime_r(&ts.tv_sec, &tmNow);
    int ms = (int)(ts.tv_nsec / 1000000);
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                  tmNow.tm_year + 1900, tmNow.tm_mon+1, tmNow.tm_mday,
                  tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec, ms);
#endif
    return buf;
}


// Файл для лога (все процессы пишут в один "program.log")

static const char* LOG_FILENAME = "program.log";


// Файл, который используем для межпроцессной блокировки
// (чтобы синхронно менять счётчик и писать в лог)

static const char* LOCK_FILENAME = "global_lock.tmp";


// Кроссплатформенная функция lockGlobal() / unlockGlobal()
//  - На Windows используем LockFileEx / UnlockFileEx
//  - На POSIX используем fcntl(F_SETLKW) / fcntl(F_SETLK)

#ifdef _WIN32

static HANDLE g_lockFileHandle = INVALID_HANDLE_VALUE;

bool lockGlobal() {
    if (g_lockFileHandle == INVALID_HANDLE_VALUE) {
        // открываем/создаём файл-блокировки
        HANDLE h = CreateFileA(
                LOCK_FILENAME,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем другим тоже открывать
                NULL,
                OPEN_ALWAYS, // если нет - создать
                FILE_ATTRIBUTE_NORMAL,
                NULL
        );
        if (h == INVALID_HANDLE_VALUE) {
            std::cerr << "lockGlobal: CreateFile failed, err=" << GetLastError() << std::endl;
            return false;
        }
        g_lockFileHandle = h;
    }
    // Теперь LockFileEx (эксклюзивная блокировка на весь файл)
    OVERLAPPED ov = {0};
    if (!LockFileEx(g_lockFileHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, 0xFFFFFFFF, 0xFFFFFFFF, &ov)) {
        std::cerr << "lockGlobal: LockFileEx failed, err=" << GetLastError() << std::endl;
        return false;
    }
    return true;
}

void unlockGlobal() {
    if (g_lockFileHandle == INVALID_HANDLE_VALUE) return;
    OVERLAPPED ov = {0};
    if (!UnlockFileEx(g_lockFileHandle, 0, 0xFFFFFFFF, 0xFFFFFFFF, &ov)) {
        std::cerr << "unlockGlobal: UnlockFileEx failed, err=" << GetLastError() << std::endl;
    }
}

#else // POSIX

static int g_lockFileFd = -1;

bool lockGlobal() {
    if (g_lockFileFd < 0) {
        // открываем/создаём файл
        int fd = ::open(LOCK_FILENAME, O_RDWR | O_CREAT, 0666);
        if (fd < 0) {
            perror("lockGlobal: open");
            return false;
        }
        g_lockFileFd = fd;
    }
    // ставим «блокировку на запись» на весь файл
    struct flock fl;
    fl.l_type   = F_WRLCK;  // эксклюзивная блокировка
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;        // блокируем весь файл
    if (fcntl(g_lockFileFd, F_SETLKW, &fl) == -1) {
        perror("lockGlobal: fcntl(F_SETLKW)");
        return false;
    }
    return true;
}

void unlockGlobal() {
    if (g_lockFileFd < 0) return;
    struct flock fl;
    fl.l_type   = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;
    if (fcntl(g_lockFileFd, F_SETLK, &fl) == -1) {
        perror("unlockGlobal: fcntl(F_SETLK)");
    }
}

#endif


// Функция записи строки в общий лог (под глобальной блокировкой)

void writeToLog(const std::string& message) {
    if (!lockGlobal()) {
        std::cerr << "writeToLog: cannot lock!\n";
        return;
    }
    {
        // запись в файл
        std::ofstream ofs(LOG_FILENAME, std::ios::app);
        if (ofs.is_open()) {
            ofs << message << std::endl;
            ofs.close();
        } else {
            std::cerr << "writeToLog: cannot open " << LOG_FILENAME << " for writing!\n";
        }
    }
    unlockGlobal();
}


// Инициализация / подключение к общему сегменту памяти

bool initSharedMemory() {
#ifdef _WIN32
    // CreateFileMapping
    HANDLE hMap = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL,
                                     PAGE_READWRITE,
                                     0,
                                     sizeof(SharedData),
                                     SHM_NAME);
    if (!hMap) {
        std::cerr << "initSharedMemory: CreateFileMapping fail, err=" << GetLastError() << std::endl;
        return false;
    }
    // Если уже существовал
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        g_isMaster = false;
    } else {
        g_isMaster = true;
    }

    // map
    void* view = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
    if (!view) {
        std::cerr << "initSharedMemory: MapViewOfFile fail, err=" << GetLastError() << std::endl;
        CloseHandle(hMap);
        return false;
    }
    g_shared = reinterpret_cast<SharedData*>(view);

    // Если мастер, инициализация
    if (g_isMaster) {
        g_shared->counter = 0;
        g_shared->copy1Active = false;
        g_shared->copy2Active = false;
    }

    // закрытие hMap
    CloseHandle(hMap);
    return true;

#else
    // POSIX: shm_open
    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd >= 0) {
        // Успешно создали => мастер
        g_isMaster = true;
        if (ftruncate(fd, sizeof(SharedData)) != 0) {
            perror("initSharedMemory: ftruncate");
            close(fd);
            return false;
        }
    } else {
        if (errno == EEXIST) {
            // Уже существует => slave
            g_isMaster = false;
            fd = shm_open(SHM_NAME, O_RDWR, 0666);
            if (fd < 0) {
                perror("initSharedMemory: shm_open");
                return false;
            }
        } else {
            perror("initSharedMemory: shm_open");
            return false;
        }
    }
    void* addr = mmap(nullptr, sizeof(SharedData), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("initSharedMemory: mmap");
        close(fd);
        return false;
    }
    close(fd);
    g_shared = reinterpret_cast<SharedData*>(addr);

    if (g_isMaster) {
        // Инициализация
        g_shared->counter = 0;
        g_shared->copy1Active = false;
        g_shared->copy2Active = false;
    }
    return true;
#endif
}


// Завершение / освобождение

void cleanupSharedMemory() {
#ifdef _WIN32
    if (g_shared) {
        UnmapViewOfFile(g_shared);
    }
#else
    if (g_shared) {
        munmap(g_shared, sizeof(SharedData));
    }
    if (g_isMaster) {
        // Мастер может удалить shm
        shm_unlink(SHM_NAME);
    }
#endif
    // Закрытие файла блокировки
#ifdef _WIN32
    if (g_lockFileHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(g_lockFileHandle);
        g_lockFileHandle = INVALID_HANDLE_VALUE;
    }
#else
    if (g_lockFileFd >= 0) {
        close(g_lockFileFd);
        g_lockFileFd = -1;
    }
#endif
}


// Получение пути к исполняемому файлу (чтобы перезапускать копии)

std::string getExecutablePath() {
#ifdef _WIN32
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) {
        return "main.exe";
    }
    return std::string(buf);
#elif __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        // path ок
        // для полного пути (realpath)
        char resolved[1024];
        if (realpath(path, resolved)) {
            return resolved;
        } else {
            return path;
        }
    }
    return "./main";
#else
    // Linux
    char buf[1024];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if (len != -1) {
        buf[len] = '\0';
        return std::string(buf);
    }
    return "./main";
#endif
}


// Порождаем копию (mode=1 или mode=2) => отдельный процесс

bool spawnCopy(int mode, const std::string &exePath) {
#ifdef _WIN32
    // Формирование командной строки
    std::string cmd = "\"" + exePath + "\" --child " + std::to_string(mode);
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcessA(
            NULL,
            const_cast<char*>(cmd.c_str()),
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi
    );
    if (!ok) {
        std::cerr << "[spawnCopy] CreateProcess failed, err=" << GetLastError() << std::endl;
        return false;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
#else
    pid_t pid = fork();
    if (pid < 0) {
        perror("[spawnCopy] fork");
        return false;
    }
    if (pid == 0) {
        // child
        // exec: ./main --child <mode>
        std::string modeStr = std::to_string(mode);
        execl(exePath.c_str(), exePath.c_str(), "--child", modeStr.c_str(), (char*)NULL);
        // Если exec не удался
        perror("[spawnCopy] exec");
        _exit(1);
    }
    // parent
    return true;
#endif
}

// Реализация логики копии

int runCopy1() {
    int pid = getPID();
    {
        std::string msg = "[COPY1] Start, PID=" + std::to_string(pid) +
                          ", time=" + currentTimeString();
        writeToLog(msg);
    }
    // +10 к счётчику
    if (lockGlobal()) {
        g_shared->counter += 10;
        unlockGlobal();
    }
    // Логирование выхода
    int val = 0;
    if (lockGlobal()) {
        val = g_shared->counter;
        unlockGlobal();
    }
    {
        std::string msg = "[COPY1] End,   PID=" + std::to_string(pid) +
                          ", time=" + currentTimeString() +
                          ", counter=" + std::to_string(val);
        writeToLog(msg);
    }
    // Снятие флага активности
    if (lockGlobal()) {
        g_shared->copy1Active = false;
        unlockGlobal();
    }
    return 0;
}

// Реализация логики копии

int runCopy2() {
    int pid = getPID();
    {
        std::string msg = "[COPY2] Start, PID=" + std::to_string(pid) +
                          ", time=" + currentTimeString();
        writeToLog(msg);
    }
    // x2
    if (lockGlobal()) {
        g_shared->counter *= 2;
        unlockGlobal();
    }
    // Сон 2 сек
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // /2
    if (lockGlobal()) {
        g_shared->counter /= 2;
        unlockGlobal();
    }
    // Логирование выхода
    int val = 0;
    if (lockGlobal()) {
        val = g_shared->counter;
        unlockGlobal();
    }
    {
        std::string msg = "[COPY2] End,   PID=" + std::to_string(pid) +
                          ", time=" + currentTimeString() +
                          ", counter=" + std::to_string(val);
        writeToLog(msg);
    }
    // снятие флага
    if (lockGlobal()) {
        g_shared->copy2Active = false;
        unlockGlobal();
    }
    return 0;
}

// Поток пользовательского ввода
//  ввод числа => уст. счётчик
//  ввод "q" => завершить программу

static std::atomic<bool> g_running(true);

void userInputThread() {
    while (g_running) {
        std::cout << "Введите целое число (или q для выхода): ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            // EOF
            break;
        }
        if (line == "q" || line == "Q") {
            // завершение
            g_running = false;
            break;
        }
        try {
            int val = std::stoi(line);
            // счётчик
            if (lockGlobal()) {
                g_shared->counter = val;
                unlockGlobal();
            }
            // запись в лог
            {
                std::string msg = "[USER] Установлен счётчик=" + std::to_string(val) +
                                  " PID=" + std::to_string(getPID()) +
                                  ", time=" + currentTimeString();
                writeToLog(msg);
            }
        } catch (...) {
            std::cout << "Некорректный ввод\n";
        }
    }
}

// Основная функция

int main(int argc, char* argv[]) {
    // Проверка, не копия ли мы
    if (argc >= 3 && std::strcmp(argv[1], "--child") == 0) {
        // Подключение к уже существ. shared memory
        // проверка всё равно внутри на всякий случай
        if (!initSharedMemory()) {
            std::cerr << "Child can't init shared memory\n";
            return 1;
        }
        int mode = std::atoi(argv[2]);
        // Выполненние
        int ret = 0;
        if (mode == 1) {
            // copy1
            if (lockGlobal()) {
                g_shared->copy1Active = true; // выставление флага (в теории уже могут быть)
                unlockGlobal();
            }
            ret = runCopy1();
        } else if (mode == 2) {
            if (lockGlobal()) {
                g_shared->copy2Active = true;
                unlockGlobal();
            }
            ret = runCopy2();
        }
        // Завершение
        cleanupSharedMemory();
        return ret;
    }

    // Иначе – обычный процесс
    if (!initSharedMemory()) {
        std::cerr << "[ERROR] initSharedMemory failed.\n";
        return 1;
    }
    // Логирование старта
    {
        int pid = getPID();
        std::string msg = "[MAIN] Start: PID=" + std::to_string(pid) +
                          ", time=" + currentTimeString() +
                          (g_isMaster ? " (MASTER)" : " (SLAVE)");
        writeToLog(msg);
    }

    // Запуск потока ввода пользователя
    std::thread thUser(userInputThread);

    // Время для контроля: каждые 300 мс +1
    auto tp300 = std::chrono::steady_clock::now();
    auto tp1s  = std::chrono::steady_clock::now();
    auto tp3s  = std::chrono::steady_clock::now();

    std::string exePath = getExecutablePath();

    while (g_running) {
        auto now = std::chrono::steady_clock::now();

        // Каждые 300 мс,  counter++
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - tp300).count() >= 300) {
            tp300 = now;
            if (lockGlobal()) {
                g_shared->counter++;
                unlockGlobal();
            }
        }

        // Только мастер: раз в 1сек, лог по типу (время, PID, счётчик)
        if (g_isMaster &&
            std::chrono::duration_cast<std::chrono::milliseconds>(now - tp1s).count() >= 1000)
        {
            tp1s = now;
            int val = 0;
            if (lockGlobal()) {
                val = g_shared->counter;
                unlockGlobal();
            }
            std::string msg = "[MASTER] " + currentTimeString() +
                              ", PID=" + std::to_string(getPID()) +
                              ", counter=" + std::to_string(val);
            writeToLog(msg);
        }

        // Только мастер: раз в 3сек, попытка породить 2 копии
        if (g_isMaster &&
            std::chrono::duration_cast<std::chrono::milliseconds>(now - tp3s).count() >= 3000)
        {
            tp3s = now;
            bool c1Active, c2Active;
            if (lockGlobal()) {
                c1Active = g_shared->copy1Active;
                c2Active = g_shared->copy2Active;
                unlockGlobal();
            } else {
                c1Active = c2Active = true; //  пропуск запуска
            }
            //Если копии не завершились, новые не запускаются
            if (c1Active || c2Active) {
                std::string msg = "[MASTER] " + currentTimeString() +
                                  " Старые копии ещё работают, пропуск запуска.";
                writeToLog(msg);
            } else {
                // запуск copy1 и copy2
                bool ok1 = spawnCopy(1, exePath);
                bool ok2 = spawnCopy(2, exePath);
                if (ok1 && ok2) {
                    std::string msg = "[MASTER] Запущены копии #1 и #2.";
                    writeToLog(msg);
                } else {
                    std::string msg = "[MASTER] Ошибка при запуске копий #1/#2!";
                    writeToLog(msg);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // Ожидание потока ввода
    thUser.join();

    // Логирование выход
    {
        int pid = getPID();
        std::string msg = "[MAIN] End: PID=" + std::to_string(pid) +
                          ", time=" + currentTimeString() +
                          (g_isMaster ? " (MASTER)" : " (SLAVE)");
        writeToLog(msg);
    }

    cleanupSharedMemory();
    return 0;
}
