#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tokens.h"
#include "vec.h"

void execute_arg(char * argv[])
{
    int cpid;
        if ((cpid = fork())) 
        {
            int status;
            waitpid(cpid, &status, 0);
        }
        else
        {
            char* arg[64];
            memcpy(arg, &argv[1], sizeof(char*) * sizeof(*argv));
            execvp(argv[1], arg);
        }
}
/**
 *  Given a vector of inputs, executes the command.
 */
void
execute(vec* input)
{
    int cpid;

    if ((cpid = fork())) {
        // parent process

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

    }
    else {
        // child process
        
        char* program = input->data[0];
        // The argv array for the child.
        // Terminated by a null pointer.
        char* args[input->size]; 
        for (int i = 0; i < input->size; i++)
        {
            args[i] = input->data[i];
        }   
        args[input->size] = '\0';

        execvp(program, args);
    }
}

/**
 *  Entry point for nush. Runs loop that reads a line of input
 *  and executes it. Terminates upon an 'exit' command or at end of file.
 */
int
main(int argc, char* argv[])
{
    //run the script argument (if there is one)
    if (argc != 1)
    {
        execute_arg(argv);
    }

    char cmd[256];
    while (1)
    {
        printf("nush$ ");
        fflush(stdout);
        if (fgets(cmd, 256, stdin) == NULL)
        {
            printf("\n");
            exit(0);
        }

        if(cmd[0] == '\n')
        {
            continue;
        }

        vec* input = make_vec();
        tokenize(cmd, input);
        if (strcmp("exit", input->data[0]) == 0)
        {
            free_vec(input);
            exit(0);
        }

        if (strcmp("cd", input->data[0]) == 0)
        {
            chdir(input->data[1]);
            free_vec(input);
            continue;
        }

        
        
        execute(input);
        printf("\n");
        fflush(stdout);
        free_vec(input);
    }

    return 0;
}
