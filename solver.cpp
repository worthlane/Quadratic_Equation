#include <math.h>

#include "solver.h"

int Compare(const double a, const double b)       // comparing two double numbers with EPSILON accuracy
{
  double diff = a - b;
  int result;
  if (abs(diff) < EPSILON)                      // a = b conditions
  {
    result = EQUAL;
  } else                 
  {
    result = (diff > EPSILON) ? MORE : LESS;
  }
  return result;
}
// -----------------------------------------------------------------------------------------
struct QuadSolutions* QuadSolver(const double a, const double b, const double c)
{
  if (Compare(a, 0) == EQUAL)
  {
    return (LinearSolver(b, c));
  } else                                        // ax^2 + bx + c = 0
  {
    struct QuadSolutions ans = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};
    struct QuadSolutions* ptr = &ans;
    double D = b*b - 4*a*c;                            // discriminant formula
    double zero_d_compare = Compare(D, 0);
    if (zero_d_compare == LESS)                  // zero roots conditions
    {
      ptr->amount = ZERO_ROOTS;
    } else if (zero_d_compare == EQUAL)          // one root condition
    {
      ptr->amount = ONE_ROOT;
      ptr->first = (-b / (2 * a));
    } else {                                    // two roots conditions
      ptr->amount = TWO_ROOTS;
      D = sqrt(D);                              // D = sqrt(discriminant)
      double double_a = 2 * a;
      ptr->first = ((-b - D) / double_a);
      ptr->second = ((-b + D) / double_a);
    }
    return ptr;
  }
}

struct QuadSolutions* LinearSolver(const double b, const double c)
{
    struct QuadSolutions ans = {NAN_INT, NAN_DOUBLE, NAN_DOUBLE};
    struct QuadSolutions* ptr = &ans;
    if (Compare(b, 0) == EQUAL)
    {
      if (Compare(c, 0) == EQUAL)               
      {
        ptr->amount = INF_ROOTS;
      } else {                                 
        ptr->amount = ZERO_ROOTS;
      }
    } else {                                    
      ptr->amount = ONE_ROOT;
      ptr->first = (-c / b);
    }
    return ptr;
}