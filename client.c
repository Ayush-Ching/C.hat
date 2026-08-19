#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "socketutil.h"

void startListeningAndPrintMessagesOnNewThread(int socketFD);
void *listenAndPrint(void *arg);

int main(){

    int socketFD = createTCPIpv4Socket();

    char *ip = NULL;
    size_t ipSize = 0;
    printf("Please enter ip address of the server : ");
    ssize_t ipCount = getline(&ip, &ipSize, stdin);
    ip[ipCount-1] = '\0';

    struct sockaddr_in* address = createIpv4Address(ip, 2000);

    int result = connect(socketFD,(struct sockaddr*)address, sizeof(*address));

    if(result == 0){
        printf("Successful Connection\n");
    }

    char *name = NULL;
    size_t nameSize = 0;
    printf("Please enter your name : ");
    ssize_t nameCount = getline(&name, &nameSize, stdin);
    name[nameCount-1] = '\0';

    char *line = NULL;
    size_t lineSize = 0;
    printf("Send a message...\n");

    startListeningAndPrintMessagesOnNewThread(socketFD);

    char buffer[1024];

    while(true){
        ssize_t charCount = getline(&line, &lineSize, stdin);
        line[charCount-1] = '\0';

        sprintf(buffer, "%s : %s", name, line);

        if(charCount > 0){
            if(strcmp(line, "exit") == 0) break;

            ssize_t amountSent = send(socketFD, buffer, strlen(buffer), 0);
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
            printf("%s\n", buffer);
        }

        if(amountReceived <= 0) break;
    }

    close(socketFD);

    return NULL;
}
