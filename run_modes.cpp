#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "solver.h"
#include "getout_info.h"
#include "run_modes.h"

/* Test file format:
  a coef | b coef | c coef | roots amn | first root | second root   (first&second roots if they exist)
  __.___   __.___   __.___   _________   ___._______   ___.________   (1 space between numbers)
*/


void RunTest()                                          // runs tests from TEST_FILE
{
    FILE*fp = fopen(TEST_FILE, "r");                    // opens TEST_FILE
    
    if (!fp)
    {
        PrintError(ErrorList::OpenTestError);
        return;
    }

    const int param_amount = 6;     // amount of parameters, that program need to get to check is equation answer correct
    int test_number = 1;            // test numeration
    double a = NAN_DOUBLE;          // a coef initialization
    double b = NAN_DOUBLE;          // b coef initialization
    double c = NAN_DOUBLE;          // c coef initalization
    struct QuadSolutions test = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};
    struct QuadSolutions init = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};
    struct QuadSolutions* ans = &init;

    while ((fscanf(fp, "%lf%lf%lf%d%lf%lf", &a, &b, &c, &test.amount, &test.first, &test.second)) == param_amount)
    {
        QuadSolver(a, b, c, ans);              // solving the equation
        if (ans->amount != test.amount)         // comparing the amount of roots
        {
            printf("TEST %d: Incorrect amount of roots\n", test_number++);
            printf("\tTEST: %d, PROGRAM: %d\n", test.amount, ans->amount);
            continue;
        }
        if (test.amount == ZERO_ROOTS || test.amount == INF_ROOTS)   // from this time test and program roots can't be different
        {
            printf("TEST %d: Correct\n", test_number++);
            continue;
        } else if (test.amount == ONE_ROOT)
        {
            if (Compare(test.first, ans->first) == EQUAL)           // comparing one root
            {
                printf("TEST %d: Correct\n", test_number++);
                continue;
            } else 
            {
                printf("TEST %d: Incorrect answer\n", test_number++);
                printf("\tTEST: %lg | PROGRAM: %lg\n", test.first, ans->first);
                continue;
            }
        } else if (test.amount == TWO_ROOTS)
        {
            if ((Compare(test.first, ans->first) == EQUAL && Compare(test.second, ans->second) == EQUAL) 
            || (Compare(test.first, ans->second) == EQUAL && Compare(test.second, ans->first) == EQUAL))   // comparing two roots
            {
                printf("TEST %d: Correct\n", test_number++);
                continue;
            } else 
            {
                printf("TEST %d: Incorrect answer\n", test_number++);
                printf("\tTEST: %lg %lg | PROGRAM: %lg %lg\n", test.first, test.second, ans->first, ans->second);
                continue;
            }
        }
    }
    if (fclose(fp)) 
        PrintError(ErrorList::CloseTestError);
    printf("Tests runned succesfully\n");
}

//------------------------------------------------------------------------------------------------------------------

void PrintHelp()
{
    printf("Quadratic equation solver\n\n"
           "equation format: ax^2 + bx + c = 0\n"
           "Type flags:\n"
           "-int            interactive type of equation enter\n"
           "-std            standart type of equation enter (a1x^2 + b1x - a2x^2 + c1 ...) (coming soon)\n"
           "Input flags:\n"
           "-fromfile       flag for input from file\n"
           "-stdin          flag for input from stdin\n"
           "-console a b c  flag for console input (a b c - coefficients)\n"
           "Output flags:\n"
           "-tofile         flag for output into file\n"
           "-stdout         flag for output in stdout\n"
           "Mode flags:\n"
           "-test flag for test mode activation (for developers)\n"
           "-help flag for help\n\n");
}

//------------------------------------------------------------------------------------------------------------------

// Runs process of solving equation

int RunSolve(struct Param* param, double* a, double* b, double* c, struct QuadSolutions* ans, const char* argv[], const int argc)
{
    if (!ReadCoefficients(param, a, b, c, argv, argc)) return Failure;                               // reads coefficients
    QuadSolver(*a, *b, *c, ans);                                                                     // solves quadratic equation
    char outfile_name[LEN] = "no name";
    FILE *fpout = nullptr;
    if (param->output == ToFileFlag && !GetFileName(outfile_name, "output")) return Failure;         // getting output file name
    if (param->output == ToFileFlag)
        fpout = (strcmp(outfile_name, "stdout") == 0) ? stdout : fopen(outfile_name, "a");           // stdout check
    else
        fpout = stdout; 
    FPrintRoots(ans->amount, ans->first, ans->second, fpout);
    if (fpout != stdout && fclose(fpout))
        PrintError(ErrorList::CloseOutputError);
    return Success;
}
