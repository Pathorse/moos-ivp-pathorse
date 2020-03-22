/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

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

   static bool pointComp(const std::string & p1, const std::string & p2);

 private: // Configuration variables
        bool m_points_loaded;

        std::list<std::string> m_visiting_points;

        XYSegList m_seglist;

        double m_nav_x;
        double m_nav_y;


 private: // State variables
};

#endif 
