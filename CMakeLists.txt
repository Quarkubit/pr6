# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(Pr6 C)

# Настройка стандарта C и предупреждений
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Обязательно подключаем потоки
find_package(Threads REQUIRED)

# Добавляем первую программу — поиск последнего вхождения
add_executable(task1 src/task1.c)
target_link_libraries(task1 Threads::Threads)

# Добавляем вторую программу — все вхождения в порядке убывания
add_executable(task2 src/task2.c)
target_link_libraries(task2 Threads::Threads)
