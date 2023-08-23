#include <stdio.h>

#include "solver.h"
#include "getout_info.h"
#include "run_modes.h"

/*
console flags:
-file : file input and output (stdout in output file for stdout stream)
-int  : interactive stdin input
-test : test mode (for developer)
-help : help mode

no flag - stdin input
*/

int main(const int argc, const char* argv[]) 
{
    int times_run = 1;
    int console_ran = 0;
    double a = NAN_DOUBLE;                                          // coefficients initialization
    double b = NAN_DOUBLE;
    double c = NAN_DOUBLE;

    struct Param param = {IntFlag, StdinFlag, StdoutFlag, SolveFlag};

    struct QuadSolutions  ans     = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};
    
    if (argc != 1)
        ReadFlags(argc, argv, &param);

    while (true) 
    {
        switch (param.mode)
        {
            case HelpFlag:
                PrintHelp();
                if (!RepeatQuestion("solve equation in interactive mode"))        
                    return Success;
                else
                {
                    param.mode = IntFlag;
                    continue;
                }
            case TestFlag:
                RunTest();
                return Success;
            default:
                break;
        }
        if (RunSolve(&param, &a, &b, &c, &ans, argv, argc) == Failure) return Failure;
        if (!RepeatQuestion("solve another equation?"))        
            return Success;
        else
        {
            param.mode = IntFlag;
            continue;
        }
    }
    times_run++;                                                    // solved equations counter
    printf("Bye Bye\n");
    return Success;
}
