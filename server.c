#include "socketutil.h"

int main(){

    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in* serverAddress = createIpv4Address("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    if(result == 0) printf("Socket bound successfully\n");

    int listenResult = listen(serverSocketFD, 10);

    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);

    char buffer[1024];
    recv(clientSocketFD, buffer, 1024, 0);

    printf("Response : %s\n", buffer);

    return 0;
}