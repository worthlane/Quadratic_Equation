#include "getout_info.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void FileRun(FILE * fpin, FILE * fpout)
{
  const int read_amount = 3;
  float a = NAN;                  // coef initialization
  float b = NAN;
  float c = NAN;
  int equat_number = 1;           // equations counter
  struct solve ans = {NAN, NAN, NAN};
  while (fscanf(fpin, "%f%f%f", &a, &b, &c) == read_amount)   // reading data for one equation
  {
    ans = *(QuadSolver(a, b, c));                             // solving equation
    fprintf(fpout, "%d: ", equat_number++);                   // equation number output
    FPrintRoots(ans.amount, ans.first, ans.second, fpout);
  }
}
//------------------------------------------------------------------------------------------------------------------
bool GetFileName(char * file_name, char mode[])                       // getting file name 
{
  printf("Please, enter the %s file directory: (q to quit) ", mode);  
  if (scanf("%s", file_name) == 0)
  {
    printf("ERROR: FAILED TO GET FILE NAME");
    return false;
  }
  if (strcmp(file_name, "q") == 0)      // user decided to quit program
  {
    printf("Bye Bye");
    return false;
  }
  ClearInput();
  if (strcmp(mode, "input") == 0)
  {
    printf("Input from: %s\n", file_name);
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FGetCoef(FILE * fp, float * a)                     // getting coefficient from file
{
  if (fscanf(fp, "%f", a) == 0)
  {
    printf("ERROR: INCORRECT FILE INPUT");              // (not "coef =" format)
    fclose(fp);
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------
void FSkipFormatText(FILE * fp)             // skipping "coef =" part
{
  fgetc(fp);                                // skipping "coef"
  fgetc(fp);                                // skipping " "
  fgetc(fp);                                // skipping "="
}
//------------------------------------------------------------------------------------------------------------------
void ClearInput(void)                       // clears input from '\n' char and else trash
{
  while (getchar() != '\n') {}
}
//------------------------------------------------------------------------------------------------------------------
bool GetCoef(float * a, const char ch)                  // gets coefficient from STDIN
{
  printf("Input coefficient %c: ", ch);
  while (scanf("%f", a) == 0)
  {
    printf("ERROR: INVALID COEFFICIENT\n");             // not a coefficient conditions
    printf("Do you want to continue? (1 - Yes): ");
    ClearInput();
    int flag = 0;
    scanf("%d", &flag);
    if (flag == 1)                                      // user wants to try again
    {
      printf("Input coefficient %c: ", ch);
      continue;
    } else                                              // user doesn't want to continue
    {
      printf("PROGRAM SHUT DOWN");
      return false;
    }
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------
void FPrintRoots(int roots, float x1, float x2, FILE * fp)    // prints roots in fp stream
{
  switch (roots)
  {
    case ZERO_ROOTS:
      fprintf(fp, "Your equation has zero roots.\n");
      break;
    case ONE_ROOT:
      fprintf(fp, "Your equation has one root: x = %g\n", x1);
      break;
    case TWO_ROOTS:
      fprintf(fp, "Your equation has two roots: x1 = %g, x2 = %g\n", x1, x2);
      break;
    case INF_ROOTS:
      fprintf(fp, "Your equation has infinite amount of roots.\n");
      break;
    default:
      printf("ERROR: UNEXPECTED AMOUNT OF ROOTS.\n");
      break;
  }
}
//------------------------------------------------------------------------------------------------------------------
bool ReadConsoleCoef(char * argv, float * a)           // reads coefficient from console
{
  *a = atof(argv);                                     // changes coefficient type from string to float
  if (!a)
  {
    printf("ERROR: FAILED TO READ CONSOLE COEFFFICIENT");
    return false;
  } 
  return true;
}
//------------------------------------------------------------------------------------------------------------------
void PrintHelp(void)
{
  printf("default mode: input coefficients in console (./a.out a b c)\n");
  printf("-file flag for input from file\n");
  printf("-std  flag for input from stdin\n");
  printf("-test flag for test mode activation (for developers)\n");
  printf("-help flag for help\n");
  printf("Do you want to solve equation in interactive mode? (1+ - Yes): ");
}
//------------------------------------------------------------------------------------------------------------------
bool RepeatQuestion()
{
  printf("Do you want to repeat (1+ - Yes): ");
  bool repeat_flag = false;
  scanf("%d", &repeat_flag);                                      
  ClearInput();
  return repeat_flag;
}
