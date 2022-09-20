#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include<stdbool.h>
void helper(int fd);
int main(int argc,char*argv){
    int p0[2];
    pipe(p0);
    int pid = fork();
    if(pid > 0){
        for (int i = 2; i <= 35; i++){
            write(p0[1],&i,4);
        }
        close(p0[0]);
        close(p0[1]);
        wait(0);    
    }else if(pid == 0){
        close(p0[1]);
        helper(p0[0]);
    }else if (pid<0)
    {
        fprintf(2,"fork error!\n");
    }
    
    return 0;
}
void helper(int fd){
    int num;//get from parent
    int passnum;
    bool needfork = true; //have no child for now
    int pid;
    read(fd,&num,4);
    printf("prime %d\n",num); 
    if(num == 31){
        close(fd);
        exit(0);
    }
    int p[2];
    while (read(fd,&passnum,4)!=0)
    {
        if (needfork)
        {
            pipe(p);
            needfork = false;
            pid = fork();
            if (pid == 0)
            {
                close(p[1]);
                helper(p[0]);
            }else{
                close(p[0]);
            }
        }
        if(passnum%num!=0){
            write(p[1],&passnum,4);
        }
    }
    close(p[1]);
    wait(0);
    exit(0);
}