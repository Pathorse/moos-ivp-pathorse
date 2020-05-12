/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg2.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iostream>
#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg2.h"
#include "XYRangePulse.h"
#include <unistd.h>
#include "ZAIC_PEAK.h"
#include "XYPoint.h"


using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg2::BHV_ZigLeg2(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("NAV_HEADING");
  addInfoVars("WPT_INDEX", "no_warning");
  addInfoVars("VIEW_POINT", "no_warning");

  // Set default config params
  m_zig_angle = 45;
  m_zig_duration = 10;

  // Set default state variables
  m_nav_x = 0;
  m_nav_y = 0;
  m_wpt_index = 0;
  m_wpt_capture_angle = 0;
  m_curr_time = getBufferCurrTime();
  m_wpt_timestamp = 0;
  m_timestamp = 0;
  m_new_wpt = false;

  m_desired_heading = 0;
  m_zig_left = false;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg2::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }
  else if (param == "zig_angle" && isNumber(val)) {
    m_zig_angle = double_val;
    return(true);
  }
  else if (param == "zig_duration" && isNumber(val) && double_val > 0) {
    m_zig_duration = double_val;
    return(true);
  }


  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_ZigLeg2::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg2::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg2::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg2::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg2::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg2::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg2::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg2::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;


  bool ok1, ok2, ok3, ok4, ok5;

  m_curr_time = getBufferCurrTime();

  m_nav_x       = getBufferDoubleVal("NAV_X", ok1);
  m_nav_y       = getBufferDoubleVal("NAV_Y", ok2);
  m_nav_heading = getBufferDoubleVal("NAV_HEADING", ok3);

  //if(!ok1 || !ok2 || !ok3) {
  //  postWMessage("No ownship X/Y/Heading info in buffer.");
  //  return (0);
  //}

  double next_wpt_index = getBufferDoubleVal("WPT_INDEX", ok4);
  string next_wpt       = getBufferStringVal("VIEW_POINT", ok5);

  if (ok5)
  {
    m_next_wpt.set_vx(stof(tokStringParse(next_wpt, "x", ',', '=')));
    m_next_wpt.set_vy(stof(tokStringParse(next_wpt, "y", ',', '=')));
  }


  if ( next_wpt_index != m_wpt_index )
  {
    m_wpt_timestamp = m_curr_time;
    m_wpt_capture_angle = relAng(m_nav_x, m_nav_y, m_next_wpt.x(), m_next_wpt.y());
    m_new_wpt = true;
  }

  //if ( m_new_wpt && m_curr_time - m_wpt_timestamp >= 5 )
  //{
  //  if ( (m_curr_time - m_wpt_timestamp) <= m_zig_duration + 5 )
  //  {
  //    ipf = buildFunctionWithZAIC();
  //  }
  //  else
  //  {
  //    m_new_wpt = false;
  //  }
  //}
  //

  //int curr_time_int = int((m_curr_time));
  //int zig_duration_int = int((m_zig_duration));

  //if ( m_curr_time - m_wpt_timestamp >= 5 )
  //{
  //  // If zig_duration has passed switch direction
  //  if ( curr_time_int % zig_duration_int == 0)
  //  {
  //    if (m_zig_left)
  //    {
  //      m_desired_heading = m_wpt_capture_angle - m_zig_angle;
  //      m_zig_left = false;
  //    }
  //    else
  //    {
  //      m_desired_heading = m_wpt_capture_angle + m_zig_angle;
  //      m_zig_left = true;
  //    }
  //  }
  //  ipf = buildFunctionWithZAIC();
  //}

  if ( m_new_wpt && m_curr_time - m_wpt_timestamp >= 5 )
  {
    if ( (m_curr_time - m_wpt_timestamp) <= m_zig_duration + 5 )
    {
      if (m_zig_left)
      {
        m_desired_heading = m_wpt_capture_angle - m_zig_angle;
      }
      else
      {
        m_desired_heading = m_wpt_capture_angle + m_zig_angle;
      }
      ipf = buildFunctionWithZAIC();
    }
    else
    {
      m_zig_left = !m_zig_left;
      m_wpt_timestamp = getBufferCurrTime() + m_zig_duration;
      //m_new_wpt = false;
    }
  }


  m_wpt_index = next_wpt_index;


  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}


IvPFunction* BHV_ZigLeg2::buildFunctionWithZAIC()
{
  // Step 2 - Create the ZAIC_PEAK with the domain and variable name
  ZAIC_PEAK  crs_zaic(m_domain, "course");

  // Step 3 - Configure the ZAIC_PEAK parameters
  //double rel_ang_to_wpt = m_nav_heading;

  crs_zaic.setSummit(m_desired_heading);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false)
  {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  // Step 4 - Extract the IvP function
  IvPFunction *ivp_function = 0;
  if(crs_zaic.stateOK())
    ivp_function = crs_zaic.extractIvPFunction();
  else
    cout << crs_zaic.getWarnings();
  return(ivp_function);
}
