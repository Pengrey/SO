#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char **argv)
{
  if(argc == 4){
    float result = 0;
    double num1, num2;
    char *opr;
    char *end;
    num1 = strtod(argv[1], &end);
    num2 = strtod(argv[3], &end);
    opr = argv[2];
    switch(*opr){
      case '+' :
        result = num1 + num2;
        break;
      
      case '-' :
        result = num1 - num2;
        break;
    
      case 'x':
        result = num1 * num2;
        break;

      case '/':
        result = num1 / num2;
        break;
    
      case 'p':
        result = pow(num1,num2);
        break;
    }
    printf("%.1f %s %.1f = %.1f", num1, opr, num2, result);
  }else{
    printf("Nùmero errado de argumentos!!! %d",argc);
  }  
}
