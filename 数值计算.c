#include<stdio.h>
#include<string.h>
int main(){
	char arr[20]={1,-2,3,5,-1};
	int maxsum=0,i;
	for(i=0;i<strlen(arr);i++){
		if(arr[i]>0)
			maxsum=maxsum+arr[i];
	}
	printf("%d",maxsum);
	return 0;
}
