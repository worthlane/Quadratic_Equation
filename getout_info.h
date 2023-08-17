#include <stdio.h>

#ifndef QUADRATIC_EQUATION__GET_INFO_H_
#define QUADRATIC_EQUATION__GET_INFO_H_

void ClearInput(void);                            // clearing input from '\n' char and else trash
void FSkipFormatText(FILE * fp);                  // skipping "coef =" part
bool FGetCoef(FILE * fp, float * a);              // getting coefficient from file
bool GetCoef(float * a, const char ch);           // getting coefficient from STDIN

void PrintRoots(int roots, float x1, float x2);   // printing roots

enum Roots
{
  ZERO_ROOTS = 0,
  ONE_ROOT   = 1,
  TWO_ROOTS  = 2,
  INF_ROOTS  = 99
};
#endif

