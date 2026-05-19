#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = NULL;  // указатель на динамический массив
    int initial_size = 10;  // начальный размер массива
    int new_size = 20;      // новый размер массива после realloc

    // Выделение памяти для 10 чисел с помощью malloc
    arr = (int*)malloc(initial_size * sizeof(int));
    
    // Проверка результата выделения памяти
    if (arr == NULL) {
        fprintf(stderr, "Ошибка выделения памяти с помощью malloc!\n");
        return 1;
    }

    printf("Память для %d элементов успешно выделена с помощью malloc.\n", initial_size);

    // Заполнение массива начальными значениями (для демонстрации)
    for (int i = 0; i < initial_size; i++) {
        arr[i] = i * 10;  // например, 0, 10, 20, ..., 90
    }

    // Вывод начальных значений
    printf("Начальные значения массива:\n");
    for (int i = 0; i < initial_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Увеличение размера массива до 20 элементов с помощью realloc
    int *temp = (int*)realloc(arr, new_size * sizeof(int));

    // Проверка результата realloc
    if (temp == NULL) {
        fprintf(stderr, "Ошибка перераспределения памяти с помощью realloc!\n");
        free(arr);  // освобождаем ранее выделенную память
        return 1;
    }

    arr = temp;  // обновляем указатель на массив
    printf("Память успешно перераспределена до %d элементов с помощью realloc.\n", new_size);

    // Заполнение новых элементов массива (для демонстрации)
    for (int i = initial_size; i < new_size; i++) {
        arr[i] = i * 5;  // например, 50, 55, 60, ..., 95
    }

    // Вывод обновлённого массива
    printf("Обновлённые значения массива (после realloc):\n");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Освобождение выделенной памяти
    free(arr);
    arr = NULL;  // хорошая практика — обнулить указатель после free

    printf("Память освобождена с помощью free.\n");

    return 0;
}
