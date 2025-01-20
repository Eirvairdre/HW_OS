#pragma once
#include <atomic>
#include <csignal>

class SignalHandler {
public:
    static void init();
    static bool should_stop();

private:
    static std::atomic<bool> stop_flag;
    static void handle_signal(int sig);
};