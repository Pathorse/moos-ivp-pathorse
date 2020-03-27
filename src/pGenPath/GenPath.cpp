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
  m_firstpoint_loaded = false;
  m_lastpoint_loaded  = false;
  m_num_points        = 0;

  m_visiting_points = {};

  m_nav_x = 0;
  m_nav_y = 0;
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

    if (MOOSStrCmp(key, "VISIT_POINT"))
    {
      if ( sval == "firstpoint" )
        m_firstpoint_loaded = true;
      else if ( sval == "lastpoint" )
        m_lastpoint_loaded = true;
      else
      {
        double x    = stof(tokStringParse(sval, "x", ',', '='));
        double y    = stof(tokStringParse(sval, "y", ',', '='));
        int    id   = stoi(tokStringParse(sval, "id", ',', '='));

        point p = {x, y};

        m_visiting_points.push_back(p);
        m_num_points += 1;
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

  if ( m_lastpoint_loaded )
  {
    generatePath();
    m_lastpoint_loaded = false;
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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

void GenPath::RegisterVariables()
{
  // Register("FOOBAR", 0);

  Register("VISIT_POINT", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}


void GenPath::generatePath()
{
  point curr_pos = {m_nav_x, m_nav_y};

  list<point> shortestRoute;
  shortestRoute = findShortestRoute(m_visiting_points, curr_pos); // From Utilities.h

  list<point>::iterator p_it;
  for (p_it = shortestRoute.begin(); p_it != shortestRoute.end(); p_it++)
  {
    m_seglist.add_vertex(p_it->x, p_it->y);

    string updates_str  = "points = ";
    updates_str       += m_seglist.get_spec();
    Notify("TRANSIT_UPDATES", updates_str);
  }
}
