#ifndef BACKGROUND_LAUNCHER_H
#define BACKGROUND_LAUNCHER_H

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

// Запуск программы в фоновом режиме
int launch_background(const char* program, const char* args, intptr_t* process_id);

// Ожидание завершения программы и получение кода возврата
int wait_for_completion(intptr_t process_id, unsigned int* exit_code);

#endif