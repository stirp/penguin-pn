#ifndef DEFINATION_H_INCLUDED
#define DEFINATION_H_INCLUDED

#include <wchar.h>

#define FALSE 0
#define TRUE (!(FALSE))
#define MAX_LENGTH_OF_CHINESE 11

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
