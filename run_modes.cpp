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

void PrintHelp(struct FlagInfo* FlagInfo[])
{
    printf(CYAN "Quadratic equation solver\n\n"
           "equation format: ax^2 + bx + c = 0\n"
           "[argument] - flag arguments\n" STD);

    int flag = FindFlag(FlagInfo[help_flag]->argument, FlagInfo);

    if (flag != unknown_flag)
        printf(GRUN "%-4s, %-25s%s" STD, FlagInfo[flag]->SHORT_FLAG,
                FlagInfo[flag]->LONG_FLAG, FlagInfo[flag]->help_info);
    else
        for (int i = 0; i < flag_amount; i++)
            printf(GRUN "%-4s, %-25s%s" STD, FlagInfo[i]->SHORT_FLAG,
                    FlagInfo[i]->LONG_FLAG, FlagInfo[i]->help_info);

    printf(CYAN "\nMade by MKA\n"
           "                                                               08.2023\n\n" STD);
}

//------------------------------------------------------------------------------------------------------------------

// Runs process of solving equation

ErrorList RunSolve(struct FlagInfo* FlagInfo[], struct ProgramCondition* pointers)
{
    assert (FlagInfo);
    assert (pointers);

    double a = NAN;          // a coef initialization
    double b = NAN;          // b coef initialization
    double c = NAN;          // c coef initalization

    ErrorList read_error = FlagInfo[pointers->input_ptr]->FlagFunc(&a, &b, &c,
                                                                   FlagInfo[pointers->input_ptr]);
    if (read_error != ErrorList::NOT_AN_ERROR)
    {
        if (read_error == ErrorList::USER_QUIT || !RepeatQuestion("try again"))
            return ErrorList::USER_QUIT;
        else
            return ErrorList::NOT_AN_ERROR;
    }

    ErrorList output_error = FlagInfo[pointers->output_ptr]->FlagFunc(&a, &b, &c,
                                                                      FlagInfo[pointers->output_ptr]);


    return output_error;
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
    return ErrorList::NOT_AN_ERROR;
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

void FlagParse(const int argc, const char* argv[], struct FlagInfo* FlagInfo[],
               struct ProgramCondition* pointers)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            LongFlagCheck(argc, argv, FlagInfo, pointers, &i);
        }
        if (argv[i][0] == '-' && argv[i][1] != '-')
        {
            ShortFlagCheck(argc, argv, FlagInfo, pointers, &i);
        }
    }
}

// -----------------------------------------------------------------------------------------

