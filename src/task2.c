#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 4        // Оптимальное количество потоков

// Глобальные переменные
int* array = NULL;            // Основной массив для поиска
int array_size = 0;           // Размер массива
int target = 0;               // Искомое значение

int* result_array = NULL;     // Динамический массив для результатов
int result_count = 0;         // Количество найденных элементов
int result_capacity = 0;      // Текущая ёмкость массива результатов

pthread_mutex_t mutex;        // Мьютекс для синхронизации доступа

// Структура для передачи параметров потокам
typedef struct {
    int start;                // Начальный индекс (включительно)
    int end;                  // Конечный индекс (не включительно)
} ThreadArgs;

// Функция поиска для потоков
void* search(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;

    for (int i = args->start; i < args->end; ++i) {
        if (array[i] == target) {
            pthread_mutex_lock(&mutex);

            // Динамическое расширение массива при необходимости
            if (result_count >= result_capacity) {
                result_capacity = result_capacity ? result_capacity * 2 : 10;
                int* temp = realloc(result_array, result_capacity * sizeof(int));
                if (!temp) {
                    perror("Ошибка расширения массива результатов");
                    exit(EXIT_FAILURE);
                }
                result_array = temp;
            }

            result_array[result_count++] = i;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

// Функция сравнения для сортировки по убыванию
int compare_desc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

int main() {
    // Ввод данных
    printf("Введите размер массива: ");
    scanf("%d", &array_size);

    array = malloc(array_size * sizeof(int));
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

    const int chunk_size = array_size / THREAD_COUNT;

    // Распределение работы между потоками
    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i].start = i * chunk_size;
        args[i].end = (i == THREAD_COUNT-1) ? array_size : (i+1)*chunk_size;
        pthread_create(&threads[i], NULL, search, &args[i]);
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Сортировка результатов
    qsort(result_array, result_count, sizeof(int), compare_desc);

    // Вывод результатов
    if (result_count > 0) {
        printf("Найдено %d вхождений. Индексы по убыванию:\n", result_count);
        for (int i = 0; i < result_count; ++i) {
            printf("%d ", result_array[i]);
        }
        printf("\n");
    } else {
        printf("Значение %d не найдено\n", target);
    }

    // Освобождение ресурсов
    pthread_mutex_destroy(&mutex);
    free(array);
    free(result_array);

    return EXIT_SUCCESS;
}
