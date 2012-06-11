/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Wang Changhai
 * phone:       15201329144
 * function:    reference of Trie tree
 * version:     0.1
 * changlog:    2011\01\02 Wang Changhai version0.1
 **/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "../include/defination.h"

static void openIndex(indexes *root,FILE *fp);

char *findMatch(pychar *str,indexes *currentIndex);

indexes *openMyDic(void)
                    //open my customed diction"MYDIC.dat"
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

static void openIndex(indexes *root,FILE *fp)
                    //a recursion function to open root in "MYDIC.dat"
{
    int i = 0;

    if ((*root).sequanceLength != 0)    //means it contain at least a Chinese
                                        //word
    {
        (*root).wordSequance = calloc((*root).sequanceLength ,  \
                                      sizeof(wordstruct));
        fread((*root).wordSequance,sizeof(wordstruct),  \
              (*root).sequanceLength,fp);
        for (i = 0; i < (*root).sequanceLength; i++)
        {
            if ((*root).wordSequance[i].pinyinLength != 0)
                    //find and read PinYin
            {
                (*root).wordSequance[i].pinyin = calloc(sizeof(char) ,  \
                                (*root).wordSequance[i].pinyinLength + 1);
                fread((*root).wordSequance[i].pinyin,sizeof(char),  \
                      (*root).wordSequance[i].pinyinLength,fp);
                (*root).wordSequance[i].pinyin[(*root)  \
                .wordSequance[i].pinyinLength] = '\0';
            }
            if ((*root).wordSequance[i].subIndex.sequanceLength != 0)
                    //find and read subIndex
            {
                openIndex(&((*root).wordSequance[i].subIndex),fp);
            }
        }
    }
}

char *findMatch(pychar *str,indexes *currentIndex)
                    //find str,and return PinYin pointer, indicating address
{
    char *temp = NULL;
    intl start,i,end;
    BOOL find = FALSE;

    if (wcslen(str) > (*currentIndex).maxLength)    //input too long,Not find
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
            if (str[1] == L'\0')
            {
                return (*currentIndex).wordSequance[i].pinyin;
            }
            else
            {
                temp = findMatch(str + 1,       \
                                 &((*currentIndex).wordSequance[i].subIndex));
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
    }
    while(1);
    return NULL;
}
