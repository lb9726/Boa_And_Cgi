#include "TcpClient.h"
#include "cgic.h"
#include<unistd.h>
int cgiMain()
{
    cgiHeaderContentType("text/html");
    char pass[1024];
    getHtmlClientData(stdin, pass);    
    
    fprintf(cgiOut, "%s\n", pass);
    generateBuffer(pass);
    initTcpSocket(pass, strlen(pass));
   
    return 0;
}

