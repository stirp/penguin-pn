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

#include "defination.h"

#ifndef PYFILE_H_INCLUDED
#define PYFILE_H_INCLUDED

pychar *changeFormat(char * strFrom, int n);                //转换文本格式，从多字符变宽字符，strTo指针为空，要分配。

char *readfile(char *strFrom,intl n);//读入文件。读入失败时返回NULL。成功时返回文本指针。

BOOL writefile(char *strFrom,intl m, char *strTo,intl n);//将strFrom指向的长度为n的文本保存到strTo（长度m）指向的文本文件中去。失败时返回FALSE,否则返回TRUE。

#endif // PYFILE_H_INCLUDED
