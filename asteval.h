#ifndef AST_EVAL_H
#define AST_EVAL_H


#include "ast.h"
#include "vec.h"

int evaluate(shell_ast* ast);
int execute(char** input);

int handle_semi(shell_ast* ast);
int handle_and(shell_ast* ast);
int handle_or(shell_ast* ast);
int handle_rin(shell_ast* ast);

#endif