/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                 */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PointAssign.h"
#include "XYPoint.h"
#include <string>
#include <stdlib.h>
#include <cstdlib>


using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  m_points_assigned  = false;
  m_assign_by_region = true;
  m_vehicles         = {"HENRY", "GILDA"};

  m_visiting_points  = {};
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if (MOOSStrCmp(key, "VISIT_POINT"))
    {
      m_visiting_points.push_back(sval);
      m_points_assigned = false;
    }
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   RegisterVariables();
   //Notify("UTS_PAUSE", "false");

   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
  if ( !m_points_assigned )
  {
    assignPoints();
    m_points_assigned = true;
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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

      //if(param == "vname")
      //  m_vehicles.push_back(value);

    }
  }
  
  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PointAssign::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("VISIT_POINT", 0);
}


void PointAssign::assignPoint(string point, string vehicle, bool all_vehicles)
{

  if ( all_vehicles )
  {
    list<string>::iterator v_it;
    for (v_it = m_vehicles.begin(); v_it != m_vehicles.end(); v_it++)
      Notify("VISIT_POINT_" + *v_it, point);
  }
  else
  {
    double x    = stof(tokStringParse(point, "x", ',', '='));
    double y    = stof(tokStringParse(point, "y", ',', '='));
    string id   = tokStringParse(point, "id", ',', '=');

    Notify("VISIT_POINT_" + vehicle, point);
    if ( vehicle == "GILDA" )
      postViewPoint(x, y, id, "yellow");
    if ( vehicle == "HENRY" )
      postViewPoint(x, y, id, "red");
  }
}

void PointAssign::assignPoints()
{
  int num_vehicles  = m_vehicles.size();
  int num_points    = m_visiting_points.size();

  int p_per_vehicle = num_points / num_vehicles;

  list<string>::iterator vehicle = m_vehicles.begin();
  list<string>::iterator point;

  for (point = m_visiting_points.begin(); point!= m_visiting_points.end(); point++)
  {
    if ( *point == "firstpoint" )
    {
      assignPoint("firstpoint", "", true);
    }
    else if ( *point == "lastpoint" )
    {
      assignPoint("lastpoint", "", true);
    }
    else
    {
      if (m_assign_by_region)
      {
        double x    = stof(tokStringParse(*point, "x", ',', '='));
        double y    = stof(tokStringParse(*point, "y", ',', '='));
        string id   = tokStringParse(*point, "id", ',', '=');
        if ( x >= 87.5 )
          assignPoint(*point, "HENRY");
        else
          assignPoint(*point, "GILDA");

      }
      else
      {
        if ( vehicle == m_vehicles.end() )
          vehicle = m_vehicles.begin();

        assignPoint(*point, *vehicle);

        vehicle++;
        //point = m_visiting_points.erase(point);
      }
    }



  }
}

void PointAssign::postViewPoint(double x, double y, string label, string color)
{
  XYPoint point(x, y);
  point.set_label(label);
  point.set_color("vertex", color);
  point.set_param("vertex_size", "2");

  string spec = point.get_spec();
  Notify("VIEW_POINT", spec);
}
