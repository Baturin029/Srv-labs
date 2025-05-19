#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <string.h>
#include <ctype.h>

void server(void) {
    int rcvid;
    int chid;
    char message[512];
    char response[512];

    printf("Server started.\n");

    chid = ChannelCreate(0);
    if (chid == -1) {
        perror("Failed to create channel");
        return;
    }

    printf("Channel ID: %d\n", chid);
    printf("PID: %d\n", getpid());

    while (1) {
        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        if (rcvid == -1) {
            perror("Failed to receive message");
            continue;
        }

        printf("Message received, rcvid: %X\n", rcvid);
        printf("Original message: \"%s\"\n", message);

        // Сдвиг строки вправо на 2 символа
        int len = strlen(message);
        if (len > 0) {
            // Для строк длиной 1 символ просто копируем его
            if (len == 1) {
                strcpy(response, message);
            } 
            // Для строк длиной 2 символа меняем их местами
            else if (len == 2) {
                response[0] = message[1];
                response[1] = message[0];
                response[2] = '\0';
            }
            // Для строк длиной >2 символов делаем циклический сдвиг
            else {
                int shift = 2 % len; // На случай, если сдвиг больше длины строки
                strncpy(response, message + len - shift, shift);
                strncpy(response + shift, message, len - shift);
                response[len] = '\0';
            }
        } else {
            strcpy(response, ""); // Пустая строка
        }

        // Отправка ответа
        if (MsgReply(rcvid, EOK, response, strlen(response) + 1) == -1) {
            perror("Failed to send response");
        } else {
            printf("Response sent: \"%s\"\n", response);
        }
    }

    ChannelDestroy(chid);
}

int main(void) {
    printf("Server program\n");
    server();
    printf("Baturin Artyom Andreevich i914B.\n");
    return 0;
}