/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                       */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "Utilities.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include "XYSegList.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"


class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();

 protected:
   void RegisterVariables();

   void generatePath();



 private: // Configuration variables

        double m_visit_radius;
        double m_epsilon;


 private: // State variables

        bool m_firstpoint_loaded;
        bool m_lastpoint_loaded;

        int  m_num_points;
        int  m_num_points_visited;
        int  m_num_points_unvisited;

        std::list<point> m_visiting_points;

        XYSegList m_seglist;

        double m_nav_x;
        double m_nav_y;
        double m_nav_heading;

        double m_wpt_x;
        double m_wpt_y;

        string m_vname;
};

#endif 
