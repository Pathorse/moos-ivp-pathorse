/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle.cpp                          */
/*    DATE: 02.25.2020                                      */
/************************************************************/

#define PI 3.14159265

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle.h"
#include "CommunicationVessel.h"
#include "Utilities.h"

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle::CommunicationAngle()
{
  // Configuration variables
  m_surface_sound_speed  = 1480;
  m_sound_speed_gradient = 0.016;
  m_water_depth          = 6000;
  m_user_id              = "pathorse@mit.edu";

  // State variables
  m_theta0               = 0;
  m_z0                   = 0;
  m_R                    = 0;
  m_s                    = 0;
  m_elev_angle           = 0;
  m_transmission_loss    = 0;
  m_connectivity_x       = 0;
  m_connectivity_y       = 0;
  m_connectivity_depth   = 0;

  m_init["NAV_X"]       = false;
  m_init["NAV_Y"]       = false;
  m_init["NAV_DEPTH"]   = false;
  m_init["NAV_HEADING"] = false;
  m_init["NAV_SPEED"]   = false;


  m_init["NEPTUNE_NAV_X"]       = false;
  m_init["NEPTUNE_NAV_Y"]       = false;
  m_init["NEPTUNE_NAV_DEPTH"]   = false;
  m_init["NEPTUNE_NAV_HEADING"] = false;
  m_init["NEPTUNE_NAV_SPEED"]   = false;

  m_p                    = {};
}

//---------------------------------------------------------
// Destructor

