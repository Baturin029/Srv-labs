#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

// Обработчик прерывания таймера
void timer_handler(int sig, siginfo_t *info, void *context) {
    printf("Timer expired! Signal: %d\n", sig);
}

int main() {
    timer_t timer_id;
    struct sigevent sev;
    struct itimerspec its;
    struct sigaction sa;

    // Настройка обработчика сигнала
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    // Создание таймера
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timer_id;
    timer_create(CLOCK_REALTIME, &sev, &timer_id);

    // Установка интервала таймера (1 секунда)
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;  // Периодический таймер
    its.it_interval.tv_nsec = 0;
    timer_settime(timer_id, 0, &its, NULL);

    printf("Timer started. Press Ctrl+C to exit.\n");
    while (1) {
        pause();  // Ожидание сигналов
    }

printf("Baturin Artyom Andreevich i914B\n");
    return 1;
}