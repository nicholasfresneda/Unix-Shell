#include "asteval.h"
#include "vec.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int evaluate(shell_ast* ast)
{
    if (ast->op[0] == '=') //not an op
    {
        execute(ast->cmds);
    }

    else // is an op, call functions for each op
    {
        char* op = ast->op;
        if (strcmp(op, "|") == 0)
        {
            //return i;
        }

        if (strcmp(op, "||") == 0)
        {
           // return i;
        }

        if (strcmp(op, "&") == 0)
        {
           // return i;
        }

        if (strcmp(op, "&&") == 0)
        {
            //return i;
        }

        if (strcmp(op, "<") == 0)
        {
           // return i;
        }

        if (strcmp(op, ">") == 0)
        {
           // return i;
        }

        if(strcmp(op, ";") == 0)
        {
            return handle_semi(ast);
        }
    }
}

/**
 *  Given a vector of inputs, executes the command.
 */
int
execute(char** input)
{
    if (strcmp("cd", input[0]) == 0)
        {
           return chdir(input[1]);
            
        }
    int cpid;

    int status;
    if ((cpid = fork())) {
        // parent process

        // Child may still be running until we wait.

        
        waitpid(cpid, &status, 0);
    }
    else {
        // child process
        int size = sizeof(input) / sizeof(char*);
        char * args[size+1];
        for(int i = 0; i <= size; i++)
        {
            args[i] = input[i];
        }
        args[size+1] = '\0';
        char* program = args[0];
    
        execvp(program, args);
    }


    return status;
}



int handle_semi(shell_ast* ast)
{
    int return1, return2 = 0;
    return1 = evaluate(ast->arg0);
    return2 = evaluate(ast->arg1);
    return return1 + return2;
}