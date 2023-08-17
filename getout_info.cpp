#include "getout_info.h"
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------------------------------------
bool FGetCoef(FILE * fp, float * a)  					// getting coefficient from file
{
  if (fscanf(fp, "%f", a) == 0)
  {
	printf("ERROR: INCORRECT FILE INPUT");  			// (not "coef =" format)
	fclose(fp);
	return false;
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------
void FSkipFormatText(FILE * fp) 						// skipping "coef =" part
{
  fgetc(fp);											// skipping "coef"
  fgetc(fp);											// skipping " "
  fgetc(fp);											// skipping "="
}
//------------------------------------------------------------------------------------------------------------------
void ClearInput(void)									// clearing input from '\n' char and else trash
{
  while (getchar() != '\n') {}
}
//------------------------------------------------------------------------------------------------------------------
bool GetCoef(float * a, const char ch)					// getting coefficient from STDIN
{
  printf("Input coefficient %c: ", ch);
  while (scanf("%f", a) == 0)
  {
	printf("ERROR: INVALID COEFFICIENT\n");				// not a coefficient conditions
	printf("Do you want to continue? (1 - Yes): ");
	ClearInput();
	int flag = 0;
	scanf("%d", &flag);
	if (flag == 1)										// user wants to try again
	{
	  printf("Input coefficient %c: ", ch);
	  continue;
	} else												// user doesn't want to continue
	{
	  printf("PROGRAM SHUT DOWN");
	  return false;
	}
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------
void PrintRoots(int roots, float x1, float x2)
{
  switch (roots)
  {
	case ZERO_ROOTS:
	  printf("Your equation has zero roots.\n");
	  break;
	case ONE_ROOT:
	  printf("Your equation has one root: x = %g\n", x1);
	  break;
	case TWO_ROOTS:
	  printf("Your equation has two roots: x1 = %g, x2 = %g\n", x1, x2);
	  break;
	case INF_ROOTS:
	  printf("Your equation has infinite amount of roots.\n");
	  break;
	default:
	  printf("ERROR: UNEXPECTED AMOUNT OF ROOTS.\n");
	  break;
  }
}