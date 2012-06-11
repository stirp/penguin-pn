/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Shang Rnepeng
 * phone:       15201329144
 * function:    assert if it's a char
 * version:     0.1
 * changlog:    2010\12\10 Shang Rnepeng version0.1
 **/

#include <wchar.h>
#include <ctype.h>
#include "../include/defination.h"

BOOL isChinese(pychar ch)       //assertion whether ch is a Chinese caracter
{
    if (iswalpha(ch) && (!iswupper(ch)) && (!iswlower(ch)) && (!iswpunct(ch)))
                                //iswalpha can detect Chinese.It work with your
                                //setlocale function
        return TRUE;
    else
        return FALSE;
}
