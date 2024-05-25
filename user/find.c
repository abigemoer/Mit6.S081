#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char* fmtname(char *path);
void find(char *path, char *re);

int 
main(int argc, char** argv){
    if(argc < 2){
      printf("Parameters are not enough\n");
    }
    else{
      //在路径path下递归搜索文件 
      find(argv[1], argv[2]);
    }
    exit(0);
}

// 对ls中的fmtname，去掉了空白字符串
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  // printf("len of p: %d\n", strlen(p));
  if(strlen(p) >= DIRSIZ)
    return p;
  memset(buf, 0, sizeof(buf));
  memmove(buf, p, strlen(p));
  //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void 
find(char *path, char *re){

  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
      fprintf(2, "find: cannot open %s\n", path);
      return;
  }

  if(fstat(fd, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", path);
      close(fd);
      return;
  }
  
  switch(st.type){
  case T_FILE:
      //printf("File re: %s, fmtpath: %s\n", re, fmtname(path));
      if(strcmp(re, fmtname(path))==0)
          printf("%s\n", path);
      break;
          //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);

  case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
          printf("find: path too long\n");
          break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
          if(de.inum == 0)
              continue;
          memmove(p, de.name, DIRSIZ);
          p[DIRSIZ] = 0;
          if(stat(buf, &st) < 0){
              printf("find: cannot stat %s\n", buf);
              continue;
          }

          char* lstname = fmtname(buf);

          if(strcmp(".", lstname) == 0 || strcmp("..", lstname) == 0){
            continue;
          }
          else{
            find(buf, re);
          }
      }
      break;
  }
  close(fd);
}