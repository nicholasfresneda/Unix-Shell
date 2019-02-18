//CS3650 Challenge Assignment 1
//Nicholas Fresneda



#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tokens.h"
#include "vec.h"
#include "ast.h"
#include "asteval.h"

//returns the location of an operator (starting from the end) in the given vector
int get_op(vec* inputVec)
{
    for(int i = inputVec->size -1; i >= 0; i--)
    {
        if (strcmp(inputVec->data[i], "|") == 0)
        {
            return i;
        }

        if (strcmp(inputVec->data[i], "||") == 0)
        {
            return i;
        }

        if (strcmp(inputVec->data[i], "&") == 0)
        {
            return i;
        }

        if (strcmp(inputVec->data[i], "&&") == 0)
        {
            return i;
        }

        if (strcmp(inputVec->data[i], "<") == 0)
        {
            return i;
        }

        if (strcmp(inputVec->data[i], ">") == 0)
        {
            return i;
        }

        if (strcmp(inputVec->data[i], ";") == 0)
        {
            return i;
        }
    }

    return -1;
}

//slice function written by Nat Tuck, taken from lecture notes 
vec*
slice(vec* xs, int i0, int i1)
{
    vec* ys = make_vec();
    for (int ii = i0; ii < i1; ++ii) {
        push_vec(ys, xs->data[ii]);
    }
    return ys;
}

//function that takes a vector and converts it into an abstract syntax tree,
//originally written by Nat Tuck, taken from lecture notes and modified
shell_ast* convert_to_ast(vec* inputVec)
{

    int opIndex = get_op(inputVec);
    if(opIndex == -1) // no op
    {
        shell_ast* cmdNode = make_ast_cmd(inputVec->data, 0 , inputVec->size);
        return cmdNode;
    }

    vec* left = slice(inputVec, 0, opIndex);
    vec* right = slice(inputVec, opIndex+1, inputVec->size);
    shell_ast* opNode = make_ast_op(inputVec->data[opIndex],
     convert_to_ast(left), convert_to_ast(right));
    
    free_vec(left);
    free_vec(right);
    return opNode;
}

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
 *  Entry point for nush. Runs loop that reads a line of input
 *  and executes it. Terminates upon an 'exit' command or at end of file.
 */
int
main(int argc, char* argv[])
{
    char cmd[256];
    //run the script argument (if there is one)
    if (argc != 1)
    {
        FILE *ptr = fopen(argv[1], "r");
        if (ptr == NULL)
        {
            printf("Incorrect file path!\n");
            exit(1);
        }
        while(1)
        {
            if (fgets(cmd, 256, ptr) == NULL)
            {
                fclose(ptr);
                exit(0);
            }
            fflush(ptr);
            vec* input = make_vec();
            tokenize(cmd, input);
            if (strcmp("exit", input->data[0]) == 0)
            {
                free_vec(input);  
                fclose(ptr); 
                exit(0);
            }

            shell_ast* ast = convert_to_ast(input);
            evaluate(ast);
            free_ast(ast);

            free_vec(input);
        }
        
        fclose(ptr);
        return 0;
    }

    
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

        shell_ast* ast = convert_to_ast(input);
        evaluate(ast);
        free_ast(ast);
        free_vec(input);
    }

    return 0;
}
