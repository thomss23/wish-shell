#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid
#include <unistd.h>  // STDERR_FILENO, fork, exec, access, exit, chdir
#include <string.h>

#define DELIM " \n" 
#define BUFFERSIZE 256
#define EXIT "exit"

char *trimString(char *str) {
    
    char *end;

    while(isspace((unsigned char)*str)) {
        str++;
    } 

    if(*str == 0) {
        return str;
    }
    
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    } 

    end[1] = '\0';

    return str;
}

void printError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void executeCommandsFromUserInput(char *buffer, size_t bufferSize) {

    int characters = getline(&buffer, &bufferSize, stdin);
    int index = 0;
    char *myargv[256] = {NULL};

    for (char *p = strtok(buffer, DELIM); p; p = strtok(NULL, DELIM)) {
        myargv[index] = p;
        index++;
        if(index == BUFFERSIZE - 1) {
            break;
        }
    }

    if(strcmp(myargv[0], EXIT) == 0) {
        exit(EXIT_SUCCESS);
    }

    free(buffer);
}

void executeCommandsFromFile(FILE * fp, char *buffer, size_t bufferSize) {

    int characters = getline(&buffer, &bufferSize, fp);

    while(characters >= 0) {

        // do something

        characters = getline(&buffer, &bufferSize, fp);

    }

    free(buffer);
    fclose(fp);

}



int main(int argc, char *argv[]) {

    int mode;
    const int INTERACTIVE_MODE = 1;
    const int BATCH_MODE = 2;

    char *buffer;
    size_t bufferSize = 256;
    size_t characters;


    if(argc > 1) {
        mode = BATCH_MODE;

        if (argc > 2 || (stdin = fopen(argv[1], "r")) == NULL) {
            printError();
            exit(EXIT_FAILURE);
        }
    } else {
        mode = INTERACTIVE_MODE;
    }

    while(1) {  

        if(mode == INTERACTIVE_MODE) {
            printf("wish>");
        }

        buffer = (char *) malloc(256 * sizeof(char));

        if(buffer == NULL) {
            printError();
            exit(EXIT_FAILURE);
        }

        if(mode == INTERACTIVE_MODE) {
            executeCommandsFromUserInput(buffer, bufferSize);
        } else {
            FILE *fp = fopen(argv[1], "r");
            executeCommandsFromFile(fp, buffer, bufferSize);

        }
        

    }

}