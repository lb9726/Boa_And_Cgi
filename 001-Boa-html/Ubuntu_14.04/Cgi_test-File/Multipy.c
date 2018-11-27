#include<stdio.h>
#include<stdlib.h>
#include "cgic.h"

int cgiMain(void)
{
    char *data;
    long m, n;
    cgiHeaderContentType("Content-type:text/html;charset=utf-8");
    fprintf(cgiOut, "<html>\n");
    fprintf(cgiOut, "<head>\n<title>³Ë·¨½á¹û</title>\n</head>\n");
    fprintf(cgiOut, "<body>\n");
    data = getenv("QUERY_STRING");
    if (!data)
    {
        fprintf(cgiOut, "<P>get no datas and it's wrong.</P>\n");
    }
    else if (sscanf(data,"m=%ld&n=%ld", &m, &n) != 2)
    {
        fprintf(cgiOut, "<P>the input must be numbers</P>\n");
    }
    else
    {
        fprintf(cgiOut, "<P>%ld and %ld multi= %ld</P>\n", m, n, m*n);
    }
    fprintf(cgiOut, "</body>\n");
    fprintf(cgiOut, "</html>\n"); 
    return 0;
}
