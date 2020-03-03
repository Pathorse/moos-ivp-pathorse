/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationVessel.h                           */
/*    DATE: 02.25.2020                                      */
/************************************************************/

#include "MOOS/libMOOS/MOOSLib.h"
#include <math.h>

using namespace std;

#ifndef CommunicationVessel_HEADER
#define CommunicationVessel_HEADER

class CommunicationVessel
{
    public:

        CommunicationVessel();
        ~CommunicationVessel();


        void    setName(string s)                   {m_name  = s;};
        void    setNavX(double val)                 {m_nav_x = val;};
        void    setNavY(double val)                 {m_nav_y = val;};
        void    setNavDepth(double val)             {m_nav_depth = val;};
        //bool    setNavDepth(double val)             {m_nav_depth = val; return val >= 0;};
        bool    setNavHeading(double val)           {m_nav_heading = val; return val >= 0 && val <= 360;};
        bool    setNavSpeed(double val)             {m_nav_speed = val; return val >= 0;};

        string  getName()                           {return m_name;};
        double  getNavX()                           {return m_nav_x;};
        double  getNavY()                           {return m_nav_y;};
        double  getRadius()                         {return sqrt(pow(m_nav_x, 2) + pow(m_nav_y, 2));};
        double  getDepth()                          {return m_nav_depth;};


     protected:
        string   m_name;
        double   m_nav_x;
        double   m_nav_y;
        double   m_nav_depth;
        double   m_nav_heading; // Degrees
        double   m_nav_speed;
};
#endif
