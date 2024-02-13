#include<stdio.h>
int print_bin(int bin_num);
int main()
{
    int num,var1,var2;

    printf("Enter the number");
    scanf("%d",&num);
     print_bin(num);
    var1 = num>>8;
    var2 = num<<8;

    num = var1 | var2;

    printf("after operation number %d\n",num);
     print_bin(num);

}

int print_bin(int bin_num)
{
    int i; 
    for(i=31;i>=0;i--)
    {
        if(bin_num&(1<<i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
}