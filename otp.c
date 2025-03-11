#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int generator(int num){
    int i;
    char alpha;
    int random_number;
    int max = 27;
    int min = 1;
    char key[num +1];
    key[0] = '\0';
    //char convert[numb + 1];
    for (i=0; i < num; i++) {
        random_number = (rand() % (max - min + 1) + min);
        if (random_number == 27){
            key[i] = ' ';
        } else {
            key[i] = random_number + 64;
        }
        
        printf("i: %c\n", key[i]);}
        key[num] = '\n';
        printf("key: %s\n", key);
    return 0;
}

int main(int argc, char *argv[]) {
    int final;
    int randomGen = atoi(argv[1]);
    generator(randomGen);
    return 0;
}