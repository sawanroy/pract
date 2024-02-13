#include<stdio.h>

int main(){

    int fd[2],id;

    pipe(fd);
    id = fork();

    if(id == 0)
    {
        printf("id = %d",id);
        printf("child process\n");
    }
    else
    {
        printf("id = %d",id);
        printf("parent process\n");
    }

    return 0;
}