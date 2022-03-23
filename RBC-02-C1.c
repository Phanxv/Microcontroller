#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool isLeapYear(int year);
int getCenturyValue(int year);
int getYearValue(int year);
int getMonthValue(int month,int year);

int main(){
    int day, month, year, weekDay;
    do{
        printf("\n\nEnter the date\n\n");
        printf("\tDay (1-31) : ");
        scanf("%d",&day);
        printf("\tMonth (1-12) : ");
        scanf("%d",&month);
        printf("\tYear (in AD) : ");
        scanf("%d",&year);
        weekDay = (day + getMonthValue(month,year) + getYearValue(year) + getCenturyValue(year)) % 7;
        printf("\nThe day of the week of %d-%d-%d is ",day,month,year);
        switch(weekDay){
            case 0 :
                printf("Sunday");
                break;
            case 1 :
                printf("Monday");
                break;
            case 2 :
                printf("Tuesday");
                break;
            case 3 :
                printf("Wednesday");
                break;
            case 4 :
                printf("Thursday");
                break;
            case 5 :
                printf("Friday");
                break;
            case 6 :
                printf("Saturday");
                break;
        }
    }while(day!=0&&month!=0&&year!=0);
}

bool isLeapYear(int year){
    if(year%4 == 0||(year%4==0 && year%100 != 0))
        return true;
    else
        return false;
}

int getCenturyValue(int year){
    return (3-((year/100)%4))*2;
}

int getYearValue(int year){
    return ((year%100)/4)+(year%100);
}

int getMonthValue(int month,int year){
    if(month==1&&isLeapYear(year)==false)
        return 0;
    if(month==1&&isLeapYear(year)==true) 
        return 6;
    if(month==2&&isLeapYear(year)==false)
        return 3;
    if(month==2&&isLeapYear(year)==true)
        return 2;
    if(month==3)
        return 3;
    if(month==4)
        return 6;
    if(month==5)
        return 1; 
    if(month==6)
        return 4; 
    if(month==7)
        return 6;
    if(month==8)
        return 2; 
    if(month==9)
        return 5;
    if(month==10)
        return 0;
    if(month==11)
        return 3;
    if(month==12)
        return 5;
}