#include <math.h>
#include <assert.h>

#include "solver.h"

int Compare(const double a, const double b)       // comparing two double numbers with EPSILON accuracy
{
    assert (isfinite(a));
    assert (isfinite(b));

    double diff = a - b;

    if (fabs(diff) < EPSILON)                     // a = b conditions
        return EQUAL;
    else                 
        return (diff > EPSILON) ? MORE : LESS;
}

// -----------------------------------------------------------------------------------------

void QuadSolver(const double a, const double b, const double c, struct QuadSolutions* ans)
{
    assert (isfinite (a));
    assert (isfinite (b));
    assert (isfinite (c));
    assert (ans);

    if (Compare(a, 0) == EQUAL)
    {
        LinearSolver(b, c, ans);
        return;
    } 
    else                                             // ax^2 + bx + c = 0
    {
        double D = b*b - 4*a*c;                      // discriminant formula
        double zero_d_compare = Compare(D, 0);

        if (zero_d_compare == LESS)                  // zero roots conditions
        {
            ans->amount = ZERO_ROOTS;
        } 
        else if (zero_d_compare == EQUAL)            // one root condition
        {
            ans->amount = ONE_ROOT;
            ans->first = (-b / (2 * a));
        } 
        else 
        {                                            // two roots conditions
            ans->amount = TWO_ROOTS;
            D = sqrt(D);                             // done for better optimization
            double double_a = 2 * a;
            ans->first  = ((-b - D) / double_a);
            ans->second = ((-b + D) / double_a);
        }
    }
}

// -----------------------------------------------------------------------------------------

void LinearSolver(const double b, const double c, struct QuadSolutions* ans) // solves linear equation
{
    assert (isfinite (b));
    assert (isfinite (c));
    assert (ans);

    if (Compare(b, 0) == EQUAL)
    {
        if (Compare(c, 0) == EQUAL)               
            ans->amount = INF_ROOTS;
        else                                 
            ans->amount = ZERO_ROOTS;
    } 
    else 
    {                                    
        ans->amount = ONE_ROOT;
        ans->first = (-c / b);
    }
}