#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "getout_info.h"
#include "solver.h"

#define RED  "\x1b[31;1m"
#define YEL  "\x1b[33;1m"
#define STD  "\x1b[0m"
#define CYAN "\x1b[36;1m"

/* File input format:
 * a1 b1 c1
 * a2 b2 c2      */

inline int Max(const int a, const int b);

ErrorList GetFileName(char* file_name, const char mode[])                       // getting file name
{
    assert(file_name);

    char BUF[LEN] = {};

    printf("Please, enter the %s file directory: (q to quit) ", mode);

    fgets(BUF, LEN, stdin);

    if (strlen(BUF) >= LEN - 1)
    {
        PrintError(ErrorList::BUFF_OVERSIZE_ERROR, nullptr);
        fflush(stdin);
        return ErrorList::BUFF_OVERSIZE_ERROR;
    }

    if (sscanf(BUF, "%s", file_name) == 0) // TODO
    {
        PrintError(ErrorList::GET_FILE_NAME_ERROR, nullptr);
        return ErrorList::GET_FILE_NAME_ERROR;
    }

    if (strcmp(file_name, "q") == 0)        // user decided to quit program
    {
        printf("Bye Bye\n");
        return ErrorList::USER_QUIT;
    }

    if (strcmp(mode, "input") == 0)         // case: getting input file name
    {
        printf("Input from: %s\n", file_name);
    }

    return ErrorList::NOT_AN_ERROR;
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
    return true;
}

//------------------------------------------------------------------------------------------------------------------

void PrintRoots(const int roots, const double x1, const double x2, FILE* fp)    // prints roots in fp stream
{
    assert (fp);

    switch (roots)
    {
        case ZERO_ROOTS:    fprintf(fp,CYAN "Your equation has zero roots.\n" STD);
                            break;

        case ONE_ROOT:      fprintf(fp,CYAN "Your equation has one root: x = %lg, and I'm Groot.\n" STD, x1);
                            break;

        case TWO_ROOTS:     fprintf(fp,CYAN "Your equation has two roots: x1 = %lg, x2 = %lg\n" STD, x1, x2);
                            break;

        case INF_ROOTS:     fprintf(fp,CYAN "Your equation has infinite amount of roots.\n" STD);
                            break;

        default:            PrintError(ErrorList::ROOTS_AMOUNT_ERROR, nullptr);
                            break;
  }
}

//------------------------------------------------------------------------------------------------------------------

bool GetConsole(char* string, double* a, double* b, double* c)
{
    assert (a);
    assert (b);
    assert (c);

    if (sscanf(string, "%lf%lf%lf", a, b, c) == 0)
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

    if (repeat_flag != 1) printf("Bye Bye");
    return (repeat_flag == 1) ? true : false;
}

//------------------------------------------------------------------------------------------------------------------

void ReadFlags(const int argc, const char* argv[], struct Param* param, struct CommandLine* arguments)     // reads flags
{
    char coef1[LEN] = "";
    char coef2[LEN] = "";
    char coef3[LEN] = "";
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
                strncpy(coef1, argv[i + 1], LEN);
                strncpy(coef2, argv[i + 2], LEN);
                strncpy(coef3, argv[i + 3], LEN);

                TripleString(coef1, coef2, coef3, arguments->consolecoefs);
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
        case ErrorList::BUFF_OVERSIZE_ERROR: perror(RED "ERROR: TOO LONG INPUT" STD);
                                             break;

        case ErrorList::FLAG_ERROR:          perror(RED "ERROR: UNEXPECTED FLAGS" STD);
                                             break;

        case ErrorList::GET_FILE_NAME_ERROR: perror(RED "ERROR: FAILED TO GET FILE NAME" STD);
                                             break;

        case ErrorList::FILE_INPUT_ERROR:    perror(RED "ERROR: INCORRECT FILE INPUT" STD);
                                             break;

        case ErrorList::INVALID_COEF_ERROR:  perror(RED "ERROR: INVALID COEFFICIENT" STD);
                                             break;

        case ErrorList::ROOTS_AMOUNT_ERROR:  perror(RED "ERROR: UNEXPECTED AMOUNT OF ROOTS." STD);
                                             break;

        case ErrorList::READ_CONSOLE_ERROR:  perror(RED "ERROR: FAILED TO READ CONSOLE COEFFFICIENT" STD);
                                             break;

        case ErrorList::OPEN_TEST_ERROR:     perror(RED "ERROR: FAILED TO OPEN TEST FILE" STD);
                                             fprintf(stderr,RED "FILE NAME: \"%s\"\n" STD, file_name);
                                             break;

        case ErrorList::OPEN_INPUT_ERROR:    perror(RED "ERROR: FAILED TO OPEN INPUT FILE" STD);
                                             fprintf(stderr,RED "FILE NAME: \"%s\"\n" STD, file_name);
                                             break;

        case ErrorList::CLOSE_TEST_ERROR:    perror(YEL "WARNING: FAILED TO CLOSE TEST FILE" STD);
                                             fprintf(stderr,YEL "FILE NAME: \"%s\"\n" STD, file_name);
                                             break;

        case ErrorList::CLOSE_INPUT_ERROR:   perror(YEL "WARNING: FAILED TO CLOSE INPUT FILE" STD);
                                             fprintf(stderr,YEL "FILE NAME: \"%s\"\n" STD, file_name);
                                             break;

        case ErrorList::OPEN_OUTPUT_ERROR:   perror(YEL "WARNING: FAILED TO OPEN OUTPUT FILE" STD);
                                             fprintf(stderr,YEL "FILE NAME: \"%s\"\n" STD, file_name);
                                             break;

        case ErrorList::CLOSE_OUTPUT_ERROR:  perror(YEL "WARNING: FAILED TO CLOSE OUTPUT FILE" STD);
                                             fprintf(stderr,YEL "FILE NAME: \"%s\"\n" STD, file_name);
                                             break;

        case ErrorList::NOT_AN_ERROR:        break;

        default:                             perror(RED "ERROR: UNKNOWN ERROR" STD);
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

        case Param::Console:                                       // reads coefs from console
        {
            if (!GetConsole(arguments->consolecoefs, a, b, c)) return ErrorList::READ_CONSOLE_ERROR;

            return ErrorList::NOT_AN_ERROR;
        }

        case Param::FromFile:                                      // reads coefs from file
        {
            FILE* fpin = nullptr;
            static char infile_name[LEN] = "no name";
            if (!strlen(arguments->infile))
            {
                fpin = OpenInputFile(infile_name);
                if (!fpin)
                    return ErrorList::USER_QUIT;
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
        ErrorList get_name = GetFileName(infile_name, "input");
        if (get_name == ErrorList::USER_QUIT) return nullptr;

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

inline int Max(const int a, const int b) // returns maximum of two numbers
{
    return (a > b) ? a : b;
}

// -----------------------------------------------------------------------------------------

void TripleString(char* string1, char* string2, char* string3, char* outstring)
{
    strncat(string1, " ", LEN);
    strncat(string2, " ", LEN);
    strncat(outstring, string1, LEN);
    strncat(string2, string3, LEN);
    strncat(outstring, string2, LEN);
}

