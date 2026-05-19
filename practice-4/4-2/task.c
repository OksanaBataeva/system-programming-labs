#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *filename = "mapped_file.txt";
    const char *text_to_write = "Hello, mmap world!";
    int fd;
    struct stat file_stat;
    char *mapped_addr;
    size_t file_size = strlen(text_to_write) + 1; // +1 для нуль‑терминатора

    // Шаг 1: Создание файла
    fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    // Шаг 2: Установка размера файла
    if (ftruncate(fd, file_size) == -1) {
        perror("Ошибка при установке размера файла");
        close(fd);
        return EXIT_FAILURE;
    }

    // Шаг 3: Получение информации о файле
    if (fstat(fd, &file_stat) == -1) {
        perror("Ошибка при получении информации о файле");
        close(fd);
        return EXIT_FAILURE;
    }

    // Убедимся, что размер файла соответствует ожиданиям
    if ((size_t)file_stat.st_size != file_size) {
        fprintf(stderr, "Неожиданный размер файла: %ld, ожидался: %zu\n",
                file_stat.st_size, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Шаг 4: Отображение файла в память
    mapped_addr = mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                       MAP_SHARED, fd, 0);
    if (mapped_addr == MAP_FAILED) {
        perror("Ошибка при отображении файла в память");
        close(fd);
        return EXIT_FAILURE;
    }

    // Шаг 5: Запись данных в отображённую область
    strcpy(mapped_addr, text_to_write);
    printf("Данные записаны в файл через mmap: '%s'\n", mapped_addr);

    // Шаг 6: Освобождение ресурсов
    if (munmap(mapped_addr, file_size) == -1) {
        perror("Ошибка при освобождении памяти");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) == -1) {
        perror("Ошибка при закрытии файла");
        return EXIT_FAILURE;
    }

    printf("Программа завершена успешно.\n");
    return EXIT_SUCCESS;
}
