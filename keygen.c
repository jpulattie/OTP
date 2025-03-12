#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *generator(int num, char *key)
{
    int i;
    char alpha;
    int random_number;
    int max = 27;
    int min = 1;
    printf("num: %d\n", num);
    // printf("num equals: %d\n", num);
    key[0] = '\0';
    // char convert[numb + 1];
    for (i = 0; i < num; i++)
    {
        random_number = (rand() % (max - min + 1) + min);
        printf("random number: %d \n", random_number);
        printf("random + 64 %d\n", random_number + 64);
        if (random_number == 27)
        {
            key[i] = ' ';
        }
        // if (i == num -1){
        //    printf("last cha\n");
        //    key[i] = '\n';
        //    printf("should be a blank line above\n");
        //}
        else
        {
            key[i] = random_number + 64;
        }
        key[num] = '\0';
        //printf("i %d: %c\n", i, key[i]);
    }
        //printf("-%d-", key[num]);
        printf("%s\n", key);
        return key;
    }


int main(int argc, char *argv[])
{
    int randomGen = atoi(argv[1]);
    printf("random gen in main: %d", randomGen);
    char key[randomGen];
    printf("length of key:%ld\n", strlen(key));
    int final;
    srand(time(NULL));
    generator(randomGen, key);
    return 0;
}
