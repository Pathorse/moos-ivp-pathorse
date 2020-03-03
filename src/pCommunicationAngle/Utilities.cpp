/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: Utilities.cpp                                   */
/*    DATE: 02.25.2020                                      */
/************************************************************/


#include "Utilities.h"
#include <math.h>

using namespace std;


double slope(point a, point b)
{
    return (b.z - a.z)/(b.r - a.r);
}

double distance(point a, point b)
{
    return sqrt(pow(a.z - b.z, 2) + pow(a.r - b.r, 2));
}

point midPoint(point a, point b)
{
    point m = {(a.r + b.r)/2, (a.z + b.z)/2};

    return m;
}


/* Input:
 *       point a - A point in the rz-plane
 *       point b - A point in the rz plane
 *       depth   - z value for desired circle center
 * Output:
 *       point c - Circle centre for a circle going
 *                 through point a and b, where the
 *                 centre height is determined by
 *                 input variable depth
 * Comment:
 *        The circle centre coordinate r is calculated using
 *        the line perpendicular to linesegment a-b and going
 *        through the midpoint m, i.e. found by solving
 *
 *        (z_centre - z_midpoint) = slope * (r_centre - r_midpoint)
 */
point circleCenter(point a, point b, double depth)
{
    point m = midPoint(a, b); // Midpoint between point a, b
    double s = - 1/slope(a, b); // Slope perpendicular to linesegment a-b
    double r = 1/s * (depth - m.z) + m.r; // Circle center coordinate r

    point c = {r, depth};

    return c;
}
