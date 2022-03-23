#include<stdio.h>

int main(){
	int num;
	printf("Enter 3-digit number : ");
	scanf("%d",&num);
	printf("The rightmost digit is %d\n",num%10);
	printf("The middle digit is %d\n",(num/10)%10);
	printf("The leftmost digit is %d\n",(num/100)%10);
}