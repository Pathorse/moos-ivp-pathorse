/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "MBUtils.h"
#include <iterator>
#include "GenPath.h"
#include "XYSegList.h"
#include <cstdlib>

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_points_loaded = false;

  m_vname = "unknown";
  m_visiting_points = {};
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


    if (MOOSStrCmp(key, "VISIT_POINT"))
    {
      m_visiting_points.push_back(sval);
      if ( sval == "lastpoint" )
        m_points_loaded = true;
    }

    if ( m_points_loaded )
    {
      generatePath();
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


      if ( param == "vname" )
        m_vname = value;
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
}




void GenPath::generatePath()
{
  list<string>::iterator point;
  for (point = m_visiting_points.begin(); point != m_visiting_points.end(); point ++)
  {
      double x    = stof(tokStringParse(point, "x", ',', '='));
      double y    = stof(tokStringParse(point, "y", ',', '='));
      int    id   = stoi(tokStringParse(point, "id", ',', '='));

      m_seglist.add_vertex(x, y);

      string updates_str  = "points = ";
      updates_str       += m_seglist.get_spec();
      Notify("TRANSIT_UPDATES", updates_str);
  }
}
