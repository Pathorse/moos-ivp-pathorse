/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle.h                            */
/*    DATE: 02.25.2020                                      */
/************************************************************/

#ifndef CommunicationAngle_HEADER
#define CommunicationAngle_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "CommunicationVessel.h"


class CommunicationAngle : public CMOOSApp
{
    public:
        CommunicationAngle();
        ~CommunicationAngle();

    protected: // Standard MOOSApp functions to overload  
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

    protected:
        void RegisterVariables();

        double range(double s);
        double depth(double s);
        double soundSpeed(double depth);
        double pressureFieldAmplitude(double s);
        double crossSectionalArea(double s, double delta_s);
        double transmissionLoss(double s);

        bool   validPath();
        bool   calculateConnectivityLocation();
        bool   initComplete();

    private: // Configuration variables
        int     m_surface_sound_speed;
        double  m_sound_speed_gradient;
        int     m_water_depth;
        string  m_user_id;

    private: // State variables
        double  m_theta0; // TODO maybe remove?
        double  m_z0; // TODO remove, part of vessel object
        double  m_R;  // TODO maybe remove
        double  m_s;
        double  m_elev_angle;
        double  m_transmission_loss; // TODO possibly unused and replaced by function instead
        double  m_connectivity_x;
        double  m_connectivity_y;
        double  m_connectivity_depth;

        list<double> m_p;

        map<string,bool> m_init;

        CommunicationVessel m_ownship;
        CommunicationVessel m_collaborator;

};

#endif 
