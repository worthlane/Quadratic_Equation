

#ifndef QUADRATIC_EQUATION__SOLVER_H_
#define QUADRATIC_EQUATION__SOLVER_H_

struct solve
{
  int amount;		// amount of roots
  float first;		// first root
  float second;		// second root
};

struct solve QuadSolver(float a, float n, float c);		// solving equation

#endif
