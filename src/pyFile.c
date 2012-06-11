/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Shang Renpeng
 * phone:       15201329144
 * function:    File functions
 * version:     0.2
 * changlog:    2010\12\10 version0.1  Shang Renpeng make functions
 *              2010\12\23 version0.2  Shang Renpeng add functions
 **/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "../include/defination.h"

long filesize(FILE *stream);    //get file size

pychar *changeFormat(char *strFrom, int n)
        //change char string to wchar_t string
{
    strFrom[n] = '\0';
    pychar *strTo = (pychar *)calloc(n,sizeof(pychar));

    if ( strTo == NULL)
    {
        printf("Not enough memory when transform file location！\n");
        abort();
    }

    mbstowcs (strTo,strFrom,n);

    return strTo;
}

char *readfile(char *strFrom,intl n)
        //if faile,return NULL.if OK,return char string
{
    FILE *fp = NULL;
    char *text = NULL;
    intl textLength = 0;

    if ( strFrom == NULL)
        return NULL;
    else
    {
        fp = fopen(strFrom , "r");
        if (fp == NULL)     //file not open
        {
            return NULL;
        }
        else
        {
            text = calloc(filesize(fp) + 1,sizeof(char));   //+1 is for‘\0’
            if (text == NULL)       //fail to calloc memory
            {
                printf("Not enough memory!\n");
                fclose(fp);
                return NULL;
            }
            else        //read file into memory
            {
                textLength = fread(text, sizeof(char), filesize(fp), fp);
                //in case of two English read as a Chinese
                text[textLength] = '\0';
                fclose(fp);
                return text;
            }
        }
    }
    fclose(fp);
    n = n + 1;      //n is not used ,just to avoid Warning
}

BOOL writefile(char *strFrom,intl m, char *strTo, intl n)
//save char string strFrom to file strTo indicates
//if fail return False
{
    FILE *fp = NULL;

    fp = fopen(strTo,"w+");

    if (fp == NULL)
    {
        printf("File does not exist!\n");
        return FALSE;
    }
    else
    {
        fwrite( strFrom,sizeof(char),m,fp);
        fclose(fp);
        return TRUE;
    }

    fclose(fp);

    n = n + 1;      //n is not used ,just to avoid Warning
    return FALSE;
}


intl filesize(FILE *stream)
{
    intl currentPosition,length;

    currentPosition=ftell(stream);
    fseek(stream,0L,SEEK_END);
    length=ftell(stream);
    fseek(stream,currentPosition,SEEK_SET);
    return length;
}
