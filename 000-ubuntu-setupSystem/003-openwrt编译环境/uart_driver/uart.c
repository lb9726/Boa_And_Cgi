/*
 * Copyright (c) 2016 Dropbeats, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/stat.h>     
#include <fcntl.h>
#include <termios.h>
#include <errno.h> 
#include <string.h>
#include "db_types.h"
#include "uart.h"

int UART_Open(char* ttyname)
{
    int fd = -1;
     
    fd = open(ttyname, O_RDWR|O_NOCTTY|O_NDELAY);  
    if (fd < 0)  
    {  
        printf("Error: Cannot Open Serial Port (%s)!\n", ttyname);  
        return RET_FAIL;
    }
    
    if (fcntl(fd, F_SETFL, 0) < 0)  
    {  
        printf("Error: Failed to call fcntl()!\n"); 
        return RET_FAIL;
    }

    if (0 == isatty(fd))  
    {  
        printf("Error: Not a terminal device!\n");  
        return RET_FAIL;  
    }
  
    return fd;  
}

void UART_Close(int fd)
{  
    close(fd);  
}  

int UART_Set(int fd, uart_cfg_t *pcfg)
{  
    int             i; 
    int             found = 0;
    int             status; 
    int             default_idx = 0; 
    int             speed_defs[]  = {B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200};  
    int             speed_value[] = {115200,  57600,  38400,  19200,  9600,  4800,  2400,  1200};  
    struct termios  options;
     
    if (tcgetattr(fd, &options) !=  0)  
    {  
        printf("Error: Failed to call tcgetattr().\n");
        return RET_FAIL;   
    }  
    
    for (i = 0;  i < sizeof(speed_defs)/sizeof(int);  i++)  
    {
        if (pcfg->speed == speed_value[i])  
        {
            found = 1;
            cfsetispeed(&options, speed_defs[i]);
            cfsetospeed(&options, speed_defs[i]); 
            break;  
        }  
    } 
    
    if (!found)
    {
        printf("Error: Unsupported speed!\n");
        return RET_FAIL;
    }
     
    options.c_cflag |= CLOCAL;  
    options.c_cflag |= CREAD;  
    
    switch(pcfg->flow_ctrl)  
    {  
        case 0:
            /* no flow ctrl */
            options.c_cflag &= ~CRTSCTS;  
            break;     
        
        case 1:
            /* h/w */
            options.c_cflag |= CRTSCTS;  
            break;  
       
        case 2:
            /* s/w */
            options.c_cflag |= IXON | IXOFF | IXANY;  
            break;
            
        default:
            /* no flow ctrl */
            printf("Error: Unsupported flow_ctrl!\n");
            return RET_FAIL;
    } 
    
    options.c_cflag &= ~CSIZE;  

    switch (pcfg->databits)  
    {    
        case 5:  
            options.c_cflag |= CS5;  
            break;
            
        case 6:  
            options.c_cflag |= CS6;  
            break;  
        
        case 7    :      
            options.c_cflag |= CS7;  
            break;  
       
        case 8:      
            options.c_cflag |= CS8;  
            break;    
       
        default:     
            printf("Error: Unsupported data size!\n");
            return RET_FAIL;
    } 
    
    switch (pcfg->parity)  
    {    
        case 'n':  
        case 'N':
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;      
            break;
            
        case 'o':    
        case 'O':
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;   
            break;

        case 'e':
        case 'E':
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;

        case 's':
        case 'S':
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
            
        default:
            printf("Error: Unsupported parity!\n");      
            return RET_FAIL;
    }
    
    switch (pcfg->stopbits)  
    {    
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;

        case 2:     
            options.c_cflag |= CSTOPB;
            break;
            
        default:
            printf("Error: Unsupported stop bits!\n");   
            return RET_FAIL;
    }  
     
    options.c_oflag &= ~OPOST;  
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
     
    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 1; 
     
    tcflush(fd, TCIFLUSH);  
     
    if (tcsetattr(fd, TCSANOW, &options) != 0)    
    {
        printf("Error: Failed to configure UART!\n");    
        return RET_FAIL;
    }
      
    return RET_OK;   
}

int UART_Init(int fd, uart_cfg_t *pcfg)
{
    return UART_Set(fd, pcfg);
}

int UART_Recv(int fd, char *rbuf, int rlen, int wait_1s)
{  
    int             len = 0;
    int             pos = 0;
    int             nfds;  
    fd_set          fds_rd;  
    struct timeval  wait, *pwait;  
     
    FD_ZERO(&fds_rd);  
    FD_SET(fd, &fds_rd);  
    
    if (wait_1s < 0)
    {
         pwait = NULL;
    }
    else
    {
        wait.tv_sec = wait_1s;  
        wait.tv_usec = 0;
        pwait = &wait;
    }
    
    while (1)
    {
        FD_ZERO(&fds_rd);  
        FD_SET(fd, &fds_rd);  
         
        nfds = select(fd+1, &fds_rd, NULL, NULL, pwait);  
        if (nfds == -1) 
        {
            break;
        }
        else if (nfds)
        {
            len = read(fd, rbuf + pos, 1);
            if (-1 == len)
            {
                break;
            }
            
            pos ++;
            if (rlen <= pos)
            {
                break;
            }
        }
        else
        {
            break;
        }
    } 

    return pos;
}  

int UART_Send(int fd, char *sbuf, int slen)
{  
    int len = 0;  
     
    len = write(fd, sbuf, slen);
      
    if (len == slen )  
    { 
        return len;  
    }       
    else     
    {  
        tcflush(fd, TCOFLUSH);  
        return RET_FAIL;  
    }  
}  
   
/* EOF */
