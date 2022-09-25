#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include<stdbool.h>
// recursion helper function used to traverse file system
void helper(char *path, char *filename);
// print str if str contains _find
void strmatch(const char *str, const char *_find);
// return the content after the last '/' in path
char *fmtname(char *path);
int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(2, "Usage: find [path] [filename]\n");
		exit(-1);
	}
	else
	{
		helper(argv[1], argv[2]);
	}
	return 0;
}

void helper(char *path, char *_find)
{
	struct stat st;
	struct dirent dir_msg;
	int fd;
	char buf[512];
	char *ptr;
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
		if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
		{
			printf("path too long\n");
			break;
		}
		strcpy(buf, path); // convert path into array
		ptr = buf + strlen(path);
		*ptr++ = '/';
		while (read(fd, &dir_msg, sizeof(dir_msg)) == sizeof(dir_msg))
		{
			if (dir_msg.inum == 0)
			{
				continue; // Dir is empty
			}
			memmove(ptr, dir_msg.name, DIRSIZ);
			ptr[DIRSIZ] = 0;
			if (stat(buf, &st) < 0) // get metadata under this dir
			{
				printf("cannot stat\n");
				continue;
			}
			switch (st.type)
			{
			case T_DIR:
				bool a = !strcmp(ptr,".");
				bool b = !strcmp(ptr,"..");
				if(a||b){
					break;
				}
				helper(buf, _find);
				break;
			case T_DEVICE:
			case T_FILE:
				strmatch(buf, _find);
				break;
			default:
				break;
			}
			//printf("%s\n", ptr);
		}
		break;
	case T_DEVICE:
	case T_FILE:
		printf("this is a file\n");
		strmatch(fmtname(path), _find);
		return;
	default:
		fprintf(2, "error!\n");
		exit(-1);
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

char *fmtname(char *path)
{
	static char buf[DIRSIZ + 1];
	char *p;

	// Find first character after last slash. from the last postion to the front postion
	for (p = path + strlen(path); p >= path && *p != '/'; p--)
		;
	p++; // remove /

	// Return blank-padded name.
	if (strlen(p) >= DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));						  // copy p into buf
	memset(buf + strlen(p), ' ', DIRSIZ - strlen(p)); // fill the rest of the buf with ' '
	return buf;
}
