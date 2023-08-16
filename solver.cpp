#include "solver.h"
#include <math.h>
#include "getout_info.h"

struct solve QuadSolver(float a, float b, float c)
{
  struct solve ans;
  if (a == 0)
  {
	if (b == 0)
	{
	  if (c == 0)				// 0 + 0 + 0 = 0
	  {
		ans.amount = INF_ROOTS;
		ans.first = 0;
		ans.second = 0;
	  } else {					// 0 + 0 + c = 0
		ans.amount = ZERO_ROOTS;
		ans.first = 0;
		ans.second = 0;
	  }
	} else {					// bx + c = 0
	  ans.amount = ONE_ROOT;
	  ans.first = (-c / b);
	  ans.second = 0;
	}
  } else						// ax^2 + bx + c = 0
  {
	float D; 					// Discriminant
	D = b * b - 4 * a * c;		// Discriminant formula
	if (D < 0)					// zero roots conditions
	{
	  ans.amount = ZERO_ROOTS;
	  ans.first = 0;
	  ans.second = 0;
	} else if (D == 0)			// one root condition
	{
	  ans.amount = ONE_ROOT;
	  ans.first = (-b / (2 * a));
	  ans.second = 0;
	} else {					// two roots conditions
	  float root1, root2;
	  ans.amount = TWO_ROOTS;
	  ans.first = ((-b - sqrt(D)) / (2 * a));
	  ans.second = ((-b + sqrt(D)) / (2 * a));
	}
  }
  return ans;
}