CommunicationAngle::~CommunicationAngle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;

  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
  

    string key   = msg.GetKey();
    string sval  = msg.GetString();
    double dval  = msg.GetDouble();

    m_init[key]  = true;
    
    // Ownship -----------------------------------------
    if (MOOSStrCmp(key, "VEHICLE_NAME"))
      m_ownship.setName(sval);

    if (MOOSStrCmp(key, "NAV_X"))
      m_ownship.setNavX(dval);

    if (MOOSStrCmp(key, "NAV_Y"))
      m_ownship.setNavY(dval);

    if (MOOSStrCmp(key, "NAV_DEPTH"))
      m_ownship.setNavDepth(dval);

    if (MOOSStrCmp(key, "NAV_HEADING"))
      m_ownship.setNavHeading(dval);
    
    if (MOOSStrCmp(key, "NAV_SPEED"))
      m_ownship.setNavSpeed(dval);
    // --------------------------------------------------

    
    // Collaborator -------------------------------------
    if (MOOSStrCmp(key, "NEPTUNE_VEHICLE_NAME"))
      m_collaborator.setName(sval);
    
    if (MOOSStrCmp(key, "NEPTUNE_NAV_X"))
      m_collaborator.setNavX(dval);
    
    if (MOOSStrCmp(key, "NEPTUNE_NAV_Y"))
      m_collaborator.setNavY(dval);

    if (MOOSStrCmp(key, "NEPTUNE_NAV_DEPTH"))
      m_collaborator.setNavDepth(dval);

    if (MOOSStrCmp(key, "NEPTUNE_NAV_HEADING"))
      m_collaborator.setNavHeading(dval);
    
    if (MOOSStrCmp(key, "NEPTUNE_NAV_SPEED"))
      m_collaborator.setNavSpeed(dval);
    // --------------------------------------------------
}
  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle::OnConnectToServer()
{
  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle::Iterate()
{

  if ( initComplete() )
  {
    point q = {m_ownship.getNavX(), m_ownship.getNavY()};
    point r = {m_collaborator.getNavX(), m_collaborator.getNavY()};



    point a  = {0, m_ownship.getDepth()}; // Ownship pos in rz-plane
    m_z0 = a.z; // TODO quickfix not to be permanent
    point b  = {distance(q,r), m_collaborator.getDepth()}; // Collaborator pos in rz-plane
    point c  = circleCenter(a, b, - m_surface_sound_speed/m_sound_speed_gradient); // Circle center
    Notify("MSG1", "Circle center: r=" + to_string(c.r) + ", z=" + to_string(c.z));

    // Update Circle Radius R and initial grazing angle theta0
    m_R      = distance(c, a);
    Notify("MSG2", "Radius to circle center: R=" + to_string(m_R));

    m_theta0 = - atan( abs((a.r - c.r)/abs(a.z - c.z)) );
    Notify("MSG3", "Elev angle, theta0=" + to_string(m_theta0 * 180/PI));


    // Update arc length s between point a, b
    double L = distance(a, b);
    m_s      = m_R * acos( (2 * pow(m_R, 2) - pow(L, 2))/(2 * pow(m_R, 2)) );
    Notify("MSG4", "Arc length: s=" + to_string(m_s));

 
    // Update ACOUSTIC_PATH
    if (validPath())
    {
      Notify("ACOUSTIC_PATH",
             "elev_angle=" + to_string(m_theta0)  +
             ", transmission_loss=" + to_string(transmissionLoss(m_s)) +
             ", id=" + m_user_id);
      Notify("CONNECTIVITY_LOCATION",
             "x=" + to_string(m_ownship.getNavX()) +
             ", y=" + to_string(m_ownship.getNavY()) +
             ", depth=" + to_string(m_ownship.getDepth()) +
             ", id=" + m_user_id);
    } else
    {
      Notify("ACOUSTIC_PATH", "NaN");

      if (calculateConnectivityLocation())
        Notify("CONNECTIVITY_LOCATION",
               "x=" + to_string(m_connectivity_x) +
               "y=" + to_string(m_connectivity_y) +
               "depth=" + to_string(m_connectivity_depth) +
               "id=" + m_user_id);
      else
        Notify("WARNING","Failed to calculate connectivity");

    }
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void CommunicationAngle::RegisterVariables()
{
  Register("VEHICLE_NAME", 0);
  Register("COLLABORATOR_NAME", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_DEPTH", 0);
  Register("NAV_HEADING", 0);
  Register("NAV_SPEED", 0);
  Register("NEPTUNE_NAV_X", 0);
  Register("NEPTUNE_NAV_Y", 0);
  Register("NEPTUNE_NAV_DEPTH", 0);
  Register("NEPTUNE_NAV_HEADING", 0);
  Register("NEPTUNE_NAV_SPEED", 0);
}

bool CommunicationAngle::initComplete()
{
  map<string,bool>::iterator it;
  for ( it = m_init.begin(); it != m_init.end(); it++ )
  {
    if ( !it->second )
      return false;
  }
  return true;
}

double CommunicationAngle::range(double s)
{
  return m_R * (sin( m_theta0 ) + sin( s/m_R - m_theta0 ));
}


double CommunicationAngle::depth(double s)
{
  return m_R * cos( s/m_R - m_theta0 ) - m_surface_sound_speed/m_sound_speed_gradient;
}


double CommunicationAngle::soundSpeed(double z)
{
  return m_surface_sound_speed + m_sound_speed_gradient * z;
}


//double CommunicationAngle::crossSectionalArea(double curr_s, double prev_s, double& prev_theta)
//{
//  double dr = range(curr_s) - range(prev_s);
//  double dz = depth(curr_s) - depth(prev_s);
//
//  double theta = atan(dr/dz);
//  double dtheta = theta - prev_theta;
//
//  prev_theta = theta;
//
//  //return m_R * range(prev_s) * ( range(curr_s) - range(prev_s) )/( depth(curr_s) - depth(prev_s) );
//  return range(prev_s)/sin(theta) * dr/dtheta;
//}



//double CommunicationAngle::pressureFieldAmplitude(double s)
//{
//  double ds     = 10;
//  double prev_s = 0;
//  double prev_theta = m_theta0;
//
//  for (double curr_s = ds; curr_s <= s; curr_s += ds)
//  {
//
//    double p = 1/(4*PI) *
//                  sqrt(abs(((soundSpeed(depth(prev_s)))
//                            * cos(m_theta0))/(soundSpeed(m_z0)
//                                              * crossSectionalArea(curr_s, prev_s, prev_theta))));
//
//    m_p.push_back(p);
//
//    prev_s = curr_s;
//  }
//  Notify("MSG6", "p_it=" + to_string(*m_p.end()));
//  return *m_p.end();
//}


double CommunicationAngle::pressureFieldAmplitude(double s)
{
  double dr = 0;
  double dz = 0;
  double dtheta = 0;


  double ds = 10; // stepsize

  double curr_theta = m_theta0;
  double prev_theta = m_theta0;
  double curr_s     = 0;
  double prev_s     = 0;

  double J = 0;
  double p = 0;

  for (curr_s = ds; curr_s <= s; curr_s += s)
  {
    dr = range(curr_s) - range(prev_s);
    dz = depth(curr_s) - depth(prev_s);

    curr_theta = atan(dr/dz);

    dtheta = curr_theta - prev_theta;

    J = range(prev_s)/sin(curr_theta) * dr/dtheta;


    p = 1/(4*PI) * sqrt( abs( ( soundSpeed(depth(prev_s)) * cos(m_theta0) /
                                (soundSpeed(depth(0)) * J) ) ) );

    m_p.push_back(p);

    prev_s     = curr_s;
    prev_theta = curr_theta;
  }

  return p;

}


double CommunicationAngle::transmissionLoss(double s)
{
  Notify("MSG5", "P(s)=" + to_string(pressureFieldAmplitude(s)) + ", P(1)=" + to_string(pressureFieldAmplitude(1)));
  return -20 * log10( pressureFieldAmplitude(s)/pressureFieldAmplitude(1) );
}


bool CommunicationAngle::validPath()
{
  return true; // TODO implement
}


bool CommunicationAngle::calculateConnectivityLocation()
{
  return false; // TODO implement
}
