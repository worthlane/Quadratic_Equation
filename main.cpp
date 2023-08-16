#include "getout_info.h"
#include "solver.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>


/* File input format:
 * a = ___
 * b = ___
 * c = ___			*/

int main(int argc, char *argv[]) // for input from file type direction in terminal
{
  int repeat_flag = 1;
  int times_run = 1;
  while (repeat_flag == 1) {
	float a, b, c;
	if (argc == 1 || times_run > 1) { 	// input from stdin
	  printf("Equation format: ax^2 + bx + c = 0\n");
	  Get(&a, 'a');			// input from stdin (a coefficient)
	  Get(&b, 'b');			// input from stdin (b coefficient)
	  Get(&c, 'c');			// input from stdin (c coefficient)
	} else {						// file input case
	  FILE *fp;
	  if ((fp = fopen(argv[1], "r")) == NULL)
	  {
		printf("Failed to open file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	  } else
	  {
		Fskip(fp);			// skipping "a =" part
		Fget(fp, &a);	// getting a coefficient
		fgetc(fp);
		Fskip(fp);			// skipping '\n' char
		Fget(fp, &b);	// getting b coefficient
		fgetc(fp);
		Fskip(fp);
		Fget(fp, &c);	// getting c coefficient
		if (fclose(fp))
		{
		  printf("Failed to close file: %s\n", argv[1]);
		}
	  }
	}
	struct solve ans;
	ans = QuadSolver(a, b, c);		// solving part
	PrintRoots(ans.amount, ans.first, ans.second);		// printing roots
	printf("Do you want to solve another equation? (1 - Yes): ");
	repeat_flag = 0;
	scanf("%d", &repeat_flag);		// repeat conditions
	Clear_Input();
	times_run++;					// solved equations counter
  }
  printf("Bye Bye\n");
}
