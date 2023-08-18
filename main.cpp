#include "getout_info.h"
#include "solver.h"
#include "testmode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// g++ main.cpp getout_info.cpp solver.cpp testmode.cpp
// file:///Users/amigo/Documents/GitHub/Quadratic_Equation/doc/html/index.html
#define LEN 100
/* File input format:
 * a1 b1 c1 
 * a2 b2 c2      */

/* Test file format:
  a coef | b coef | c coef | roots amn | first root | second root   (first&second roots if they exist)
  __.___   __.___   __.___   _________   ___._______   ___.________   (1 space between numbers)
*/

/*
console flags:
-file : file input and output (stdout in output file for stdout stream)
-std  : stdin input
-test : test mode (for developer)
-help : help mode

no flag - stdin input
*/

int main(int argc, char *argv[]) 
{
  int times_run = 1;
  int console_ran = 0;
  while (true) {
    float a = NAN;          // coefficients initialization
    float b = NAN;
    float c = NAN;
    if (argc == 1 || (strcmp(argv[1], "-std") == 0) || console_ran == 1)         // input from stdin
     {
      printf("Equation format: ax^2 + bx + c = 0\n");
      if (!GetCoef(&a, 'a'))                                 // getting a coefficient
      {
        return EXIT_FAILURE;                                 // failed to get a coefficient
      }
      if (!GetCoef(&b, 'b'))                                 // getting b coefficient
      {
        return EXIT_FAILURE;                                 // failed to get b coefficient
      }
      if (!GetCoef(&c, 'c'))                                 // getting c coefficient
      {
        return EXIT_FAILURE;                                 // failed to get c coefficient
      }
    } else if (strcmp(argv[1], "-file") == 0){               // file input case
      char infile_name[LEN] = "no name";
      char outfile_name[LEN] = "no name";
      if (!GetFileName(infile_name, "input")) { return EXIT_FAILURE; }        // getting input file name
      if (!GetFileName(outfile_name, "output")) { return EXIT_FAILURE; }      // getting output file name
      FILE *fpin = fopen(infile_name, "r");
      FILE *fpout = 0;
      if (strcmp(outfile_name, "stdout") == 0)                                // output in stdout from file input
      {
        fpout = stdout;
      } else 
      {
        fpout = fopen(outfile_name, "a");
      }
      if (!fpout)
      {
        fpout = stdout;                                                       // output file did not open ( O_O )
      }
      if (!fpin)
      {
        printf("ERROR: FAILED TO OPEN  %s\n", infile_name);
        return EXIT_FAILURE;
      } else
      {
        FileRun(fpin, fpout);
        if (fclose(fpin))
        {
          printf("WARNING: FAILED TO CLOSE %s\n", infile_name);
        }
        if (fpout != stdout)
        {
          if (fclose(fpout))
          {
            printf("WARNING: FAILED TO CLOSE %s\n", outfile_name);
          }
        }
      }
      if (!RepeatQuestion())                      // asking user does he want to repeat
      {
        printf("Bye Bye");
        return EXIT_SUCCESS;
      } else 
      {
        continue;
      }
    } else if (strcmp(argv[1], "-test") == 0)     // test mode
    {
      RunTest();
      return EXIT_SUCCESS;
    } else if(strcmp(argv[1], "-help") == 0)      // help mode 
    {
      PrintHelp();
      int repeat_flag = 0;
      scanf("%d", &repeat_flag);                                     
      ClearInput();
      if (!repeat_flag)
      {
        break;
      } else { continue; }
      console_ran = 1;
    } else                                        // console read mode
    {
      if (!ReadConsoleCoef(argv[1], &a)) { return EXIT_FAILURE; }
      if (!ReadConsoleCoef(argv[2], &b)) { return EXIT_FAILURE; }
      if (!ReadConsoleCoef(argv[3], &c)) { return EXIT_FAILURE; }
      console_ran = 1;
    }
    struct solve * ans = QuadSolver(a, b, c);                       // solving part
    FPrintRoots(ans->amount, ans->first, ans->second, stdout);      // printing roots
    if (!RepeatQuestion())
    {
      break;
    }
    times_run++;                                                    // solved equations counter
  }
  printf("Bye Bye\n");
}
