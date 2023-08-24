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
 *
 * @param[in] arguments gives info about specific flag
 ************************************************************/

void PrintHelp(struct CommandLine* arguments);

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
 * @return enum Success if program successfully worked
 * @return enum Failure if there was an error while solving program
 ************************************************************/

ErrorList RunSolve(struct Param* param, struct CommandLine* arguments);

void OneTest(const double a, const double b, const double c, const struct QuadSolutions* test);
