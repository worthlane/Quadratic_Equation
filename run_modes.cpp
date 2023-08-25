#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "solver.h"
#include "getout_info.h"
#include "run_modes.h"

#define STD  "\x1b[0m"
#define CYAN "\x1b[36;1m"
#define GRUN "\x1b[32;1m"
#define RED  "\x1b[31;1m"


/* Test file format:
  a coef | b coef | c coef | roots amn | first root | second root   (first&second roots if they exist)
  __.___   __.___   __.___   _________   ___._______   ___.________   (1 space between numbers)
*/

static int test_number = 1;

void OneTest(const double a, const double b, const double c, const struct QuadSolutions* test)
{
    extern int test_number;
    struct QuadSolutions ans  = {UNDEFINED_ROOTS, NAN, NAN};

    QuadSolver(a, b, c, &ans);              // solving the equation
    if (ans.amount != test->amount)         // comparing the amount of roots
    {
        printf(RED "TEST %d: Incorrect amount of roots\n" STD, test_number++);
        printf(RED "\tTEST: %d, PROGRAM: %d\n" STD, test->amount, ans.amount);
        return;
    }

    if      (test->amount == ZERO_ROOTS ||
             test->amount == INF_ROOTS)     // from this time test and program roots can't be different
    {
        printf(CYAN "TEST %d: Correct\n" STD, test_number++);
    }
    else if (test->amount == ONE_ROOT)
    {
        if (Compare(test->first, ans.first) == EQUAL)           // comparing one root
        {
            printf(CYAN "TEST %d: Correct\n" STD, test_number++);
        }
        else
        {
            printf(RED "TEST %d: Incorrect answer\n" STD, test_number++);
            printf(RED "\tTEST: %lg | PROGRAM: %lg\n" STD, test->first, ans.first);
        }
    }
    else if (test->amount == TWO_ROOTS)
    {
        // comparing two roots
        if ((Compare(test->first, ans.first)  == EQUAL && Compare(test->second, ans.second) == EQUAL) ||
            (Compare(test->first, ans.second) == EQUAL && Compare(test->second, ans.first)  == EQUAL))
        {
            printf(CYAN "TEST %d: Correct\n" STD, test_number++);
        }
        else
        {
            printf(RED "TEST %d: Incorrect answer\n" STD, test_number++);
            printf(RED "\tTEST: %lg %lg | PROGRAM: %lg %lg\n" STD, test->first, test->second, ans.first, ans.second);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------


void RunTest()                                           // runs tests from TEST_FILE
{
    FILE* fp = fopen(TEST_FILE, "r");                    // opens TEST_FILE

    if (!fp)
    {
        PrintError(ErrorList::OPEN_TEST_ERROR, TEST_FILE);
        return;
    }

    static const int param_amount = 6;     // amount of parameters, that program need to get to check is equation answer correct
    double a = NAN;          // a coef initialization
    double b = NAN;          // b coef initialization
    double c = NAN;          // c coef initalization

    struct QuadSolutions test = {UNDEFINED_ROOTS, NAN, NAN};

    while ((fscanf(fp, "%lf%lf%lf%d%lf%lf", &a, &b, &c, &test.amount, &test.first, &test.second)) == param_amount)
        OneTest(a, b, c, &test);

    if (fclose(fp))
        PrintError(ErrorList::CLOSE_TEST_ERROR, TEST_FILE);

    printf(GRUN "Tests runned succesfully\n" STD);
}

//------------------------------------------------------------------------------------------------------------------

void PrintHelp(struct CommandLine* arguments)
{

    printf("Quadratic equation solver\n\n");
    // Type flags
    if (!strcmp(arguments->helpflag,    LONG_INTERACTIVE_FLAG) ||
        !strcmp(arguments->helpflag,    SHORT_INTERACTIVE_FLAG))
    {
        printf("%s, %-25sinteractive type of equation enter (default config)\n",
                SHORT_INTERACTIVE_FLAG, LONG_INTERACTIVE_FLAG);
        return;
        \
    }

    // Input flags
    if (!strcmp(arguments->helpflag,    LONG_FROMFILE_FLAG)    ||
        !strcmp(arguments->helpflag,    SHORT_FROMFILE_FLAG))
    {
        printf("%s, %-25sflag for input from file\n",
                SHORT_FROMFILE_FLAG,    LONG_FROMFILE_FLAG);
        return;
    }

    if (!strcmp(arguments->helpflag,    LONG_CONSOLE_FLAG)     ||
        !strcmp(arguments->helpflag,    SHORT_CONSOLE_FLAG))
    {
        printf("%s a b c, %s a b c    flag for console input (a b c - coefficients)\n",
                SHORT_CONSOLE_FLAG,     LONG_CONSOLE_FLAG);
        return;
    }

    if (!strcmp(arguments->helpflag,    LONG_STDIN_FLAG)       ||
        !strcmp(arguments->helpflag,    SHORT_STDIN_FLAG))
    {
        printf("%s, %-25sflag for input from stdin (default config)\n",
                SHORT_STDIN_FLAG,       LONG_STDIN_FLAG);
        return;
    }

    // Output flags
    if (!strcmp(arguments->helpflag,    LONG_TOFILE_FLAG)      ||
        !strcmp(arguments->helpflag,    SHORT_TOFILE_FLAG))
    {
        printf("%s, %-25sflag for output into file\n",
                SHORT_TOFILE_FLAG,      LONG_TOFILE_FLAG);
        return;
    }

    if (!strcmp(arguments->helpflag,    LONG_STDOUT_FLAG)      ||
        !strcmp(arguments->helpflag,    SHORT_STDOUT_FLAG))
    {
        printf("%s, %-25sflag for output in stdout (default config)\n",
                SHORT_STDOUT_FLAG,      LONG_STDOUT_FLAG);
        return;
    }

    // Mode flags
    if (!strcmp(arguments->helpflag,    LONG_HELP_FLAG)        ||
        !strcmp(arguments->helpflag,    SHORT_HELP_FLAG))
    {
        printf("%s [target], %s [target] calls help and prints information about [target]\n",
                SHORT_HELP_FLAG,        LONG_HELP_FLAG);
        return;
    }

    if (!strcmp(arguments->helpflag,    LONG_TEST_FLAG)        ||
        !strcmp(arguments->helpflag,    SHORT_TEST_FLAG))
    {
        printf("%s, %-25sfor test mode activation (for developers)\n",
                SHORT_TEST_FLAG,        LONG_TEST_FLAG);
        return;
    }

    if (!strcmp(arguments->helpflag,    LONG_SOLVE_FLAG)       ||
        !strcmp(arguments->helpflag,    SHORT_SOLVE_FLAG))
    {
        printf("%s, %-25sfor solve mode activation (default config)\n",
                SHORT_SOLVE_FLAG,       LONG_SOLVE_FLAG);
        return;
    }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    printf("equation format: ax^2 + bx + c = 0\n");

    printf("Type flags:\n");

    printf("%s, %-25sinteractive type of equation enter (default config)\n",
            SHORT_INTERACTIVE_FLAG, LONG_INTERACTIVE_FLAG);

    printf("Input flags:\n");

    printf("%s, %-25sflag for input from stdin (default config)\n",
            SHORT_STDIN_FLAG,       LONG_STDIN_FLAG);

    printf("%s, %-25sflag for input from file\n",
            SHORT_FROMFILE_FLAG,    LONG_FROMFILE_FLAG);

    printf("%s a b c, %s a b c    flag for console input (a b c - coefficients)\n",
            SHORT_CONSOLE_FLAG,     LONG_CONSOLE_FLAG);

    printf("Output flags:\n");

    printf("%s, %-25sflag for output into file\n",
            SHORT_TOFILE_FLAG,      LONG_TOFILE_FLAG);

    printf("%s, %-25sflag for output in stdout (default config)\n",
            SHORT_STDOUT_FLAG,      LONG_STDOUT_FLAG);

    printf("Mode flags:\n");

    printf("%s, %-25sfor solve mode activation (default config)\n",
            SHORT_SOLVE_FLAG,       LONG_SOLVE_FLAG);

    printf("%s, %-25sfor test mode activation (for developers)\n",
            SHORT_TEST_FLAG,        LONG_TEST_FLAG);

    printf("%s [target], %s [target] calls help and prints information about [target]\n",
            SHORT_HELP_FLAG,        LONG_HELP_FLAG);

    printf("\nMade by MKA\n"
           "                                                               08.2023\n\n");
}

//------------------------------------------------------------------------------------------------------------------

// Runs process of solving equation

ErrorList RunSolve(struct Param* param, struct CommandLine* arguments)
{
    assert (param);

    double a = NAN;          // a coef initialization
    double b = NAN;          // b coef initialization
    double c = NAN;          // c coef initalization

    struct QuadSolutions ans = {UNDEFINED_ROOTS, NAN, NAN};                 // answer initialization

    ErrorList read_error = ErrorList::UNKNOWN_ERROR;
    if ((read_error = ReadCoefficients(param, &a, &b, &c, arguments)) != ErrorList::NOT_AN_ERROR)
    {
        if (read_error == ErrorList::USER_QUIT || !RepeatQuestion("try again"))
            return ErrorList::USER_QUIT;
        else
            return ErrorList::NOT_AN_ERROR;
    }

    QuadSolver(a, b, c, &ans);                                              // solves quadratic equation

    char outfile_name[LEN] = "no name";
    FILE *fpout = nullptr;

    if (!strlen(arguments->outfile))
    {
        if (param->output == Param::ToFile)
        {
            if (GetFileName(outfile_name, "output") != ErrorList::NOT_AN_ERROR)
                return ErrorList::GET_FILE_NAME_ERROR;
            fpout = (strcmp(outfile_name, "stdout") == 0) ? stdout : fopen(outfile_name, "a");           // stdout check
        }
        else
            fpout = stdout;
    }
    else
    {
        fpout = fopen(arguments->outfile, "a");
        strncpy(outfile_name, arguments->outfile, LEN);
        memset(arguments->outfile, 0, LEN);
    }

    if (!fpout)
    {
        fpout = stdout;
    }

    PrintRoots(ans.amount, ans.first, ans.second, fpout);
    if (fpout != stdout && fclose(fpout))
        PrintError(ErrorList::CLOSE_OUTPUT_ERROR, outfile_name);
    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList StdinInput(double* a, double* b, double* c, struct FlagInfo* param)
{
    printf("Equation format: ax^2 + bx + c = 0\n");
    while (true)
    {
        if (!GetCoef(a, 'a'))
        {
            if (!RepeatQuestion("try again"))
                return ErrorList::USER_QUIT;
        }
        else
             break;
    }
    while (true)
    {
        if (!GetCoef(b, 'b'))
        {
            if (!RepeatQuestion("try again"))
                return ErrorList::USER_QUIT;
        }
        else
            break;
    }
    while (true)
    {
        if (!GetCoef(c, 'c'))
        {
            if (!RepeatQuestion("try again"))
                return ErrorList::USER_QUIT;
        }
        else
            break;
    }

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList FileInput(double* a, double* b, double* c, struct FlagInfo* param)
{
    FILE* fpin = nullptr;
    static char infile_name[LEN] = "no name";
    if (!strlen(param->argument))
    {
        fpin = OpenInputFile(infile_name);
        if (!fpin)
        {
            PrintError(ErrorList::OPEN_INPUT_ERROR, infile_name);
            return ErrorList::USER_QUIT;
        }
    }
    else
    {
        fpin = fopen(param->argument, "r");
        strncpy(infile_name, param->argument, LEN);
        if (!fpin)
        {
            PrintError(ErrorList::OPEN_INPUT_ERROR, infile_name);
            return ErrorList::OPEN_INPUT_ERROR;
        }
        memset(param->argument, 0, LEN);
    }

    assert (fpin);

    if (!FileGetCoef(fpin, a)) return ErrorList::INVALID_COEF_ERROR;
    if (!FileGetCoef(fpin, b)) return ErrorList::INVALID_COEF_ERROR;
    if (!FileGetCoef(fpin, c)) return ErrorList::INVALID_COEF_ERROR;

    if (fclose(fpin))
        PrintError(ErrorList::CLOSE_INPUT_ERROR, infile_name);

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList ConsoleInput(double* a, double* b, double* c, struct FlagInfo* param)
{
    if (!GetConsole(param->argument, a, b, c)) return ErrorList::READ_CONSOLE_ERROR;

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList StdoutOutput(double* a, double* b, double* c, struct FlagInfo* param)
{
    struct QuadSolutions ans = {};
    QuadSolver(*a, *b, *c, &ans);
    PrintRoots(ans.amount, ans.first, ans.second, stdout);
}

// -----------------------------------------------------------------------------------------

ErrorList FileOutput(double* a, double* b, double* c, struct FlagInfo* param)
{
    struct QuadSolutions ans = {};
    QuadSolver(*a, *b, *c, &ans);

    char outfile_name[LEN] = "no name";
    FILE *fpout = nullptr;

    if (!strlen(param->argument))
    {
        if (GetFileName(outfile_name, "output") != ErrorList::NOT_AN_ERROR)
            return ErrorList::GET_FILE_NAME_ERROR;
        fpout = (strcmp(outfile_name, "stdout") == 0) ? stdout : fopen(outfile_name, "a");
    }
    else
    {
        fpout = fopen(param->argument, "a");
        strncpy(outfile_name, param->argument, LEN);
        memset(param->argument, 0, LEN);
    }
    PrintRoots(ans.amount, ans.first, ans.second, fpout);
    if (fpout != stdout && fclose(fpout))
        PrintError(ErrorList::CLOSE_OUTPUT_ERROR, outfile_name);
    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

void FlagParse(const int argc, const char* argv[], struct FlagInfo* FlagList[],
               struct ProgramCondition* pointers)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            LongFlagCheck(argc, argv, FlagList, pointers, i);
        }
        if (argv[i][0] == '-' && argv[i][1] != '-')
        {
            ShortFlagCheck(argc, argv, FlagList, pointers, i);
        }
    }
}

// -----------------------------------------------------------------------------------------

void LongFlagCheck(const int argc, const char* argv[], struct FlagInfo* FlagList[],
                   struct ProgramCondition* pointers, int i)
{
    for (int flag_ptr = 0; flag_ptr < flag_amount; flag_ptr++)
    {
        if (argv[i] == FlagList[flag_ptr]->LONG_FLAG)
        {
            if (stdin_flag <= flag_ptr && flag_ptr <= console_input_flag)
            {
                if (flag_ptr > pointers->input_ptr)
                {
                    memset(FlagList[pointers->input_ptr]->argument, 0, LEN);
                    pointers->input_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagList, pointers, i, flag_ptr);
                }
            }
            else if (stdout_flag == flag_ptr || flag_ptr == file_output_flag)
            {
                if (flag_ptr > pointers->output_ptr)
                {
                    memset(FlagList[pointers->output_ptr]->argument, 0, LEN);
                    pointers->output_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagList, pointers, i, flag_ptr);
                }
            }
            else
            {
                if (flag_ptr > pointers->mode_ptr)
                {
                    memset(FlagList[pointers->mode_ptr]->argument, 0, LEN);
                    pointers->mode_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagList, pointers, i, flag_ptr);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------------------

void ShortFlagCheck(const int argc, const char* argv[], struct FlagInfo* FlagList[],
                   struct ProgramCondition* pointers, int i)
{
    for (int flag_ptr = 0; flag_ptr < flag_amount; flag_ptr++)
    {
        if (argv[i] == FlagList[flag_ptr]->SHORT_FLAG)
        {
            if (stdin_flag <= flag_ptr && flag_ptr <= console_input_flag)
            {
                if (flag_ptr > pointers->input_ptr)
                {
                    memset(FlagList[pointers->input_ptr]->argument, 0, LEN);
                    pointers->input_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagList, pointers, i, flag_ptr);
                }
            }
            else if (stdout_flag == flag_ptr || flag_ptr == file_output_flag)
            {
                if (flag_ptr > pointers->output_ptr)
                {
                    memset(FlagList[pointers->output_ptr]->argument, 0, LEN);
                    pointers->output_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagList, pointers, i, flag_ptr);
                }
            }
            else
            {
                if (flag_ptr > pointers->mode_ptr)
                {
                    memset(FlagList[pointers->mode_ptr]->argument, 0, LEN);
                    pointers->mode_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagList, pointers, i, flag_ptr);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------------------

void ReadArgument(const int argc, const char* argv[], struct FlagInfo* FlagList[],
                  struct ProgramCondition* pointers, int i, int flag_ptr)
{
    char coef1[LEN] = "";
    char coef2[LEN] = "";
    char coef3[LEN] = "";
    if (flag_ptr == file_input_flag || flag_ptr == file_output_flag)
    {
        if (argv[i + 1][0] != '-')
            strncpy(FlagList[flag_ptr]->argument, argv[i + 1], LEN);
    }
    if (flag_ptr == console_input_flag)
    {
        if (argv[i + 1][0] != '-' && i + 3 < argc)
        {
            strncpy(coef1, argv[i + 1], LEN);
            strncpy(coef2, argv[i + 2], LEN);
            strncpy(coef3, argv[i + 3], LEN);
            TripleString(coef1, coef2, coef3, FlagList[flag_ptr]->argument);
        }
    }
    if (flag_ptr == help_flag)
    {
        if (argv[i + 1][0] == '-')
           strncpy(FlagList[flag_ptr]->argument, argv[i + 1], LEN);
    }

}

