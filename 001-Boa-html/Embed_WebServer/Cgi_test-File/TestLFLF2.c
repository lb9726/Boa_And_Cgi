#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void getHtmlClientData(char *pStr);
void getHtmlClientData(char *pStr)
{
    char *req_method = NULL;
    FILE *fp = stdin;
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

        //        writeHtmlTextLengthToFile(QString("/home/libo/Desktop/000-libo-len.txt"), len);
        //        writeHtmlTextLengthToFile(QString("/usr/bst/000-libo-len.txt"), len);
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
    //    writeHtmlTextToFile(QString("/home/libo/Desktop/000-libo.txt"), pStr);
    //    writeHtmlTextToFile(QString("/usr/bst/000-libo.txt"), pStr);
}

int cgiMain(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    char pass[1024];
    getHtmlClientData(pass);

    printf("Content-type: text/html\n\n");
    //printf("Your String is wo kao\n");
    printf("Your String is %s\n", pass);
    return 0;
}
