#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#define PORT  60001
#define IPADD  "127.0.0.1"
#include "TcpClient.h"

#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BVT_STX (0x80)        /* 帧起始字符 */
#define BVT_ETX (0x81)        /* 帧结束字符 */
#define BVT_ESC (0x1B)        /* 转换字符 */

#define BVT_STX_AF (0xE7)     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF (0xE8)     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF (0x00)     /* 转换字符转换后增加字符 */

void writefile(const char *filename, char *str)
{
    int fd = open(filename, O_CREAT|O_RDWR, 0666);
    if (fd > 0)
    {
        write(fd, str, strlen(str));
    }
    else 
    {
        printf("file open error!\n");
    }
}

char getCrcVerify(char *buf, int length)
{
    char crc = 0x00;
    int i = 0;
    for (i = 0; i < length; ++i)
    {
        crc ^= buf[i];
    }
    return crc & 0x7f;
}

void generateBuffer(char *buf)
{
    char tmpBuf[1024];
    int len = strlen(buf);
    char crc = getCrcVerify(buf, len);
    buf[len] = crc;
    buf[len+1] = '\0';
    len = len + 1;
    memcpy(tmpBuf, buf, len);
    int i = 0;
    int index = 0;
    if (len > 0)
    {
        buf[index] = (char)(BVT_STX);
        ++index;
    }
    else
    {
        printf("the string len is 0\n");
        return;
    }
    for (i = 0; i < len; ++i)
    {
        if (BVT_STX == tmpBuf[i])
        {
            buf[index] = (char)BVT_ESC;
            ++index;
            buf[index] = (char)BVT_STX_AF;
            ++index;
        }
        else if (BVT_ETX == tmpBuf[i])
        {
            buf[index] = (char)BVT_ESC;
            ++index;
            buf[index] = (char)BVT_ETX_AF;
            ++index;
        }
        else if (BVT_ESC == tmpBuf[i])
        {
            buf[index] = (char)BVT_ESC;
            ++index;
            buf[index] = (char)BVT_ESC_AF;
            ++index;
        }
        else
        {
            buf[index] = tmpBuf[i];
            ++index;
        }
    }
    buf[index] = (char)BVT_ETX;
    ++index;
    buf[index] = '\0';
   // writefile("/usr/bst/writeData.txt", buf);
    //printf("buf is in generatebuf = %s\n", buf);
}

int initTcpSocket(char *buf, int len)
{
    int sockfd;
    int connfd;
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
    
    if (-1 == (connfd = connect(sockfd, (struct sockaddr *)&server, sizeof(server))))
    {
        printf("connect () error \n");
        writefile("/usr/bst/error.txt", strerror(errno));
        close(sockfd);
        return -1;
    }
    else
    {
       // printf("connect () success \n");       
    }

    if (-1 == write(sockfd, buf, len))
    {
        printf("write error() \n");
        close(connfd);
        close(sockfd);
        return -1;
    }
    else
    {
       // printf("write () success \n");
    }
    close(sockfd);
    close(connfd);
    return 0;
}






