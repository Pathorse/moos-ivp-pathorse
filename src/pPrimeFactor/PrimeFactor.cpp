/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactor.h"
#include "PrimeEntry.h"
#include <cstdint>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  m_total_num_received    = 0;
  m_total_num_calculated  = 0;
  m_received_numbers    = {};
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{

}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
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

    string key      = msg.GetKey();
    int dval        = msg.GetDouble();
    string sval     = msg.GetString();

    if (MOOSStrCmp(key, "NUM_VALUE"))
    {
      // Create PrimeEntry object and set init values
      PrimeEntry e;
      e.setOriginalVal(strtoul(sval.c_str(), NULL, 0));
      e.setReceivedIndex(m_total_num_received);

      m_total_num_received++; // Increment
     
      m_received_numbers.push_back(e); // Add entry to list of entries
    }
  }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  list<PrimeEntry>::iterator p;
  for (p=m_received_numbers.begin(); p!=m_received_numbers.end(); ++p){
    {
      PrimeEntry e = *p;
      if (!e.done())
      {
        e.setDone(e.factor(1000));
        if (e.done())
        {
          e.setCalculatedIndex(m_total_num_calculated);

          m_total_num_calculated++;

          Notify("PRIME_RESULT", e.getReport());
        }
      }
    }




    /* From previous iteration below
    if (num%2==0)
      Notify("NUM_RESULT", to_string(num) + ", even");
    else
      Notify("NUM_RESULT", to_string(num) + ", odd");
    p = m_received_numbers.erase(p); */
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
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

void PrimeFactor::RegisterVariables()
{
  // Register("FOOBAR", 0);

  Register("NUM_VALUE", 0);
}

