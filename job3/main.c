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

void * write_to_buffer(void * args){
    int * buffer = (int *) args;
    char msg[100];

    for(int i=0; i<50; i++){
      buffer[i] = rand();
      snprintf(msg, 100 , "[producao]: Numero gerado: %d", buffer[i]);
      write_to_file(msg);
      usleep(100000); //sleep for 100 ms
    }
}

void create_producer_thread(int * buffer){
  pthread_t producer_thread;

  pthread_create(&producer_thread, NULL, write_to_buffer, buffer);
  pthread_join(producer_thread, NULL);
}


int main(int argc, const char * argv[]){
  srand((unsigned)time(NULL));

  int * buffer = (int*) malloc(sizeof(int) * 50);
  create_producer_thread(buffer);
  return 0;
}
