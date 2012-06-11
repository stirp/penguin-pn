/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Wang Changhai
 * phone:       15201329144
 * function:    reference of Trie tree
 * version:     0.1
 * changlog:    2011\01\02 Wang Changhai version0.1
 **/

#ifndef OPENDIC_H_INCLUDED
#define OPENDIC_H_INCLUDED

indexes *openMyDic(void);                                        //打开自定义的字典

char *findMatch(pychar *str,indexes *currentIndex);                              //找到最后匹配字符串的索引,指向真正的保存地址

#endif // OPENDIC_H_INCLUDED
