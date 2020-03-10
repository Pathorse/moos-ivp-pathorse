/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class PointAssign : public CMOOSApp
{
    public:
        PointAssign();
        ~PointAssign();

    protected: // Standard MOOSApp functions to overload
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

    protected:
        void RegisterVariables();

        void handlePoints(list<string> &points);

        bool initComplete();

    private: // Configuration variables

    private: // State variables
        list<string>     m_vehicles;
        list<string>     m_visiting_points;
        map<string,bool> m_init;
};

#endif 
