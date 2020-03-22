/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: Utilities.cpp                                   */
/*    DATE: 02.25.2020                                      */
/************************************************************/


#include "Utilities.h"
#include <math.h>
#include <list>

using namespace std;


double slope(point a, point b)
{
    return (b.y - a.y)/(b.x - a.x);
}

double distance(point a, point b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

point midPoint(point a, point b)
{
    point m = {(a.x + b.x)/2, (a.y + b.y)/2};

    return m;
}

void updateRoutes(list<point> & shortestRoute, list<point> & points, point p)
{
    shortestRoute.push_back(p);
    points.remove(p);
}


point getNextPoint(list<point> & points, point p)
{
    point t = *points.begin();

    list<point>::iterator it;
    for (it = points.begin(); it != points.end(); it++)
    {
        if ( distance(p, *it) < distance(p, t) )
            t = *it;
    }

    return t;
}

list<point> findShortestRoute(list<point> points, point origin)
{
  list<point> shortestRoute;
  point p = origin;
  updateRoutes(shortestRoute, points, p);
  while ( points.size() >= 1 )
  {
      p = getNextPoint(points, p);
      updateRoutes(shortestRoute, points, p);
  }


  return shortestRoute;
}
