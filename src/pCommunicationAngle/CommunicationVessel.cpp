/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationVessel.cpp                         */
/*    DATE: 02.25.2020                                      */
/************************************************************/

#include "CommunicationVessel.h"


using namespace std;


//---------------------------------------------------------
// Constructor

CommunicationVessel::CommunicationVessel()
{

    m_name          = "";
    m_nav_x         = 0;
    m_nav_y         = 0;
    m_nav_depth     = 0;
    m_nav_heading   = 0;
    m_nav_speed     = 0;
}

//---------------------------------------------------------
// Destructor

CommunicationVessel::~CommunicationVessel()
{

}
