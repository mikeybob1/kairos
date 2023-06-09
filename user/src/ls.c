#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

// #pragma GCC optimize ("O0")

#define ITEM_PER_LINE 5

void
ls(char *path)
{

  char buf[512];
  int fd;
  struct linux_dirent64 *de = (struct linux_dirent64 *)buf;

  if((fd = open(path, 0)) < 0){
    printf("ls: cannot open %s\n", path);
    return;
  }
  int nread;
  int nr = 1;
  while(1) {
    if((nread = getdents(fd, (struct linux_dirent64 *)buf, 512)) == -1) {
      printf("ls: getdentes fail\n");
      break;
    }

    if(nread == 0) break;

    int pos = 0;

    while(pos < nread) {
      de = (struct linux_dirent64 *)&buf[pos];
      char *name = de->d_name;
      if (de->d_type == DIR) {
        printf(bl("%-17s"), name);
      } else {
        printf("%-17s", name);
      }
      

      if (nr % ITEM_PER_LINE == 0) {
        printf("\n");
      }
      pos += de->d_reclen;
      nr++;
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  return 0;
}
