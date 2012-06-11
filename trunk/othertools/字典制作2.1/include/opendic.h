#ifndef OPENDIC_H_INCLUDED
#define OPENDIC_H_INCLUDED

char *readdic(void);           //读入文件。读入失败时返回NULL。成功时返回文本指针。

indexes *createIndex(char *dic);                       //创建一个索引，返回索引的指针

pychar *changeFormat(char * strFrom, intl n);   //转换文本格式，从多字符变宽字符，strTo指针为空，要分配。

#endif // OPENDIC_H_INCLUDED
