#include "Utilities.h"
#include <iostream>
#include <list>
#include <string>

using namespace std;

int main()
{
    point o = {0,0};

    point a = {1,1};
    point b = {2,3};
    point c = {50,20};
    point d = {100,200};
    point e = {0.1, 0.3};
    point f = {124, 49};
    point g = {0.2, 0.01};

    list<point> points;

    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    points.push_back(d);
    points.push_back(e);
    points.push_back(f);
    points.push_back(g);


    list<point> sp;
    sp = findShortestRoute(points, o);

    point p = getNextPoint(points, o);
    cout << "Closest point to the origin: " << "x=" << p.x << "y=" << p.y << endl;

    int d1 = distance(o, a);
    int d2 = distance(o, b);
    cout << "dist 1: " << d1 << endl;
    cout << "dist 2: " << d2 << endl;

    int i = 1;
    list<point>::iterator p_it;
    for (p_it = sp.begin(); p_it != sp.end(); p_it++)
    {
        cout << i << " Point: x=" << p_it->x << "y=" << p_it->y << endl;
        i++;
    }
 }
