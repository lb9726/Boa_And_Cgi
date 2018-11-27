#include "TcpClient.h"
#include "cgic.h"
int cgiMain()
{
    char pass[1024];
    getHtmlClientData(stdin, pass);
    int len = strlen(pass);
    initTcpSocket(pass, len);
    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "Your String is %s\n", pass);
    return 0;
}

