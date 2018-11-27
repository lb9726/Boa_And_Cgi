#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<netdb.h>

#define PORT  60001
#define IPADD  "192.168.1.10"
#include "TcpClient.h"
#include "error.h"
#include <unistd.h>
int initTcpSocket(char *buf, int len)
{
    int sockfd;
    int num ;

    struct sockaddr_in server ;
    
    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Socket() error \n");
        return -1;
    }
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_aton(IPADD, &(server.sin_addr));
    
    if (-1 == connect(sockfd, (struct sockaddr *)&server, sizeof(server)))
    {
        printf("connect () error \n");
        perror("connect error");
        return -1;
    }

    if (-1 == write(sockfd, buf, len))
    {
        printf("write error() \n");
        return -1;
    }
    return sockfd;
}






