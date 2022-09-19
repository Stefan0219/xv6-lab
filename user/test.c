#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv){
    if(argc != 1){
        fprintf(2,"error\n");
    }
    printf("here\n");
    close(1);
    printf("not here\n");
    return 0;
}