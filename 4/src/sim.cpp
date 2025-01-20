#include "../include/serial_port.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <port> [baudrate]\n";
        return 1;
    }

    // Аргументы командной строки
    std::string port = argv[1];
    int baudrate = (argc > 2) ? std::stoi(argv[2]) : 9600;

    try {
        // Создаём объект SerialPort
        SerialPort serial(port, baudrate);

        std::srand(static_cast<unsigned>(std::time(nullptr)));
        std::cout << "Temperature sensor simulator started...\n";

        while (true) {
            // Генерируем случайное значение "температуры"
            double temp = 20.0 + (std::rand() % 1000) / 100.0; // от 20.00 до ~29.99
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << temp << "\n";

            // Готовим строку к отправке
            std::string data = oss.str();

            // Печатаем в консоль (отладочно)
            std::cout << "Sending: " << data;

            // Отправляем в последовательный порт
            if (!serial.write_data(data)) {
                std::cerr << "Error: failed to write data to the serial port\n";
                break;
            }

            // Ждём 1 секунду
            std::this_thread::sleep_for(1s);
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
