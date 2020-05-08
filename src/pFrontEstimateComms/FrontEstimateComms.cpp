/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: FrontEstimateComms.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "FrontEstimateComms.h"
#include "NodeMessage.h"
#include "NodeMessageUtils.h"

using namespace std;

//---------------------------------------------------------
// Constructor

FrontEstimateComms::FrontEstimateComms()
{
  m_vname = "archie";
}

//---------------------------------------------------------
// Destructor

FrontEstimateComms::~FrontEstimateComms()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool FrontEstimateComms::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if (MOOSStrCmp(key, "UCTD_MSMNT_REPORT"))
    {
      // Create NodeMessage
      NodeMessage node_message;

      // Add information
      node_message.setSourceNode("archie");
      node_message.setDestNode("all");
      node_message.setVarName("UCTD_MSMNT_REPORT");
      node_message.setStringVal(sval);

      string message = node_message.getSpec();

      // Publish
      Notify("NODE_MESSAGE_LOCAL", message);
    }
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool FrontEstimateComms::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool FrontEstimateComms::Iterate()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool FrontEstimateComms::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "vname") {
        //handled
        //m_vname = value;
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

void FrontEstimateComms::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("UCTD_MSMNT_REPORT", 0);
}

