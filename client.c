#include "socketutil.h"

int main(){

    int socketFD = createTCPIpv4Socket();

    struct sockaddr_in* address = createIpv4Address("127.0.0.1", 2000);


    int result = connect(socketFD,(struct sockaddr*)address, sizeof(*address));

    if(result == 0){
        printf("Successful Connection\n");
    }

    char* message;
    message = "GET / HTTP/1.1\r\nHost: google.com\r\n\r\n";
    send(socketFD, message, strlen(message), 0);

    char buffer[1024];
    recv(socketFD, buffer, 1024, 0);

    printf("Response : %s\n", buffer);

    return 0;
}
