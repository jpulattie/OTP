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
    // printf("num equals: %d\n", num);
    key[0] = '\0';
    // char convert[numb + 1];
    for (i = 0; i < num; i++)
    {
        random_number = (rand() % (max - min + 1) + min);
        // printf("number: %d ", random_number);
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
        //key[num] = '\n';
        //printf("i %d: %c\n", i, key[i]);
    }
        //printf("-%d-", key[num]);
        printf("%s\n", key);
        return key;
    }


int main(int argc, char *argv[])
{
    int randomGen = atoi(argv[1]);

    char key[randomGen];
    //printf("random gen:%d\n", randomGen);
    int final;
    srand(time(NULL));
    generator(randomGen, key);
    return 0;
}
