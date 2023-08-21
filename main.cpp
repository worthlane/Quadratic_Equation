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

int main(int argc, char* argv[]) 
{
    int times_run = 1;
    int console_ran = 0;
    double a = NAN_DOUBLE;                                          // coefficients initialization
    double b = NAN_DOUBLE;
    double c = NAN_DOUBLE;

    WorkingMode RunMode = ReadFlag(argc, argv);                     // flag reading
    struct QuadSolutions ans = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};

    while (true) 
    {
        switch (RunMode)
        {
            case WorkingMode::TestMode:                                             // test mode
                RunTest();
                return 0;
            case WorkingMode::HelpMode:                                             // help mode
                PrintHelp();
                if (!RepeatQuestion("solve equation in interactive mode"))          // asks user for solve in interactive
                    return 0;
                else
                {
                    RunMode = WorkingMode::IntMode;                                 // changes mode
                    continue;
                }
            case WorkingMode::IntMode:                                              // interactive mode
                if (RunInt(&a, &b, &c, &ans) == Failure)
                    return Failure;
                if (!RepeatQuestion("solve another equation"))                      // asks user to solve equation again
                    return Success;
                else
                    continue;
            case WorkingMode::ConsoleMode:                                          // console input mode
                if (RunConsole(argv, &a, &b, &c, &ans) == Failure)
                    return Failure;
                if (!RepeatQuestion("solve equation in interactive mode"))          // asks user to solve equation in interactive mode
                    return Success;
                else
                {
                    RunMode = WorkingMode::IntMode;                                 // changes mode
                    continue;
                }
            case WorkingMode::FileMode:                                             // file mode
                if (RunFile() == Failure)
                    return Failure;
                if (!RepeatQuestion("solve equation in interactive mode"))          // asks user to solve equation in interactive mode
                    return Success;
            default:
                PrintError(ErrorList::FlagError);
                return Failure;
        }
    }
    times_run++;                                                    // solved equations counter
    printf("Bye Bye\n");
    return Success;
}
