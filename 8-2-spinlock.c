#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>

// Объявляем спин-лок
static pthread_spinlock_t spinlock;

// Функция потока, которая использует спин-лок
void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    
    printf("Поток %d пытается захватить спин-лок...\n", thread_num);
    
    // Захватываем спин-лок
    pthread_spin_lock(&spinlock);
    
    printf("Поток %d захватил спин-лок. Работает...\n", thread_num);
    
    // Имитация работы
    delay(500); // Задержка 500 мс
    
    printf("Поток %d освобождает спин-лок.\n", thread_num);
    
    // Освобождаем спин-лок
    pthread_spin_unlock(&spinlock);
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread_num1 = 1, thread_num2 = 2;
    
    // Инициализируем спин-лок
    if (pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE) != 0) {
        perror("Ошибка инициализации спин-лока");
        return EXIT_FAILURE;
    }
    
    printf("Создаем потоки...\n");
    
    // Создаем два потока
    pthread_create(&thread1, NULL, thread_func, &thread_num1);
    pthread_create(&thread2, NULL, thread_func, &thread_num2);
    
    // Ждем завершения потоков
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // Уничтожаем спин-лок
    pthread_spin_destroy(&spinlock);
    
    printf("Все потоки завершили работу.\n");
    
printf("Baturin Artyom Andreevich i914B\n");
    return 1;
}