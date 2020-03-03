/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: Utilities.h                                     */
/*    DATE: 02.25.2020                                      */
/************************************************************/


#ifndef Utilities_HEADER
#define Utilities_HEADER

#include <math.h>


using namespace std;

struct point
{
    double r;
    double z;
};

double  slope(point a, point b);
double  distance(point a, point b);
point   midPoint(point a, point b);
point   circleCenter(point a, point b, double depth);

#endif
