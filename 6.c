#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define MEM_NAME "ourmemory"

int threads(void);
void *thread_1(void *);
void *thread_2(void *);
int process_1(void);
int process_2(void);

// Структура разделяемой памяти
typedef struct {
    sem_t sem;
    int value;
} sh_mem_t;

pthread_mutex_t mut;   // Мьютекс
sh_mem_t *sm;          // Указатель на разделяемую память

int main(void) {
    int md;             // Дескриптор разделяемой памяти
    int childpid;       // ID дочернего процесса

    // На случай, если память не удалена
    shm_unlink(MEM_NAME);

    // Создание разделяемой памяти
    md = shm_open(MEM_NAME, O_RDWR | O_CREAT, 0777);
    if (md == -1) {
        perror("shm_open()");
        return -1;
    }

    // Установка размера разделяемой памяти
    if (ftruncate(md, sizeof(*sm)) == -1) {
        perror("ftruncate()");
        return -1;
    
printf("Baturin Artyom Andreevich i914B\n");
    return 1;
}

    // Отображение разделяемой памяти
    sm = mmap(NULL, sizeof(*sm), PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
    if (sm == MAP_FAILED) {
        perror("mmap()");
        return -1;
    }

    // Инициализация семафора (межпроцессный семафор)
    if (sem_init(&(sm->sem), 1, 1) == -1) {
        perror("sem_init()");
        exit(EXIT_FAILURE);
    }

    // Начальное значение переменной
    sm->value = 0;

    // Создание дочернего процесса
    childpid = fork();
    if (childpid == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }

    if (childpid) {
        // Родительский процесс
        if (process_1() != 0) {
            perror("process_1()");
            exit(EXIT_FAILURE);
        }
        wait(NULL); // Ожидание завершения дочернего процесса
    } 
    else {
        // Дочерний процесс
        if (process_2() != 0) {
            perror("process_2()");
            exit(EXIT_FAILURE);
        }
        if (threads() != 0) {
            perror("threads()");
            exit(EXIT_FAILURE);
        }
    }

    // Закрытие и удаление разделяемой памяти
    close(md);
    shm_unlink(MEM_NAME);

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mut);

    return (EXIT_SUCCESS);
}

// Функция первого процесса
int process_1(void) {
    sem_wait(&(sm->sem));
    printf("First process.\n(1) Previous value: %i\n", sm->value);

    sm->value = 10;
    sm->value += 2;
    sm->value *= 4;
    sm->value -= 15;

    printf("(1) Result: %i\n", sm->value);
    sem_post(&(sm->sem));
    return 0;
}

// Функция второго процесса
int process_2(void) {
    sem_wait(&(sm->sem));
    printf("Second process.\n(2) Previous value: %i\n", sm->value);

    sm->value = 7;
    sm->value += 5;
    sm->value *= 2;
    sm->value -= 1;

    printf("(2) Result: %i\n", sm->value);
    sem_post(&(sm->sem));
    return 0;
}

// Функция создания и управления потоками
int threads(void) {
    pthread_t thid_1, thid_2;

    // Инициализация мьютекса
    if (pthread_mutex_init(&mut, NULL) == 1) {
        perror("pthread_mutex_init()");
        return -1;
    }

    // Создание потоков
    if (pthread_create(&thid_1, NULL, thread_1, NULL) != 0) {
        perror("pthread_create(thread_1)");
        return -1;
    }
    if (pthread_create(&thid_2, NULL, thread_2, NULL) != 0) {
        perror("pthread_create(thread_2)");
        return -1;
    }

    // Ожидание завершения потоков
    pthread_join(thid_1, NULL);
    pthread_join(thid_2, NULL);

    return 0;
}

// Поток 1
void *thread_1(void *arg) {
    pthread_mutex_lock(&mut);
    printf("First lock\n");
    sleep(2);
    printf("First unlock\n");
    pthread_mutex_unlock(&mut);
    return NULL;
}

// Поток 2
void *thread_2(void *arg) {
    pthread_mutex_lock(&mut);
    printf("Second lock\n");
    sleep(2);
    printf("Second unlock\n");
    pthread_mutex_unlock(&mut);
    return NULL;
}

