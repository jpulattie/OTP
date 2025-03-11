#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int generator(int num){
    int i;
    char alpha;
    int random_number;
    int max = 26;
    int min = 0;
    char key[num +1];
    //char convert[numb + 1];
    for (i=0; i < num; i++) {
        random_number = (rand() % (max - min + 1) + min);
        if (random_number == 0){
            strcat(key," ");
        } else {
            alpha = (char)(random_number + 64);
            strcat(key,alpha);
        }
        
        printf("i: %d\n", i);}
    return 0;
}

int main(int argc, char *argv[]) {
    int final;
    int randomGen = atoi(argv[1]);
    generator(randomGen);
    return 0;
}