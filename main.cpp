#include <stdio.h>
#include <math.h>
#include <string.h>

#include "solver.h"
#include "getout_info.h"
#include "run_modes.h"

#define TEST_MODE

int main(const int argc, const char* argv[])
{
    static struct Param param = {Param::Interactive, Param::Stdin,
                                 Param::Stdout,      Param::Solve}; // program parameters
    static struct CommandLine arguments = {};

    if (argc != 1)
        ReadFlags(argc, argv, &param, &arguments);

    while (true)
    {
        switch (param.mode)
        {
            case Param::Help:

                PrintHelp(&arguments);
                if (!Menu(&param))
                    return (int) ErrorList::NOT_AN_ERROR;
                else
                {
                    param.mode = Param::Solve;
                    continue;
                }

            case Param::Test:
                #ifdef TEST_MODE
                RunTest();
                return Success;
                #endif
            default:
                break;
        }

        ErrorList run_error = ErrorList::UNKNOWN_ERROR;

        if ((run_error = RunSolve(&param, &arguments)) != ErrorList::NOT_AN_ERROR) return (int) run_error;

        if (run_error == ErrorList::USER_QUIT || !Menu(&param))
            return (int) ErrorList::USER_QUIT;
        else
            continue;
    }

    printf("Bye Bye\n");
    return (int) ErrorList::NOT_AN_ERROR;
}
