#include <stdio.h>
/*! \file
* \brief Contains functions, that can input and output data. Supports FILE input/output and STDIN input.
*/
#ifndef QUADRATIC_EQUATION__GET_INFO_H_
#define QUADRATIC_EQUATION__GET_INFO_H_

/************************************************************//**
 * @brief Function clears buffer from useless symbols, until it meets '\\n'
 * 
 ************************************************************/
static inline void ClearInput(void);
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
 * @param fp output stream
 ************************************************************/
void FPrintRoots(int roots, float x1, float x2, FILE * fp);
/************************************************************//**
 * @brief Function gets the file name from STDIN stream
 * 
 * @param file_name name of the file
 * @param mode "input"/"output" file type
 * @return true if function got the name succesfully
 * @return false if function did not get name or user exited program
 ************************************************************/
bool GetFileName(char * file_name, char * mode);
/************************************************************//**
 * @brief Function read coefficient from the console (console input)
 * 
 * @param argv string with coefficient data
 * @param a pointer to coefficient (float type)
 * @return true if coefficient read succesfully
 * @return false if function failed to get coefficient
 ************************************************************/
bool ReadConsoleCoef(char argv[], float * a);
/************************************************************//**
 * @brief Prints help information (if -help flag called)
 * 
 ************************************************************/
void PrintHelp(void);
/************************************************************//**
 * @brief Function asks user, does he want to repeat program running
 * 
 * @return true if user want to repeat
 * @return false if user want to quit
 ************************************************************/
bool RepeatQuestion(void);
/************************************************************//**
 * @brief Reads equation coefficients from input file\n 
 *        Prints answers to output file
 * 
 * @param fpin pointer to input file 
 * @param fpout pointer to output file (may be STDOUT)
 ************************************************************/
void FileRun(FILE * fpin, FILE * fpout);

enum Roots {
  ZERO_ROOTS = 0,       ///< equation has no roots
  ONE_ROOT   = 1,       ///< equation has one root
  TWO_ROOTS  = 2,       ///< equation has two different roots
  INF_ROOTS  = -1       ///< equation has infinite amount of roots
};
#endif

