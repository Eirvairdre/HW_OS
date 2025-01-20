#include "../include/statistics.h"

void Statistics::add_measurement(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    measurements.push_back({
                                   value,
                                   std::chrono::system_clock::now()
                           });
}

double Statistics::hourly_average() const {
    return calculate_average(std::chrono::hours(1));
}

double Statistics::daily_average() const {
    return calculate_average(std::chrono::hours(24));
}