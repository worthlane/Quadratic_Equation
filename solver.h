#ifndef QUADRATIC_EQUATION__SOLVER_H_
#define QUADRATIC_EQUATION__SOLVER_H_

struct solve
{
  int amount;         // amount of roots
  float first;        // first root
  float second;       // second root
};

enum comp
{
  LESS  = -1,
  EQUAL = 0,
  MORE  = 1,
};

int Compare(const float a, const float b);                                // comparing two float numbers with EPSILON accuracy.
/* if a = b: return EQUAL
 * if a > b: return MORE
 * if a < b: return LESS */

struct solve * QuadSolver(const float a, const float n, const float c);   // solving equation


#endif

