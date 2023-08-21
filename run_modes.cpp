#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct QuadSolutions* ans = nullptr;

    while ((fscanf(fp, "%lf%lf%lf%d%lf%lf", &a, &b, &c, &test.amount, &test.first, &test.second)) == param_amount)
    {
        ans = QuadSolver(a, b, c);              // solving the equation
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
                printf("\tTEST: %g | PROGRAM: %g\n", test.first, ans->first);
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
                printf("\tTEST: %g %g | PROGRAM: %g %g\n", test.first, test.second, ans->first, ans->second);
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
    printf("default mode: input coefficients in console (./a.out a b c)\n"
           "-file flag for input from file\n"
           "-int  flag for interactive input from stdin\n"
           "-test flag for test mode activation (for developers)\n"
           "-help flag for help\n");
}

//------------------------------------------------------------------------------------------------------------------

int RunInt(double* a, double* b, double* c, struct QuadSolutions* ans)
{
    printf("Equation format: ax^2 + bx + c = 0\n");
    if (!GetCoef(a, 'a'))                                   // getting a coefficient
    {
        return Failure;                                     // failed to get a coefficient
    }
    if (!GetCoef(b, 'b'))                                   // getting b coefficient
    {
        return Failure;                                     // failed to get b coefficient
    }
    if (!GetCoef(c, 'c'))                                   // getting c coefficient
    {
        return Failure;                                     // failed to get c coefficient
    }
    ans = QuadSolver(*a, *b, *c);                           // solving part
    FPrintRoots(ans->amount, ans->first, ans->second, stdout);
    return Success;  
}

//------------------------------------------------------------------------------------------------------------------
int RunConsole(char *argv[], double* a, double* b, double* c, struct QuadSolutions* ans)
{
    if (!ReadConsoleCoef(argv[1], a)) return Failure;           // getting coefs from console
    if (!ReadConsoleCoef(argv[2], b)) return Failure;
    if (!ReadConsoleCoef(argv[3], c)) return Failure;
    ans = QuadSolver(*a, *b, *c);
    FPrintRoots(ans->amount, ans->first, ans->second, stdout);  // printing roots
    return Success;  
}

//------------------------------------------------------------------------------------------------------------------

int RunFile()
{
    char infile_name[LEN] = "no name";                              // file names initialization
    char outfile_name[LEN] = "no name";
    if (!GetFileName(infile_name, "input"))   return Failure;       // getting input file name
    if (!GetFileName(outfile_name, "output")) return Failure;       // getting output file name
    FILE *fpin = fopen(infile_name, "r");
    FILE *fpout = nullptr;
    fpout = (strcmp(outfile_name, "stdout") == 0) ? stdout : fopen(outfile_name, "a");      // stdout check
    if (!fpout)
        fpout = stdout;                                                                     // output file did not open ( O_O )
    if (!fpin)
    {
        PrintError(ErrorList::OpenInputError);
        return Failure;
    } else
    {
        FileRun(fpin, fpout);
        if (fclose(fpin))
            PrintError(ErrorList::CloseInputError);
        if (fpout != stdout && fclose(fpout))
            PrintError(ErrorList::CloseOutputError);
    }
}