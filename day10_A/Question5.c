#include <stdio.h>

int main() {
    int n;
    scanf("%d",&n);
    for(int i=1;i<n;i++)
    {
        for(int j=1;j<n-i;j++)
        {
            printf(" ");
        }
        
        for(int k=1;k<=i;k++)
        {
            printf("%d",k);
        }
        for(int k=i-1;k>=1;k--)
        {
            printf("%d",k);
        }
        printf("\n");
    }



    
   
    

    return 0;
}




#include <stdio.h>

int main() {
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        for(int j=1;j<=n-i-1;j++)
        {
            printf(" ");
        }
        
        for(int k=0;k<=i;k++)
        {
            printf("%c",'A'+k);
        }
        for(int k=i-1;k>=0;k--)
        {
            printf("%c",'A'+k);
        }
        printf("\n");
    }
    
   
    

    return 0;
}