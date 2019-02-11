// CS3650 HW04 Starter Code
// Author: Nat Tuck

#include <stdlib.h>
#include <string.h>
#include "vec.h"

/*
typedef struct vec {
    long* data;
    long size;
    long cap;
} vec;
*/
//Changes to vec  class taken from svec.c and svec.h lecture notes
//example written by Nat Tuck
// found at 
//http://www.ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/svec.c
vec*
make_vec()
{
    vec* xs = malloc(sizeof(vec));
    xs->data = malloc(4 * sizeof(char *));
    xs->size = 0;
    xs->cap  = 4;
    memset(xs->data, 0, 4 * sizeof(char*));
    return xs;
}

void
free_vec(vec* xs)
{
    for (int ii = 0; ii < xs->size; ++ii) {
        if (xs->data[ii] != 0) {
            free(xs->data[ii]);
        }
    }
    free(xs->data);
    free(xs);
}

void
push_vec(vec* xs, char* xx)
{
    if (xs->size >= xs->cap) {
        xs->cap *= 2;
        xs->data = (char**) realloc(xs->data, xs->cap * sizeof(char*));
    }

    xs->data[xs->size] = strdup(xx);
    xs->size += 1;
}

