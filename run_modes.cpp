#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "solver.h"
#include "getout_info.h"
#include "run_modes.h"
#include "colorlib.h"


/* Test file format:
  a coef | b coef | c coef | roots amn | first root | second root   (first&second roots if they exist)
  __.___   __.___   __.___   _________   ___._______   ___.________   (1 space between numbers)
*/

void OneTest(const double a, const double b, const double c,
             const struct QuadSolutions* test, int* test_number)
{
    struct QuadSolutions ans  = {UNDEFINED_ROOTS, NAN, NAN};

    QuadSolver(a, b, c, &ans);              // solving the equation
    if (ans.amount != test->amount)         // comparing the amount of roots
    {
        PrintRedText(stdout, "TEST %d: Incorrect amount of roots\n", (*test_number)++);
        PrintRedText(stdout, "\tTEST: %d, PROGRAM: %d\n", test->amount, ans.amount);
        return;
    }

    if      (test->amount == ZERO_ROOTS ||
             test->amount == INF_ROOTS)     // from this time test and program roots can't be different
    {
        PrintCyanText(stdout, "TEST %d: Correct\n", (*test_number)++);
    }
    else if (test->amount == ONE_ROOT)
    {
        if (Compare(test->first, ans.first) == EQUAL)           // comparing one root
        {
            PrintCyanText(stdout, "TEST %d: Correct\n", (*test_number)++);
        }
        else
        {
            PrintRedText(stdout, "TEST %d: Incorrect answer\n", (*test_number)++);
            PrintRedText(stdout, "\tTEST: %lg | PROGRAM: %lg\n", test->first, ans.first);
        }
    }
    else if (test->amount == TWO_ROOTS)
    {
        // comparing two roots
        if ((Compare(test->first, ans.first)  == EQUAL && Compare(test->second, ans.second) == EQUAL) ||
            (Compare(test->first, ans.second) == EQUAL && Compare(test->second, ans.first)  == EQUAL))
        {
            PrintCyanText(stdout, "TEST %d: Correct\n", (*test_number)++);
        }
        else
        {
            PrintRedText(stdout, "TEST %d: Incorrect answer\n", (*test_number)++);
            PrintRedText(stdout, "\tTEST: %lg %lg | PROGRAM: %lg %lg\n",
                                 test->first, test->second, ans.first, ans.second);
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

    SkipHeader(fp);
    SkipHeader(fp);

    static const int param_amount = 6;     // amount of parameters, that program need to get to check is equation answer correct
    double a = NAN;                        // a coef initialization
    double b = NAN;                        // b coef initialization
    double c = NAN;                        // c coef initalization

    struct QuadSolutions test = {UNDEFINED_ROOTS, NAN, NAN};

    int test_number = 1;

    while ((fscanf(fp, "%lf%lf%lf%d%lf%lf", &a, &b, &c,
                                            &test.amount, &test.first, &test.second)) == param_amount)
        OneTest(a, b, c, &test, &test_number);

    if (fclose(fp))
        PrintError(ErrorList::CLOSE_TEST_ERROR, TEST_FILE);

    PrintGreenText(stdout, "Tests ran succesfully (%d tests checked)\n", test_number - 1);
}

//------------------------------------------------------------------------------------------------------------------

void PrintHelp(struct FlagInfo* FlagInfo[])                             // prints info about flags
{
    printf(CYAN "Quadratic equation solver\n\n"
           "equation format: ax^2 + bx + c = 0\n"
           "[argument] - flag arguments\n" STD);

    int flag = FindFlag(FlagInfo[help_flag]->argument, FlagInfo);       // user asked info about all flags

    if (flag != unknown_flag)                                           // user asked info about one flag
        PrintGreenText(stdout, "%-4s, %-25s%s",  FlagInfo[flag]->SHORT_FLAG,
                                FlagInfo[flag]->LONG_FLAG, FlagInfo[flag]->help_info);

    else
        for (int i = 0; i < flag_amount; i++)
            PrintGreenText(stdout, "%-4s, %-25s%s" STD, FlagInfo[i]->SHORT_FLAG,
                                    FlagInfo[i]->LONG_FLAG,   FlagInfo[i]->help_info);

    PrintCyanText(stdout, "\nMade by MKA\n"
                          "                                                         08.2023\n\n", "");
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

    char in_argument[LEN] = "";

    strncpy(in_argument, FlagInfo[pointers->input_ptr]->argument, LEN);

    struct Coefficients coefs = {a, b, c, in_argument};

    ErrorList read_error = FlagInfo[pointers->input_ptr]->FlagFunc(&coefs);

    if (read_error != ErrorList::NOT_AN_ERROR)
    {
        if (read_error == ErrorList::USER_QUIT || !RepeatQuestion("try again"))
            return ErrorList::USER_QUIT;
        else
            return ErrorList::NOT_AN_ERROR;
    }
    struct QuadSolutions ans = {};
    char out_argument[LEN] = "";

    strncpy(out_argument, FlagInfo[pointers->output_ptr]->argument, LEN);
    ans.arg = out_argument;

    QuadSolver(coefs.a, coefs.b, coefs.c, &ans);

    ErrorList output_error = FlagInfo[pointers->output_ptr]->FlagFunc(&ans);

    return output_error;
}

// -----------------------------------------------------------------------------------------

ErrorList StdinInput(void* params) // gets coefs from stdin
{
    struct Coefficients* coefs = (struct Coefficients* ) params;

    printf("Equation format: ax^2 + bx + c = 0\n");
    while (!GetCoef(&(coefs->a), 'a'))
    {
        if (!RepeatQuestion("try again"))
            return ErrorList::USER_QUIT;

    }
    while (!GetCoef(&(coefs->b), 'b'))
    {
        if (!RepeatQuestion("try again"))
            return ErrorList::USER_QUIT;

    }
    while (!GetCoef(&(coefs->c), 'c'))
    {
        if (!RepeatQuestion("try again"))
            return ErrorList::USER_QUIT;

    }

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList FileInput(void* params) // gets coefs from file
{
    struct Coefficients* coefs = (struct Coefficients* ) params;

    FILE* fpin = nullptr;
    static char infile_name[LEN] = "no name";
    if (!strlen(coefs->arg))
    {
        fpin = OpenInputFile(infile_name);                          // gets name of input file
        if (!fpin)
        {
            PrintError(ErrorList::OPEN_INPUT_ERROR, infile_name);
            return ErrorList::USER_QUIT;
        }
    }
    else
    {
        fpin = fopen(coefs->arg, "r");                         // input file name is an argument
        strncpy(infile_name, coefs->arg, LEN);
        if (!fpin)
        {
            PrintError(ErrorList::OPEN_INPUT_ERROR, infile_name);
            return ErrorList::OPEN_INPUT_ERROR;
        }
        memset(coefs->arg, 0, LEN);
    }

    assert (fpin);

    if (!FileGetCoef(fpin, &(coefs->a))) return ErrorList::INVALID_COEF_ERROR;
    if (!FileGetCoef(fpin, &(coefs->b))) return ErrorList::INVALID_COEF_ERROR;
    if (!FileGetCoef(fpin, &(coefs->c))) return ErrorList::INVALID_COEF_ERROR;

    if (fclose(fpin))
        PrintError(ErrorList::CLOSE_INPUT_ERROR, infile_name);

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList ConsoleInput(void* params)     // gets coefs from console
{
    struct Coefficients* coefs = (struct Coefficients* ) params;

    if (!GetConsole(coefs->arg, &(coefs->a), &(coefs->b), &(coefs->c)))
        return ErrorList::READ_CONSOLE_ERROR;

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList StdoutOutput(void* params)
{
    struct QuadSolutions* ans = (struct QuadSolutions* ) params;

    PrintRoots(ans->amount, ans->first, ans->second, stdout);
    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

ErrorList FileOutput(void* params)
{
    struct QuadSolutions* ans = (struct QuadSolutions* ) params;

    char outfile_name[LEN] = "no name";
    FILE *fpout = nullptr;

    if (!strlen(ans->arg))
    {
        if (GetFileName(outfile_name, "output") != ErrorList::NOT_AN_ERROR)
            return ErrorList::GET_FILE_NAME_ERROR;
        fpout = (strcmp(outfile_name, "stdout") == 0) ? stdout : fopen(outfile_name, "a");
    }
    else
    {
        fpout = fopen(ans->arg, "a");
        strncpy(outfile_name, ans->arg, LEN);
        memset(ans->arg, 0, LEN);
    }
    PrintRoots(ans->amount, ans->first, ans->second, fpout);
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
        if (argv[i][0] == '-' && argv[i][1] == '-')                 // flag is long
        {
            LongFlagCheck(argc, argv, FlagInfo, pointers, &i);
        }
        if (argv[i][0] == '-' && argv[i][1] != '-')                 // flag is short
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
            if (stdin_flag <= flag_ptr && flag_ptr <= console_input_flag)       // input flag
            {
                if (flag_ptr > pointers->input_ptr)
                {
                    memset(FlagInfo[pointers->input_ptr]->argument, 0, LEN);
                    pointers->input_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, i, flag_ptr);
                }
            }
            else if (stdout_flag == flag_ptr || flag_ptr == file_output_flag)   // output flag
            {
                if (flag_ptr > pointers->output_ptr)
                {
                    memset(FlagInfo[pointers->output_ptr]->argument, 0, LEN);
                    pointers->output_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, i, flag_ptr);
                }
            }
            else
            {
                if (flag_ptr > pointers->mode_ptr)                              // mode flag
                {
                    memset(FlagInfo[pointers->mode_ptr]->argument, 0, LEN);
                    pointers->mode_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, i, flag_ptr);
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
            if (stdin_flag <= flag_ptr && flag_ptr <= console_input_flag)       // input flag
            {
                if (flag_ptr > pointers->input_ptr)
                {
                    memset(FlagInfo[pointers->input_ptr]->argument, 0, LEN);
                    pointers->input_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, i, flag_ptr);
                }
            }
            else if (stdout_flag == flag_ptr || flag_ptr == file_output_flag)   // output flag
            {
                if (flag_ptr > pointers->output_ptr)
                {
                    memset(FlagInfo[pointers->output_ptr]->argument, 0, LEN);
                    pointers->output_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, i, flag_ptr);
                }
            }
            else
            {
                if (flag_ptr > pointers->mode_ptr)                              // mode flag
                {
                    memset(FlagInfo[pointers->mode_ptr]->argument, 0, LEN);
                    pointers->mode_ptr = flag_ptr;
                    ReadArgument(argc, argv, FlagInfo, i, flag_ptr);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------------------

void ReadArgument(const int argc, const char* argv[], struct FlagInfo* FlagInfo[], int* i, int flag_ptr)
{
    char coef1[LEN] = "";
    char coef2[LEN] = "";
    char coef3[LEN] = "";
    if (*i + 1 >= argc)
        return;
    if (flag_ptr == file_input_flag || flag_ptr == file_output_flag)        // input argument
    {
        if (argv[*i + 1][0] != '-')
        {
            strncpy(FlagInfo[flag_ptr]->argument, argv[*i + 1], LEN);
            (*i)++;
        }
    }
    if (flag_ptr == console_input_flag)                                     // output argument
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
    if (flag_ptr == help_flag)                                              // help argument
    {
        if (argv[*i + 1][0] == '-')
        {
           strncpy(FlagInfo[flag_ptr]->argument, argv[*i + 1], LEN);
           (*i)++;
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

// compares flag with flag list and returns enumerated flag number

int FindFlag(const char* flag_name, struct FlagInfo* FlagInfo[])
{
    if (flag_name[0] == '-' && flag_name[1] == '-')
    {
        for (int i = 0; i < flag_amount; i++)
        {
            if (!strncmp(flag_name, FlagInfo[i]->LONG_FLAG, LEN))
                return i;
        }
        return unknown_flag;
    }
    if (flag_name[0] == '-' && flag_name[1] != '-')
    {
        for (int i = 0; i < flag_amount; i++)
        {
            if (!strncmp(flag_name, FlagInfo[i]->SHORT_FLAG, LEN))
                return i;
        }
        return unknown_flag;
    }
    return unknown_flag;
}

// -----------------------------------------------------------------------------------------

inline ErrorList TripleString(char* string1, char* string2, char* string3, char* outstring)
{
    if (strlen(string1) + strlen(string2) + strlen(string3) > LEN)
        return ErrorList::BUFF_OVERSIZE_ERROR;

    strncat(string1, " ", LEN);
    strncat(string2, " ", LEN);

    strncat(outstring, string1, LEN);
    strncat(string2, string3, LEN);

    strncat(outstring, string2, LEN);

    return ErrorList::NOT_AN_ERROR;
}

// -----------------------------------------------------------------------------------------

void SkipHeader(FILE* fp)
{
    int ch = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
}
