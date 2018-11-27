#include "getCgiData.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void getHtmlClientData(FILE *fp, char *pStr)
{
    char *req_method = NULL;
    char *input = NULL;
    req_method = getenv("REQUEST_METHOD");
    int i = 0;
    int len = 0;
    if (!strcmp(req_method, "GET"))
    {
        input = getenv("QUERY_STRING");
        memcpy(pStr, input, strlen(input));
    }
    else if (!strcmp(req_method, "POST"))
    {
        len = atoi(getenv("CONTENT_LENGTH"));

        if (0 == len)
        {
            pStr[0] = '\0';
        }
        else
        {
            while(1)
            {
                pStr[i] = (char)fgetc(fp);
                len--;
                if (0 == len)
                {
                    pStr[i+1] = '\0';
                    break;
                }
                ++i;
            }
        }
    }
}
