#ifndef SHELL_AST_H
#define SHELL_AST_H

#include <stdlib.h>

/**
 * This data structure was taken from
 * http://www.ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/ast.h
 * Originally written by Nat Tuck, modified by Nicholas Fresneda
 */ 
typedef struct shell_ast {
    char *op;
    // Op is either:
    //  one of: <, >, |, &, &&, ||, or ; for an operation
    //      or: '=' for arg
    struct shell_ast* arg0;
    struct shell_ast* arg1;
    char** cmds;
} shell_ast;

shell_ast* make_ast_cmd(char **cmds, int start, int end);
shell_ast* make_ast_op(char* op, shell_ast* a0, shell_ast* a1);
void free_ast(shell_ast* ast);
int ast_eval(shell_ast* ast);

#endif