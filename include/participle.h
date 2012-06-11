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

#ifndef PARTICIPLE_H_INCLUDED
#define PARTICIPLE_H_INCLUDED

char *matchText(char *inText,int Length);             //将wchar_t型的字符文本切分后，返回相应的拼音。拼音所占内存将在函数中得到分配

#endif // PARTICIPLE_H_INCLUDED
