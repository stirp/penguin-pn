#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "../include/defination.h"
#include "../include/opendic.h"

static BOOL finishIndex( indexes *root, char *dic);             //完善整个Root index

static long filesize(FILE *stream);                //获得文件大小

static pychar *getChinese(char **dic);              //得到当前词条的中文,同时修改dic指针指向中文及空格后的字符
static char *getpinyin(char **dic);

static indexes *addToLastMatch(pychar *str,indexes *currentIndex,char *pinyin);      //找到最后匹配字符串的索引,并添加拼音

int cmp( const void *a ,const void *b);              //qsort使用的比较函数

static void mySort( indexes *currentIndex);                     //为了封装qsort使用的排序方法

void adjustPinyin(char *pinyin);

char *readdic(void)//读入文件。读入失败时返回NULL。成功时返回文本指针。
{
    FILE *fp = NULL;
    char *text = NULL;
    intl textLength = 0;

    fp = fopen("dicANSI.txt" , "r");
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
    fclose(fp);
}

static intl filesize(FILE *stream)
{
    intl currentPosition,length;

    currentPosition=ftell(stream);
    fseek(stream,0L,SEEK_END);
    length=ftell(stream);
    fseek(stream,currentPosition,SEEK_SET);
    return length;
}

pychar *changeFormat(char * strFrom, intl n)                //转换文本格式，从多字符变宽字符，strTo指针为空，要分配。
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

indexes *createIndex(char *dic)                                //创建一个空的索引，返回索引的指针
{
    indexes *temp = NULL;
    BOOL creatOK = FALSE;

    temp = malloc(sizeof(indexes));
    if (temp == NULL)
        abort();
    else
    {
        (*temp).maxLength = 0;                                  //将该索引置空
        (*temp).sequanceLength = 0;
        (*temp).wordSequance = NULL;

        creatOK = finishIndex( temp,dic);

        mySort(temp);

        if (creatOK == TRUE)
            return temp;
        else
            return NULL;
    }
}

static BOOL finishIndex( indexes *temp, char *dic)              //
{
    indexes *root = temp;
    indexes *currentIndex = root;
    BOOL finish = FALSE;
    pychar *chinese = NULL;
    char *pinyin;
    char *dicCurrent = dic;

    do
    {
        currentIndex = root;
        if (*dicCurrent == '\0')
        {
            finish = TRUE;
            break;
        }

        chinese = getChinese(&dicCurrent);

        while (*dicCurrent++ != ' ')                 //将本行的空格跳过
            ;

        pinyin = getpinyin(&dicCurrent);

        while (*dicCurrent++ != '\n')                 //将本行剩下的字符跳过
            ;
        currentIndex = addToLastMatch(chinese, root, pinyin);

        if (chinese != NULL)
            free(chinese);
        if (pinyin != NULL)
            free(pinyin);

    }while (1);

    return finish;
}

static pychar *getChinese(char **dic)
{
    char str[3 * MAX_LENGTH_OF_CHINESE + 1];            //UTF-8 用3个字节来保存
    char *diction = *dic;
    pychar *ans;
    int i;

    for(i = 0; i < 3 * MAX_LENGTH_OF_CHINESE + 1; i++)
    {
        if (*(diction + i) == ' ')
        {
            str[i] = '\0';
            break;
        }

        str[i] = *(diction + i);
    }

    ans = changeFormat(str,strlen(str));

    *dic = diction + i + 1;

    return ans;
}

static char *getpinyin(char **dic)
{
    char str[8 * (MAX_LENGTH_OF_CHINESE + 1)];
    char *diction = *dic;
    char *ans;
    int i;

    for(i = 0; i < 8 * MAX_LENGTH_OF_CHINESE; i++)
    {
        if ((*(diction + i) == ' ') || (*(diction + i) == '\n'))
        {
            str[i] = '\0';
            break;
        }

        str[i] = *(diction + i);
    }

    *dic = diction + i;

    adjustPinyin(str);

    ans = malloc((strlen(str) + 1) * sizeof(char));

    strcpy(ans,str);

    return ans;
}

