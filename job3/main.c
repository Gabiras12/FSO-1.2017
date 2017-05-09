#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_BUFFER_SIZE 50
#define MAX_RAND_NUMBER 5000
#define PRODUCER_SLEEP_TIME 100
#define CONSUMER_SLEEP_TIME 150

int max_number = -MAX_RAND_NUMBER;
int min_number = MAX_RAND_NUMBER;
int max_buffer_utilization = 0;
int buffer_size = 0;

typedef struct {
  int buf[MAX_BUFFER_SIZE]; // the buffer
  pthread_mutex_t mutex; // needed to add/remove data from the buffer
  pthread_cond_t can_produce; // signaled when items are removed
  pthread_cond_t can_consume; // signaled when items are added
} buffer_t;

typedef struct{
  char name[1]; // thread name
  buffer_t * buffer;
} consumer_struct;

char * logFileName;

// Threads Ids
pthread_t producerID, consumer1ID, consumer2ID;
void * producerStatus;
void * consumer1Status;
void * consumer2Status;

void write_to_file(char * message);
int check_max_number(int * buffer);
int check_min_number(int * buffer);
void * write_to_buffer(void * args);
void * read_from_buffer(void * args);
void signalHandler(int sig);
void printEndingInfos();
void requestThreadsCancelation();
void cleanupHandler(void * args);

int main(int argc, const char **argv){

  // check if log file name was defined by the user
  if (argv[1] == NULL) {
    fprintf(stderr, "Log file not defined by user.\nPlease run again passing the file name as a param.\n");
    return 0;
  } else {
    logFileName = argv[1];
  }

  //Set signal handler to catch ctrl-c
  signal(SIGINT, signalHandler);

  //Set seed for random numbers
  srand((unsigned)time(NULL));

  // Create buffer struct and initialize mutex and condition variables
  buffer_t buffer = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .can_produce = PTHREAD_COND_INITIALIZER,
    .can_consume = PTHREAD_COND_INITIALIZER,
  };

  // Create consumer struct
  consumer_struct consumer_1 = {
    .buffer = &buffer
  };

  // Create consumer struct
  consumer_struct consumer_2 = {
    .buffer = &buffer
  };

  // Identify consumer structs
  strcpy(consumer_1.name, "a");
  strcpy(consumer_2.name, "b");


  // Create threads to write and read
  pthread_create(&producerID, NULL, write_to_buffer, (void*)&buffer);
  pthread_create(&consumer1ID, NULL, read_from_buffer, (void*)&consumer_1);
  pthread_create(&consumer2ID, NULL, read_from_buffer, (void*)&consumer_2);

  // Wait threads returns to end program
  pthread_join(producerID, &producerStatus);
  pthread_join(consumer1ID, &consumer1Status);
  pthread_join(consumer2ID, &consumer2Status);

  return 0;
}

// write message to log file
void write_to_file(char * message){

  FILE * output_file = fopen(logFileName, "a");

  if (output_file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(output_file, "%s\n", message);

  fclose(output_file);
}

// check the biggest number in the buffer
int check_max_number(int * buffer){

    for(int i=0; i<MAX_BUFFER_SIZE; i++){
      if(buffer[i] > max_number){
        max_number = buffer[i];
      }
    }
    return max_number;
}

// check the smallest number in the buffer
int check_min_number(int * buffer){

    for(int i=0; i<MAX_BUFFER_SIZE; i++){
      if(buffer[i] < min_number && buffer[i] != 0){
        min_number = buffer[i];
      }
    }
    return min_number;
}

void cleanupHandler(void * args) {
  
}

// right random number to buffer
void * write_to_buffer(void * args){
  // cast args back to buffer_t type
  buffer_t *buffer = (buffer_t*) args;
  char msg[100];
  int old_cancel_state;

  while(1){
    // Lock the mutex. pthread_mutex_lock works like this:
    // if the lock is available, the function locks it and returns immediately
    // if not available wait until lock set available and block the thread
    pthread_mutex_lock(&buffer->mutex);	/* protect buffer */

    // If buffer_size == MAX_BUFFER_SIZE buffer is full so it's not possible to write to it
    if(buffer_size == MAX_BUFFER_SIZE) {
      // wait until some elements are consumed
      pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
    }

    // Block cancel thread after this point is beening execulted
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);

    // Write random number to buffer
    buffer->buf[buffer_size] = rand()%(2*MAX_RAND_NUMBER) - MAX_RAND_NUMBER;

    //Write to log file
    snprintf(msg, 100 , "[producao]: Numero gerado: %d", buffer->buf[buffer_size]);
    write_to_file(msg);

    // Incremente next available possition in the buffer
    buffer_size ++;

    // Update max_buffer_utilization if new used possition
    if (buffer_size > max_buffer_utilization) {
      max_buffer_utilization = buffer_size;
    }

    // signal the fact that new items may be consumed
    pthread_cond_signal(&buffer->can_consume);
    pthread_mutex_unlock(&buffer->mutex);

    // Release cancel thread after this point is beening execulted
    pthread_setcancelstate(old_cancel_state, NULL);
    pthread_testcancel();
    usleep(PRODUCER_SLEEP_TIME*1000); //sleep for 100 ms
  }
}


