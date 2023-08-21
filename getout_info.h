#include <stdio.h>
/*! \file
* \brief Contains functions, that can input and output data. Supports FILE input/output and STDIN input.
*/

/************************************************************//**
 * @brief enums working mode
 ************************************************************/

enum class WorkingMode
{
  UnknownMode = -1,             ///< unknown mode
  HelpMode    =  1,             ///< help mode
  IntMode     =  2,             ///< interactive mode
  TestMode    =  3,             ///< test mode
  ConsoleMode =  4,             ///< input from console mode
  FileMode    =  5              ///< input from file mode
};

/************************************************************//**
 * @brief enums errors
 ************************************************************/

enum class ErrorList
{
    FlagError,
    GetFileNameError,
    FileInputError,
    InvalidCoefError,
    RootsAmountError,
    ReadConsoleError,
    OpenTestError,
    CloseTestError,
    OpenInputError,
    CloseInputError,
    CloseOutputError
};

/************************************************************//**
 * @brief Function clears STDIN from useless symbols, until it meets '\\n'
 * 
 * @param[in] fp file
 ************************************************************/

void ClearInput(FILE* fp);

/************************************************************//**
 * @brief Function skips useless for program text:
 *        "coef ="
 * 
 * @param[in] fp file
 ************************************************************/

void FSkipFormatText(FILE* fp);

/************************************************************//**
 * @brief Function gets coefficient from a file
 * 
 * @param[in] fp file
 * @param[in] a contains coefficient value
 * @return true if function succesfully got the coefficient
 * @return false if file had incorrect coefficient input (program stops)
 ************************************************************/

bool FGetCoef(FILE* fp, double* a);

/************************************************************//**
 * @brief Function gets coefficient from STDIN 
 * 
 * @param[in] a contains coefficient value
 * @param[in] ch shows which coefficient function is getting
 * @return true if function succesfully got the coefficient
 * @return false if user inputed invalid data, and chose to shut down the program
 ************************************************************/

bool GetCoef(double* a, const char ch);

/************************************************************//**
 * @brief Function prints roots in STDOUT.
 * 
 * @param[in] roots amount of roots, that the equation has
 * @param[in] x1 first root (it may be the only root, if "roots" parameter is equal to 1)
 * @param[in] x2 second root (if it exists)
 * @param[in] fp output stream
 ************************************************************/

void FPrintRoots(const int roots, const double x1, const double x2, FILE* fp);

/************************************************************//**
 * @brief Function gets the file name from STDIN stream
 * 
 * @param[in] file_name name of the file
 * @param[in] mode "input"/"output" file type
 * @return true if function got the name succesfully
 * @return false if function did not get name or user exited program
 ************************************************************/

bool GetFileName(char* file_name, const char mode[]);

/************************************************************//**
 * @brief Function read coefficient from the console (console input)
 * 
 * @param[in] argv command line
 * @param[in] a coefficient
 * @return true if coefficient read succesfully
 * @return false if function failed to get coefficient
 ************************************************************/

bool ReadConsoleCoef(const char argv[], double* a);

/************************************************************//**
 * @brief Function asks user, does he want to repeat program running
 * 
 * @param[in] mode modifies question
 * @return true if user want to repeat
 * @return false if user want to quit
 ************************************************************/

bool RepeatQuestion(char mode[]);

/************************************************************//**
 * @brief Reads equation coefficients from input file\n 
 *        Prints answers to output file
 * 
 * @param[in] fpin input file 
 * @param[in] fpout output file (may be STDOUT)
 ************************************************************/

void FileRun(FILE* fpin, FILE* fpout);

/************************************************************//**
 * @brief Returns mode, which program must work
 * 
 * @param[in] argc amount of console arguments
 * @param[in] argv console argument
 * @return WorkingMode 
 ************************************************************/

WorkingMode ReadFlag(int argc, char* argv[]);

/************************************************************//**
 * @brief Prints error
 * 
 * @param[in] error 
 ************************************************************/

void PrintError(ErrorList error);