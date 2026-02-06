Задание 2-2: Синхронизация с мьютексами
Требования:
•
Используйте pthread_mutex_t
•
Функции: pthread_mutex_init(), pthread_mutex_lock(), pthread_mutex_unlock(), pt
hread_mutex_destroy()

Вариант 3: Глобальная переменная balance=1000. Создать 3 потока. Каждый
выполняет 100 операций balance+=10. С мьютексом результат должен быть 4000
