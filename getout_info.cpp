#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "getout_info.h"
#include "solver.h"

/* File input format:
 * a1 b1 c1 
 * a2 b2 c2      */


bool GetFileName(char* file_name, const char mode[])                       // getting file name 
{
    assert(file_name != NULL);

    printf("Please, enter the %s file directory: (q to quit) ", mode);  
    if (scanf("%s", file_name) == 0)
    {
        PrintError(ErrorList::GetFileNameError);
        return false;
    }
    if (strcmp(file_name, "q") == 0)        // user decided to quit program
    {
        printf("Bye Bye\n");
        return false;
    }
    ClearInput(stdin);
    if (strcmp(mode, "input") == 0)         // case: getting input file name
    {
        printf("Input from: %s\n", file_name);
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool FGetCoef(FILE* fp, double* a)                          // getting coefficient from file
{
    assert (a != NULL);

    if (fscanf(fp, "%lf", a) == 0)
    {
        PrintError(ErrorList::FileInputError);              // (not "coef =" format)
        fclose(fp);
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

void FSkipFormatText(FILE* fp)                  // skipping "coef =" part
{
    fgetc(fp);                                  // skipping "coef"
    fgetc(fp);                                  // skipping " "
    fgetc(fp);                                  // skipping "="
}

//------------------------------------------------------------------------------------------------------------------

void ClearInput(FILE* fp)                       // clears input from '\n' char and else trash
{
    int ch = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF);
}

//------------------------------------------------------------------------------------------------------------------

bool GetCoef(double* a, const char ch)                       // gets coefficient from STDIN
{
    assert(a != NULL);

    printf("Input coefficient %c: ", ch);
    while (scanf("%lf", a) == 0)
    {
        PrintError(ErrorList::InvalidCoefError);             // not a coefficient conditions
        printf("Do you want to continue? (1 - Yes): ");
        ClearInput(stdin);
        bool flag = false;
        scanf("%d", &flag);
        if (flag)                                            // user wants to try again
        {
        printf("Input coefficient %c: ", ch);
        continue;
        } else                                               // user doesn't want to continue
        {
            printf("PROGRAM SHUT DOWN");
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

void FPrintRoots(const int roots, const double x1, const double x2, FILE* fp)    // prints roots in fp stream
{
    switch (roots)
    {
        case ZERO_ROOTS:    fprintf(fp, "Your equation has zero roots.\n");
                            break;
        
        case ONE_ROOT:      fprintf(fp, "Your equation has one root: x = %lg\n", x1);
                            break;
        
        case TWO_ROOTS:     fprintf(fp, "Your equation has two roots: x1 = %lg, x2 = %lg\n", x1, x2);
                            break;
        
        case INF_ROOTS:     fprintf(fp, "Your equation has infinite amount of roots.\n");
                            break;
        
        default:            PrintError(ErrorList::RootsAmountError);
                            break;
  }
}

//------------------------------------------------------------------------------------------------------------------

bool GetConsole(const char string[], double* a)
{
    if (sscanf(string, "%lf", a) == 0)
    {
        PrintError(ErrorList::ReadConsoleError);
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool RepeatQuestion(const char mode[])
{
    printf("Do you want to %s? (1 - Yes): ", mode);
    bool repeat_flag = false;
    scanf("%d", &repeat_flag);                                      
    ClearInput(stdin);
    if (!repeat_flag) printf("Bye Bye");
    return repeat_flag;
}

//------------------------------------------------------------------------------------------------------------------

void ReadFlags(const int argc, const char* argv[], struct Param* param)     // reads flags
{ 
    int flag = UnknownFlag;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')                                              // checks all arguments
        {
            flag = DefineFlag(argv[i]);                                     // defines flag
            FlagCheck(flag, param);                                         // compares flag with flag list
        }
    }
}

//------------------------------------------------------------------------------------------------------------------

void FlagCheck(const int flag, struct Param* param)                         // checks flag type and applies it to program
{
    switch (flag)
    {
        case IntFlag:           param->type = Max(param->type, IntFlag);
                                break;
        
        case StdFlag:           param->type = Max(param->type, StdFlag);
                                break;
        
        case StdinFlag:         param->input = Max(param->input, StdinFlag);
                                break;

        case FromFileFlag:      param->input = Max(param->input, FromFileFlag);
                                break;

        case ConsoleFlag:       param->input = Max(param->input, ConsoleFlag);
                                break;

        case StdoutFlag:        param->output = Max(param->output, StdoutFlag);
                                break;

        case ToFileFlag:        param->output = Max(param->output, ToFileFlag);
                                break;

        case SolveFlag:         param->mode = Max(param->mode, SolveFlag);
                                break;

        case HelpFlag:          param->mode = Max(param->mode, HelpFlag);
                                break;

        case TestFlag:          param->mode = Max(param->mode, TestFlag);
                                break;

        default:                break;
    }
}

//------------------------------------------------------------------------------------------------------------------

int DefineFlag(const char flag[])               // defines flag
{
    if (!strcmp(flag, "-int"))
        return IntFlag;

    if (!strcmp(flag, "-fromfile"))
        return FromFileFlag;

    if (!strcmp(flag, "-tofile"))
        return ToFileFlag;

    if (!strcmp(flag, "-help"))
        return HelpFlag;

    if (!strcmp(flag, "-test"))
        return TestFlag;

    if (!strcmp(flag, "-std"))
        return StdFlag;

    if (!strcmp(flag, "-console"))
        return ConsoleFlag;

    if (!strcmp(flag, "-stdin"))
        return StdinFlag;

    if (!strcmp(flag, "-stdout"))
        return StdoutFlag;

    if (!strcmp(flag, "-solve"))
        return SolveFlag;

    return UnknownFlag;
}

//------------------------------------------------------------------------------------------------------------------

void PrintError(ErrorList error)            // prints errors from errorlist
{
    switch (error)
    {
        case ErrorList::FlagError:          perror("ERROR: UNEXPECTED FLAGS");
                                            break;
        
        case ErrorList::GetFileNameError:   perror("ERROR: FAILED TO GET FILE NAME");
                                            break;
        
        case ErrorList::FileInputError:     perror("ERROR: INCORRECT FILE INPUT");
                                            break;
        
        case ErrorList::InvalidCoefError:   perror("ERROR: INVALID COEFFICIENT");
                                            break;
        
        case ErrorList::RootsAmountError:   perror("ERROR: UNEXPECTED AMOUNT OF ROOTS.");
                                            break;
        
        case ErrorList::ReadConsoleError:   perror("ERROR: FAILED TO READ CONSOLE COEFFFICIENT");
                                            break;
        
        case ErrorList::OpenTestError:      perror("ERROR: FAILED TO OPEN TEST FILE");
                                            break;
        
        case ErrorList::OpenInputError:     perror("ERROR: FAILED TO OPEN INPUT FILE");
                                            break;
        
        case ErrorList::CloseTestError:     perror("WARNING: FAILED TO CLOSE TEST FILE");
                                            break;

        case ErrorList::CloseInputError:    perror("WARNING: FAILED TO CLOSE INPUT FILE");
                                            break;
        
        case ErrorList::CloseOutputError:   perror("WARNING: FAILED TO CLOSE OUTPUT FILE");
                                            break;
        
        default:                            perror("ERROR: UNDEFINED ERROR");
                                            break;
    }
}

//------------------------------------------------------------------------------------------------------------------

bool ReadCoefficients(struct Param* param, double* a, double* b, double* c, const char* argv[], const int argc)
{
    switch (param->input)
    {
        case StdinFlag:                                         // reads coefs from stdin
        {    
            printf("Equation format: ax^2 + bx + c = 0\n");
            if (!GetCoef(a, 'a')) return false;                                     
            if (!GetCoef(b, 'b')) return false;                            
            if (!GetCoef(c, 'c')) return false;   
            return true;
        }
        case ConsoleFlag:                                       // reads coefs from console
        {    
            for (int i = 1; i < argc; i++)
            {
                if (DefineFlag(argv[i]) == ConsoleFlag)
                {
                    if ((i + 3) >= argc)
                    {
                        PrintError(ErrorList::ReadConsoleError);
                        return false;
                    }
                    if (!GetConsole(argv[i + 1], a)) return false;
                    if (!GetConsole(argv[i + 2], b)) return false;
                    if (!GetConsole(argv[i + 3], c)) return false;
                    param->input = StdinFlag;
                    return true;
                }
            }
            return false;
        }
        case FromFileFlag:                                      // reads coefs from file
        {    
            char infile_name[LEN] = "no name"; 
            if (!GetFileName(infile_name, "input")) return false;
            FILE* fpin = fopen(infile_name, "r");
            if (!fpin)
            {
                PrintError(ErrorList::OpenInputError);
                return false;
            }
            if (fscanf(fpin, "%lf%lf%lf", a, b, c) != read_amount)
            {
                PrintError(ErrorList::FileInputError);
                return false;
            }
            if (fclose(fpin))
                PrintError(ErrorList::CloseInputError);
            return true;
        }
        default:
        {
            PrintError(ErrorList::FlagError);
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool Menu(struct Param* param)
{
    printf("\n");
    printf("How do you want to continue?\n"
           "Input:\n"
           "(1) STDIN      (2) From file\n"
           "(q) Quit\n");
    int inchoice = 0;
    if (scanf("%d", &inchoice) == 0)
    {
        printf("Bye Bye\n");
        return false;
    }
    else
    {
        if (inchoice == 1)
            param->input = StdinFlag;
        else if (inchoice == 2)
            param->input = FromFileFlag;
        else
        {
            printf("Bye Bye\n");
            return false;
        }
        
    }
    printf("Output:\n"
           "(1) STDOUT      (2) To file\n"
           "(q) Quit\n");
    int outchoice = 0;
    if (scanf("%d", &outchoice) == 0)
    {
        printf("Bye Bye\n");
        return false;
    }
    else
    {
        if (outchoice == 1)
            param->output = StdoutFlag;
        else if (outchoice == 2)
            param->output = ToFileFlag;
        else
        {
            printf("Bye Bye\n");
            return false;
        }
    }
    return true;
}
