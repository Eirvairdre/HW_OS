#include "../include/serial_port.h"
#include "../include/logger.h"
#include "../include/statistics.h"
#include "../include/signal_handler.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
    SignalHandler::init();
    Logger logger;
    Statistics stats;

    std::string port = "/dev/ttyUSB0";
    int baudrate = 9600;

    if(argc > 1) port = argv[1];
    if(argc > 2) baudrate = std::stoi(argv[2]);

    try {
        SerialPort serial(port, baudrate);
        std::cout << "Connected to port: " << port << std::endl;

        std::thread processor([&]{
            while(!SignalHandler::should_stop()) {
                std::this_thread::sleep_for(1h);
                logger.log(Logger::LogType::HOURLY, stats.hourly_average());
                logger.cleanup_old_entries();

                auto now = std::chrono::system_clock::now();
                auto hours = std::chrono::duration_cast<std::chrono::hours>(
                        now.time_since_epoch()
                ).count();

                if(hours % 24 == 0) {
                    logger.log(Logger::LogType::DAILY, stats.daily_average());
                }
            }
        });

        while(!SignalHandler::should_stop()) {
            std::string line;
            if(serial.read_line(line)) {
                std::cout << "[DEBUG] Получена строка: " << line << std::endl; // Отладочный вывод
                try {
                    double value = std::stod(line);
                    stats.add_measurement(value);
                    logger.log(Logger::LogType::ALL, value);
                    std::cout << "Принято значение: " << value << "°C" << std::endl;
                }
                catch(...) {
                    std::cerr << "Ошибка преобразования данных: " << line << std::endl;
                }
            } else {
                std::cerr << "No data received" << std::endl; // Логирование, если данные не получены
            }
            std::this_thread::sleep_for(100ms);
        }

        processor.join();
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}