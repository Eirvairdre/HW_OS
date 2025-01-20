#include "background_launcher.h"
#include <stdio.h>
#include <string.h>

// Основная программа для тестирования библиотеки запуска процессов
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program> [args]\n", argv[0]);
        return 1;
    }

    intptr_t process_id;
    unsigned int exit_code;
    const char* program = argv[1];
    const char* args = (argc > 2) ? argv[2] : "";

#ifdef _WIN32
    if (strcmp(program, "ping") == 0) {
        program = "C:\\Windows\\System32\\ping.exe";
        args = "-n 1 localhost";
    }
#endif

    // Запуск программы в фоновом режиме
    if (launch_background(program, args, &process_id) != 0) {
        printf("Failed to launch program.\n");
        return 1;
    }
    printf("Launched process with ID: %ld\n", (long)process_id);

    // Ожидание завершения процесса и получение кода возврата
    if (wait_for_completion(process_id, &exit_code) != 0) {
        printf("Failed to wait for process completion.\n");
        return 1;
    }
    printf("Process exited with code: %u\n", exit_code);
    return 0;
}
