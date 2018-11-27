#include "TcpClient.h"
#include "cgic.h"
#include<unistd.h>
int cgiMain()
{
    char pass[1024];
    getHtmlClientData(stdin, pass);
    
    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "Your String is %s\n", pass);
    generateBuffer(pass);
    initTcpSocket(pass, strlen(pass));
   
    return 0;
}

