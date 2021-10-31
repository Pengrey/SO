#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  time_t t;
  srand((unsigned) time(&t)); 
  int num = rand() % 50;
  int guess;
  printf("%d\n",num);
  printf("Lets play HighLow!!!\n\n");
  printf("Give me a guess:");
  scanf("%d",&guess);
  do{
    if(guess>num){
      printf("High!\n");
    }else if(guess<num){
      printf("Low!\n");
    }
    printf("Try again: ");
    scanf("%d",&guess);
  }while((guess != num));
  printf("You did it!!! \nCongrats m8 :3\n");
  return 0;
}
