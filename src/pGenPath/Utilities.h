/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: Utilities.h                                     */
/*    DATE: 02.25.2020                                      */
/************************************************************/


#ifndef Utilities_HEADER
#define Utilities_HEADER

#include <math.h>
#include <list>


using namespace std;

struct point
{
    double x;
    double y;

    bool operator == (const point &p) const { return x == p.x && y == p.y; };
    bool operator != (const point &p) const { return !operator==(p); };
};

double  slope(point a, point b);
double  distance(point a, point b);
point   midPoint(point a, point b);

list<point> findShortestRoute(list<point>  points);
void        updateRoutes(list<point> & shortestRoute, list<point> & points, point p);
point       getNextPoint(list<point> & points, point p);

#endif
