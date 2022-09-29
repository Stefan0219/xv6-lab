#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    char a[2][2];
    a[0][0] = '1';
    a[0][1] = '\0';
    a[1][0] = '2';
    a[1][1] = '\0';
    char * A[1];
    A[0] = a[0];
    printf("%s\n",A[0]);
    return 0;
} 