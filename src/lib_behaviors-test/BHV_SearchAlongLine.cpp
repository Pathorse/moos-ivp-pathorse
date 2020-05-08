/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchAlongLine.cpp                         */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include <math.h>
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "BHV_SearchAlongLine.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_SearchAlongLine::BHV_SearchAlongLine(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, NAV_HEADING");


  // Set default config parameters
  m_desired_speed   = 2;
  m_offset_dist     = 0;
  m_px              = 0;
  m_py              = 0;
  m_alpha           = 45;
  m_kp              = 1;


  // Set default state variables
  m_nav_x       = 0;
  m_nav_y       = 0;
  m_nav_heading = 0;

  m_cross_track_error = 0;
  m_desired_heading   = 0;

}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_SearchAlongLine::setParam(string param, string val)
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
  else if((param == "speed") && isNumber(val)) {
    m_desired_speed = double_val;
    return(true);
  }
  else if((param == "offset_dist") && isNumber(val)) {
    m_offset_dist = double_val;
    return(true);
  }
  else if((param == "px") && isNumber(val)) {
    m_px = double_val;
    return(true);
  }
  else if((param == "py") && isNumber(val)) {
    m_py = double_val;
    return(true);
  }
  else if((param == "alpha") && isNumber(val)) {
    m_alpha = double_val;
    return(true);
  }
  else if((param == "kp") && isNumber(val)) {
    m_kp = double_val;
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

void BHV_SearchAlongLine::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_SearchAlongLine::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_SearchAlongLine::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_SearchAlongLine::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_SearchAlongLine::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_SearchAlongLine::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_SearchAlongLine::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_SearchAlongLine::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;

  bool ok1, ok2, ok3;

  m_nav_x       = getBufferDoubleVal("NAV_X", ok1);
  m_nav_y       = getBufferDoubleVal("NAV_Y", ok2);
  m_nav_heading = getBufferDoubleVal("NAV_HEADING", ok3);

  // Calculate the cross track error
  //if (ok1 && ok2)
  m_cross_track_error = -(m_nav_x - m_px)*sin(degToRadians(m_alpha)) + (m_nav_y - m_py)*cos(degToRadians(m_alpha));

  // Find desired course
  m_desired_heading = m_alpha + radToDegrees(atan(- m_kp * (m_cross_track_error - m_offset_dist) ));

  // Build ivp function
  ipf = buildFunctionWithZAIC();

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}


IvPFunction *BHV_SearchAlongLine::buildFunctionWithZAIC()
{
  // Step 2 - Create the ZAIC_PEAK with the domain and variable name
  ZAIC_PEAK spd_zaic(m_domain, "speed");

  // Step 3 - Configure the ZAIC_PEAK parameters
  //double rel_ang_to_wpt = m_nav_heading;
  spd_zaic.setSummit(m_desired_speed);
  spd_zaic.setPeakWidth(0.5);
  spd_zaic.setBaseWidth(1.0);
  spd_zaic.setSummitDelta(0.8);
  if(spd_zaic.stateOK() == false) {
    string warnings = "Speed ZAIC problems " + spd_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  // Step 2 - Create the ZAIC_PEAK with the domain and variable name
  ZAIC_PEAK crs_zaic(m_domain, "course");

  // Step 3 - Configure the ZAIC_PEAK parameters
  //double rel_ang_to_wpt = m_nav_heading;
  //crs_zaic.setSummit(m_desired_heading);
  crs_zaic.setSummit(m_alpha);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  // Step 4 - Extract the IvP function
  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();

  OF_Coupler coupler;
  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  return(ivp_function);
}
