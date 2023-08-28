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


