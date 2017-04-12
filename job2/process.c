#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>


#define TIMEOUT 30

void endProgramAfterTimeout(pid_t parent, pid_t passive, pid_t active);
void passiveProcessRoutine(int pipe[]);
void activeProcessRoutine(int pipe[]);
void handleMessageFromChildren(int activePipe[], int passivePipe[]);
void writePipeToFile(int pipe[], clock_t start);
void writeToPipe(int pipe[], const char * message, int number, double timestamp);
void cleanOutPutFile();
double timestampInMiliseconds (clock_t start, clock_t end);

int main(int argc, const char * argv[]) {
    //clean previous output file
    cleanOutPutFile();

    //Create pipes for comunication
    int passiveComunicationPipe[2];
    int activeComunicationPipe[2];
    pipe(passiveComunicationPipe);
    pipe(activeComunicationPipe);

    //Children processes
    pid_t activeProcess;
    pid_t passiveProcess = fork();

    if (passiveProcess != 0) {
        //Parent process. Create activeProcess
        activeProcess = fork();

        if (activeProcess != 0) {
            //Create process to kill program
            endProgramAfterTimeout(getpid(), passiveProcess, activeProcess);

            //Parent process
            handleMessageFromChildren(activeComunicationPipe, passiveComunicationPipe);
        } else {
            //Active process
            activeProcessRoutine(activeComunicationPipe);
        }
    } else {
        //Passive Process. Send messagens to parent
        passiveProcessRoutine(passiveComunicationPipe);
    }

    return 0;
}

//Delete outputfile if exist
void cleanOutPutFile() {
    remove("output.txt");
}

//handle messages from children in the parent process
void handleMessageFromChildren(int activePipe[], int passivePipe[]) {
    //Read message from children
    //Close write end of stream comunication
    close (passivePipe[1]);
    close(activePipe[1]);

    // get intial time for parent process
    clock_t start = clock();

    //Continious reading from pipe
    while (1) {
        writePipeToFile(passivePipe, start);
        writePipeToFile(activePipe, start);
    }
}

// Active process. Handle user input and send to parent process
void activeProcessRoutine(int pipe[]) {
    char message[50];

    printf("Entre com as menssagens: \n");

    //Get current local time to compute timestamp
    clock_t start, end;
    double cpuTimeUsed;
    start = clock(); // get intial time

    int i = 1;
    while (1) {
        scanf("%s", message);

        char userMessage[80] = "do usuario: <";
        strcat(userMessage, message);
        strcat(userMessage, ">");

        end = clock(); // get end of each messagem
        // compute timestamp from start time
        cpuTimeUsed = timestampInMiliseconds(start, end);

        writeToPipe(pipe, userMessage, i, cpuTimeUsed);
        i++;
    }
}

// Passive process. Send message to pipe every random seconds.
void passiveProcessRoutine(int pipe[]) {
    //Get current local time to compute timestamp
    clock_t start, end;
    double cpuTimeUsed;
    start = clock(); // get intial time

    int i = 1;
    while (1) {
        sleep(rand()%3); // wait to write to pipe
        end = clock(); // get end of each messagem

        // compute timestamp from start time
        cpuTimeUsed = timestampInMiliseconds(start, end);

        writeToPipe(pipe, "do filho dorminhoco", i, cpuTimeUsed);
        i++;
    }
}

// Child process to kill parent process after timeout
void endProgramAfterTimeout(pid_t parent, pid_t passive, pid_t active) {
    pid_t child = fork();

    if (child != 0) {
        // Parent process do nothing
        return;
    } else {
        //Child process. Wait for timeout seconds to kill process!
        sleep(TIMEOUT);
        fprintf(stderr, "\n\nKilling parent process and children. After %d seconds timeout\n\n", TIMEOUT);
        kill(passive, SIGKILL);
        kill(active, SIGKILL);
        kill(parent, SIGKILL);
        raise(SIGKILL);
    }
}

//Write message to input end to target pipe
void writeToPipe(int pipe[], const char * message, int number, double timestamp) {
    //Create file stream for comunication
    FILE* stream;

    //Close read end of pipe.
    close (pipe[0]);

    //Open write end of pipe and point to file stream
    stream = fdopen (pipe[1], "w");
    fprintf (stream, "%lf Mensagem %d %s\n", timestamp, number, message);
    fflush (stream);
}

//Get output from pipe and writes to target file
void writePipeToFile(int pipe[], clock_t start) {
    //Prepare output file
    FILE * outputFile = fopen("output.txt", "a");

    //check if file is openned
    if (outputFile == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    // start non-breaking reading from pipe
    fd_set set;
    struct timeval timeout;

    /* Initialize the file descriptor set. */
    FD_ZERO(&set);
    FD_SET(pipe[0], &set);

    /* Initialize the timeout data structure. */
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int result = select(FD_SETSIZE, &set, NULL, NULL, &timeout);

    // a return value of 0 means that the time expired
    // without any acitivity on the file descriptor
     if (result > 0) {

        // there was activity on the file descripor
        // Open pipe stream read end
        FILE* stream;
        stream = fdopen (pipe[0], "r");

        //Ready file and print to output
        char buffer[1024];
        if ((fgets (buffer, sizeof (buffer), stream) != NULL)) {
            clock_t end = clock();
            double cpuTimeUsed = timestampInMiliseconds(start, end);
            fprintf(outputFile, "%lf %s", cpuTimeUsed, buffer);
        }
    }

    //Close output file
    fclose(outputFile);
}

double timestampInMiliseconds(clock_t start, clock_t end) {
    return ((double) (end - start)) / (CLOCKS_PER_SEC/1000);
}
