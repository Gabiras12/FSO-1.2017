#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 50

typedef struct {
  int buf[MAX_BUFFER_SIZE]; // the buffer
  size_t len; // number of items in the buffer
  pthread_mutex_t mutex; // needed to add/remove data from the buffer
  pthread_cond_t can_produce; // signaled when items are removed
  pthread_cond_t can_consume; // signaled when items are added
  int max_number; // larger number in buffer
  int min_number; // smaller number in buffer
} buffer_t;

void write_to_file(char * message);
int check_max_number(int max_number, int * buffer);
int check_min_number(int min_number, int * buffer);
void * write_to_buffer(void * args);
void * read_to_buffer(void * args);


void write_to_file(char * message){

  FILE * output_file = fopen("teste.txt", "a");

  if (output_file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(output_file, "%s\n", message);

  fclose(output_file);
}

int check_max_number(int max_number, int * buffer){

    for(int i=0; i<MAX_BUFFER_SIZE; i++){
      if(buffer[i] > max_number){
        max_number = buffer[i];
      }
    }
    return max_number;
}

int check_min_number(int min_number, int * buffer){

    for(int i=0; i<MAX_BUFFER_SIZE; i++){
      if(buffer[i] < min_number){
        min_number = buffer[i];
      }
    }
    return min_number;
}

void * write_to_buffer(void * args){
  buffer_t *buffer = (buffer_t*) args;
  char msg[100];

  while(1){
    pthread_mutex_lock(&buffer->mutex);	/* protect buffer */

    if(buffer->len == MAX_BUFFER_SIZE) {
      // wait until some elements are consumed
      pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
    }

    buffer->buf[buffer->len] = rand()%30;
    snprintf(msg, 100 , "[producao]: Numero gerado: %d", buffer->buf[buffer->len]);
    write_to_file(msg);

    buffer->len ++;

    // signal the fact that new items may be consumed
    pthread_cond_signal(&buffer->can_consume);
    pthread_mutex_unlock(&buffer->mutex);

    usleep(100000); //sleep for 100 ms
  }
}


void * read_to_buffer(void * args){
  buffer_t *buffer = (buffer_t*) args;
  char msg[100];

  while(1){
    pthread_mutex_lock(&buffer->mutex); /* protect buffer */

    while(buffer->len == 0) {
      // wait for new items to be appended to the buffer
      pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
    }

    buffer->len --;

    buffer->min_number = check_min_number(buffer->min_number, buffer->buf);
    buffer->max_number = check_max_number(buffer->max_number, buffer->buf);

    //printf("MAX NUMBER: %d\n", buffer->max_number);
    //printf("MIN NUMBER: %d\n", buffer->min_number);
    snprintf(msg, 100, " [consumo]: Numero lido: %d" , buffer->buf[buffer->len]);
    write_to_file(msg);

    // signal the fact that new items may be produced
    pthread_cond_signal(&buffer->can_produce);
    pthread_mutex_unlock(&buffer->mutex);

    usleep(150000); //sleep for 150 ms
  }
}


int main(int argc, const char * argv[]){

  srand((unsigned)time(NULL));

  buffer_t buffer = {
    .len = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .can_produce = PTHREAD_COND_INITIALIZER,
    .can_consume = PTHREAD_COND_INITIALIZER,
    .max_number = 0,
    .min_number = 0
  };

  pthread_t producer, consumer1, consumer2;
  // Initialize the mutex and condition variables

  pthread_create(&producer, NULL, write_to_buffer, (void*)&buffer);

  pthread_create(&consumer1, NULL, read_to_buffer, (void*)&buffer);
  pthread_create(&consumer2, NULL, read_to_buffer, (void*)&buffer);

  pthread_join(producer, NULL);
  pthread_join(consumer1, NULL);
  pthread_join(consumer2, NULL);

  return 0;
}
