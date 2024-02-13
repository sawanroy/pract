#include<stdio.h>

int main()
{
    int num=0,i; 
    printf("Enter the number");
    scanf("%d",&num);
    for(i=15;i>=0;i--)
    {
        if(num&(1<<i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
  
}