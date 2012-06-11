//copyrights    :All reserved by Penguin group
//company       :Penguin group
//developer     :lynx
//phone         :15201329144
//function      :assert if it's a char
//version       :0.1
//changlog      :2010\12\10 version0.1

#ifndef ASSERTION_H_INCLUDED
#define ASSERTION_H_INCLUDED


#include <wchar.h>
#include <ctype.h>
#include <string.h>
#include "../include/defination.h"

BOOL isEnglish(pychar ch)
{
    return (iswupper(ch) || iswlower(ch));
}//判断一个字符是否是英文
BOOL isNumber(pychar ch)
{
    return (iswdigit(ch));
}//判断一个字符是否是数字
BOOL isSign(pychar ch)
{
    return (iswpunct(ch));
}//判断一个字符是否是标点符号
BOOL isChinese(pychar ch)
{
    if (iswalpha(ch) && (!iswupper(ch)) && (!iswlower(ch)))    //iswalpha也会检测中文的。他根据local工作
        return TRUE;
    else
        return FALSE;
}//判断一个字符是否是中文
BOOL isOthers(pychar ch)
{
    if ((!isEnglish(ch)) && (!isNumber(ch)) && (!isSign(ch)) && (!isChinese(ch)))
        return TRUE;
    else
        return FALSE;
}//判断一个字符是否是其他

int IsGB(wchar_t *pText)
{
unsigned char sqChar[20];
sqChar[0]=*pText;
if (sqChar[0]>=0xa1)
if (sqChar[0]==0xa3)
return 1;//全角字符
else
return 2;//汉字
else
return 0;//英文、数字、英文标点
}


#endif // ASSERTION_H_INCLUDED
