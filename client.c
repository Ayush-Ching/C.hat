#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "socketutil.h"

void startListeningAndPrintMessagesOnNewThread(int socketFD);
void *listenAndPrint(void *arg);

int main(){

    int socketFD = createTCPIpv4Socket();

    struct sockaddr_in* address = createIpv4Address("127.0.0.1", 2000);

    int result = connect(socketFD,(struct sockaddr*)address, sizeof(*address));

    if(result == 0){
        printf("Successful Connection\n");
    }

    char *line = NULL;
    size_t lineSize = 0;
    printf("type some shit...\n");

    startListeningAndPrintMessagesOnNewThread(socketFD);

    while(true){
        ssize_t charCount = getline(&line, &lineSize, stdin);

        if(charCount > 0){
            if(strcmp(line, "exit\n") == 0) break;

            ssize_t amountSent = send(socketFD, line, charCount, 0);
        }

    }

    close(socketFD);

    return 0;
}

void startListeningAndPrintMessagesOnNewThread(int socketFD){

    pthread_t id;
    pthread_create(&id, NULL, listenAndPrint, (void *)(intptr_t)socketFD);

    
}

void *listenAndPrint(void *arg){
    int socketFD = (int)(intptr_t)arg;

    char buffer[1024];

    while(true){
        ssize_t amountReceived = recv(socketFD, buffer, 1023, 0);

        if(amountReceived > 0){
            buffer[amountReceived] = '\0';
            printf("Response : %s\n", buffer);
        }

        if(amountReceived <= 0) break;
    }

    close(socketFD);

    return NULL;
}
