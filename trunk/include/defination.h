//copyrights    :All reserved by Penguin group
//company       :Penguin group
//developer     :lynx
//phone         :15201329144
//function      :defination of all files
//version       :0.1
//changlog      :2010\12\10 version0.1

#ifndef DEFINATION_H_INCLUDED
#define DEFINATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <string.h>
#include <ctype.h>

#ifndef FALSE
#define FALSE 0
#define TRUE !(FALSE)
#endif

typedef wchar_t pychar;
typedef long int intl;
typedef int BOOL;

struct wordstruct;
typedef struct wordstruct wordstruct;

struct inde
{
    wordstruct *wordSequance;     //该级别按顺序排列的索引
    intl sequanceLength;          //该级别索引的长度
    unsigned int maxLength;               //该级别索引中存在的最长字串长度，可以快速判断某词是否存在
};

typedef struct inde indexes;

struct wordstruct
{
    pychar word;                    //这个变量保存着当前索引的字
    indexes subIndex;                //如果该文字不是词组的最后一个字，那么索引中的指针不为空
    char *pinyin;                  //如果该文字是该顺序的最后一个字，那么这个指针保存着整个文字的拼音
    int pinyinLength;              //表明拼音文本的长度，读取时方便(每个拼音占用8字节,因此使用时拼音文本的大小是长度*8)
};


#endif // DEFINATION_H_INCLUDED
