#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 50

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int current_buffer_size;

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

  while(current_buffer_size < MAX_BUFFER_SIZE){
    pthread_mutex_lock(&the_mutex);	/* protect buffer */

    buffer[current_buffer_size] = rand();
    snprintf(msg, 100 , "[producao]: Numero gerado: %d", buffer[current_buffer_size]);
    write_to_file(msg);
    current_buffer_size++;
    
    pthread_cond_signal(&condc); /* wake up consumer */
    pthread_mutex_unlock(&the_mutex); /* release the buffer */
    usleep(100000); //sleep for 100 ms
  }
}


void * read_to_buffer(void * args){
  int * buffer = (int*) args;
  char msg[100];

  for(int i=0; i<MAX_BUFFER_SIZE ; i++){
    pthread_mutex_lock(&the_mutex); /* protect buffer */

    while (current_buffer_size <= 0) /* If there is nothing in the buffer then wait */
      pthread_cond_wait(&condc, &the_mutex);

    snprintf(msg, 100, " [consumo]: Numero lido: %d" , buffer[i]);
    current_buffer_size --;
    write_to_file(msg);
    pthread_mutex_unlock(&the_mutex); /* release the buffer */
    usleep(150000); //sleep for 150 ms
  }
}


int main(int argc, const char * argv[]){

  srand((unsigned)time(NULL));
  int * buffer = (int*) malloc(sizeof(int) * 50);

  pthread_t producer, consumer1, consumer2;
  // Initialize the mutex and condition variables

  pthread_mutex_init(&the_mutex, NULL);
  pthread_cond_init(&condp, NULL);		/* Initialize producer condition variable */
  pthread_cond_init(&condc, NULL);		/* Initialize consumer condition variable */

  pthread_create(&producer, NULL, write_to_buffer, buffer);

  pthread_create(&consumer1, NULL, read_to_buffer, buffer);
  //pthread_create(&consumer2, NULL, read_to_buffer, buffer);

  pthread_join(producer, NULL);
  pthread_join(consumer1, NULL);

  //pthread_join(consumer2, NULL);

  pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
  pthread_cond_destroy(&condc); /* Free up consumer condition variable */
  pthread_cond_destroy(&condp); /* Free up producer condition variable */


  free(buffer);

  printf("buffer: %d", current_buffer_size);
  return 0;
}
