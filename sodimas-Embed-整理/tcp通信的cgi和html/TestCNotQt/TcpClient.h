#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

extern int initTcpSocket(char *buf, int len);
extern void generateBuffer(char *buf);
#endif
