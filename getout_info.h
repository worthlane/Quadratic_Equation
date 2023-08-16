#include <stdio.h>

#ifndef QUADRATIC_EQUATION__GET_INFO_H_
#define QUADRATIC_EQUATION__GET_INFO_H_

void Clear_Input(void);					// clearing input from '\n' char and else trash
void Fskip(FILE * fp);					// skipping "coef =" part
void Fget(FILE * fp, float * a);		// getting coefficient from file
bool Get(float * a, char ch);			// getting coefficient from STDIN

void PrintRoots(int roots, float x1, float x2);		// printing roots

enum Roots
{
  ZERO_ROOTS = 0,
  ONE_ROOT = 1,
  TWO_ROOTS = 2,
  INF_ROOTS = 99
};
#endif

