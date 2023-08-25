/*! \file
* \brief contains funtions declaration, that needed for test mode
*/

#define TEST

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

struct ProgramCondition
{
    int input_ptr;
    int output_ptr;
    int mode_ptr;
};

ErrorList StdinInput(double* a, double* b, double* c, struct FlagInfo* param);

ErrorList FileInput(double* a, double* b, double* c, struct FlagInfo* param);

ErrorList ConsoleInput(double* a, double* b, double* c, struct FlagInfo* param);

ErrorList StdoutOutput(double* a, double* b, double* c, struct FlagInfo* param);

ErrorList FileOutput(double* a, double* b, double* c, struct FlagInfo* param);

// ErrorList ConsoleInput(double* a, double* b, double* c, struct FlagInfo* param);

static struct FlagInfo STD_INPUT = {LONG_STDIN_FLAG, SHORT_STDIN_FLAG, nullptr, StdinInput};

static struct FlagInfo FILE_INPUT = {LONG_FROMFILE_FLAG, SHORT_FROMFILE_FLAG, nullptr, FileInput};

static struct FlagInfo CONSOLE_INPUT = {LONG_CONSOLE_FLAG, SHORT_CONSOLE_FLAG, nullptr, ConsoleInput};

static struct FlagInfo STD_OUTPUT = {LONG_STDOUT_FLAG, SHORT_STDOUT_FLAG, nullptr, StdoutOutput};

static struct FlagInfo FILE_OUTPUT = {LONG_TOFILE_FLAG, SHORT_TOFILE_FLAG, nullptr, FileOutput};

static struct FlagInfo SOLVE_MODE = {LONG_SOLVE_FLAG, SHORT_SOLVE_FLAG, nullptr, nullptr};

static struct FlagInfo HELP_MODE = {LONG_HELP_FLAG, SHORT_HELP_FLAG, nullptr, nullptr};
#ifdef TEST
static struct FlagInfo TEST_MODE = {LONG_TEST_FLAG, SHORT_TEST_FLAG, nullptr, nullptr};
static const int flag_amount = 8;
#else
static const int flag_amount = 7;
#endif

enum Flags
{
    stdin_flag = 1,
    file_input_flag = 2,
    console_input_flag = 3,

    stdout_flag = 4,
    file_output_flag = 5,

    solve_flag = 6,
    help_flag = 7,
    #ifdef TEST
    test_flag = 8
    #endif
};

static struct FlagInfo* FlagList[] = {&STD_INPUT, &FILE_INPUT, &CONSOLE_INPUT, &STD_OUTPUT,
                                      &FILE_OUTPUT, &SOLVE_MODE, &HELP_MODE};

void FlagParse(const int argc, const char* argv[], struct FlagInfo* FlagList[],
               struct ProgramCondition* pointers);

void LongFlagCheck(const int argc, const char* argv[], struct FlagInfo* FlagList[],
                   struct ProgramCondition* pointers, int i);

void ShortFlagCheck(const int argc, const char* argv[], struct FlagInfo* FlagList[],
                   struct ProgramCondition* pointers, int i);

void ReadArgument(const int argc, const char* argv[], struct FlagInfo* FlagList[],
                  struct ProgramCondition* pointers, int i, int flag_ptr);
