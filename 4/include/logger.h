#pragma once
#include <string>
#include <mutex>
#include <chrono>

class Logger {
public:
    enum class LogType { ALL, HOURLY, DAILY };

    Logger();
    void log(LogType type, double value);
    void cleanup_old_entries();

private:
    std::mutex mutex;
    const std::chrono::hours ALL_LOG_TTL = std::chrono::hours(24);
    const std::chrono::hours HOURLY_LOG_TTL = std::chrono::hours(720);
    const std::chrono::hours DAILY_LOG_TTL = std::chrono::hours(8760);

    std::string get_filename(LogType type) const;
    void cleanup_file(const std::string& filename, std::chrono::system_clock::time_point cutoff);
};