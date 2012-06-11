#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "../include/defination.h"
#include "../include/mydic.h"

indexes *openMyDic(void)                                        //打开自定义的字典
{
    FILE *fp = NULL;
    indexes *root = NULL;

    fp = fopen("MYDIC.dat","rb");

    root = malloc(sizeof(indexes));

    if (fp == NULL)
    {
        printf("File does not exist!\n");
        fclose(fp);
        abort();
    }
    else
    {

        fread(root,sizeof(indexes),1,fp);
        openIndex(root,fp);

        fclose(fp);

        return root;
    }
    fclose(fp);
    return NULL;
}

void openIndex(indexes *root,FILE *fp)                          //打开自定义字典的递归函数，用来打开下级索引
{
    int i = 0;

    if ((*root).sequanceLength != 0)
    {
        (*root).wordSequance = calloc((*root).sequanceLength , sizeof(wordstruct));
        fread((*root).wordSequance,sizeof(wordstruct), (*root).sequanceLength,fp);
        for (i = 0; i < (*root).sequanceLength; i++)
        {
            if ((*root).wordSequance[i].pinyinLength != 0)
            {
                (*root).wordSequance[i].pinyin = calloc(sizeof(char) , (*root).wordSequance[i].pinyinLength + 1);
                fread((*root).wordSequance[i].pinyin,sizeof(char), (*root).wordSequance[i].pinyinLength,fp);
                (*root).wordSequance[i].pinyin[(*root).wordSequance[i].pinyinLength] = '\0';
            }
            if ((*root).wordSequance[i].subIndex.sequanceLength != 0)
            {
                openIndex(&((*root).wordSequance[i].subIndex),fp);
            }
        }
    }
}

pychar *changeFormat(char * strFrom, int n)                //转换文本格式，从多字符变宽字符，strTo指针为空，要分配。
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


char *findMatch(pychar *str,indexes *currentIndex)                              //找到最后匹配字符串的索引,指向真正的保存地址
{
    char *temp = NULL;
    intl start,i,end;
    BOOL find = FALSE;

    if (wcslen(str) > (*currentIndex).maxLength)
        return NULL;

    start = 0;
    end = (*currentIndex).sequanceLength;
    do
    {
        if (start > end)
            return NULL;
        i = ( start + end) / 2;
        if (str[0] == (*currentIndex).wordSequance[i].word)
        {
            find = TRUE;
            if (str[1] == L'\0')                                                    //如果字串就这么长，那么当前串是以前的某个串的子串。
            {
                return (*currentIndex).wordSequance[i].pinyin;
            }
            else                                                                    //否则在下级索引中继续查找最后一个匹配的字
            {
                temp = findMatch(str + 1,&((*currentIndex).wordSequance[i].subIndex));
                return temp;
            }
        }
        else if (str[0] > (*currentIndex).wordSequance[i].word)
        {
            start = i + 1;
        }
        else if (str[0] < (*currentIndex).wordSequance[i].word)
        {
            end = i - 1;
        }
        else
            return NULL;
    }while(1);
    return NULL;
}
