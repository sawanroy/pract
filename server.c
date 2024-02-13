#include<stdio.h>
#include<fcntl.h> 
int main ()
{   
    int servfd = open("servfifo",O_RDONLY);
    char buf[10];
    if(servfd<0)
    {
        if(mkfifo("servfifo",0640)<0)
        {
            printf("mkfifo error");
        }
        else
        {
            servfd = open("servfifo",O_RDONLY);
        }
    }

    read(servfd,buf,10);
    
    printf("%s",buf);

}