#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc<2){
        printf("There is no paremater!!\n");
        exit(1);
    }
    int time=atoi(argv[1]);

    int re=sleep(time);
    if(re==-1){
        printf("sleep wrong\n");
        exit(1);
    }
    exit(0);

}