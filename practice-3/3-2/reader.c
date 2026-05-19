#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define FIFO_NAME "/tmp/word_count_fifo"
#define BUFFER_SIZE 1024

int count_words(const char *text) {
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isspace((unsigned char)text[i])) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }

    return word_count;
}

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    int total_words = 0;

    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия FIFO для чтения");
        exit(EXIT_FAILURE);
    }
    printf("Писатель подключился. Чтение данных...\n");

    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        total_words += count_words(buffer);
    }

    // Проверка ошибки чтения
    if (bytes_read == -1) {
        perror("Ошибка чтения из FIFO");
        close(fd);
        unlink(FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    close(fd);
    printf("Подсчёт завершён. Общее количество слов: %d\n", total_words);

    // Удаление FIFO
    if (unlink(FIFO_NAME) == -1) {
        perror("Ошибка удаления FIFO");
        exit(EXIT_FAILURE);
    }
    printf("FIFO удалён.\n");

    return 0;
}
