#include "getout_info.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>


/* File input format:
 * a = ___
 * b = ___
 * c = ___			*/

int main(int argc, char *argv[])			// for input from file type direction in terminal
{
  int times_run = 1;
  while (true) {
    float a, b, c;
    if (argc == 1 || times_run > 1) 		// input from stdin
     {
      printf("Equation format: ax^2 + bx + c = 0\n");
      if (!GetCoef(&a, 'a'))				// getting a coefficient
      {
        return 1;							// failed to get a coefficient
      }
      if (!GetCoef(&b, 'b'))				// getting b coefficient
      {
        return 1;							// failed to get b coefficient
      }
      if (!GetCoef(&c, 'c'))				// getting c coefficient
      {
        return 1;							// failed to get c coefficient
      }
    } else {	 							// file input case
      FILE *fp = fopen(argv[1], "r");
      if (!fp)
      {
        printf("Failed to open file: %s\n", argv[1]);
        return 1;
      } else
      {
        FSkipFormatText(fp);				// skipping "a =" part
        if (!FGetCoef(fp, &a))  			// getting a coefficient from file
        {
          return 1; 						// failed to get coefficient
        }
        fgetc(fp);
        FSkipFormatText(fp);				// skipping '\n' char
        if (!FGetCoef(fp, &b))  			// getting b coefficient from file
        {
          return 1; 						// failed to get coefficient
        }
        fgetc(fp);
        FSkipFormatText(fp);
        if (!FGetCoef(fp, &c))              // getting c coefficient from file
        {
          return 1;                         // failed to get coefficient
        }
        if (fclose(fp))
        {
          printf("Failed to close file: %s\n", argv[1]);
        }
      }
    }
    struct solve * ans;
    ans = QuadSolver(a, b, c);                                      // solving part
    PrintRoots(ans->amount, ans->first, ans->second);               // printing roots
    printf("Do you want to solve another equation? (1 - Yes): ");
    int repeat_flag = 0;
    scanf("%d", &repeat_flag);                                      // repeat conditions
    ClearInput();
    if (!repeat_flag)
    {
      break;
    }
    times_run++;                                                    // solved equations counter
  }
  printf("Bye Bye\n");
}
