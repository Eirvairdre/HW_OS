#include "../include/signal_handler.h"
#include <iostream>

std::atomic<bool> SignalHandler::stop_flag(false);

void SignalHandler::init() {
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);
#ifdef _WIN32
    std::signal(SIGBREAK, handle_signal);
#endif
}

bool SignalHandler::should_stop() {
    return stop_flag.load();
}

void SignalHandler::handle_signal(int sig) {
    std::cout << "\nReceived stop signal: " << sig << std::endl;
    stop_flag.store(true);
}