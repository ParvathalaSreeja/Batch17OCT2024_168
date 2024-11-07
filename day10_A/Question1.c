#include <stdio.h>
int isPrime(int num)
{
    for(int j=2;j<=num/2;j++)
    {
        if(num%j==0)
            return 0;
    }
    return 1;
}
void find(int num)
{
    int count=0;
    for(int i=2;i<=num/2;i++)
    {
        if(isPrime(i) && isPrime(num-i))
        {
            printf("%d =%d+%d\n",num,i,num-i);
            count++;
        }
    }
    if(count==0)
        printf("Number ways=-1");
    else
        printf("Number of ways=%d",count);
}

int main() {
    int num;
    printf("\nEnter the number:\n");
    scanf("%d",&num);
    find(num);
    return 0;
}