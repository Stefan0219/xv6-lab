#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc , char *argv[]){
    if(argc < 2){
        fprintf(2,"Usage: xargs [command]");
        exit(-1);
    }
    char *argument_v[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        argument_v[i-1] = argv[i];
    }
    char p = '0';
    char av[MAXARG][30];
    int row = 0;
    int col = 0;
    while (read(0,&p,1)!=0)
    {
        if (p == ' ' ||  p == '\n'){
            av[row][col] = '\0';
            col = 0;
            row ++;
            continue;
        }
        av[row][col] = p;
        col ++;
    }
    for (int i =0; i < row; i++)
    {
        argument_v[argc -1 + i] = av[i];
    }
    
    int pid = fork();
    if (pid == 0)
    {
        exec(argv[1],argument_v);
    }
    wait(0);
    return 0;
}