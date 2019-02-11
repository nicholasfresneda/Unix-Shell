// CS3650 HW04 Starter Code
// Author: Nat Tuck

#ifndef VEC_H
#define VEC_H

//Changes to vec  class taken from svec.c and svec.h lecture notes
//example written by Nat Tuck
// found at 
//http://www.ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/svec.c
typedef struct vec {
    char** data;
    long size;
    long cap;
} vec;

vec* make_vec();
void free_vec(vec* xs);
void push_vec(vec* xs, char* xx);

#endif
