#include<stdio.h>
#include<fcntl.h>
int main ()
{   
    int servfd = open("servfifo",O_WRONLY);
    char buf[10];
    if(servfd<0)
    {
        printf("run the server frist frist");
        // if(mkfifo("servfifo",0640)<0)
        // {
        //     printf("mkfifo error");
        // }
        // else
        // {
        //     servfd = open("servfifo",O_WRONLY);
        // }
    }
    printf("Enter the buf name");
    scanf("%s",buf);
    write(servfd,buf,10);
    
   // printf("%s",buf);

}