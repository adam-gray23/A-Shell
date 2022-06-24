---------------------------


NAME:
    A-Shell


SYNOPSIS:
    ./myshell
    ./myshell [filename]


DESCRIPTION:

    This is a program which mimics the operation of a LINUX Shell
    It supports 8 internal commands, IO redirection, batchfiles,
    background execution and other external LINUX commands


COMMANDS:

    INTERNAL COMMANDS -

        - "quit" : $ quit
            - stops the running of the shell

        - "clr" : $ clr
            - clears the screen for the user

        - "dir" : $ dir
                  $ dir [directory name]
            - given no arguments prints the contents of the current directory
            - given a directory name lists contents of that directory if the directory exists

        - "echo" : $ echo [message]
            - outputs the given message given to the command user for them to see

        - "pause" : $ pause
            - stops the running of the shell until the user presses ENTER

        - "cd" : $ cd
                 $ cd [directory]
            - given no directory it prints the path to the current directory
            - given a directory it changes into the directory if the directory exists

        - "environ" : $ environ
            - outputs the environment strings out to the user

        - "help" : $ help
            - shows the user manual for this C Shell Program (but you aready found this one)

    OTHER COMMANDS -

        FORKING + EXECUTING:

            When the user provides input that is none of the above commands,
            the program forks and creates a child process.
            
            It then sets the current environment and checks
            for IO redirection (described below).
            
            Then using the execvp() function it takes the arguments 
            passed and attempts to execute any possible command given.
            
            If no command is found, exit the child process.
            
            If the fork fails, the Shell provides a suitable
            error and exits the forking process.
            
            Otherwise, if there is no background excecution
            called, wait for child process to finish and exit.


SHELL ENVIRONMENT:
    Environemnt variables are important information that apply to the current
    shell as well as any sub shells or processes created.

    Changing these variables will pass these changes forwards to following
    sub-processes but not back to the parent.

    Any changes made to the environment inside of the shell do not
    impact the environment variables outside of the shell
    
    The environment variables for this shell are gotten by calling
    a system(env) in the is_command() function.

    Before this the shell updates the environment path is updated
    by adding "/myshell" to the path of the current directory


IO REDIRECTION:
    IO Redirection is used to allow the user to decide where stdin comes
    from using "<" and decide where stdout goes using ">". The user can
    also direct error to a file also.

    In this shell, when an input or output symbol is found, that character
    is made NULL and the required file is recognised to be the next position.

    Input:
        - If the user passes input (e.g. python test.py < test.txt) then the program
        will set a variable to true and if true, will open the file for reading.

        - It will then read in the file line by line as stdin.
    
    Output:
        - If the user directs the output to a file (e.g. echo hello > test.txt) the
        program will set the variable to true and if true, will open the output file
        created for reading.
        
        - It will then write the stdout to the required file in the current directory

        - The same is also true for redirecting errors, they will also be written out
        to an output file provided.


BACKGROUND PROCESSES:
    A background process is a process initialised by the user the completes in the background
    (silently) and will finish while potentially allowing other commands to be run.

    In this Shell, a command can be moved to the background using "&".
    
    This will run the command in the background and the result will be
    outputed once the child process is finished running, if no background
    process the command is executed instantly 

REFERENCES:
    For Basic Operating System Concepts:
    Peterson, J.L. and Silberschatz, A., 1985. Operating system concepts. Addison-Wesley Longman Publishing Co., Inc..

    For more information about File IO:
    Love, R., 2013. Linux system programming: talking directly to the kernel and C library. " O'Reilly Media, Inc.".

    For more information about Forking a Child Process:
    Stevens, W.R., Rago, S.A. and Ritchie, D.M., 1992. Advanced programming in the UNIX environment
    (Vol. 4). New York.: Addison-Wesley.

    For more information on Background Execution:
    Sarwar, S.M. and Koretsky, R.M., 2016. UNIX The Textbook, Third Edition. New York.: Chapman and Hall/CRC

AUTHOR:
    Adam Gray
    Last Update: 24/06/2022 17:04pm

---------------------------

