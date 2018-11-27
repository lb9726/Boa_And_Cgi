#include "TcpClient.h"
#include "cgic.h"
#include<unistd.h>

int cgiMain()
{
    cgiHeaderContentType("text/html");
    char pass[1024];
    getHtmlClientData(stdin, pass);     /*接收客户端网页发给服务器的数据*/           
    generateBuffer(pass);               /*生成和服务器端通信的字符串，加上帧头帧尾和校验位*/        
    initTcpSocket(pass, strlen(pass));  /*通过socket发送生成的字符串给到设备*/
//    parseRecvBuffer(pass);            /*解析接收到的设备回复的数据，去掉帧头帧尾等操作*/
    fprintf(cgiOut, "%s\n", pass);      /*将生成的数据发给网页端*/
    return 0;
}

