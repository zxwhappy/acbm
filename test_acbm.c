#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "acsmx.h"


int MatchFound(void *id, int index, void *data){
    printf("id:%s\n", (char *) id);
    printf("index:%d\n",index);
    return 0;
}

int main(int argc, char **argv)
{
    int start_state = 0;
    ACSM_STRUCT * acsm;
    int i, nocase = 0;
    char text[50];

    if (argc < 3){
        printf("Usage: acsm ToBeSearchedPattern keyword-1 keyword-2 ... keyword-n -nocase\n");
        exit(0);
    }
    
    acsm = acsmNew();
    strcpy(text, argv[1]);
    for (i = 1; i < argc; i++){
        if (strcmp (argv[i], "-nocase") == 0){
            nocase = 1;
        }
    }
    for (i = 2; i < argc; i++){
        if (argv[i][0] == '-')
            continue;
        //acsmAddPattern函数中倒数第二个参数就是 MatchFound 函数中的 id 参数，所以 MatchFound 中 id 的实际数据类型就是 acsmAddPattern 的倒数第二个参数的实际数据类型
        acsmAddPattern (acsm, argv[i], strlen(argv[i]), nocase, 0, 0, argv[i], i - 2);
    }
    acsmCompile (acsm);
    //acsmSearch函数的倒数第二个参数就是 MatchFound 函数中的 data 参数，所以 MatchFound 中 data 的实际数据类型就是 acsmSearch 的倒数第二个参数的实际数据类型
    acsmSearch(acsm, text, strlen(text), MatchFound, (void *) 0, &start_state);
    printf("start_state:%d\n",start_state);
    acsmFree(acsm);
    return 0;
}
