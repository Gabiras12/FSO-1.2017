#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libseno.h"


int main(int argc, char *argv[]){

  double arg;

  if(argv[1] == NULL) {
    printf("Parâmetros de entrada não definidos!\n");
  }
  else if(strcmp(argv[1],"-a") == 0){
    arg = atof(argv[2]);
    printf("arc_seno (%.2f) = %.2f\n", arg, arc_seno(arg));
  }
  else if(strcmp(argv[1],"-s") == 0) {
    arg = atof(argv[2]);
    printf("seno (%.2f) = %.2f\n", arg, seno(arg));
  }
  else{
    printf("Argumento inválido\n");
  }

}
