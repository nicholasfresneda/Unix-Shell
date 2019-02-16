#include "asteval.h"
#include "vec.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int  evaluate(shell_ast* ast)
{
    if (ast->op[0] == '=') //not an op
    {
        return execute(ast->cmds);
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
           return handle_or(ast);
        }

        if (strcmp(op, "&") == 0)
        {
           // return i;
        }

        if (strcmp(op, "&&") == 0)
        {
            return handle_and(ast);
        }

        if (strcmp(op, "<") == 0)
        {
            return handle_rin(ast);
        }

        if (strcmp(op, ">") == 0)
        {
           // return i;
        }

        if(strcmp(op, ";") == 0)
        {
            return handle_semi(ast);
        }

        return 1;
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
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }

        return 1;
    }
    else {
        
        char* program = input[0];
    
        execvp(program, input);
    }

    
}



int handle_semi(shell_ast* ast)
{
    int return1, return2 = 0;
    return1 = evaluate(ast->arg0);
    return2 = evaluate(ast->arg1);
    return return1 + return2;
} 

int handle_and(shell_ast* ast)
{
    int return1, return2 = 0;
    return1 = evaluate(ast->arg0);
    if(return1)
    {
        return return1;
    }

    return2 = evaluate(ast->arg1);
    return return1 == 0 && return2 == 0;
}

int handle_or(shell_ast* ast)
{
    int return1, return2 = 0;
    return1 = evaluate(ast->arg0);
    if (!return1)
    {
        return return1;
    }
    return2 = evaluate(ast->arg1);
    return return1 == 0 || return2 == 0;
}

int handle_rin(shell_ast* ast)
{
    return execute_rin(ast->arg0, ast->arg1);
}