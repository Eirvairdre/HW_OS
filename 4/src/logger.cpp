#include "../include/logger.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>

Logger::Logger() {
    std::ofstream(get_filename(LogType::ALL));
    std::ofstream(get_filename(LogType::HOURLY));
    std::ofstream(get_filename(LogType::DAILY));
}

void Logger::log(LogType type, double value) {
    std::lock_guard<std::mutex> lock(mutex);
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::ofstream file(get_filename(type), std::ios::app);
    file << time << " " << value << "\n";
}

std::string Logger::get_filename(LogType type) const {
    switch(type) {
        case LogType::ALL: return "log_all_measurements.log";
        case LogType::HOURLY: return "log_hourly_averages.log";
        case LogType::DAILY: return "log_daily_averages.log";
        default: return "unknown.log";
    }
}

void Logger::cleanup_file(const std::string& filename,
                          std::chrono::system_clock::time_point cutoff) {
    std::ifstream in_file(filename);
    if(!in_file) return;

    std::vector<std::string> valid_entries;
    std::string line;

    while(std::getline(in_file, line)) {
        std::istringstream iss(line);
        time_t timestamp;
        double value;

        if(iss >> timestamp >> value) {
            auto entry_time = std::chrono::system_clock::from_time_t(timestamp);
            if(entry_time > cutoff) {
                valid_entries.push_back(line);
            }
        }
    }
    in_file.close();

    std::ofstream out_file(filename, std::ios::trunc);
    for(const auto& entry : valid_entries) {
        out_file << entry << "\n";
    }
}

void Logger::cleanup_old_entries() {
    auto now = std::chrono::system_clock::now();
    cleanup_file(get_filename(LogType::ALL), now - ALL_LOG_TTL);
    cleanup_file(get_filename(LogType::HOURLY), now - HOURLY_LOG_TTL);
    cleanup_file(get_filename(LogType::DAILY), now - DAILY_LOG_TTL);
}