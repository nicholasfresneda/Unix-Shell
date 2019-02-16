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


int get_op(vec* inputVec)
{
    for(int i = 0; i < inputVec->size; i++)
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
    // int location = 0;
    // while(1)
    // {
    //     int curLoc = get_op(inputVec, location);
    //     printf("%d\n", curLoc);
    //     int leftStart = location;
    //     int leftEnd = curLoc -1;
    //     int rightStart = curLoc + 1;
    //     if(nextLoc != -1)
    //     {
    //         int rightEnd = nextLoc - 1;
    //     }
        
         
    //     location = curLoc + 1;
    // }
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
// /**
//  *  Given a vector of inputs, executes the command.
//  */
// void
// execute(vec* input)
// {
//     int cpid;

//     if ((cpid = fork())) {
//         // parent process

//         // Child may still be running until we wait.

//         int status;
//         waitpid(cpid, &status, 0);

//     }
//     else {
//         // child process
        
//         char* program = input->data[0];
//         // The argv array for the child.
//         // Terminated by a null pointer.
//         char* args[input->size]; 
//         for (int i = 0; i < input->size; i++)
//         {
//             args[i] = input->data[i];
//         }   
//         args[input->size] = '\0';

//         execvp(program, args);
//     }
// }

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
        while(1)
        {
            if (fgets(cmd, 256, ptr) == NULL)
            {
                fclose(ptr);
                exit(0);
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
            
            // execute(input);
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

        // if (strcmp("cd", input->data[0]) == 0)
        // {
        //     chdir(input->data[1]);
        //     free_vec(input);
        //     continue;
        // }

        // for(int i = 0; i < input->size; i++)
        // {
        //     printf("%s\n", input->data[i]);
        // }
        shell_ast* ast = convert_to_ast(input);
        evaluate(ast);
        free_ast(ast);
        
        // execute(input);
        free_vec(input);
    }

    return 0;
}
