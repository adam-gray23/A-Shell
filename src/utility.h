/*
Adam Gray

This is my utility file used to hold many of the functions
I used in my main myshell.c file. This helps keep the main
file clear and is easier to find the used function in this file
*/

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
//define max buffer and args to be used
#define MAX_BUFFER 1024
#define MAX_ARGS 64
//global variables to be used for IO Redirection
char* input_file;           //name of input file
char* output_file;          //name of output file
bool file_input = false;    //set the status of a file being passed to false
bool output = false;        //set the status of outputting to a file to false
bool is_error = false;      //set status or returning an error to false

//this function resets the status of the above boolean values so they are returned to false for the next loop
void reset(){
    file_input = false;
    output = false;
    is_error = false;
}

//below functions used to determine IO redirection
//will be used later in the readwrite function called by execArgs

//for input direction...
void inputs(char** args, int i){
    args[i] = NULL;                 //remove the < character
    input_file = args[i + 1];       //the input file is next position
    file_input = true;              //set file_input to true
}

//for output direction
void outputs(char** args, int i){
    args[i] = NULL;                 //remove the > character
    output_file = args[i + 1];      //output file is next position
    output = true;                  //set output to true
}

//for error direction
void errors(char** args, int i){
    args[i] = NULL;                 //remove >> characters
    output_file = args[i + 1];      //output file is still next position
    is_error = true;                //set is_error to true
}

//function to determine if IO redirection is needed
void read_write(){
    if(file_input){
        freopen(input_file, "r", stdin);    //if input then open file for reading and pass to stdin
    }
    if(output){
        freopen(output_file, "w", stdout);  //if out then open output file for writing and write to stdout
    }
    if (is_error){
        freopen(output_file, "w", stdout);  //if error then open output file for writing and write to stdout
    }
}

//function used to fork a child process and execute the arguments passed if a command is found
void fork_exec(char** args, bool check)
{
    // Forking a child
    pid_t pid = fork();                                 //fork and create child process getting the pid

    if (pid == 0) {                                     //if child forks correctly...
        read_write();                                   //check for IO redirection
        if ((execvp(args[0], args) < 0)){
            printf("Not a command...\n");               //if not a command, reset loop and go again
        }
        exit(0);                                        //exit the child process
    }
    else if (pid == -1) {
        printf("Failed forking child process...\n");    //if minus 1, fork failed, so exit
        exit(0);
    }                                                //and exit
    else if(!check){ //check if background execution, if not wait on the child process, else dont
        wait(NULL);                                     // wait for the child process to finish executing
    }
    reset();                                            //reset bool values for input and output in case used
}

//function to get the path
void Path(){
    char path_is[MAX_BUFFER];           //initialise path_is
    getcwd(path_is, sizeof(path_is));   //get the path using getcwd() function to retreive current directory
    printf("%s", path_is);              //print the path
}

//function to set the environment
void get_env(){
    char path_is[MAX_BUFFER];                   //initialise path_is
    getcwd(path_is, sizeof(path_is));           //get cwd
    strcat(path_is, "/myshell");                //add my shell to environment
    setenv("shell", path_is, 1);                //set environment
}

//function to clear screen
void clear_screen(){
    system("clear");                    //clear the screen using system("clear")
}

//function to exit the shell 
void Exit_Shell(){
    printf("Exiting Shell...\n");
    sleep(1);                           //inform user what is happening, while sleeping for 1 second
    clear_screen();                     //then clear screen and exit
    exit(0);
}

//function to list contents of current directory or directory passed
void dir(char** args, bool check){
    char* new_dir[3];                   //make a new string called new_dir to be used to change the command
    new_dir[0] = "ls";
    new_dir[1] = "-al";                 //make the command an ls -al command
    new_dir[2] = NULL;                  //last position must be NULL
    if (!args[1]){                       //if no directory passed          
        fork_exec(new_dir, check);              //execute the args of new_dir, i.e. do an ls -al
    }
    else{
        if (chdir(args[1]) == 0){           //else if directory exists
            chdir(args[1]);                 //change to directory
            fork_exec(new_dir, check);          //ls -al in current directory
            chdir("..");                    //return back to starter directory
        }
        else{
            printf("Could not find Directory %s\n", args[1]);   //print if no directory exists
        }
    }
}

//function to echo a message typed by user
void echo(char** args, bool check){
    fork_exec(args, check);                          //execute the args on echo and the string used, i.e. do an echo command
}

//function to change directory, or print current dir if no arg found 
void cd(char** args){
    char path[MAX_BUFFER];                                      //initialise path
    if (!args[1]){                                              //if no arg 1
        Path();                                                  //print current directory
        printf("\n");
    }
    else if (args[1]){                                          //else
        if (strcmp(args[1], "..") == 0){                        // arg1 is .. then change dir back one
            chdir("..");
        }
        else if (chdir(args[1]) == 0){                          //else change to dir given if possible
            chdir(args[1]);
        }
        else{
            printf("Could not find Directory %s\n", args[1]);   //if not possible, print error
        }
    }
}

//function to check if process should run in the background (&)
bool is_background(char** args){
    int i = 0;
    while(args[i]){                         //loop through the args
        if (strcmp(args[i], "&") == 0){     //if & found set to null
            args[i] = NULL;
            return true;                    //return true
        }
        i = i + 1;                          //otherwise advance in args
    }
    return false;                           //if no &, return false
}

//simple function to print pause command messages
void pause_mes(){
    printf("WARNING!\n");
    printf("Operation of Shell has been Paused\n");     //print messages out for pause command
    printf("Press ENTER when you wish to Continue\n");
}

//simple function to print a quick welcome message to the user
void welcome(){
    clear_screen();
    printf("***********************\n");
    for (int i = 0; i < 3; i++)
    {
        if (i == 1){                                //when i is 1, print messages
            printf("Welcome to A-Shell\n");
            printf("\n");
            printf("Shell is Starting Up\n");
            printf("Please Wait...\n");
            continue;
        }
        else{
            printf("\n");                           //else new line is printed
        }
    }
    
    printf("***********************\n");
    sleep(3);
    clear_screen();                                 //sleep for 3 seconds, then clear the screen
    get_env();                                      //set the environment
}
