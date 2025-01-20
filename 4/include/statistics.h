#pragma once
#include <deque>
#include <chrono>
#include <mutex>

class Statistics {
public:
    void add_measurement(double value);
    double hourly_average() const;
    double daily_average() const;

private:
    struct Measurement {
        double value;
        std::chrono::system_clock::time_point timestamp;
    };

    mutable std::mutex mutex;
    std::deque<Measurement> measurements;

    template<typename Duration>
    double calculate_average(Duration duration) const {
        std::lock_guard<std::mutex> lock(mutex);
        const auto cutoff = std::chrono::system_clock::now() - duration;

        double sum = 0.0;
        int count = 0;

        for(const auto& m : measurements) {
            if(m.timestamp > cutoff) {
                sum += m.value;
                count++;
            }
        }

        return count > 0 ? sum / count : 0.0;
    }
};