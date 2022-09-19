#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char*argv){
    if (argc!=1)
    {
        fprintf(2,"Usage: pingpong\n");
        exit(1);
    }
    int p[2];
    int p1[2];
    pipe(p);
    pipe(p1);
    char buf = '1';
    int pid = fork();
    int this_pid = getpid();
    if (pid>0){//parent
        write(p[1],&buf,1);
        wait(0);
        read(p1[0],&buf,1);
        printf("%d: received pong\n",this_pid);    
    }else if(pid==0){//child
        read(p[0],&buf,1);
        printf("%d: received ping\n",this_pid);
        write(p1[1],&buf,1);
        exit(0); 
    }
    return 0;
}