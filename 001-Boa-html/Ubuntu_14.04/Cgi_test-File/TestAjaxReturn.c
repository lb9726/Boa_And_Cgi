#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgic.h"
extern char *cgiQueryString;
int cgiMain()
{
    int m = 55;
    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "%d", m);
    m = 14;
    fprintf(cgiOut, "%d", m);
    return 0;
}
