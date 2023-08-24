#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "getout_info.h"
#include "solver.h"

/* File input format:
 * a1 b1 c1
 * a2 b2 c2      */

static inline int Max(const int a, const int b);

bool GetFileName(char* file_name, const char mode[])                       // getting file name
{
    assert(file_name);

    printf("Please, enter the %s file directory: (q to quit) ", mode);

    if (scanf("%s", file_name) == 0) // TODO
    {
        PrintError(ErrorList::GET_FILE_NAME_ERROR, nullptr);
        return false;
    }

    ClearInput(stdin);

    if (strcmp(file_name, "q") == 0)        // user decided to quit program
    {
        printf("Bye Bye\n");
        return false;
    }

    if (strcmp(mode, "input") == 0)         // case: getting input file name
    {
        printf("Input from: %s\n", file_name);
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool FileGetCoef(FILE* fp, double* a)                          // getting coefficient from file
{
    assert (a);

    if (fscanf(fp, "%lf", a) == 0)
    {
        PrintError(ErrorList::FILE_INPUT_ERROR, nullptr);               // (not "coef =" format)
        fclose(fp);
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

void ClearInput(FILE* fp)                       // clears input from '\n' char and else trash
{
    int ch = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
}

//------------------------------------------------------------------------------------------------------------------

bool GetCoef(double* a, const char ch)                         // gets coefficient from STDIN
{
    assert(a);

    printf("Input coefficient %c: ", ch);

    while (scanf("%lf", a) == 0)
    {
        ClearInput(stdin);

        PrintError(ErrorList::INVALID_COEF_ERROR, nullptr);             // not a coefficient conditions

        return false;
    }
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF)
    {
        if (!isspace(character))
            {
                PrintError(ErrorList::INVALID_COEF_ERROR, nullptr);
                ClearInput(stdin);
                return false;
            }
    }
     //TODO 2371jdshqdh invalid input (isspace)
    //TODO -0 output fix;
    return true;
}

//------------------------------------------------------------------------------------------------------------------

void PrintRoots(const int roots, const double x1, const double x2, FILE* fp)    // prints roots in fp stream
{
    assert (fp);

    switch (roots)
    {
        case ZERO_ROOTS:    fprintf(fp, "Your equation has zero roots.\n");
                            break;

        case ONE_ROOT:      fprintf(fp, "Your equation has one root: x = %lg, and I'm Groot.\n", x1);
                            break;

        case TWO_ROOTS:     fprintf(fp, "Your equation has two roots: x1 = %lg, x2 = %lg\n", x1, x2);
                            break;

        case INF_ROOTS:     fprintf(fp, "Your equation has infinite amount of roots.\n");
                            break;

        default:            PrintError(ErrorList::ROOTS_AMOUNT_ERROR, nullptr);
                            break;
  }
}

//------------------------------------------------------------------------------------------------------------------

bool GetConsole(char string[], double* a)
{
    assert (a);

    if (sscanf(string, "%lf", a) == 0)
    {
        PrintError(ErrorList::READ_CONSOLE_ERROR, nullptr);
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool RepeatQuestion(const char mode[])
{
    printf("Do you want to %s? (1 - Yes): ", mode);

    int repeat_flag = false;
    scanf("%d", &repeat_flag);
    ClearInput(stdin);

    if (!repeat_flag) printf("Bye Bye");
    return repeat_flag;
}

//------------------------------------------------------------------------------------------------------------------

void ReadFlags(const int argc, const char* argv[], struct Param* param, struct CommandLine* arguments)     // reads flags
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')                                                                           // checks all arguments
        {
            ChangeParams(argv[i], param);

            if ((!strcmp(argv[i], LONG_FROMFILE_FLAG) || !strcmp(argv[i], SHORT_FROMFILE_FLAG)) &&
                (i + 1) < argc && (argv[i + 1][0] != '-'))
                strncpy(arguments->infile,         argv[i + 1], LEN);

            if ((!strcmp(argv[i], LONG_TOFILE_FLAG)   || !strcmp(argv[i], SHORT_TOFILE_FLAG))   &&
                (i + 1) < argc && (argv[i + 1][0] != '-'))
                strncpy(arguments->outfile,        argv[i + 1], LEN);

            if ((!strcmp(argv[i], LONG_CONSOLE_FLAG ) || !strcmp(argv[i], SHORT_CONSOLE_FLAG))  &&
                (i + 3) < argc && (argv[i + 1][0] != '-'))
            {
                strncpy(arguments->consolecoef[0], argv[i + 1], LEN);
                strncpy(arguments->consolecoef[1], argv[i + 2], LEN);
                strncpy(arguments->consolecoef[2], argv[i + 3], LEN);
            }
            if ((!strcmp(argv[i], LONG_HELP_FLAG)   || !strcmp(argv[i], SHORT_HELP_FLAG))       &&
                (i + 1) < argc)
                strncpy(arguments->helpflag,       argv[i + 1], LEN);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------

void ChangeParams(const char flag[], struct Param* param)               // defines flag
{
    assert(param);

    // Type flags
    if (!strcmp(flag, LONG_INTERACTIVE_FLAG) ||
        !strcmp(flag, SHORT_INTERACTIVE_FLAG))
        param->type   = Max(param->type,   Param::Interactive);

    // Input flags
    if (!strcmp(flag, LONG_FROMFILE_FLAG)    ||
        !strcmp(flag, SHORT_FROMFILE_FLAG))
        param->input  = Max(param->input,  Param::FromFile);

    if (!strcmp(flag, LONG_CONSOLE_FLAG)     ||
        !strcmp(flag, SHORT_CONSOLE_FLAG))
        param->input  = Max(param->input,  Param::Console);

    if (!strcmp(flag, LONG_STDIN_FLAG)       ||
        !strcmp(flag, SHORT_STDIN_FLAG))
        param->input  = Max(param->input,  Param::Stdin);

    // Output flags
    if (!strcmp(flag, LONG_TOFILE_FLAG)      ||
        !strcmp(flag, SHORT_TOFILE_FLAG))
        param->output = Max(param->output, Param::ToFile);

    if (!strcmp(flag, LONG_STDOUT_FLAG)      ||
        !strcmp(flag, SHORT_STDOUT_FLAG))
        param->output = Max(param->output, Param::Stdout);

    // Mode flags
    if (!strcmp(flag, LONG_HELP_FLAG)        ||
        !strcmp(flag, SHORT_HELP_FLAG))
        param->mode   = Max(param->mode,   Param::Help);

    if (!strcmp(flag, LONG_TEST_FLAG)        ||
        !strcmp(flag, SHORT_TEST_FLAG))
        param->mode   = Max(param->mode,   Param::Test);

    if (!strcmp(flag, LONG_SOLVE_FLAG)       ||
        !strcmp(flag, SHORT_SOLVE_FLAG))
        param->mode   = Max(param->mode,   Param::Solve);
}

//------------------------------------------------------------------------------------------------------------------

void PrintError(ErrorList error, const char file_name[])             // prints errors from errorlist
{
    switch (error)
    {
        case ErrorList::FLAG_ERROR:          perror("ERROR: UNEXPECTED FLAGS");
                                             break;

        case ErrorList::GET_FILE_NAME_ERROR: perror("ERROR: FAILED TO GET FILE NAME");
                                             break;

        case ErrorList::FILE_INPUT_ERROR:    perror("ERROR: INCORRECT FILE INPUT");
                                             break;

        case ErrorList::INVALID_COEF_ERROR:  perror("ERROR: INVALID COEFFICIENT");
                                             break;

        case ErrorList::ROOTS_AMOUNT_ERROR:  perror("ERROR: UNEXPECTED AMOUNT OF ROOTS.");
                                             break;

        case ErrorList::READ_CONSOLE_ERROR:  perror("ERROR: FAILED TO READ CONSOLE COEFFFICIENT");
                                             break;

        case ErrorList::OPEN_TEST_ERROR:     perror("ERROR: FAILED TO OPEN TEST FILE");
                                             fprintf(stderr, "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::OPEN_INPUT_ERROR:    perror("ERROR: FAILED TO OPEN INPUT FILE");
                                             fprintf(stderr, "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::CLOSE_TEST_ERROR:    perror("WARNING: FAILED TO CLOSE TEST FILE");
                                             fprintf(stderr, "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::CLOSE_INPUT_ERROR:   perror("WARNING: FAILED TO CLOSE INPUT FILE");
                                             fprintf(stderr, "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::OPEN_OUTPUT_ERROR:   perror("WARNING: FAILED TO OPEN OUTPUT FILE");
                                             fprintf(stderr, "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::CLOSE_OUTPUT_ERROR:  perror("WARNING: FAILED TO CLOSE OUTPUT FILE");
                                             fprintf(stderr, "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::NOT_AN_ERROR:        break;

        default:                             perror("ERROR: UNKNOWN ERROR");
                                             break;
    }
}

//------------------------------------------------------------------------------------------------------------------

ErrorList ReadCoefficients(struct Param* param, double* a, double* b, double* c, struct CommandLine* arguments)
{
    assert (param);
    assert (a);
    assert (b);
    assert (c);
    assert (a != b);
    assert (b != c);
    assert (a != c);

    switch (param->input)
    {
        case Param::Stdin:                                         // reads coefs from stdin
        {
            printf("Equation format: ax^2 + bx + c = 0\n");

            if (!GetCoef(a, 'a')) return ErrorList::INVALID_COEF_ERROR;
            if (!GetCoef(b, 'b')) return ErrorList::INVALID_COEF_ERROR;
            if (!GetCoef(c, 'c')) return ErrorList::INVALID_COEF_ERROR;

            return ErrorList::NOT_AN_ERROR;
        }

        case Param::Console:                                       // reads coefs from console
        {
            if (!GetConsole(arguments->consolecoef[0], a)) return ErrorList::READ_CONSOLE_ERROR;
            if (!GetConsole(arguments->consolecoef[1], b)) return ErrorList::READ_CONSOLE_ERROR;
            if (!GetConsole(arguments->consolecoef[2], c)) return ErrorList::READ_CONSOLE_ERROR;

            param->input = Param::Stdin;
            return ErrorList::NOT_AN_ERROR;
        }

        case Param::FromFile:                                      // reads coefs from file
        {
            FILE* fpin = nullptr;
            static char infile_name[LEN] = "no name";
            if (!arguments->infile)
            {
                fpin = OpenInputFile(infile_name);
            }
            else
            {
                fpin = fopen(arguments->infile, "r");
                strncpy(infile_name, arguments->infile, LEN);
                if (!fpin)
                {
                    PrintError(ErrorList::OPEN_INPUT_ERROR, infile_name);
                    return ErrorList::OPEN_INPUT_ERROR;
                }
                memset(arguments->infile, 0, LEN);
            }

            assert (fpin);

            if (!FileGetCoef(fpin, a)) return ErrorList::INVALID_COEF_ERROR;
            if (!FileGetCoef(fpin, b)) return ErrorList::INVALID_COEF_ERROR;
            if (!FileGetCoef(fpin, c)) return ErrorList::INVALID_COEF_ERROR;

            if (fclose(fpin))
                PrintError(ErrorList::CLOSE_INPUT_ERROR, infile_name);

            return ErrorList::NOT_AN_ERROR;
        }

        default:
        {
            PrintError(ErrorList::FLAG_ERROR, nullptr);
            return ErrorList::FLAG_ERROR;
        }
    }
    return ErrorList::NOT_AN_ERROR;
}

//------------------------------------------------------------------------------------------------------------------

bool Menu(struct Param* param)      // calls menu
{
    assert (param);

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
            param->input = Param::Stdin;

        else if (inchoise == 2)
            param->input = Param::FromFile;

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
            param->output = Param::Stdout;

        else if (outchoise == 2)
            param->output = Param::ToFile;

        else
        {
            printf("Bye Bye\n");
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

FILE* OpenInputFile(char* infile_name)      // opens input file
{
    assert(infile_name);

    while (true)
    {
        if (!GetFileName(infile_name, "input")) return nullptr;

        FILE* fpin = fopen(infile_name, "r");
        if (!fpin)
        {
            PrintError(ErrorList::OPEN_INPUT_ERROR, infile_name);

            if (RepeatQuestion("try again"))
                continue;
            else
                return nullptr;
        }

        return fpin;
    }
}

// -----------------------------------------------------------------------------------------

static inline int Max(const int a, const int b) // returns maximum of two numbers
{
    return (a > b) ? a : b;
}
