#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
// recursion helper function used to traverse file system
void helper(char *path, char *filename);
// print str if str contains _find
void strmatch(const char *str, const char *_find);
// return the content after the last '/' in path
char* fmtname(char *path);
int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    fprintf(2, "Usage: find [path] [filename]\n");
    exit(-1);
  }
  else
  {
     helper(argv[1],argv[2]);
  }
  return 0;
}


void helper(char *path, char *_find)
{
  struct stat st;
  int fd;
  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "open error!\n");
    exit(-1);
  }
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch (st.type)
  {
  case T_DIR:
    printf("this is a dir\n");
    //printf("T_DIR\n");
    //printf("%s\n", st.type);
    // TODO helper(,filename);
    break;
  case T_DEVICE:
    break;
  case T_FILE:
    // char *filename[100];
    printf("this is a file\n");
    strmatch(fmtname(path),_find);
    break;
  }
}

void strmatch(const char *str, const char *_find)
{
  int len_str = strlen(str);
  int len_find = strlen(_find);
  int count = 0;
  if (len_str < len_find)
  {
    return;
  }
  for (int i = 0; i < len_str; i++)
  {
    count = 0;
    for (int j = 0; j < len_find; j++)
    {
      if (str[i] == _find[j])
      {
        count++;
        if (count == len_find)
        {
          printf("%s\n", str);
          return;
        }
        i++;
      }
      else
      {
        break;
      }
    }
  }
}


char* fmtname(char *path) 
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash. from the last postion to the front postion
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++; //remove /

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p)); //copy p into buf
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p)); // fill the rest of the buf with ' ' 
  return buf;
}
