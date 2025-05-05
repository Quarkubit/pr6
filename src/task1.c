#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 4     // Оптимальное количество потоков

// Глобальные переменные для разделения между потоками
int* array = NULL;         // Динамический массив
int array_size = 0;        // Размер массива
int target = 0;            // Искомое значение
int last_index = -1;       // Индекс последнего вхождения

pthread_mutex_t mutex;     // Синхронизация доступа к last_index

typedef struct {
    int start;             // Начальный индекс для потока
    int end;               // Конечный индекс (не включительно)
} ThreadArgs;

void* search(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;

    // Поиск с конца участка для раннего обнаружения
    for (int i = args->end - 1; i >= args->start; --i) {
        if (array[i] == target) {
            pthread_mutex_lock(&mutex);
            if (i > last_index) last_index = i;  // Обновление максимума
            pthread_mutex_unlock(&mutex);
            break;  // Прерывание после первого найденного (оптимизация)
        }
    }
    pthread_    (NULL);
}

int main() {
    // Ввод данных
    printf("Введите размер массива: ");
    scanf("%d", &array_size);

    array = (int*)malloc(array_size * sizeof(int));
    if (!array) {
        perror("Ошибка выделения памяти");
        return EXIT_FAILURE;
    }

    printf("Введите элементы массива:\n");
    for (int i = 0; i < array_size; ++i) {
        scanf("%d", &array[i]);
    }

    printf("Введите искомое значение: ");
    scanf("%d", &target);

    // Инициализация потоков
    pthread_t threads[THREAD_COUNT];
    ThreadArgs args[THREAD_COUNT];
    pthread_mutex_init(&mutex, NULL);

    const int chunk = array_size / THREAD_COUNT;

    // Распределение работы
    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i].start = i * chunk;
        args[i].end = (i == THREAD_COUNT-1) ? array_size : (i+1)*chunk;
        pthread_create(&threads[i], NULL, search, &args[i]);
    }

    // Ожидание завершения
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Освобождение ресурсов
    pthread_mutex_destroy(&mutex);
    free(array);

    // Вывод результата
    if (last_index != -1) {
        printf("Последнее вхождение: индекс [%d]\n", last_index);
    } else {
        printf("Значение %d не найдено!\n", target);
    }

    return EXIT_SUCCESS;
}
