/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Shang Rnepeng
 * phone:       15201329144
 * function:    Main Function
 * version:     1.0
 * changlog:    2010\12\10 version0.1  Shang Rnepeng make a Non-GUI process
 *              2010\12\23 version0.2  Shang Rnepeng add GUI interface
 * changlog:    2010\12\31 version0.3  Shang Rnepeng add FenCi interface
 *              2011\01\04 version1.0  Shang Rnepeng fix some bugs
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "../include/defination.h"
#include "../include/pyFile.h"
#include "../include/pyGraphic.h"
#include "../include/mydic.h"
#include "../include/participle.h"

indexes *root = NULL;               //because of Interface didn't include this
                                    //root index,so make it a global variary
                                    //to easy use.

int main(int argc,char *argv[])
{
    char *text = NULL;
    char *ans = NULL;
    BOOL saveSuccess = FALSE;

    setlocale(LC_ALL, "");          //This function make it local
                                    //setting as your OS setting

    root = openMyDic();

    if (argc != 3)                  //GUI mode
    {
        tuxing(argc, argv);
    }
    else                            //Non-GUI mode
    {
        text = readfile(argv[1],strlen(argv[1]));
                                    //read a file

        if (text == NULL)
        {
            printf("File didn not read in!\n");
            abort();
        }

        ans = matchText(text,strlen(text));

        saveSuccess = writefile(ans,strlen(ans),argv[2],strlen(argv[2]));
                                    //write answers to a flie as local ANSI
        if (saveSuccess == FALSE)
        {
            printf("Fail to save file!\n");
            abort();
        }
    }

    return 0;
}
