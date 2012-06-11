//这次自定义的字典实现了排序以及折半查找
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include "../include/defination.h"
#include "../include/opendic.h"

indexes *openDic(void);                       //打开字典

char *findMatch(pychar *str,indexes *currentIndex);      //找到最后匹配字符串的索引

BOOL saveDic(indexes *root);                   //保存结构化后的字典

void saveIndex(indexes *root,FILE *fp);         //保存字典的具体实现

indexes *openMyDic(void);                       //打开自己的字典，测试用

void openIndex(indexes *root,FILE *fp);                    //打开字典的具体实现

int main()
{
    int i = 0;
    indexes *root = NULL;
    indexes *root1 = NULL;
    BOOL saveOK = FALSE;
    pychar *test;
    char *ans;
    char testchar[4][88] = {"解放巴勒斯坦人民阵线","什么","与其杀是僮","大不了"
                            };

    setlocale(LC_ALL, "");

    root = openDic();

    if (root == NULL)
        printf("Open failed!\n");
    else
    {
        saveOK = saveDic(root);

        if (saveOK == FALSE)
            printf("Save failed!\n");
    }

    root1 = openMyDic();

    for (i = 0; i < 4; i++)
    {
        test = changeFormat(testchar[i],strlen(testchar[i]));
        ans = findMatch(test,root1);
        if (ans != NULL)
            printf("%s = %s\n",testchar[i],ans);
        else
            printf("%s Not Found!\n",testchar[i]);
        free(test);
    }
    return 0;
}

indexes *openDic(void)                        //open dic
{
    char *diction = NULL;
    indexes *root = NULL;

    diction = readdic();
    if (diction == NULL)
    {
        printf("No Diction!\n");
        abort();
    }
    else
    {
        root = createIndex(diction);
    }

    free(diction);

    return root;
}


BOOL saveDic(indexes *root)
{
    FILE *fp = NULL;

    fp = fopen("MYDIC.dat","wb");

    if (fp == NULL)
    {
        printf("File does not exist!\n");
        return FALSE;
    }
    else
    {

        fwrite (root,sizeof(indexes),1,fp);
        saveIndex(root,fp);

        fclose(fp);
        return TRUE;
    }

    fclose(fp);

    return FALSE;
}

void saveIndex(indexes *root,FILE *fp)
{
    int i = 0;

    if ((*root).sequanceLength != 0)
    {
        fwrite( (*root).wordSequance,sizeof(wordstruct), (*root).sequanceLength,fp);
        for (i = 0; i < (*root).sequanceLength; i++)
        {
            if ((*root).wordSequance[i].pinyinLength != 0)
                fwrite( (*root).wordSequance[i].pinyin,sizeof(char), (*root).wordSequance[i].pinyinLength,fp);
            if ((*root).wordSequance[i].subIndex.sequanceLength != 0)
                saveIndex(&((*root).wordSequance[i].subIndex),fp);
        }
    }
}

indexes *openMyDic(void)
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

void openIndex(indexes *root,FILE *fp)
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
