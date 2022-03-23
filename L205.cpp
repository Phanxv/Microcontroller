#include<stdio.h>

int main(){
	float x1,x2,x3;
	printf("1st number : ");
	scanf("%f",&x1);
	printf("2nd number : ");
	scanf("%f",&x2);
	printf("3rd number : ");
	scanf("%f",&x3);
	printf("The average is %.2f",(x1+x2+x3)/3.0);
}