static indexes *addToLastMatch(pychar *str,indexes *currentIndex,char *pinyin)      //找到最后匹配字符串的索引
{
    indexes *temp;
    int i;
    BOOL find = FALSE;
                                                                         //索引非空
    for (i = 0; i < (*currentIndex).sequanceLength; i++)
        if (str[0] == (*currentIndex).wordSequance[i].word)
        {
            find = TRUE;
            if (str[1] == L'\0')                                                    //如果字串就这么长，那么当前串是以前的某个串的子串。
            {
                (*currentIndex).wordSequance[i].pinyin = malloc((strlen(pinyin) + 1) * sizeof(char));
                strcpy((*currentIndex).wordSequance[i].pinyin, pinyin);
                (*currentIndex).wordSequance[i].pinyinLength = strlen(pinyin);
                return currentIndex;
            }
            else                                                                    //否则在下级索引中继续查找最后一个匹配的字
            {
                temp = addToLastMatch(str + 1,&((*currentIndex).wordSequance[i].subIndex),pinyin);
                if (wcslen(str) > (*currentIndex).maxLength)                           //如果添加的下级字串更长，那么就要修改
                    (*currentIndex).maxLength = wcslen(str);
                return temp;
            }
        }
    if (find == FALSE)                                                              //如果没有找到匹配字，说明当前位置就是目前匹配的最后一个了
        {
            int temp1 = (*currentIndex).sequanceLength + 1;
            (*currentIndex).wordSequance = realloc((*currentIndex).wordSequance,sizeof(wordstruct) * temp1);
            (*currentIndex).sequanceLength += 1;
            (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].word = str[0];
            (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].pinyinLength = 0;
            (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].pinyin = NULL;
            (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].subIndex.maxLength = 0;
            (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].subIndex.sequanceLength = 0;
            (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].subIndex.wordSequance = NULL;
            if (wcslen(str) > (*currentIndex).maxLength)
                (*currentIndex).maxLength = wcslen(str);
            if (str[1] == L'\0')
            {
                (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].pinyin = malloc((strlen(pinyin) + 1) * sizeof(char));
                strcpy((*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].pinyin, pinyin);
                (*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].pinyinLength = strlen(pinyin);
                return currentIndex;
            }
            else
            {
                temp = addToLastMatch(str + 1,&((*currentIndex).wordSequance[(*currentIndex).sequanceLength - 1].subIndex),pinyin);
                if (wcslen(str) > (*currentIndex).maxLength)                           //如果添加的下级字串更长，那么就要修改
                    (*currentIndex).maxLength = wcslen(str);
                return temp;
            }
        }

    return NULL;
}

void mySort( indexes *currentIndex)                     //为了封装qsort使用的排序方法
{
    int i = 0;

    qsort((*currentIndex).wordSequance,(*currentIndex).sequanceLength,sizeof(wordstruct),cmp);

    for( i = 0; i < (*currentIndex).sequanceLength; i++)
        if ((*currentIndex).wordSequance[i].subIndex.sequanceLength > 1)
            mySort(&(*currentIndex).wordSequance[i].subIndex);
}

int cmp( const void *a ,const void *b)              //qsort使用的比较函数
{
return (*(wordstruct *)a).word > (*(wordstruct *)b).word ? 1 : -1;
}

void adjustPinyin(char *pinyin)
{
    intl i = 0,lastTemp = 0,lastPinyin = 0 , j = 0;
    char *temp = calloc(sizeof(char),strlen(pinyin) + 1);

    strcpy(temp,pinyin);

    for(i = 0; i < (intl)strlen(temp); i++)
    {
        if((temp[i] == '\'')||(temp[i] == '\0'))
        {
            strncpy(pinyin + lastPinyin,temp + lastTemp, i - lastTemp);
            for ( j = 0; j < 8 - (i - lastTemp); j++)
                strcpy(pinyin + lastPinyin + i - lastTemp + j," ");
            lastPinyin += 8;
            lastTemp =  i + 1;
        }
    }
    strncpy(pinyin + lastPinyin,temp + lastTemp, i - lastTemp);
    for ( j = 0; j < 8 - (i - lastTemp); j++)
        strcpy(pinyin + lastPinyin + i - lastTemp + j," ");
    lastPinyin += 8;

    pinyin[lastPinyin + 1] = '\0';
    free(temp);
}
