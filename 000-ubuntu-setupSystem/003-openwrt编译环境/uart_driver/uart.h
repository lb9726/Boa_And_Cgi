#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int     speed;
    int     flow_ctrl;
    int     databits;
    int     stopbits;
    int     parity;
} uart_cfg_t;

int  UART_Open(char* ttyname);
void UART_Close(int fd);
int  UART_Set(int fd, uart_cfg_t *pcfg);
int  UART_Init(int fd, uart_cfg_t *pcfg);
int  UART_Recv(int fd, char *rbuf, int rlen, int wait_1s);
int  UART_Send(int fd, char *sbuf, int slen);


#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */
