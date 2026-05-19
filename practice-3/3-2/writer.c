#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "/tmp/word_count_fifo"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int fd;
    FILE *file;
    char buffer[BUFFER_SIZE];  // Перемещено объявление после проверок

    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (!file) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Ошибка создания FIFO");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    printf("Ожидание подключения читателя...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Ошибка открытия FIFO для записи");
        fclose(file);
        unlink(FIFO_NAME);
        exit(EXIT_FAILURE);
    }
    printf("Читатель подключился. Передача данных...\n");  // Исправлена строка: добавлена закрывающая кавычка

    while (fgets(buffer, BUFFER_SIZE, file)) {
        size_t bytes_to_write = strlen(buffer);
        ssize_t bytes_written = write(fd, buffer, bytes_to_write);
        if (bytes_written == -1) {
            perror("Ошибка записи в FIFO");
            break;
        }
    }

    close(fd);
    fclose(file);
    printf("Данные переданы. Завершение работы писателя.\n");

    return 0;
}
