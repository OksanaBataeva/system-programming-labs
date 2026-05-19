#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int line_count = 0;
    int i;

    // Создаём канал
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создаём дочерний процесс
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс: читаем файл и передаём данные через канал

        // Закрываем конец чтения канала в дочернем процессе
        close(pipefd[0]);

        int file_fd = open("input.txt", O_RDONLY);
        if (file_fd == -1) {
            perror("open");
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }

        // Читаем файл порциями и передаём в канал
        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(pipefd[1], buffer, bytes_read) == -1) {
                perror("write");
                close(file_fd);
                close(pipefd[1]);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1) {
            perror("read");
            close(file_fd);
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }

        // Закрываем файловый дескриптор файла и конец записи канала
        close(file_fd);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);

    } else {
        // Родительский процесс: читаем данные из канала и подсчитываем строки

        // Закрываем конец записи канала в родительском процессе
        close(pipefd[1]);

        // Читаем данные из канала
        while ((bytes_read = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
            // Подсчитываем количество строк в прочитанных данных
            for (i = 0; i < bytes_read; i++) {
                if (buffer[i] == '\n') {
                    line_count++;
                }
            }
        }

        if (bytes_read == -1) {
            perror("read");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        }

        // Закрываем конец чтения канала
        close(pipefd[0]);

        // Ждём завершения дочернего процесса
        wait(NULL);

        // Выводим результат
        printf("Количество строк в файле: %d\n", line_count);
    }

    return 0;
}
