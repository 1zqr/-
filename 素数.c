#include<stdio.h>
int main(){
	int n=0,i,j;
	for(i=2;i<20000;i++){
		for(j=2;j<=i/2;j++)
        {
            if(0==i%j)
                break;
            if(i/2==j)
            {
                printf("%d\t",i);
                n++;
            }
            if(5==n)
            {
                n=0;
                printf("\n");
            }         
        }
	}
	return 0;
}

