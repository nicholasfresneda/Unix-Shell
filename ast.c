#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/**
 * This data structure was taken from
 * http://www.ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/ast.c 
 * Originally written by Nat Tuck, modified by Nicholas Fresneda
 */

shell_ast*
make_ast_cmd(char** cmds, int start, int end)
{
    shell_ast* ast = malloc(sizeof(shell_ast));
    ast->op = calloc(3, sizeof(char));
    ast->op[0] = '=';
    ast->arg0 = 0;
    ast->arg1 = 0;
    ast->size = end - start;
    ast->cmds = calloc(ast->size + 1, sizeof(char*));
    for(int i = start; i < end; i++)
    {
        ast->cmds[i] = strdup(cmds[i]);
    }
    
    return ast;
}

shell_ast*
make_ast_op(char* op, shell_ast* a0, shell_ast* a1)
{
    shell_ast* ast = malloc(sizeof(shell_ast));
    ast->op = strdup(op);
    ast->arg0 = a0;
    ast->arg1 = a1;
    //just dont initialize ast->cmds
    return ast;
}

void
free_ast(shell_ast* ast)
{
    if (ast) {
        if (ast->arg0) {
            free_ast(ast->arg0);
        }

        if (ast->arg1) {
            free_ast(ast->arg1);
        }

        if (ast->op[0] == '=')
        {
            for(int i = 0; i <= ast->size; i++)
            {
                free(ast->cmds[i]);
            }
        }
        free(ast->cmds);
        free(ast->op);
        free(ast);
    }
}

// int
// ast_eval(calc_ast* ast)
// {
//     switch (ast->op) {
//     case '=':
//         return ast->value;
//     case '+':
//         return ast_eval(ast->arg0) + ast_eval(ast->arg1);
//     case '-':
//         return ast_eval(ast->arg0) - ast_eval(ast->arg1);
//     case '*':
//         return ast_eval(ast->arg0) * ast_eval(ast->arg1);
//     case '/':
//         return ast_eval(ast->arg0) / ast_eval(ast->arg1);
//     default:
//         fprintf(stderr, "Unknown op: %c\n", ast->op);
//         exit(1);
//     }
// }