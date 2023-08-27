#include <stdio.h>

/*! \file
* \brief Contains functions, that can input and output data. Supports FILE input/output and STDIN input.
*/

static const unsigned int LEN = 100;            ///< maximum length of file names or input strings
static const int read_amount = 3;               ///< amount of coefs in one file read

//------------------------------------------------------------------------------------------------------------------

static const char* LONG_INTERACTIVE_FLAG  = "--int";

static const char* LONG_FROMFILE_FLAG     = "--fromfile";

static const char* LONG_CONSOLE_FLAG      = "--console";

static const char* LONG_STDIN_FLAG        = "--stdin";

static const char* LONG_TOFILE_FLAG       = "--tofile";

static const char* LONG_STDOUT_FLAG       = "--stdout";

static const char* LONG_HELP_FLAG         = "--help";

static const char* LONG_TEST_FLAG         = "--test";

static const char* LONG_SOLVE_FLAG        = "--solve";

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

static const char* SHORT_INTERACTIVE_FLAG = "-i";

static const char* SHORT_FROMFILE_FLAG    = "-ff";

static const char* SHORT_CONSOLE_FLAG     = "-c";

static const char* SHORT_STDIN_FLAG       = "-i";

static const char* SHORT_TOFILE_FLAG      = "-tf";

static const char* SHORT_STDOUT_FLAG      = "-o";

static const char* SHORT_HELP_FLAG        = "-h";

static const char* SHORT_TEST_FLAG        = "-tt";

static const char* SHORT_SOLVE_FLAG       = "-s";

//------------------------------------------------------------------------------------------------------------------

static const char* FROMFILE_HELP     = "[file_name] flag for input from file\n";

static const char* CONSOLE_HELP      = "[a b c] flag for console input (a b c - coefficients)\n";

static const char* STDIN_HELP        = "flag for input from stdin (default config)\n";

static const char* TOFILE_HELP       = "[file_name] flag for output into file\n";

static const char* STDOUT_HELP       = "flag for output in stdout (default config)\n";

static const char* HELP_HELP         = "[target] calls help and prints information about [target]\n";

static const char* SOLVE_HELP        = "for solve mode activation (default config)\n";

/************************************************************//**
 * @brief enums errors
 ************************************************************/

enum class ErrorList
{
    USER_QUIT           = -2,

    UNKNOWN_ERROR       = -1,

    NOT_AN_ERROR        =  0,

    FLAG_ERROR          =  1,
    BUFF_OVERSIZE_ERROR =  2,
    GET_FILE_NAME_ERROR =  3,
    FILE_INPUT_ERROR    =  4,
    INVALID_COEF_ERROR  =  5,
    READ_CONSOLE_ERROR  =  6,

    ROOTS_AMOUNT_ERROR  =  7,

    OPEN_TEST_ERROR     =  8,
    CLOSE_TEST_ERROR    =  9,
    OPEN_INPUT_ERROR    = 10,
    CLOSE_INPUT_ERROR   = 11,
    OPEN_OUTPUT_ERROR   = 12,
    CLOSE_OUTPUT_ERROR  = 13
};

/************************************************************//**
 * @brief Function clears STDIN from useless symbols, until it meets '\\n'
 *
 * @param[in] fp file
 ************************************************************/

void ClearInput(FILE* fp);

/************************************************************//**
 * @brief Function gets coefficient from a file
 *
 * @param[in] fp file
 * @param[in] a contains coefficient value
 * @return true if function succesfully got the coefficient
 * @return false if file had incorrect coefficient input (program stops)
 ************************************************************/

bool FileGetCoef(FILE* fp, double* a);

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

void PrintRoots(const int roots, const double x1, const double x2, FILE* fp);

/************************************************************//**
 * @brief Function gets the file name from STDIN stream
 *
 * @param[in] file_name name of the file
 * @param[in] mode "input"/"output" file type
 * @return true if function got the name succesfully
 * @return false if function did not get name or user exited program
 ************************************************************/

ErrorList GetFileName(char* file_name, const char mode[]);

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

void PrintError(ErrorList error, const char file_name[]);

/************************************************************//**
 * @brief Get the Console object
 *
 * @param[in] string console argument
 * @param[out] a coefficient
 * @param[out] b coefficient
 * @param[out] c coefficient
 * @return true if program got the coefficients
 * @return false if there was failure
 ************************************************************/

bool GetConsole(char* string, double* a, double* b, double* c);

/************************************************************//**
 * @brief Opens input file
 *
 * @param[in] infile_name space for input file name
 * @return pointer to file
 ************************************************************/

FILE* OpenInputFile(char* infile_name);


