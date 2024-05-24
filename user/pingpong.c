#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char *argv[]){
    int fd[2];
    if(pipe(fd)==-1){
        printf("Error: pipe error!\n");
    }
    if(fork()==0){
        char buffer[1];
        read(fd[0],buffer,1);
        printf("%d: received ping\n",getpid());
        write(fd[1],buffer,1);
        exit(0);
    }
    else{
        char buffer[1];
        buffer[0]='a';
        write(fd[1],buffer,1);
        wait(0);
        read(fd[0],buffer,1);
        printf("%d: received pong\n",getpid());
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }

}