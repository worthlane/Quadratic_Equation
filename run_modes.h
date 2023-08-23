/*! \file
* \brief contains funtions declaration, that needed for test mode
*/


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
 * @brief enums result of completed program
 ************************************************************/

enum Complete
{
    Failure  = -1,      ///< program failured
    Success  =  0       ///< program ran successfully
};

/************************************************************//**
 * @brief Runs solving process
 * 
 * @param[in] param params of working equation
 * @param[in] argv console input
 * @param[in] argc amount of console arguments
 * @param[out] a coefficient
 * @param[out] b coefficient
 * @param[out] c coefficient
 * @param[out] ans equation answer
 * @return enum Success if program successfully worked
 * @return enum Failure if there was an error while solving program
 ************************************************************/

int RunSolve(struct Param* param, double* a, double* b, double* c, struct QuadSolutions* ans, const char* argv[], const int argc);

