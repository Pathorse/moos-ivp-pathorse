/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: FrontEstimateComms.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef FrontEstimateComms_HEADER
#define FrontEstimateComms_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "NodeMessage.h"

class FrontEstimateComms : public CMOOSApp
{
 public:
   FrontEstimateComms();
   ~FrontEstimateComms();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
        std::string m_vname;

};

#endif 
