#ifndef OPENDIC_H_INCLUDED
#define OPENDIC_H_INCLUDED

indexes *openMyDic(void);                                        //打开自定义的字典

void openIndex(indexes *root,FILE *fp);                          //打开自定义字典的递归函数，用来打开下级索引

pychar *changeFormat(char * strFrom, int n);                //转换文本格式，从多字符变宽字符，strTo指针为空，要分配。

char *findMatch(pychar *str,indexes *currentIndex);                              //找到最后匹配字符串的索引,指向真正的保存地址

#endif // OPENDIC_H_INCLUDED
