#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <locale.h>
#include <unistd.h> 

// Структура для передачи аргументов в поток
typedef struct thread_args {
    int id;                // Идентификатор потока
    int sleep_time;        // Время задержки потока
    int priority;          // Приоритет потока        
    const char* msg;       // Сообщение для вывода
} thread_args;

// Функция, выполняемая потоком
void *thread_func(void *args) {    
    thread_args* cur_args = (thread_args*)args; 

    // Вывод информации о начале работы потока
    printf("Thread %d: started execution. Priority = %d\n", cur_args->id, cur_args->priority);
    printf("Thread %d: %s\n", cur_args->id, cur_args->msg);

    // Имитация работы потока (задержка)
    sleep(cur_args->sleep_time); 

    // Вывод информации о завершении работы потока
    printf("Thread %d: finished execution\n", cur_args->id);

    pthread_exit(NULL);
}

// Функция создания потока с заданными параметрами
void create_thread(pthread_t* tid, int policy, int prio, void*(func)(void*), void* args) {
    pthread_attr_t attr;
    struct sched_param param;

    // Инициализация атрибутов потока
    pthread_attr_init(&attr);

    // Установка приоритета и политики планирования
    param.sched_priority = prio;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setschedpolicy(&attr, policy);

    // Явное указание наследования планирования (не от родителя)
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // Установка состояния потока (joinable)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Создание потока
    if (pthread_create(tid, &attr, func, args) != 0) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }

    // Уничтожение атрибутов потока
    pthread_attr_destroy(&attr);
}

int main() {
    // Идентификаторы потоков
    pthread_t thread_id1, thread_id2;

    // Аргументы для потоков
    thread_args args1 = {1, 5, 5, "Message from the first thread"};  // Приоритет 5
    thread_args args2 = {2, 10, 11, "Message from the second thread"};  // Приоритет 11

    // Создание потоков
    create_thread(&thread_id1, SCHED_RR, args1.priority, thread_func, (void*)&args1);
    create_thread(&thread_id2, SCHED_FIFO, args2.priority, thread_func, (void*)&args2);

    // Ожидание завершения потоков
    if (pthread_join(thread_id1, NULL) != 0) {
        perror("Error waiting for the first thread to finish");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(thread_id2, NULL) != 0) {
        perror("Error waiting for the second thread to finish");
        exit(EXIT_FAILURE);
    }

    printf("All threads have finished.\n");
    printf("Baturin Artyom Andreevich i914B.\n");
    return 0;
}