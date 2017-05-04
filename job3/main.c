#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void write_to_file(char * message){

  FILE * output_file = fopen("teste.txt", "a");

  if (output_file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(output_file, "%s\n", message);

  fclose(output_file);
}

void * generate_random_numbers(void * args){
    int * vector = (int*) malloc(sizeof(int) * 50);
    char msg[100];

    for(int i=0; i<50; i++){
      vector[i] = rand();
      snprintf(msg, 100 , "[producao]: Numero gerado: %d", vector[i]);
      write_to_file(msg);
      //usleep(100000); //sleep for 100 ms
    }
}

void create_producer_thread(){
  pthread_t producer_thread;
  
  pthread_create(&producer_thread, NULL, generate_random_numbers, NULL);
  pthread_join(producer_thread, NULL);
}

int main(int argc, const char * argv[]){
  srand((unsigned)time(NULL));
  create_producer_thread();
  return 0;
}
