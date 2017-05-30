#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "libseno.h"


int main(int argc, char *argv[]){

  double arg;
  void * lib;

  lib = dlopen("build/libseno.so", RTLD_LAZY);

	if(!lib){
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

  if(argv[1] == NULL) {
    printf("Parâmetros de entrada não definidos!\n");
  }
  else if(strcmp(argv[1],"-a") == 0){
    arg = atof(argv[2]);

    double (*arc_seno_func)(double seno) = dlsym(lib, "arc_seno");
    printf("arc_seno (%.2f) = %.2f\n", arg, (*arc_seno_func)(arg));
  }
  else if(strcmp(argv[1],"-s") == 0) {
    arg = atof(argv[2]);

    double (*seno_func)(double angulo) = dlsym(lib, "seno");
    printf("seno (%.2f) = %.2f\n", arg, (*seno)(arg));
  }
  else{
    printf("Argumento inválido\n");
  }

  dlclose(lib);

  return 0;

}
