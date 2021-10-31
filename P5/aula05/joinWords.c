#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char **argv)
{
  int count = 0;
  for(int i = 1; i < argc ; i++)
  {
    if(isalpha(argv[i][0])){count += strlen(argv[i]);}
  }
  char *str = malloc(count + 1); 
  strcpy(str,argv[1]);
  for(int i = 2 ; i < argc ; i++)
  {
    if(isalpha(argv[i][0])){strcat(str,argv[i]);}
  }
  printf("%s\n",str);
  return 0;
}
