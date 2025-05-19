#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <hw/inout.h>

/*
Для модуля ядра:

    Требуются права root и специальная компиляция (-Wc,-nostdlib).

    Обычно используется Buildfile для сборки в составе ОС.
*/

volatile uintptr_t timer_base;
volatile int counter = 0;

// Обработчик прерывания
const struct sigevent *timer_isr(void *arg, int id) {
    counter++;
    printf("Timer ISR called: %d\n", counter);
    out32(timer_base + 0x0, 0x1);  // Сброс флага прерывания
    return NULL;
}

int main() {
    int intr;
    struct sigevent event;
    struct _clockperiod period;

    // Получаем базовый адрес системного таймера (зависит от платформы)
    timer_base = (uintptr_t)mmap_device_io(4, 0x80810000);  // Пример для x86

    // Настройка прерывания
    intr = InterruptAttach(0, timer_isr, NULL, 0, 0);
    if (intr == -1) {
        perror("InterruptAttach failed");
        return EXIT_FAILURE;

    printf("Baturin Artyom Andreevich i914B\n");
    return 1;
}

    // Настройка периода таймера (например, 1 мс)
    period.nsec = 1000000;  // 1 мс
    period.fract = 0;
    ClockPeriod(CLOCK_REALTIME, &period, NULL, 0);

    printf("Timer ISR is running. Press Enter to exit...\n");
    getchar();

    InterruptDetach(intr);
    munmap_device_io((void*)timer_base, 4);
    return EXIT_SUCCESS;
}