// Function to read from buffer when lock available and there is data on it
void * read_from_buffer(void * args){
  // cast args back to buffer_t type
  consumer_struct * consumer = (consumer_struct*) args;
  char msg[100];
  int old_cancel_state;

  while(1){
    // Lock the mutex. pthread_mutex_lock works like this:
    // if the lock is available, the function locks it and returns immediately
    // if not available wait until lock set available and block the thread
    pthread_mutex_lock(&consumer->buffer->mutex);


    // if len == 0 thre is no data in the buffer
    while(buffer_size == 0) {
      // wait for new items to be appended to the buffer
      pthread_cond_wait(&consumer->buffer->can_consume, &consumer->buffer->mutex);
    }

    // Block cancel thread after this point is beening execulted
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);

    // remove item from buffer
    buffer_size --;

    //check min and max
    min_number = check_min_number(consumer->buffer->buf);
    max_number = check_max_number(consumer->buffer->buf);

    //write readed number to file
    snprintf(msg, 100, "[consumo %s]: Numero lido: %d" ,consumer->name ,consumer->buffer->buf[buffer_size]);
    write_to_file(msg);

    // signal the fact that new items may be produced
    pthread_cond_signal(&consumer->buffer->can_produce);
    pthread_mutex_unlock(&consumer->buffer->mutex);

    // Release cancel thread after this point is beening execulted
    pthread_setcancelstate(old_cancel_state, NULL);

    pthread_testcancel();
    usleep(CONSUMER_SLEEP_TIME*1000); //sleep for 150 ms
  }
}

// Handler signal in the process
void signalHandler(int sig) {
  signal(sig, SIG_IGN);
  requestThreadsCancelation();
  printEndingInfos();
  exit(0);
}

void requestThreadsCancelation() {
  char msg[100];
  snprintf(msg, 100, "[aviso]: Termino solicitado. Aguardando threads...");
  write_to_file(msg);
  printf("\n[aviso]: Termino solicitado. Aguardando threads...\n");

  pthread_cancel(producerID);
  pthread_cancel(consumer1ID);
  pthread_cancel(consumer2ID);
}

void printEndingInfos() {
  char msg[100];

  snprintf(msg, 100, "[aviso]: Maior numero gerado: %d", max_number);
  write_to_file(msg);
  snprintf(msg, 100, "[aviso]: Menor numero gerado: %d", min_number);
  write_to_file(msg);
  snprintf(msg, 100, "[aviso]: Maior ocupacao de buffer: %d", max_buffer_utilization);
  write_to_file(msg);
  snprintf(msg, 100, "[aviso]: Aplicacao encerrada.");
  write_to_file(msg);

  printf("[aviso]: Maior numero gerado: %d\n", max_number);
  printf("[aviso]: Menor numero gerado: %d\n", min_number);
  printf("[aviso]: Maior ocupacao de buffer: %d\n", max_buffer_utilization);
  printf("[aviso]: Aplicacao encerrada.\n");
}
