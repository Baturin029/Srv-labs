// client.c
#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <string.h>

int main(void) {
    char smsg[512]; // Increased buffer size to match server buffer
    char rmsg[512]; // Increased buffer size to match server buffer
    int coid;
    long serv_pid;
    char choice;

    printf("Client program, enter server PID: \n");
    scanf("%ld", &serv_pid);
    printf("Entered PID: %ld \n", serv_pid);

    do {
        // Attach to the server
        coid = ConnectAttach(0, serv_pid, 1, 0, 0);
        if (coid == -1) {
            printf("Failed to connect to the server. \n");
            return 1;
        }

        printf("Connection result: %d \nEnter your message: ", coid);

        getchar(); // Consume the remaining newline character from previous input
        fgets(smsg, sizeof(smsg), stdin); // Use fgets to allow input with spaces
        smsg[strcspn(smsg, "\n")] = '\0'; // Remove the trailing newline character
        printf("Entered message: %s \n", smsg);

        if (MsgSend(coid, smsg, strlen(smsg) + 1, rmsg, sizeof(rmsg)) == -1) {
            printf("MsgSend failed \n");
            ConnectDetach(coid);
            return 1;
        }

        printf("Response received: %s \n", rmsg);

        // Close the connection
        ConnectDetach(coid);

        // Ask the user if they want to send another message
        printf("Do you want to send another message? (y/n): ");
        fflush(stdout); // Ensure the message is immediately displayed
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    printf("Baturin Artyom Andreevich i914B.\n");
    return 0;
}