#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "getout_info.h"
#include "solver.h"
#include "colorlib.h"

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

    if (strlen(BUF) >= LEN - 1)              // oversize buffer
    {
        PrintError(ErrorList::BUFF_OVERSIZE_ERROR, nullptr);
        fflush(stdin);
        return ErrorList::BUFF_OVERSIZE_ERROR;
    }

    if (sscanf(BUF, "%s", file_name) == 0)
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
    while ((character = getchar()) != '\n' && character != EOF)         // checking for invalid input
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
    if (fp == stdout)
    {
        switch (roots)
        {
            case ZERO_ROOTS:    PrintCyanText(stdout,
                                              "Your equation has zero roots.\n", "");
                                break;

            case ONE_ROOT:      PrintCyanText(stdout,
                                              "Your equation has one root: x = %lg, and I'm Groot.\n", x1);
                                break;

            case TWO_ROOTS:     PrintCyanText(stdout,
                                              "Your equation has two roots: x1 = %lg, x2 = %lg\n", x1, x2);
                                break;

            case INF_ROOTS:     PrintCyanText(stdout,
                                              "Your equation has infinite amount of roots.\n", "");
                                break;

            default:            PrintError(ErrorList::ROOTS_AMOUNT_ERROR, nullptr);
                                break;
        }
    }
    else
    {
        switch (roots)
        {
            case ZERO_ROOTS:    fprintf(fp,"Your equation has zero roots.\n");
                                break;

            case ONE_ROOT:      fprintf(fp,"Your equation has one root: x = %lg, and I'm Groot.\n", x1);
                                break;

            case TWO_ROOTS:     fprintf(fp,"Your equation has two roots: x1 = %lg, x2 = %lg\n", x1, x2);
                                break;

            case INF_ROOTS:     fprintf(fp,"Your equation has infinite amount of roots.\n");
                                break;

            default:            PrintError(ErrorList::ROOTS_AMOUNT_ERROR, nullptr);
                                break;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------

bool GetConsole(char* string, double* a, double* b, double* c)      // gets coef from console
{
    assert (a);
    assert (b);
    assert (c);

    if (sscanf(string, "%lf%lf%lf", a, b, c) != read_amount)
    {
        PrintError(ErrorList::READ_CONSOLE_ERROR, nullptr);         // invalid coef format
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool RepeatQuestion(const char mode[])          // asks user for repeat
{
    printf("Do you want to %s? (1 - Yes): ", mode);

    int repeat_flag = false;
    scanf("%d", &repeat_flag);
    ClearInput(stdin);

    if (repeat_flag != 1) printf("Bye Bye");    // invalid answer
    return (repeat_flag == 1) ? true : false;
}

//------------------------------------------------------------------------------------------------------------------

void PrintError(ErrorList error, const char file_name[])             // prints errors from errorlist
{
    switch (error)
    {
        case ErrorList::BUFF_OVERSIZE_ERROR: PrintRedText(stderr,
                                                          "ERROR: TOO LONG INPUT (%d)\n",
                                                          ErrorList::BUFF_OVERSIZE_ERROR);
                                             break;

        case ErrorList::FLAG_ERROR:          PrintRedText(stderr,
                                                          "ERROR: UNEXPECTED FLAGS (%d)\n",
                                                          ErrorList::FLAG_ERROR);
                                             break;

        case ErrorList::GET_FILE_NAME_ERROR: PrintRedText(stderr,
                                                          "ERROR: FAILED TO GET FILE NAME (%d)\n",
                                                          ErrorList::GET_FILE_NAME_ERROR);
                                             break;

        case ErrorList::FILE_INPUT_ERROR:    PrintRedText(stderr,
                                                          "ERROR: INCORRECT FILE INPUT (%d)\n",
                                                          ErrorList::FILE_INPUT_ERROR);
                                             break;

        case ErrorList::INVALID_COEF_ERROR:  PrintRedText(stderr,
                                                          "ERROR: INVALID COEFFICIENT (%d)\n",
                                                          ErrorList::INVALID_COEF_ERROR);
                                             break;

        case ErrorList::ROOTS_AMOUNT_ERROR:  PrintRedText(stderr,
                                                          "ERROR: UNEXPECTED AMOUNT OF ROOTS (%d)\n",
                                                          ErrorList::ROOTS_AMOUNT_ERROR);
                                             break;

        case ErrorList::READ_CONSOLE_ERROR:  PrintRedText(stderr,
                                                          "ERROR: FAILED TO READ CONSOLE COEFFFICIENT (%d)\n",
                                                           ErrorList::READ_CONSOLE_ERROR);
                                             break;

        case ErrorList::OPEN_TEST_ERROR:     PrintRedText(stderr,
                                                          "ERROR: FAILED TO OPEN TEST FILE (%d)\n",
                                                          ErrorList::OPEN_TEST_ERROR);
                                             PrintRedText(stderr,
                                                          "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::OPEN_INPUT_ERROR:    PrintRedText(stderr,
                                                          "ERROR: FAILED TO OPEN INPUT FILE (%d)\n",
                                                          ErrorList::OPEN_INPUT_ERROR);
                                             PrintRedText(stderr,
                                                          "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::CLOSE_TEST_ERROR:    PrintYellowText(stderr,
                                                             "WARNING: FAILED TO CLOSE TEST FILE (%d)\n",
                                                             ErrorList::CLOSE_TEST_ERROR);
                                             PrintYellowText(stderr,
                                                             "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::CLOSE_INPUT_ERROR:   PrintYellowText(stderr,
                                                             "WARNING: FAILED TO CLOSE INPUT FILE (%d)\n",
                                                             ErrorList::CLOSE_INPUT_ERROR);
                                             PrintYellowText(stderr,
                                                             "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::OPEN_OUTPUT_ERROR:   PrintYellowText(stderr,
                                                             "WARNING: FAILED TO OPEN OUTPUT FILE (%d)\n",
                                                             ErrorList::OPEN_OUTPUT_ERROR);
                                             PrintYellowText(stderr,
                                                             "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::CLOSE_OUTPUT_ERROR:  PrintYellowText(stderr,
                                                             "WARNING: FAILED TO CLOSE OUTPUT FILE (%d)\n",
                                                             ErrorList::CLOSE_OUTPUT_ERROR);
                                             PrintYellowText(stderr,
                                                             "FILE NAME: \"%s\"\n", file_name);
                                             break;

        case ErrorList::NOT_AN_ERROR:        break;

        default:                             PrintRedText(stderr,
                                                          "ERROR: UNKNOWN ERROR (%d)",
                                                          ErrorList::UNKNOWN_ERROR);
                                             break;
    }
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


