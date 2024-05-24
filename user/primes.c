#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int *fd){
    int first_num;
    if(read(fd[0],&first_num,sizeof(int))==0){
        exit(0);
    }
    int subfd[2];
    pipe(subfd);
    printf("prime %d\n",first_num);
    if(fork()==0){
        close(subfd[1]);
        prime(subfd);
        close(subfd[0]);
    }
    else{
        close(subfd[0]);
        int a;
        while(read(fd[0],&a,sizeof(int))!=0){
            if(a%first_num!=0){
                write(subfd[1],&a,sizeof(int));
            }
        }
        close(subfd[1]);
        wait(0);
    }
    exit(0);
}

int main(){
    int fd[2];
    pipe(fd);
    if(fork()==0){
        close(fd[1]);
        prime(fd);
        close(fd[0]);
    }
    else{
        close(fd[0]);
        for(int i=2;i<=35;i++){
            write(fd[1],&i,sizeof(int));
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);

}