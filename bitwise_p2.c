#include<stdio.h>

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
    printf("\n");
}

int main()
{
    int a = 0x123456;
    //need to get the output like 0x214365
    int var1;
    int var2,var3,var4;
    int num;
    print_bin(a);

    var1 = a<<4;
    var2 = a>>4;
    var3 = a<<4;
    var4 = a<<4;

    int num2 = (0x00FF00)&(var3 | var4);
    printf("%x\n",num2);

    print_bin(num2);


    num = (0x0000FF)&(var1 | var2);


    printf("%x\n",num);

    print_bin(num);
}

