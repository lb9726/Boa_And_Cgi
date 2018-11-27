#include "cgic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern char *cgiQueryString;
int cgiMain()
{
    char name[241];
    char number[241];
    int numb;
    cgiHeaderContentType("Content-type:text/html;charset=utf-8");
    fprintf(cgiOut, "<HTML><HEAD>\n");
    fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n");
    fprintf(cgiOut, "<BODY>");
    //获取数据
    cgiFormString("text", name, 241);
    cgiFormString("number", number, 241);
    cgiFormInteger("number", &numb, 0);
    //打印数据
    fprintf(cgiOut, "<H1>%s</H1>", name);
    fprintf(cgiOut, "<H1>%s</H1>", number);
    fprintf(cgiOut, "number = %d", numb);
    fprintf(cgiOut, "</BODY>\n");
    fprintf(cgiOut, "</HTML>\n");
    return 0;
}