void LongFlagCheck(const int argc, const char* argv[], struct FlagInfo* FlagInfo[],
                   struct ProgramCondition* pointers, int* i)
{
    for (int flag_ptr = 0; flag_ptr < flag_amount; flag_ptr++)
    {
        if (!strncmp(argv[*i], FlagInfo[flag_ptr]->LONG_FLAG, LEN))
        {
            if (stdin_flag <= flag_ptr && flag_ptr <= console_input_flag)
            {
                if (flag_ptr > pointers->input_ptr)
                {
                    memset(FlagInfo[pointers->input_ptr]->argument, 0, LEN);
                    pointers->input_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, pointers, i, flag_ptr);
                }
            }
            else if (stdout_flag == flag_ptr || flag_ptr == file_output_flag)
            {
                if (flag_ptr > pointers->output_ptr)
                {
                    memset(FlagInfo[pointers->output_ptr]->argument, 0, LEN);
                    pointers->output_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, pointers, i, flag_ptr);
                }
            }
            else
            {
                if (flag_ptr > pointers->mode_ptr)
                {
                    memset(FlagInfo[pointers->mode_ptr]->argument, 0, LEN);
                    pointers->mode_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, pointers, i, flag_ptr);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------------------

void ShortFlagCheck(const int argc, const char* argv[], struct FlagInfo* FlagInfo[],
                   struct ProgramCondition* pointers, int* i)
{
    for (int flag_ptr = 0; flag_ptr < flag_amount; flag_ptr++)
    {
        if (!strncmp(argv[*i], FlagInfo[flag_ptr]->SHORT_FLAG, LEN))
        {
            if (stdin_flag <= flag_ptr && flag_ptr <= console_input_flag)
            {
                if (flag_ptr > pointers->input_ptr)
                {
                    memset(FlagInfo[pointers->input_ptr]->argument, 0, LEN); // TODO
                    pointers->input_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, pointers, i, flag_ptr);
                }
            }
            else if (stdout_flag == flag_ptr || flag_ptr == file_output_flag)
            {
                if (flag_ptr > pointers->output_ptr)
                {
                    memset(FlagInfo[pointers->input_ptr]->argument, 0, LEN);
                    pointers->output_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, pointers, i, flag_ptr);
                }
            }
            else
            {
                if (flag_ptr > pointers->mode_ptr)
                {
                    memset(FlagInfo[pointers->input_ptr]->argument, 0, LEN);
                    pointers->mode_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, pointers, i, flag_ptr);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------------------

void ReadArgument(const int argc, const char* argv[], struct FlagInfo* FlagInfo[],
                  struct ProgramCondition* pointers, int* i, int flag_ptr)
{
    char coef1[LEN] = "";
    char coef2[LEN] = "";
    char coef3[LEN] = "";
    if (*i + 1 >= argc)
        return;
    if (flag_ptr == file_input_flag || flag_ptr == file_output_flag)
    {
        if (argv[*i + 1][0] != '-')
        {
            strncpy(FlagInfo[flag_ptr]->argument, argv[*i + 1], LEN);
            *i++;
        }
    }
    if (flag_ptr == console_input_flag)
    {
        if (argv[*i + 1][0] != '-' && *i + 3 < argc)
        {
            strncpy(coef1, argv[*i + 1], LEN);
            strncpy(coef2, argv[*i + 2], LEN);
            strncpy(coef3, argv[*i + 3], LEN);
            TripleString(coef1, coef2, coef3, FlagInfo[flag_ptr]->argument);
            *i += 3;
        }
    }
    if (flag_ptr == help_flag)
    {
        if (argv[*i + 1][0] == '-')
        {
           strncpy(FlagInfo[flag_ptr]->argument, argv[*i + 1], LEN);
           *i++;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------

bool Menu(struct ProgramCondition* pointers)      // calls menu
{
    assert (pointers);

    printf("\n"
           "How do you want to continue?\n"
           "Input:\n"
           "(1) STDIN      (2) From file\n"
           "(q) Quit\n");

    int inchoise = 0;

    if (scanf("%d", &inchoise) == 0)
    {
        printf("Bye Bye\n");
        return false;
    }
    else
    {
        if (inchoise == 1)
            pointers->input_ptr = stdin_flag;

        else if (inchoise == 2)
            pointers->input_ptr = file_input_flag;

        else
        {
            printf("Bye Bye\n");
            return false;
        }

    }
    printf("Output:\n"
           "(1) STDOUT      (2) To file\n"
           "(q) Quit\n");

    int outchoise = 0;

    if (scanf("%d", &outchoise) == 0)
    {
        printf("Bye Bye\n");
        return false;
    }
    else
    {
        if (outchoise == 1)
            pointers->output_ptr = stdout_flag;

        else if (outchoise == 2)
            pointers->output_ptr = file_output_flag;

        else
        {
            printf("Bye Bye\n");
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

int FindFlag(const char* flag_name, struct FlagInfo* FlagInfo[])
{
    printf("%s\n", flag_name);
    if (flag_name[0] == '-' && flag_name[1] == '-')
    {
        printf("--\n");
        for (int i = 0; i < flag_amount; i++)
        {
            printf("%d, %s ", i, flag_name);
            if (!strncmp(flag_name, FlagInfo[i]->LONG_FLAG, LEN))
                return i;
        }
        return unknown_flag;
    }
    if (flag_name[0] == '-' && flag_name[1] != '-')
    {
        printf("-\n");
        for (int i = 0; i < flag_amount; i++)
        {
            if (!strncmp(flag_name, FlagInfo[i]->SHORT_FLAG, LEN))
                return i;
        }
        return unknown_flag;
    }
    return unknown_flag;
}
