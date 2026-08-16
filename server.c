#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include "socketutil.h"

struct AcceptedSocket{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD);
void receiveAndPrintIncomingData(int socketFD);

int main(){

    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in* serverAddress = createIpv4Address("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    if(result == 0) printf("Socket bound successfully\n");

    int listenResult = listen(serverSocketFD, 10);

    struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFD);

    // char buffer[1024];
    // while(true){
    //     ssize_t amountReceived = recv(clientSocket->acceptedSocketFD, buffer, 1024, 0);
    
    //     if(amountReceived > 0){
    //         buffer[amountReceived] = 0;
    //         printf("Response : %s\n", buffer);
    //     }

    //     if(amountReceived <= 0) break;
    // }

    receiveAndPrintIncomingData(clientSocket->acceptedSocketFD);

    close(clientSocket->acceptedSocketFD);
    shutdown(serverSocketFD, SHUT_RDWR);

    return 0;
}

void receiveAndPrintIncomingData(int socketFD){
    char buffer[1024];
    while(true){
        ssize_t amountReceived = recv(socketFD, buffer, 1024, 0);
    
        if(amountReceived > 0){
            buffer[amountReceived] = 0;
            printf("Response : %s\n", buffer);
        }

        if(amountReceived <= 0) break;
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