#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{

    int listLength = 0;
    int j, k, l;

    int count = 0;
    char tmpWord[50];      //word to add
    char wordList[50][50]; //string array
    while (tmpWord[0] != ';')
    {
        printf("Type the next word to add; To end, type ';' \n");
        scanf("%s", tmpWord);
        if (tmpWord[0] != ';')
        {
            //printf("DEBUG: Word Added: %s\n", tmpWord);
            strcpy(wordList[listLength], tmpWord);
            //how not to do this  ^^^       wordList[listLength] = *tmpWord;
            //printf("DEBUG: new word is %s \n", wordList[listLength]);
            for (j = 0; j <= listLength; j++)
            {
                //printf("DEBUG: Loop at %d \n", j);
                printf("%d : %s \n", j, wordList[j]);
            }
            listLength++;
        }
    }
    printf("number of words to sort: %d\n", listLength);
    while (count < listLength - 1)
    {
        //printf("AT: %d -- TARGET: %d", count, listLength-2);
        for (k = 0; k < listLength - 1; k++)
        {
            if (strcasecmp(wordList[k], wordList[k + 1]) > 0)
            {
                strcpy(tmpWord, wordList[k]);
                strcpy(wordList[k], wordList[k + 1]); //swap swap swap boom done
                strcpy(wordList[k + 1], tmpWord);
                //printf("DEBUG: Swap\n");
                count = 0;
            }
            else
            {
                count++;
            }
        }
    /*  
        printf("===================\n");
        for (l = 0; l < listLength; l++)
        {
            printf("%d : %s\n", l, wordList[l]);
            printf("count: %d\n",count);
        }
    */
    }
    printf("===================\n");
    for (l = 0; l < listLength; l++)
    {
        printf("%d : %s\n", l, wordList[l]);
    }
}
