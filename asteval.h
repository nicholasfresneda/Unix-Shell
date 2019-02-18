#ifndef AST_EVAL_H
#define AST_EVAL_H


#include "ast.h"
#include "vec.h"

//list of functions used to evaluate the shell abstract syntax tree
int evaluate(shell_ast* ast);
int execute(char** input);

int handle_semi(shell_ast* ast);
int handle_and(shell_ast* ast);
int handle_or(shell_ast* ast);
int handle_rin(shell_ast* ast);
int handle_rout(shell_ast* ast);
int handle_pipe(shell_ast* ast);
int handle_background(shell_ast* ast);

#endif