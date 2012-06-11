#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include "../include/defination.h"
#include "../include/mydic.h"
#include "../include/assertion.h"

#define MAX_CHINESE_VOCABULARY_LENGTH 12
#define MAX_CHINESE_TOTAL_LENGTH 1000000
#define MAX_CHINESE_PARAGRAPH_LENGTH 10000

extern indexes *root;

void sliceParagraph(pychar *inText, char *outChar, int pyLength);         //对一个段落处理,结果中已经包含排序好的中文.需要人为对拼音和汉字加上'\n'

void normalSlice(pychar *inText, char *outChar,int pyLength, int *words);//正向注音,函数调用后需要释放结果

void reverseSlice(pychar *inText, char *outChar, int pyLength, int *words);//反向注音,函数调用后需要释放结果

FILE *fp;

char *matchText(char *inText,int Length)             //将wchar_t型的字符文本切分后，返回相应的拼音。拼音所占内存将在函数中得到分配
{
    char *ans = NULL;
    pychar *text = NULL;
    int pyLength = 0;                                  //wcs文本的长度
    int currentPosition = 0;
    int slicLength = 0;                                //找到一个段落的长度
    char temp[8 * MAX_CHINESE_TOTAL_LENGTH];

    fp = fopen("save.txt","w+");

    temp[0] = '\0';
    text = changeFormat(inText,Length);

    pyLength = wcslen(text);
    ans = calloc(sizeof(char), 8 * pyLength * 4 + 1);   //8是为了拼音对齐使用的。+1为了保存'\0'.乘4是为了保存拼音和汉字

    for (currentPosition = 0; currentPosition < pyLength; currentPosition += slicLength + 1)//slicLength是指向当前切分的最后一词，而非下一个段的开始，所以要加一
    {//先分段，然后分句，最后分词
        for(slicLength = 0;currentPosition + slicLength < pyLength + 1;slicLength++)
        {
            if ((text[currentPosition + slicLength] == L'\n') || ((text[currentPosition + slicLength] == L'\0')))
            {
                sliceParagraph(text + currentPosition,temp,slicLength);               //此时slicLength指向的是非中文，既不用处理
                strcpy(ans,temp);
                break;
            }
        }
    }
    fclose(fp);
    return ans;
}

void sliceParagraph(pychar *inText, char *outChar, int pyLength)         //对一个段落处理,结果中已经包含排序好的中文.需要人为对拼音和汉字加上'\n'
{
    int currentPosition = 0;
    int slicLength = 0, normalSliceWords, reverseSliceWords;
    char ans[8 * 2 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];                             //一段话最多10000汉字
    char pinyinNormal[8 * 2 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];
    char pinyinReverse[8 * 2 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];
    char temp[8 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];                             //临时的汉字转回的全部文本
    char temppinyin[4];                                      //保存转回的非汉字字符
    char temphanzi[4];                                       //保存转回的汉字字段，否则会把上边的字符搞乱

    strcpy (ans,"");
    strcpy (temp,"");
    for (currentPosition = 0;currentPosition < pyLength; currentPosition += slicLength)
    {
        if (isChinese(inText[currentPosition]))
        {
            strcat(ans," ");
            strcat(temp," ");
            for(slicLength = 0; currentPosition + slicLength < pyLength;slicLength++)
            {
                if (!isChinese(inText[currentPosition + slicLength]))
                {
                    break;
                }
                else
                {
                    temphanzi[0] = '\0';temphanzi[1] = '\0';temphanzi[2] = '\0';temphanzi[3] = '\0';
                    wctomb(temphanzi,inText[currentPosition + slicLength]);
                    strcat(temp," ");
                    strcat(temp,temphanzi);
                    strcat(temp,"     ");
                    continue;
                }
            }
            normalSliceWords = 0;
            reverseSliceWords = 0;
            pinyinNormal[0] = '\0';
            pinyinReverse[0] = '\0';
            normalSlice(inText + currentPosition, pinyinNormal,slicLength,&normalSliceWords);
            reverseSlice(inText + currentPosition, pinyinReverse, slicLength,&reverseSliceWords);
            if (normalSliceWords < reverseSliceWords)
                strcat(ans,pinyinNormal);
            else
                strcat(ans,pinyinReverse);
            strcat(ans," ");
            strcat(temp," ");
        }
        else
        {
            slicLength = 1;
            temppinyin[0] = '\0';temppinyin[1] = '\0';temppinyin[2] = '\0';temppinyin[3] = '\0';
            wctomb(temppinyin,inText[currentPosition]);
            strcat(ans,temppinyin);
            strcat(temp,temppinyin);
        }
    }
    strcat(ans,"\n");
    strcat(ans,temp);
    strcat(ans,"\n");
    strcat(outChar,ans);
}

void normalSlice(pychar *inText, char *outChar,int pyLength, int *words)//正向注音,函数调用后需要释放结果
//inText是输入要分词注音的文本（只含中文），words返回切分后共多少个词，pylength是要注音的文本的长度，inTest[pyLength-1]是最后一个字，而非字符串结束符
{
    char *pinyintemp = NULL;                        //指向的是结构中的拼音，因此不需free
    pychar find[pyLength + 1];
    int i;
    char temphanzi[4];

    for(i = pyLength;i > 0;i--)                 //i是长度，指向最后一个字之后的字
    {
        wcsncpy (find,inText,i);
        find[i] = L'\0';
        pinyintemp = findMatch(find,root);
        if (pinyintemp == NULL)
        {
            continue;
        }
        else
        {
            (*words)++;
            strcat(outChar,pinyintemp);
            if (pyLength - i != 0)
            {
                normalSlice(inText + i,outChar,pyLength - i,words);
            }
            break;
        }
    }

    if (pinyintemp == NULL)
    {
        strcpy(outChar,"        ");
        temphanzi[0] = '\0';temphanzi[1] = '\0';temphanzi[2] = '\0';temphanzi[3] = '\0';
        wctomb(temphanzi,inText[pyLength - 1]);
        fputs( temphanzi,fp);
    }
}

void reverseSlice(pychar *inText, char *outChar, int pyLength, int *words)//反向注音,函数调用后需要释放结果
//inText是输入要分词注音的文本（只含中文），words返回切分后共多少个词，pylength是要注音的文本的长度，inTest[pyLength-1]是最后一个字，而非字符串结束符
{
    char *pinyintemp = NULL;                        //指向的是结构中的拼音，因此不需free
    pychar find[pyLength + 1];
    int i;
    char temphanzi[4];

    for(i = 0;i < pyLength;i++)                 //i是长度，指向当前开始的字
    {
        wcsncpy (find,inText + i,pyLength - i);
        find[pyLength - i] = L'\0';
        pinyintemp = findMatch(find,root);
        if (pinyintemp == NULL)
        {
            continue;
        }
        else
        {
            (*words) ++;
            if (i != 0)
            {
                reverseSlice(inText,outChar,i,words);
            }
            strcat(outChar,pinyintemp);
            break;
        }
    }

    if (pinyintemp == NULL)
    {
        strcpy(outChar,"        ");
        temphanzi[0] = '\0';temphanzi[1] = '\0';temphanzi[2] = '\0';temphanzi[3] = '\0';
        wctomb(temphanzi,inText[pyLength - 1]);
        fputs( temphanzi,fp);	//输出没有的字
    }
}
