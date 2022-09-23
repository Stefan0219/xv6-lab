#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stdbool.h>
void helper(int inStream);
int main(int argc, char *argv) {
  int p[2];
  pipe(p);
  int pid = fork();
  if (pid > 0) { // parent
    close(p[0]);
    for (int i = 2; i < 35; i++) {
      write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
  } else if (pid == 0) {
    close(p[1]);
    helper(p[0]);
  }
  //wait(0);
  return 0;
}
void helper(int inStream) {
  bool needfork = true;
  int a;
  int p[2];
  int pid;
  read(inStream, &a, 4);
  printf("primes: %d\n", a);
  if(a==31){
    close(inStream);
    exit(0);
  }
  int b;
  while (read(inStream, &b, 4) != 0) {
    if (needfork) {
      pipe(p);
      needfork = false;
      pid = fork();
      if (pid == 0) {
        close(p[1]);
        helper(p[0]);
      }else{
        close(p[0]);
      }
    }
    if (b%a != 0) {
      write(p[1], &b, 4);
    }
  }
  close(p[1]);
  wait(0);
  exit(0);
}
