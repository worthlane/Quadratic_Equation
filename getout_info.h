#include <stdio.h>
/*! \file
* \brief Contains functions, that can input and output data. Supports FILE input and STDIN inut.
*        Output stream always is STDOUT.
*/
#ifndef QUADRATIC_EQUATION__GET_INFO_H_
#define QUADRATIC_EQUATION__GET_INFO_H_

/************************************************************//**
 * @brief Function clears buffer from useless symbols, until it meets '\\n'
 * 
 ************************************************************/
void ClearInput(void);
/************************************************************//**
 * @brief Function skips useless for program text:
 *        "coef ="
 * 
 * @param fp file pointer
 ************************************************************/
void FSkipFormatText(FILE * fp);
/************************************************************//**
 * @brief Function gets coefficient from a file
 * 
 * @param fp file pointer
 * @param a pointer to a float number. That number will contain coefficient value
 * @return true if function succesfully got the coefficient
 * @return false if file had incorrect coefficient input (program stops)
 ************************************************************/
bool FGetCoef(FILE * fp, float * a);
/************************************************************//**
 * @brief Function gets coefficient from STDIN 
 * 
 * @param a pointer to a float number. That number will contain coefficient value
 * @param ch const parameter, that shows which coefficient function is getting
 * @return true if function succesfully got the coefficient
 * @return false if user inputed invalid data, and chose to shut down the program
 ************************************************************/
bool GetCoef(float * a, const char ch);
/************************************************************//**
 * @brief Function prints roots in STDOUT.
 * 
 * @param roots amount of roots, that the equation has
 * @param x1 first root (it may be the only root, if "roots" parameter is equal to 1)
 * @param x2 second root (if it exists)
 ************************************************************/
void PrintRoots(int roots, float x1, float x2);

enum Roots {
  ZERO_ROOTS = 0,       ///< equation has no roots
  ONE_ROOT   = 1,       ///< equation has one root
  TWO_ROOTS  = 2,       ///< equation has two different roots
  INF_ROOTS  = 99       ///< equation has infinite amount of roots
};
#endif

