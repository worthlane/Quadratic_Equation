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

struct FlagInfo
{
  const char* LONG_FLAG;
  const char* SHORT_FLAG;
  char* argument;
  ErrorList (*FlagFunc) (double* a, double* b, double* c, struct FlagInfo* param);
};

ErrorList StdinInput(double* a, double* b, double* c, struct FlagInfo* param);

ErrorList FileInput(double* a, double* b, double* c, struct FlagInfo* param);

ErrorList ConsoleInput(double* a, double* b, double* c, struct FlagInfo* param);

// ErrorList ConsoleInput(double* a, double* b, double* c, struct FlagInfo* param);

static struct FlagInfo STD_INPUT = {LONG_STDIN_FLAG, SHORT_STDIN_FLAG, nullptr, StdinInput};

static struct FlagInfo FILE_INPUT = {LONG_FROMFILE_FLAG, SHORT_FROMFILE_FLAG, nullptr, FileInput};

static struct FlagInfo CONSOLE_INPUT = {LONG_CONSOLE_FLAG, SHORT_CONSOLE_FLAG, nullptr, nullptr};

static struct FlagInfo STD_OUTPUT = {LONG_STDOUT_FLAG, SHORT_STDOUT_FLAG, nullptr, nullptr};

static struct FlagInfo FILE_OUTPUT = {LONG_TOFILE_FLAG, SHORT_TOFILE_FLAG, nullptr, nullptr};

static struct FlagInfo SOLVE_MODE = {LONG_SOLVE_FLAG, SHORT_SOLVE_FLAG, nullptr, nullptr};

static struct FlagInfo HELP_MODE = {LONG_HELP_FLAG, SHORT_HELP_FLAG, nullptr, nullptr};

static struct FlagInfo TEST_MODE = {LONG_TEST_FLAG, SHORT_TEST_FLAG, nullptr, nullptr};

static struct FlagInfo* FlagList[] = {&STD_INPUT, &FILE_INPUT, &CONSOLE_INPUT, &STD_OUTPUT,
                                      &FILE_OUTPUT, &SOLVE_MODE, &HELP_MODE, &TEST_MODE};
