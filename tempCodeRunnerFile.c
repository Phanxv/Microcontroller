#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char comHand(){
    srand(time(NULL));
    int i;
    char hand[2];
    for(i=0;i<2;i++){
        hand[i] = (rand() % 3) + 1;
    }
    printf("%d %d",hand[0],hand[1]);
}

int main(){
    comHand();
}