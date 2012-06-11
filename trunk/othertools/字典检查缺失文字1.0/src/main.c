#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include "../include/defination.h"
#include "../include/mydic.h"

indexes *root = NULL;

char *readfile(char *strFrom,intl n);//读入文件。读入失败时返回NULL。成功时返回文本指针。
intl filesize(FILE *stream);          //获得文件的长度

int main()
{
    char *pinyin = NULL;
    char *test = NULL;

    setlocale(LC_ALL, "");

    root = openMyDic();

    test = readfile("test.txt",strlen("test.txt"));

    if (strlen(test) > 4000000)
        printf("超过2000000字数！");
    else
    {
        pinyin = matchText(test,strlen(test));

//        printf("%s\n",pinyin);
    }
    free(test);
    free(pinyin);
    return 0;
}

char *readfile(char *strFrom,intl n)//读入文件。读入失败时返回NULL。成功时返回文本指针。
{
    FILE *fp = NULL;
    char *text = NULL;
    intl textLength = 0;

    if ( strFrom == NULL)
        return NULL;
    else
    {
        fp = fopen(strFrom , "r");
        if (fp == NULL)                             //文件没有打开
        {
            return NULL;
        }
        else                                       //打开了文件
        {
            text = calloc(filesize(fp) + 1,sizeof(char));           //+1是为了放置‘\0’
            if (text == NULL)                      //内存分配失败
            {
                printf("Not enough memory!\n");
                fclose(fp);
                return NULL;
            }
            else                                   //读入文件到内存
            {
                textLength = fread(text, sizeof(char), filesize(fp), fp);//直接用pychar类型读入时会把两个英文合并成一个字符，所以要用mbs读入，再转换wchar_t
                text[textLength] = '\0';
                fclose(fp);
                return text;
            }
        }
    }
    fclose(fp);
    n = n + 1;                  //避免n不被使用而提出warning
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
