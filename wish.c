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
#define COMMAND_DELIM "&"

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

//todo: write built in commands

/* cd always take one argument (0 or >1 args should be signaled as an error). 
To change directories, use the chdir() system call with the argument supplied by the user; 
if chdir fails, that is also an error.
*/

void execute_cd() {

}
/*
path: The path command takes 0 or more arguments, with each argument separated by whitespace from the others.
 A typical usage would be like this: wish> path /bin /usr/bin, which would add /bin and /usr/bin to the
search path of the shell. If the user sets path to be empty, 
then the shell should not be able to run any programs (except built-in commands). 
The path command always overwrites the old path with the newly specified path.
*/
void execute_path() {

}

void execute_exit() {
    exit(EXIT_SUCCESS);
}

//split input into an array of commands using the '&' separator
void parseCommandsFromInput(char *parsedCommands[], char *buffer) {

    int index = 0;
    for (char *p = strtok(buffer, "&"); p; p = strtok(NULL, "&")) {
        parsedCommands[index] = trimString(p);
        index++;
        if (index == BUFFERSIZE - 1) {
            break;
        }
    }
}

void print_test_array_of_Strings(char *string[]) {
    for(int i =0 ; string[i]!= NULL; i++) {
        printf("String %d is %s\n", i, string[i]);
    }
}

//!!! Check the return codes of all system calls from the very beginning of your work.
//!!! This will often catch errors in how you are invoking these new system calls. It's also just good programming sense.
void executeCommandsFromUserInput(char *buffer, size_t bufferSize) {

    getline(&buffer, &bufferSize, stdin);

    char *parsedCommands[256] = {NULL};
    parseCommandsFromInput(parsedCommands, buffer);

    //todo: split each command into an array of type command (command, array of args)

    //todo: define built in commands (exit(done), cd and path)

    //todo: find a way to execute commands in parallel (probably creating children of them from the main process
    //end execute them in parallel somehow


    char *myargv[256] = {NULL};

    for (char *p = strtok(buffer, DELIM); p; p = strtok(NULL, DELIM)) {
        myargv[index] = p;
        index++;
        if(index == BUFFERSIZE - 1) {
            break;
        }
    }

//     if(strcmp(myargv[0], "ls") == 0) {
//         char *binaryPath = "/bin/ls";
//         FILE *fp;
//         fp = fopen ("output", "w");
//         char *args[] = {binaryPath, "-lh", "/home", ">", "output", NULL};
//         execv(binaryPath, args);
//         fclose(fp);
//     }

    if(strcmp(myargv[0], EXIT) == 0) {
        execute_exit();
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
    exit(EXIT_SUCCESS);

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