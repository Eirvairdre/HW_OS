#pragma once
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

class SerialPort {
public:
    // Открываем порт (Windows или POSIX)
    SerialPort(const std::string& port, int baudrate);

    // Закрываем порт
    ~SerialPort();

    // Чтение строки (прерываем по '\n')
    bool read_line(std::string& line);

    // Запись данных в порт. Возвращает true, если успешно записали все байты.
    bool write_data(const std::string& data);

private:
#ifdef _WIN32
    HANDLE handle;
#else
    int fd;
#endif
    std::string buffer;
};
