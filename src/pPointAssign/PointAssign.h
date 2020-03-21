/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                   */
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

        void postViewPoint(double x, double y, std::string label, std::string color);
        void assignPoint(std::string point, std::string vehicle, bool all_vehicles=false);
        void assignPoints();

    private: // Configuration variables
        bool                        m_points_assigned;
        bool                        m_assign_by_region;
        std::list<std::string>      m_vehicles;

    private: // State variables
        std::list<std::string>      m_visiting_points;
};

#endif 
