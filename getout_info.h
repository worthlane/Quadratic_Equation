#include <stdio.h>
/*! \file
* \brief Contains functions, that can input and output data. Supports FILE input/output and STDIN input.
*/

static const unsigned int LEN = 100;            ///< maximum length of file names or input strings
static const int read_amount = 3;               ///< amount of coefs in one file read

/************************************************************//**
 * @brief enums flags
 ************************************************************/

enum Flags
{
    UnknownFlag  = -1,            ///< Unknown Flag
  
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

/************************************************************//**
 * @brief Struct of parameters, which describe solving process
 ************************************************************/

struct Param
{
    int type;           ///< type of solve (interactive or standart)
    int input;          ///< input type (STDIN / file / console)
    int output;         ///< output type (STDOUT / file)
    int mode;           ///< program mode (test / solving / help)
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
 * @brief Prints error
 * 
 * @param[in] error 
 ************************************************************/

void PrintError(ErrorList error);

/************************************************************//**
 * @brief Checks flag type and applies it to program parameters
 * 
 * @param[in] flag flag 
 * @param[in] param struct of program parameters
 ************************************************************/

void FlagCheck(const int flag, struct Param* param);

/************************************************************//**
 * @brief Reads flags from console
 * 
 * @param[in] argc amount of console words
 * @param[in] argv console input
 * @param[out] param parameters of program
 ************************************************************/

void ReadFlags(const int argc, const char* argv[], struct Param* param);

/************************************************************//**
 * @brief Reads coefficients
 * 
 * @param[in] param parameters of program working process (we need input parameter)
 * @param[in] argv console input
 * @param[in] argc amount of console arguments
 * @param[out] a coefficient
 * @param[out] b coefficient
 * @param[out] c coeficient
 * @return true if program successfuly read coefficients
 * @return false if there was an error while reading coefficients
 ************************************************************/

bool ReadCoefficients(struct Param* param, double* a, double* b, double* c, const char* argv[], const int argc);

/************************************************************//**
 * @brief Get the coefficient from console
 * 
 * @param[in] string console argument
 * @param[out] a coefficient
 * @return true if coefficient got successfully
 * @return false if there was an error while getting coefficient
 ************************************************************/

bool GetConsole(const char string[], double* a);

/************************************************************//**
 * @brief Defines flag in flag list
 * 
 * @param[in] flag console argument
 * @return enum of flags 
 ************************************************************/

int DefineFlag(const char flag[]);

/************************************************************//**
 * @brief Asks user to continue and changes program modes
 * 
 * @param[out] param parameters of program
 * @return true if user want to continue
 * @return false if user decided to quit program
 ************************************************************/

bool Menu(struct Param* param);