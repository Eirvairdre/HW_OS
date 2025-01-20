#include "background_launcher.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>

// Функция запуска программы в фоновом режиме
int launch_background(const char* program, const char* args, intptr_t* process_id) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char command_line[1024];

    // Формирование команды для запуска
    snprintf(command_line, sizeof(command_line), "%s %s", program, args ? args : "");

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Запуск процесса без создания нового окна
    if (!CreateProcess(NULL, command_line, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return -1;
    }

    // Сохранение идентификатора процесса и закрытие дескрипторов
    *process_id = (intptr_t)pi.dwProcessId;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}

// Функция ожидания завершения процесса и получения его кода возврата
int wait_for_completion(intptr_t process_id, unsigned int* exit_code) {
    HANDLE hProcess = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, (DWORD)process_id);
    if (hProcess == NULL) return -1;

    WaitForSingleObject(hProcess, INFINITE);
    DWORD dwExitCode;
    if (!GetExitCodeProcess(hProcess, &dwExitCode)) {
        CloseHandle(hProcess);
        return -1;
    }
    *exit_code = (unsigned int)dwExitCode;
    CloseHandle(hProcess);
    return 0;
}

#else // UNIX

#include <unistd.h>
#include <sys/wait.h>

// Функция запуска программы в фоновом режиме
int launch_background(const char* program, const char* args, intptr_t* process_id) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (pid == 0) {
        char* argv[256];
        int argc = 0;

        argv[argc++] = strdup(program);

        if (args && strlen(args) > 0) {
            char* args_copy = strdup(args);
            if (!args_copy) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }

            char* token = strtok(args_copy, " ");
            while (token != NULL && argc < 255) {
                argv[argc++] = strdup(token);
                token = strtok(NULL, " ");
            }
            free(args_copy);
        }

        argv[argc] = NULL;

        // Логирование перед запуском
        printf("Executing: %s\n", program);
        for (int i = 0; argv[i] != NULL; i++) {
            printf("argv[%d] = '%s'\n", i, argv[i]);
        }
        fflush(stdout);

        // Запуск программы
        execvp(program, argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        *process_id = (intptr_t)pid;
        return 0;
    }
}

// Функция ожидания завершения процесса и получения его кода возврата
int wait_for_completion(intptr_t process_id, unsigned int* exit_code) {
    int status;
    if (waitpid((pid_t)process_id, &status, 0) == -1) {
        perror("waitpid");
        return -1;
    }
    if (WIFEXITED(status)) {
        *exit_code = (unsigned int)WEXITSTATUS(status);
        return 0;
    }
    return -1;
}

#endif
