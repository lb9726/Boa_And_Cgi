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

void parseRecvBuffer(char *buf)
{
    int len = strlen(buf);
    if (0 == len)
    {
        printf("RecvBuffer is empty!\n");
        return;
    }
    int index = 0;
    int bufIndex = 0;
    int startFlag = 0;
    int hasTail = 0;
    char *tmpBuffer = NULL;
    tmpBuffer = (char *)malloc(len+1);
    if (NULL != tmpBuffer)
    {
        memcpy(tmpBuffer, buf, len);
    }    
    char ch;
    while (index < len)
    {
        ch = *(tmpBuffer + index);
        if (1 == startFlag)
        {
            if (BVT_ETX == (unsigned char)ch) // 如果是帧尾
            {
                hasTail = 1;
                buf[bufIndex] = '\0';
                break;
            }
            else if (BVT_STX == (unsigned char)ch) /*more one FrameHead*/ 
            {
                break;
            }
            else // 如果不是帧尾和帧头
            {
                if ((unsigned char)ch == BVT_ESC) // 如果为BVT_ESC则需要判断紧接着的一个字符值
                {
                    char chNext = *(tmpBuffer + index + 1);
                    if ((unsigned char)chNext == BVT_STX_AF)
                    {
                        buf[bufIndex++] = (char)(BVT_STX);
                        index += 2; /*跳过这个字符*/
                    }
                    else if ((unsigned char)chNext == BVT_ETX_AF)
                    {
                        buf[bufIndex++] = (char)(BVT_ETX);
                        index += 2; /*跳过这个字符*/
                    }
                    else if ((unsigned char)chNext == BVT_ESC_AF)
                    {
                        buf[bufIndex++] = (char)(BVT_ESC);
                        index += 2; /*跳过这个字符*/
                    }
                    else // 以上都不是的话，此帧error
                    {
                        buf[0] = '\0';
                        return ;
                    }
                }
                else
                {
                    buf[bufIndex++] = *(tmpBuffer + index);
                    ++index;
                }
            }
        }
        else if (BVT_STX != (unsigned char)ch) // 如果不是帧头，往后找
        {
            ++index;
        }
        else if (BVT_STX == (unsigned char)ch) // 如果是帧头，设置开始标志为true
        {
            startFlag = 1;
            ++index;
        }
    }
    if (NULL != tmpBuffer)
    {
        free(tmpBuffer);
        tmpBuffer = NULL;
    }
    if (!hasTail) // has no FrameTail
    {
        buf[0] = '\0';
        return;
    }
    char recvCrc = buf[bufIndex - 1];
    if (recvCrc == getCrcVerify(buf, bufIndex - 1)) /*crc校验成功时，去掉crc的值*/
    {
        buf[bufIndex -1] = '\0';
    }
    else
    {   
        buf[0] = '\0';
    }
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
       // writefile("/usr/bst/error.txt", strerror(errno));
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
    if (read(sockfd, buf, 1024) > 0)
    {
        // printf("read success\n");
    }
    else
    {
        printf("read error\n");
    }
    close(sockfd);
    close(connfd);
    return 0;
}






