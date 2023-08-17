#ifndef QUADRATIC_EQUATION__SOLVER_H_
#define QUADRATIC_EQUATION__SOLVER_H_
/*! \file
 * \brief Contains functions for solving equations
 *
 * supports solving quadratic and linear equations
 */

/************************************************************//**
 * @brief Struct contains parameters of answers of the solved equation
 * 
 ************************************************************/
struct solve
{
  int amount;         ///< amount of roots
  float first;        ///< first root
  float second;       ///< second root
};
/************************************************************//**
 * @brief  enums comparison outcomes (a \/ b)
 * 
 ************************************************************/
enum comp
{
  LESS  = -1,         ///< a < b
  EQUAL = 0,          ///< a = b
  MORE  = 1,          ///< a > b
};
/************************************************************//**
 * @brief Function compares two float numbers, with EPSILON (1e-6) accuracy
 * 
 * @param a first number
 * @param b second number
 * @return EQUAL if a = b
 * @return MORE  if a > b
 * @return LESS  if a < b
 ************************************************************/
int Compare(const float a, const float b);
/************************************************************//**
 * @brief  Solves quadratic equation.
 *         Format: ax^2 + bx + c = 0
 * @param a coefficient
 * @param b coefficient
 * @param c coefficient
 * @return pointer to structure, which contains: amount of roots, first root(if it exists), second root(if it exists)
 ************************************************************/
struct solve * QuadSolver(const float a, const float b, const float c);   // solving equation


#endif

