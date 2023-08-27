#include <stdio.h>
#include <math.h>
#include <string.h>

#include "solver.h"
#include "getout_info.h"
#include "run_modes.h"

// #define DEBUG

int main(const int argc, const char* argv[])
{

    static struct ProgramCondition pointers = {stdin_flag, stdout_flag, solve_flag};

    if (argc != 1)
        FlagParse(argc, argv, FlagList, &pointers);


    #ifdef DEBUG
    printf("%d\n", FindFlag(FlagList[help_flag]->argument, FlagList));
    printf("%d %d %d\n", pointers.input_ptr, pointers.output_ptr, pointers.mode_ptr);
    #endif

    while (true)
    {
        switch (pointers.mode_ptr)
        {
            case help_flag:
                PrintHelp(FlagList);
                if (!Menu(&pointers))
                    return (int) ErrorList::NOT_AN_ERROR;
                else
                {
                    pointers.mode_ptr = solve_flag;
                    continue;
                }
            #ifdef TEST
            case test_flag:
                RunTest();
                return (int) ErrorList::NOT_AN_ERROR;
            #endif
            default:
                break;
        }

        ErrorList run_error = ErrorList::UNKNOWN_ERROR;

        if ((run_error = RunSolve(FlagList, &pointers)) != ErrorList::NOT_AN_ERROR) return (int) run_error;

        if (run_error == ErrorList::USER_QUIT || !Menu(&pointers))
            return (int) ErrorList::USER_QUIT;
        else
            continue;
    }

    printf("Bye Bye\n");
    return (int) ErrorList::NOT_AN_ERROR;
}
