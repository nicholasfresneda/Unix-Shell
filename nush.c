#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tokens.h"
#include "vec.h"

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

int
main(int argc, char* argv[])
{
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
        vec* input = make_vec();
        tokenize(cmd, input);
        if (strcmp("exit", input->data[0]) == 0)
        {
            exit(0);
        }

        if (strcmp("cd", input->data[0]) == 0)
        {
            chdir(input->data[1]);
            continue;
        }
        execute(input);
    }

    return 0;
}
