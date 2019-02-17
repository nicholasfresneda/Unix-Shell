#include "asteval.h"
#include "vec.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
 #include <fcntl.h>



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
            return handle_pipe(ast);
        }

        if (strcmp(op, "||") == 0)
        {
           return handle_or(ast);
        }

        if (strcmp(op, "&") == 0)
        {
            return handle_background(ast);
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
            return handle_rout(ast);
        }

        if(strcmp(op, ";") == 0)
        {
            return handle_semi(ast);
        }

        return 1;
    }
}

/**
 *  Given a vector of inputs, executes the command. Based on lecture notes 
 *  written by Nat Tuck.
 */
int
execute(char** input)
{
    if (strcmp("exit", input[0]) == 0)
    {
        exit(0);
    }
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
    else 
    {
        
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
    int cpid;
    int status; 
    if ((cpid = fork()))
    {
        // parent process
        // Child may still be running until we wait.
        waitpid(cpid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }

        return 1;
    }
    else 
    {
        int fd = open(ast->arg1->cmds[0], O_RDONLY, 0644);
        close(0);
        dup(fd);
        close(fd);
        int returnVal = evaluate(ast->arg0);
        exit(returnVal);
        
    }
}

int handle_rout(shell_ast* ast)
{
    int cpid;
    int status; 
    if ((cpid = fork()))
    {
        // parent process
        // Child may still be running until we wait.
        waitpid(cpid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }

        return 1;
    }
    else 
    {
        int fd = open(ast->arg1->cmds[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        close(1);
        dup(fd);
        close(fd);
        return evaluate(ast->arg0);
        
        
        
    }
}

int handle_pipe(shell_ast* ast)
{
    int cpid;
    int status; 
    if ((cpid = fork()))
    {
        // parent process
        // Child may still be running until we wait.
        waitpid(cpid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }

        return 1;
    }
    else //fork again
    {
        int rv;
        int pipe_fds[2];
        rv = pipe(pipe_fds);
        if (rv == -1)
        {
            exit(1);
        }
        int p_read  = pipe_fds[0];
        int p_write = pipe_fds[1];

        int cpid;
        int status; 
        if ((cpid = fork()))
        {
            // parent process
            // Child may still be running until we wait.
            waitpid(cpid, &status, 0);
            if (WIFEXITED(status))
            {
                if (WEXITSTATUS(status) != 0)
                {
                    exit(WEXITSTATUS(status));
                }
            }

            close(p_write);
            close(0);
            dup(p_read);
            exit(evaluate(ast->arg1));
        
        }
        else 
        {

            close(p_read);
            close(1);
            dup(p_write);
            exit (evaluate(ast->arg0));
            
        }
        
        
    }
}

int handle_background(shell_ast* ast)
{
    int cpid;
    int status; 
    if ((cpid = fork()))
    {
        return 0;
    }
    else 
    {
        int returnVal = evaluate(ast->arg0);
        exit(returnVal);
        
    }
}