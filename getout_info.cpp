#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "getout_info.h"
#include "solver.h"

/* File input format:
 * a1 b1 c1 
 * a2 b2 c2      */


void FileRun(FILE* fpin, FILE* fpout)
{
    const int read_amount = 3;
    double a = NAN_DOUBLE;                  // coef initialization
    double b = NAN_DOUBLE;
    double c = NAN_DOUBLE;
    int equat_number = 1;                   // equations counter
    struct QuadSolutions ans = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};

    while (fscanf(fpin, "%lf%lf%lf", &a, &b, &c) == read_amount)   // reading data for one equation
    { 
        ans = *(QuadSolver(a, b, c));                               // solving equation
        fprintf(fpout, "%d: ", equat_number++);                     // equation number output
        FPrintRoots(ans.amount, ans.first, ans.second, fpout);
    }
}

//------------------------------------------------------------------------------------------------------------------

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
        printf("Bye Bye");
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

bool GetCoef(double* a, const char ch)                   // gets coefficient from STDIN
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

bool ReadConsoleCoef(const char argv[], double* a)           // reads coefficient from console
{
    assert(a != NULL);

    if (!sscanf(argv, "%lf", a)) 
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

WorkingMode ReadFlag(int argc, char* argv[])
{
    switch (argc)
    {
        case 1:
            return WorkingMode::IntMode;
        case 2:
            if (!strcmp(argv[1], "-int"))
                return WorkingMode::IntMode;
            if (!strcmp(argv[1], "-file"))
                return WorkingMode::FileMode;
            if (!strcmp(argv[1], "-help"))
                return WorkingMode::HelpMode;
            if (!strcmp(argv[1], "-test"))
                return WorkingMode::TestMode;
            else
                return WorkingMode::UnknownMode;
        case 4:
            return WorkingMode::ConsoleMode;
        default:
            return WorkingMode::UnknownMode;
    }
}

//------------------------------------------------------------------------------------------------------------------

void PrintError(ErrorList error)
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
        
        case ErrorList::CloseTestError:     perror("WARNING: FAILED TO CLOSE TEST FILE");
                                            break;
        
        case ErrorList::OpenInputError:     perror("ERROR: FAILED TO OPEN INPUT FILE");
                                            break;
        
        case ErrorList::CloseInputError:    perror("WARNING: FAILED TO CLOSE INPUT FILE");
                                            break;
        
        case ErrorList::CloseOutputError:   perror("WARNING: FAILED TO CLOSE OUTPUT FILE");
                                            break;
        
        default:                            perror("ERROR: UNDEFINED ERROR");
                                            break;
    }
}
