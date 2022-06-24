/*
Adam Gray

This is my main myshell file with a function
to loop and continue like a shell, one to see
if a command as initialised and one to check
for IO redirection. It used many functions
found in utility.h
*/

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "utility.h"
//define seperators, line buffer and args to be used
#define SEPARATORS " \t\n"  //seperate on spaces tabs and newlines
#define MAX_BUFFER 1024     //max amount of line buffer
#define MAX_ARGS 64         //max number of args that can be used

//function declarations
void loop(char* argv[]);
int is_command(char** arg, char** args, bool check);
void redirection(char** args);

//main function
int main(int argc, char *argv[])
{
    printf("\033[0;32m");       //set colour to green
    if (argc > 1){              //if has command line arguments
        freopen(argv[1], "r", stdin);    //open the file for reading to standard input
    }
    welcome();                  //call the welcome function
    loop(argv);                 //call the loop function
    return 0;
}

//function to continue to print the prompt as the shell continues, i.e. acts like a real command line
//loop function adapted from Lab 4 first shell provided for use
void loop(char *argv[]){
    char input[MAX_BUFFER];  //my inputs
    char * args[MAX_ARGS];   //string of my arguments
    char ** arg;             //pointer through the string of arguments
    //while no input taken
    while (!feof(stdin)){
        if (!argv[1]){              //only print the prompt, if no file passed to program
            Path();
            fputs("$ > ", stdout); //put prompt onto command line
        }

        //readline and token  out the inputs into an array
        if (fgets (input, MAX_BUFFER, stdin )){
            arg = args;
            *arg++ = strtok(input,SEPARATORS);          //splits on SEPERATORS, in this case most likely space or tab
            while((*arg++ = strtok(NULL, SEPARATORS))); //advances arg to tokenise input
            redirection(args);                                   //check if IO redirection is needed
            bool check = is_background(args);                   //function to return true or false if background process is envoked
            int store = is_command(arg, args, check);          //find the value of stored using the command function
            if (store == 1){
                fork_exec(args, check);                         //if 1, fork and exec if possible to do so with fork_exec in utility.h
            }
        }
        //process then continues
        continue;
    }
}

//function to find if the user passed an internally made command or if we need to go and execute the args to find a system command
int is_command(char** arg, char** args, bool check){
    if (args[0]){
        if (strcmp(args[0], "quit") == 0){ //if first arg is quit, exit the shell
            Exit_Shell();                  //call exit shell function from utility.h
            return 0;
        }
        else if (strcmp(args[0], "clr") == 0){ //if first arg is clear, clear the screen
            clear_screen();                    //call clear screen function from utility.h
            return 0;
        }
        else if (strcmp(args[0], "dir") == 0){ //if first arg is dir, do an ls
            dir(args, check);                          //call dir function from utility.h
            return 0;
        }
        else if (strcmp(args[0], "echo") == 0){    //if first arg is echo...
            echo(args, check);                            //call echo function from utility.h
            return 0;
        }
        else if(strcmp(args[0], "pause") == 0){                 //if first arg is pause
            pause_mes();                                        //call pause_mes function from utility.h to print messages
            getpass("");                                        //use getpass to return to loop() function when enter is pressed 
            return 0;
        }
        else if (strcmp(args[0], "cd") == 0){   //if first arg is cd
            cd(args);                           //call cd function from utility.h
        }
        else if (strcmp(args[0], "environ") == 0 ){     //if first arg is enviorn
            get_env();                                  //call get_env() to set environment variables
            strcpy(args[0], "env");                     //replace environ with env
            fork_exec(args, check);                      //execute the args, same as a system("env")
            return 0;
        }

        else if (strcmp(args[0], "help") == 0){              //if first arg is help
            char* new_help[3];                               //make a new string called new_help to change the command
            new_help[0] = "more";                           //make the first position the more filter
            new_help[1] = "../manual/readme.md";            //and the second the path to the manual inside the folder
            new_help[2] = NULL;                             //last position must be a NULL character
            fork_exec(new_help, false);                     //call the fork_exec command to execute the command
            return 0;
        }
//all above return 0
        else{
            return 1;   //if none of the above, return 1
        }
    }
}

//function to find if we need to use IO redirection
void redirection(char** args){
    int i = 0;
    while(!(args[i] == NULL)){                  //while there is arguments to check
        if (strcmp(args[i], "<") == 0){         //if input direction found
            inputs(args, i);                    //call inputs function from utility.h
        }
        else if (strcmp(args[i], ">") == 0){    //if output direction found
            outputs(args, i);                   //call outputs function from utility.h
            break;                              //stop the loop, only one output
        }
        else if(strcmp(args[i], ">>") == 0){    //if error direction found
            errors(args, i);                    //call errors function from utility.h
            break;                              //similar to output, stop the loop, only one error output
        }
        i = i + 1;                              //advance through the args
    }
}