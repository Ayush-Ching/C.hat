#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "socketutil.h"

struct AcceptedSocket{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD);
void *receiveAndPrintIncomingData(void *arg);
void startAcceptingIncomingConnections(int serverSocketFD);
void *acceptNewConnectionAndReceiveAndPrintItsData(void *arg);
void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket * clientSocket);
void sendReceivedMessageToOtherClients(char* buffer, int socketFD);

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;

int main(){

    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in* serverAddress = createIpv4Address("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    if(result == 0) printf("Socket bound successfully\n");

    int listenResult = listen(serverSocketFD, 10);
    
    startAcceptingIncomingConnections(serverSocketFD);
    
    shutdown(serverSocketFD, SHUT_RDWR);
    
    return 0;
}

void startAcceptingIncomingConnections(int serverSocketFD){
    
    while(true){
        struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFD);
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;

        receiveAndPrintIncomingDataOnSeparateThread(clientSocket);
    }

}

void *acceptNewConnectionAndReceiveAndPrintItsData(void *arg){
    int serverSocketFD = (int)(intptr_t)arg;

    

    return NULL;
}

void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket * clientSocket){

    pthread_t id;
    pthread_create(&id, NULL, receiveAndPrintIncomingData, (void *)(intptr_t)clientSocket->acceptedSocketFD);
}

void *receiveAndPrintIncomingData(void *arg){
    int socketFD = (int)(intptr_t)arg;

    char buffer[1024];

    while(true){
        ssize_t amountReceived = recv(socketFD, buffer, 1023, 0);

        if(amountReceived > 0){
            buffer[amountReceived] = '\0';
            printf("%s\n", buffer);

            sendReceivedMessageToOtherClients(buffer, socketFD);
        }

        if(amountReceived <= 0) break;
    }

    close(socketFD);

    return NULL;
}

void sendReceivedMessageToOtherClients(char* buffer, int socketFD){
    for(int i=0; i<acceptedSocketsCount; i++){
        if(acceptedSockets[i].acceptedSocketFD != socketFD){
            send(acceptedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
        }
    }
}

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD){
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);

    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

    if(!acceptedSocket->acceptedSuccessfully) acceptedSocket->error = clientSocketFD;

    return acceptedSocket;
}