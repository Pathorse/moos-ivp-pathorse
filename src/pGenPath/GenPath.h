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


class GenPath : public CMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

   void generatePath();



 private: // Configuration variables
        bool m_firstpoint_loaded;
        bool m_lastpoint_loaded;
        int  m_num_points;

        std::list<point> m_visiting_points;

        XYSegList m_seglist;

        double m_nav_x;
        double m_nav_y;


 private: // State variables
};

#endif 
