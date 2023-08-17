#include "solver.h"
#include <math.h>
#include "getout_info.h"

#define EPSILON 1e-6

int Compare(const float a, const float b)   	// comparing two float numbers with EPSILON accuracy
{
  float diff = a - b;
  if (abs(diff) < EPSILON)  					// a = b conditions
  {
	return EQUAL;
  } else if (diff > EPSILON)					// a > b conditions
  {
	return MORE;
  } else if (diff < -EPSILON)   				// a < b conditions
  {
	return LESS;
  }
}
// -----------------------------------------------------------------------------------------
struct solve * QuadSolver(const float a, const float b, const float c)
{
  struct solve ans = {0, 0, 0};
  struct solve * ptr = &ans;
  if (Compare(a, 0) == EQUAL)
  {
	if (Compare(b, 0) == EQUAL)
	{
	  if (Compare(c, 0) == EQUAL)   			// 0 + 0 + 0 = 0
	  {
		ptr->amount = INF_ROOTS;
	  } else {  								// 0 + 0 + c = 0
		ptr->amount = ZERO_ROOTS;
	  }
	} else {									// bx + c = 0
	  ptr->amount = ONE_ROOT;
	  ptr->first = (-c / b);
	}
  } else										// ax^2 + bx + c = 0
  {
	float D;									// discriminant
	D = b*b - 4*a*c;							// discriminant formula
	if (Compare(D, 0) == LESS)  				// zero roots conditions
	{
	  ptr->amount = ZERO_ROOTS;
	} else if (Compare(D, 0) == EQUAL)  		// one root condition
	{
	  ptr->amount = ONE_ROOT;
	  ptr->first = (-b / (2 * a));
	} else {									// two roots conditions
	  float root1, root2;
	  ptr->amount = TWO_ROOTS;
	  float discriminant_root = sqrt(D);
	  ptr->first = ((-b - discriminant_root) / (2 * a));
	  ptr->second = ((-b + discriminant_root) / (2 * a));
	}
  }
  return ptr;
}
