/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                 */
/*    DATE: 02.24.2020                                      */
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
  m_total_num_received    = 1; // Apply 1 indexing
  m_total_num_calculated  = 1; // Apply 1 indexing
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

    string key      = msg.GetKey();    // Identifier
    string sval     = msg.GetString(); // String attribute (only strings expected)

    // Condition on messages with NUM_VALUE key
    if (MOOSStrCmp(key, "NUM_VALUE"))
    {
      // Create PrimeEntry object and set attributes
      PrimeEntry e;
      e.setOriginalVal(strtoul(sval.c_str(), NULL, 0)); // strtoul used for converting string to unsiged long int
      e.setReceivedIndex(m_total_num_received);
      e.setReceivedTime(MOOSTime());
      
      m_total_num_received++; // Increment total number of received elements
     
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

  // Iterate through all prime entries and do prime factorization
  list<PrimeEntry>::iterator p;
  for (p=m_received_numbers.begin(); p!=m_received_numbers.end(); ++p){
    {
      PrimeEntry& e = *p;
      if (!e.done())
      {
        e.setDone(e.factor(1000000, 0.2, false, true)); // Do prime factorization and set done attribute accordingly

        if (e.done()) // Enter upon completion and set attributes
        {
          e.setCalculatedIndex(m_total_num_calculated);
          e.setCalculatedTime(MOOSTime());

          m_total_num_calculated++; // Increment total number of calculated elements

          Notify("PRIME_RESULT", e.getReport()); // Report back using PRIME_RESULT

          p = m_received_numbers.erase(p); // Erase to avoid iterating over completed numbers
        }
      }
    }
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

