#include "getout_info.h"
#include "solver.h"
#include "testmode.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void RunTest()                                          // runs tests from TEST_FILE
{
    FILE *fp = fopen(TEST_FILE, "r");                   // opens TEST_FILE
    if (!fp)
    {
        printf("ERROR: FAILED TO OPEN TEST FILE\n");
        return;
    }
    const int param_amount = 6;     // amount of parameters, that program need to get to check is equation answer correct
    int test_number = 1;            // test numeration
    float a = NAN;                  // a coef initialization
    float b = NAN;                  // b coef initialization
    float c = NAN;                  // c coef initalization
    struct solve test = {NAN, NAN, NAN};
    struct solve answer = {NAN, NAN, NAN};
    while ((fscanf(fp, "%f%f%f%d%f%f", &a, &b, &c, &test.amount, &test.first, &test.second)) == param_amount)
    {
        answer = *(QuadSolver(a, b, c));        // solving the equation
        if (answer.amount != test.amount)       // comparing the amount of roots
        {
            printf("TEST %d: Incorrect amount of roots\n", test_number++);
            printf("\tTEST: %d, PROGRAM: %d\n", test.amount, answer.amount);
            continue;
        }
        if (test.amount == ZERO_ROOTS || test.amount == INF_ROOTS)   // from this time test and program roots can't be different
        {
            printf("TEST %d: Correct\n", test_number++);
            continue;
        } else if (test.amount == ONE_ROOT)
        {
            if (Compare(test.first, answer.first) == EQUAL)         // comparing one root
            {
                printf("TEST %d: Correct\n", test_number++);
                continue;
            } else 
            {
                printf("TEST %d: Incorrect answer\n", test_number++);
                printf("\tTEST: %g | PROGRAM: %g\n", test.first, answer.first);
                continue;
            }
        } else if (test.amount == TWO_ROOTS)
        {
            if ((Compare(test.first, answer.first) == EQUAL && Compare(test.second, answer.second) == EQUAL) 
             || (Compare(test.first, answer.second) == EQUAL && Compare(test.second, answer.first) == EQUAL))   // comparing two roots
            {
                printf("TEST %d: Correct\n", test_number++);
                continue;
            } else 
            {
                printf("TEST %d: Incorrect answer\n", test_number++);
                printf("\tTEST: %g %g | PROGRAM: %g %g\n", test.first, test.second, answer.first, answer.second);
                continue;
            }
        }
    }
    if (fclose(fp))     // closing test file
    {
        printf("WARNING: FAILED TO CLOSE TEST FILE");
    }
    printf("Tests runned succesfully\n");
    return;
}