#include "../include/serial_port.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

SerialPort::SerialPort(const std::string& port, int baudrate) {
#ifdef _WIN32
    // Открытие порта
    handle = CreateFileA(port.c_str(),
                         GENERIC_READ | GENERIC_WRITE,
                         0, NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Can't open port " + port);
    }

    // Текущие настройки DCB
    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(handle, &dcb)) {
        CloseHandle(handle);
        throw std::runtime_error("GetCommState failed for port " + port);
    }

    // Устанавка скорости, формат кадра
    dcb.BaudRate = baudrate;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity   = NOPARITY;

    // Отключение аппаратного управления потоком
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl  = DTR_CONTROL_DISABLE;
    dcb.fRtsControl  = RTS_CONTROL_DISABLE;

    // Отключение программного flow control
    dcb.fOutX = FALSE;
    dcb.fInX  = FALSE;

    if (!SetCommState(handle, &dcb)) {
        CloseHandle(handle);
        throw std::runtime_error("SetCommState failed for port " + port);
    }

    // Настройка таймаута на чтение/запись
    COMMTIMEOUTS timeouts = {0};
    // Установка таймаутов чтобы не висло
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(handle, &timeouts)) {
        CloseHandle(handle);
        throw std::runtime_error("SetCommTimeouts failed for port " + port);
    }

#else
    // POSIX-ветка
    fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        throw std::runtime_error("Can't open port " + port);
    }

    termios tty{};
    tcgetattr(fd, &tty);

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB;      // Нет бита четности
    tty.c_cflag &= ~CSTOPB;      // Один стоп-бит
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;          // 8 бит
    tty.c_cflag |= CREAD | CLOCAL; // Включенние приёмника, игнор линии управления

    tcsetattr(fd, TCSANOW, &tty);
#endif
}

SerialPort::~SerialPort() {
#ifdef _WIN32
    CloseHandle(handle);
#else
    close(fd);
#endif
}

// Чтение строки до символа '\n'
bool SerialPort::read_line(std::string& line) {
    char buf[256];
    line.clear();

#ifdef _WIN32
    DWORD bytes_read = 0;
    if (!ReadFile(handle, buf, sizeof(buf), &bytes_read, NULL)) {
        return false;
    }
#else
    ssize_t bytes_read = ::read(fd, buf, sizeof(buf));
    if (bytes_read < 0) {
        return false;
    }
#endif

    if (bytes_read > 0) {
        buffer.append(buf, bytes_read);
        size_t pos = buffer.find('\n');
        if (pos != std::string::npos) {
            line = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            return true;
        }
    }
    return false;
}

// Запись строки в порт
bool SerialPort::write_data(const std::string& data) {
#ifdef _WIN32
    DWORD bytes_written = 0;
    if (!WriteFile(handle, data.c_str(), (DWORD)data.size(), &bytes_written, NULL)) {
        return false;
    }
    return (bytes_written == data.size());
#else
    ssize_t bytes_written = ::write(fd, data.c_str(), data.size());
    return (bytes_written == (ssize_t)data.size());
#endif
}
