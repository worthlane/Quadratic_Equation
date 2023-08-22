#include <stdio.h>
/*! \file
* \brief Contains functions, that can input and output data. Supports FILE input/output and STDIN input.
*/

static const unsigned int LEN = 100;            ///< maximum length of file names or input strings
static const int read_amount = 3;
/************************************************************//**
 * @brief enums working mode
 ************************************************************/

enum Flags
{
    UnknownFlag  = -1,            ///< UnknownFlag
  
    IntFlag      =  1,            ///< Type flag: interactive         
    StdFlag      =  2,            ///< Type flag: standart
    
    StdinFlag    =  3,            ///< Input flag: stdin input
    FromFileFlag =  4,            ///< Input flag: input from file
    ConsoleFlag  =  5,            ///< Input flag: input from console           

    StdoutFlag   =  6,            ///< Output flag: output in stdout
    ToFileFlag   =  7,            ///< Output flag: output in file

    SolveFlag    =  8,            ///< Mode flag: solving mode
    HelpFlag     =  9,            ///< Mode flag: help mode                
    TestFlag     = 10             ///< Mode flag: test mode          
};

/************************************************************//**
 * @brief enums errors
 ************************************************************/

enum class ErrorList
{
    FlagError        =  1,
    GetFileNameError =  2,
    FileInputError   =  3,
    InvalidCoefError =  4, 
    RootsAmountError =  5,
    ReadConsoleError =  6,
    OpenTestError    =  7,
    CloseTestError   =  8,
    OpenInputError   =  9,
    CloseInputError  = 10,
    CloseOutputError = 11
};

struct Param
{
    int type;
    int input;
    int output;
    int mode;
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
 * @brief Function asks user, does he want to repeat program running
 * 
 * @param[in] mode modifies question
 * @return true if user want to repeat
 * @return false if user want to quit
 ************************************************************/

bool RepeatQuestion(const char mode[]);

/************************************************************//**
 * @brief Reads equation coefficients from input file\n 
 *        Prints answers to output file
 * 
 * @param[in] fpin input file 
 * @param[in] fpout output file (may be STDOUT)
 ************************************************************/

void FileRun(FILE* fpin, FILE* fpout);

/************************************************************//**
 * @brief Prints error
 * 
 * @param[in] error 
 ************************************************************/

int DefineFlag(const char flag[]);

void PrintError(ErrorList error);

void FlagCheck(const int flag, struct Param* param);

void ReadFlags(int argc, char* argv[], struct Param* mode);

bool ReadCoefficients(struct Param* param, double* a, double* b, double* c, char* argv[], int argc);

bool GetConsole(const char string[], double* a);