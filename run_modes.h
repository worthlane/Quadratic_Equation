/*! \file
* \brief contains funtions declaration, that needed for test mode
*/

static const unsigned int LEN = 100;            ///< maximum length of file names or input strings

static const char TEST_FILE[] = "test.txt";     ///< contains test file name

/************************************************************//**
 * @brief Runs tests from TEST_FILE
 ************************************************************/

void RunTest();

/************************************************************//**
 * @brief Prints help information (if -help flag called)
 ************************************************************/

void PrintHelp();

/************************************************************//**
 * @brief Runs program in interactive mod
 * 
 * @param[in] a coefficient 
 * @param[in] b coefficient
 * @param[in] c coefficient
 * @param[out] ans answer 
 * 
 * @return Success if program ran successfully
 * @return Failure if there was an error
 ************************************************************/

int RunInt(double* a, double* b, double* c, struct QuadSolutions* ans);

/************************************************************//**
 * @brief Runs program in console input mode
 * 
 * @param[in] argv command line
 * @param[in] a coefficient
 * @param[in] b coefficient
 * @param[in] c coefficient
 * @param[out] ans answer 
 * 
 * @return Success if program ran successfully
 * @return Failure if there was an error
 ************************************************************/

int RunConsole(char *argv[], double* a, double* b, double* c, struct QuadSolutions* ans);

/************************************************************//**
 * @brief Runs program in file input mode
 * 
 * @return Success if program ran successfully
 * @return Failure if there was an error 
 ************************************************************/

int RunFile();

/************************************************************//**
 * @brief enums result of completed program
 ************************************************************/

enum Complete
{
    Failure  = -1,      ///< program failured
    Success  =  0       ///< program ran successfully
};

