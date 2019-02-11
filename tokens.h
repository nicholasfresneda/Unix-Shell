#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec.h"


//returns whether the given character is an operator
int notOp(char op);


//takes a string, tokenizes it, and stores each token in the given vector
void tokenize(char* line, vec * tokens);