#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Программа запущена. PID: %d\n", getpid());
    printf("Ожидание для анализа памяти... (Ctrl+C для завершения)\n");

    // Выделяем большой блок памяти
    char *large_block = malloc(100 * 1024 * 1024); // 100 МБ
    if (!large_block) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    // Заполняем память для гарантированного использования
    for (size_t i = 0; i < 100 * 1024 * 1024; i += 4096) {
        large_block[i] = (char)(i % 256);
    }

    while (1) {
        sleep(1); // Бесконечный цикл для анализа
    }
    return 0;
}
