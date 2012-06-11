/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Wang Dong/Shang Renpeng
 * phone:       15201329144
 * function:    participation
 * version:     1.0
 * changlog:    2010\12\10 version0.1  Wang Dong make a basic function
 *              2011\01\04 version1.0  Shang Rnepeng fix some bugs
 **/

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

static void sliceParagraph(pychar *inText, char *outChar, int pyLength);
//deal with a paragraph,also put Chinese into outChar.
//Need '\n' added in end of outChar personality.

static void normalSlice(pychar *inText, char *outChar,int pyLength, int *words);
//Normal Slice sentence,inText should't contain non-Chinese.
//words is a return to show how many vocabulary there is.

static void reverseSlice(pychar *inText, char *outChar, int pyLength, int *words);
//Reverse Slice sentence,inText should't contain non-Chinese.
//words is a return to show how many vocabulary there is.

char *matchText(char *inText,int Length)
//make every chinese sliced and pind there PinYin correctly.
//return space should allocate in this function.
{
    char *ans = NULL;
    pychar *text = NULL;
    int pyLength = 0;    //length of a wchar_t type string
    int currentPosition = 0;
    int slicLength = 0;  //length of a paragraph
    char temp[8 * MAX_CHINESE_TOTAL_LENGTH];

    temp[0] = '\0';
    text = changeFormat(inText,Length);

    pyLength = wcslen(text);
    ans = calloc(sizeof(char), 8 * pyLength * 4 + 1);
    //8 is for align Chinese and PinYin。+1 is for '\0'.
    //4 is for Chinese and PinYin all to save into this string.

    for (currentPosition = 0; currentPosition < pyLength; currentPosition += slicLength + 1)// TODO (lynx#1#): cut
    //slicLength is pointing to the last word,not '\0'
    {
        for(slicLength = 0; currentPosition + slicLength < pyLength + 1; slicLength++)
        {
            if ((text[currentPosition + slicLength] == L'\n') || ((text[currentPosition + slicLength] == L'\0')))
            {
                sliceParagraph(text + currentPosition,temp,slicLength);
                //Sliclength is pointing non-chinese
                strcpy(ans,temp);
                break;
            }
        }
    }

    return ans;
}

static void sliceParagraph(pychar *inText, char *outChar, int pyLength)
    //deal with a whole paragraph with out '\n',has to add two '\n'
{
    int currentPosition = 0;
    int slicLength = 0, normalSliceWords, reverseSliceWords;
    char ans[8 * 2 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];
    char pinyinNormal[8 * 2 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];
    char pinyinReverse[8 * 2 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];
    char temp[8 * MAX_CHINESE_PARAGRAPH_LENGTH + 1];    //temprary saved texts
    char temppinyin[4]; //save non-Chinese
    char temphanzi[4];
    //saved Chinese,if is saved above, non-chinese can never proper saved

    strcpy (ans,"");
    strcpy (temp,"");
    for (currentPosition = 0; currentPosition < pyLength; currentPosition += slicLength)
    {
        if (isChinese(inText[currentPosition]))
        {
            strcat(ans," ");
            strcat(temp," ");
            for(slicLength = 0; currentPosition + slicLength < pyLength; slicLength++)
            {
                if (!isChinese(inText[currentPosition + slicLength]))
                {
                    break;
                }
                else
                {
                    temphanzi[0] = '\0';
                    temphanzi[1] = '\0';
                    temphanzi[2] = '\0';
                    temphanzi[3] = '\0';
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
            temppinyin[0] = '\0';
            temppinyin[1] = '\0';
            temppinyin[2] = '\0';
            temppinyin[3] = '\0';
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

static void normalSlice(pychar *inText, char *outChar,int pyLength, int *words)
//Normal slice ,inText is input, outchar is output, words output how many words
{
    char *pinyintemp = NULL;        //need not free
    pychar find[pyLength + 1];
    int i;

    for(i = pyLength; i > 0; i--)   //i point at the word behind last word
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
        strcpy(outChar,"        ");
}

static void reverseSlice(pychar *inText, char *outChar, int pyLength, int *words)
//Reverse slice,inText is input, outchar is output, words output how many words
{
    char *pinyintemp = NULL;        //need not free
    pychar find[pyLength + 1];
    int i;

    for(i = 0; i < pyLength; i++)   //i point at current beginning word
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
        strcpy(outChar,"        ");
}
