#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int numberOfElementFound = 0;
int targetNumberOfElements;

int addFileToSearchResult(char * fileName, char * substring);
void queryDirectory(char * dir, char * query);

int main(int argc, const char * argv[]) {

    if (argc < 4) {
      printf("Wrong number of arguments.\n");
      exit(0);
    }

    char * initialLocation = argv[1];
    char * substring = argv[2];
    targetNumberOfElements = strtol(argv[3], NULL, 10);

    printf("Resultado de buscador -- \"%s\"­­ na pasta %s\n", substring, initialLocation);

    queryDirectory(initialLocation, substring);

    exit(0);
}

void queryDirectory(char * dir, char * query) {
    // Open target directory to search
    DIR * targetDir = opendir(dir);
    if( targetDir == NULL) {
        fprintf(stderr,"cannot open directory: %s\n Maybe it does not exist.\n", dir);
        return;
    }

    // Change working directory to target location
    chdir(dir);

    struct dirent *entry;
    struct stat stats;

    //Get next element in directory
    while((entry = readdir(targetDir)) != NULL) {

        //ignore . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        } else {
            //check if is directory
            lstat(entry->d_name, &stats);

            if (S_ISDIR(stats.st_mode)) {
                // Is a directory. Recursive call to still looking inside this directory
                queryDirectory(entry->d_name, query);
            } else {
                // Is a file. Check if file has target substring.
                if(!addFileToSearchResult(entry->d_name, query)) {
                    //number of result reached
                    break;
                }
            }
        }
    }

    // Recursive calls get into child directory.
    // At end of it, change working directory to parent one
    chdir("..");

    //close target dir
    closedir(targetDir);
}


// Add file content to result if file name has
// Target substring. Return 1 to continue search or 0 to stop it
int addFileToSearchResult(char * fileName, char * substring) {
    if (strstr(fileName, substring) != NULL) {
        if (numberOfElementFound < targetNumberOfElements) {
            // Get current work directory
            char currentWorkDirectory[1024];
            char buffer[30] = {};

            getcwd(currentWorkDirectory, sizeof(currentWorkDirectory));

            printf("   %d.%s/%s --\n", numberOfElementFound + 1, currentWorkDirectory, fileName);

            FILE * file;
            file = fopen(fileName, "r");
            fseek(file, SEEK_SET, 0);
            fread(buffer, 30, 1, file);

            printf("     %s\n", buffer);

            numberOfElementFound++;
        }

        if (numberOfElementFound == targetNumberOfElements) {
            return 0;
        }
    }
    return 1;
}
