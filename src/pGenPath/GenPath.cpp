/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                     */
/*    DATE:                                                 */
/************************************************************/

#include "Utilities.h"
#include "MBUtils.h"
#include <iterator>
#include "GenPath.h"
#include "XYSegList.h"
#include <cstdlib>
#include <math.h>

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_visit_radius = 5;
  m_epsilon      = 0.1;


  m_firstpoint_loaded = false;
  m_lastpoint_loaded  = false;

  m_num_points            = 0;
  m_num_points_visited    = 0;
  m_num_points_unvisited  = 0;

  m_visiting_points = {};

  m_nav_x       = 0;
  m_nav_y       = 0;
  m_nav_heading = 0;

  m_wpt_x = 0;
  m_wpt_y = 0;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
 
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


    string key  = msg.GetKey();
    string sval = msg.GetString();
    double dval  = msg.GetDouble();


    if (MOOSStrCmp(key, "NAV_X"))
      m_nav_x = dval;

    if (MOOSStrCmp(key, "NAV_Y"))
      m_nav_y = dval;

    if (MOOSStrCmp(key, "NAV_HEADING"))
      m_nav_heading = dval;

    if (MOOSStrCmp(key, "VISIT_POINT"))
    {
      if ( sval == "firstpoint" )
        m_firstpoint_loaded = true;
      else if ( sval == "lastpoint" )
      {
        m_lastpoint_loaded = true;
        generatePath();
      }
      else
      {
        double x    = stof(tokStringParse(sval, "x", ',', '='));
        double y    = stof(tokStringParse(sval, "y", ',', '='));
        int    id   = stoi(tokStringParse(sval, "id", ',', '='));

        point p = {x, y};

        m_visiting_points.push_back(p);
        m_num_points            += 1;
        m_num_points_unvisited  += 1;
      }
    }

    if (MOOSStrCmp(key, "VIEW_POINT"))
    {
      m_wpt_x    = stof(tokStringParse(sval, "x", ',', '='));
      m_wpt_y    = stof(tokStringParse(sval, "y", ',', '='));
    }

    if (MOOSStrCmp(key, "GENPATH_REGENERATE"))
    {
      m_seglist.clear();
      generatePath();
      //reportEvent("Regenerating path with " + intToString(m_num_points_unvisited) + " points left.");

      if ( m_visiting_points.size() != 0)
      {
        Notify("TRANSIT", "true");
        Notify("RETURN", "false");
        Notify("STATION_KEEP", "false");
        reportEvent("Regenerating path with " + intToString(m_num_points_unvisited) + " points left.");
      }
    }
  }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();

  point pos      = {m_nav_x, m_nav_y};
  point wpt      = {m_wpt_x, m_wpt_y};

  point true_wpt = contains(m_visiting_points, wpt, m_epsilon);

//  if ( (find(m_visiting_points.begin(), m_visiting_points.end(), wpt) != m_visiting_points.end()) && (distance(pos, wpt) <= m_visit_radius) )
  if ( true_wpt.x != NULL && distance(pos, wpt) <= m_visit_radius )
  {
    m_visiting_points.remove(true_wpt);
    m_num_points_visited   += 1;
    m_num_points_unvisited -= 1;
  }


  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

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

      if(param == "visit_radius")
        m_visit_radius = stof(value);
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenPath::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();

  // Register("FOOBAR", 0);

  Register("VISIT_POINT", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_HEADING", 0);
  Register("GENPATH_REGENERATE", 0);
  Register("VIEW_POINT", 0);
}

bool GenPath::buildReport()
{
   m_msgs << "First point received: " << m_firstpoint_loaded << endl;
   m_msgs << "Last point received: " << m_lastpoint_loaded << endl;
   m_msgs << "Total Points received: " << m_num_points << endl;
   m_msgs << "Visit Radius: " << m_visit_radius << endl;

   m_msgs << "\nTour Status" << endl;
   m_msgs << "----------------------------------" << endl;
   m_msgs << "Total Points Visited: " << m_num_points_visited << endl;
   m_msgs << "Total Points Unvisited: " << m_num_points_unvisited << endl;
   m_msgs << "Waypoint: (" << m_wpt_x << ", " << m_wpt_y << ")" << endl;
   m_msgs << "Position: (" << m_nav_x << ", " << m_nav_y << ")" << endl;
   m_msgs << "Heading: " << m_nav_heading << endl;

   m_msgs << "\nPoint List" << endl;
   m_msgs << "----------------------------------" << endl;
   m_msgs << "Size: " << m_visiting_points.size() << endl;

   return(true);
}

void GenPath::generatePath()
{
  point curr_pos = {m_nav_x, m_nav_y};

  list<point> shortestRoute;
  shortestRoute = findShortestRoute(m_visiting_points, curr_pos); // From Utilities.h

 // // --------------------------------------
 // // Add to avoid odd behaviour

 // point  init_p = *shortestRoute.begin();

 // double dydx = slope(curr_pos, init_p);

 // if ( m_nav_heading <= 270 && m_nav_heading >= 90 )
 // {
 //   double x = curr_pos.x - 2;
 //   double y = curr_pos.y - dydx*2;
 //   m_seglist.add_vertex(x, y);
 // }
 // else
 // {
 //   double x = curr_pos.x + 2;
 //   double y = curr_pos.y + dydx*2;
 //   m_seglist.add_vertex(x, y);
 // }
 // // --------------------------------------
 
  list<point>::iterator p_it;
  for (p_it = shortestRoute.begin(); p_it != shortestRoute.end(); p_it++)
    m_seglist.add_vertex(p_it->x, p_it->y);

  string updates_str  = "points = ";
  updates_str       += m_seglist.get_spec();
  Notify("TRANSIT_UPDATES", updates_str);
 
}
