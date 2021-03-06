/*****************************************************************/
/*    NAME: Henrik Schmidt                                       */
/*    ORGN: MIT Cambridge MA                                     */
/*    FILE: BHV_ZigZag.cpp                                 */
/*    DATE: Mar. 3, 2007                                         */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/

#ifdef _WIN32
#pragma warning(disable : 4786)
#pragma warning(disable : 4503)
#endif
#include <math.h> 
#include <stdlib.h>
#include "BHV_ZigZag.h"
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "XYPoint.h"

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_ZigZag::BHV_ZigZag(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  this->setParam("descriptor", "(d)bhv_zigzag");

  // Behavior Parameter Default Values:
  period               = 500; // meters
  amplitude            = 250;   // meters
  heading              = 0;     // degrees
  distance_traveled    = 0;   // meters
  zigzag_function = "sinusoidal";

  // Behavior State Variable Initial Values:
  first_iteration      = true;
  start_x = 0;
  start_y = 0;

  // Added by Paal -----------------
  m_new_heading_timestamp = 0;
  m_wpt_index = 0;
  m_wpt_capture_angle = 0;
  m_curr_time = getBufferCurrTime();
  m_wpt_timestamp = 0;
  m_new_wpt = false;

  // -------------------------------

  m_domain = subDomain(m_domain, "speed,course");

  m_current_compensator = false;
  north_current_spd = 0.0;
  east_current_spd = 0.0;

  // Declare the variables we will need from the info_buffer
  addInfoVars("NAV_X");
  addInfoVars("NAV_Y");
  addInfoVars("CURRENT_X");
  addInfoVars("CURRENT_Y");
  addInfoVars("OPREG_ABSOLUTE_PERIM_DIST");
  addInfoVars("WPT_INDEX", "no_warning");
  addInfoVars("VIEW_POINT", "no_warning");
}

//-----------------------------------------------------------
// Procedure: onIdleState
//      Note: This function overrides the onIdleState() virtual
//            function defined for the IvPBehavior superclass
//            This function will be executed by the helm each
//            time the behavior FAILS to meet its run conditions.

void BHV_ZigZag::onIdleState()
{
  // Do your thing here
  first_iteration      = true;
  distance_traveled    = 0;
  start_x = 0;
  start_y = 0;   
}


//-----------------------------------------------------------
// Procedure: setParam

bool BHV_ZigZag::setParam(string g_param, string g_val) 
{
  if(IvPBehavior::setParamCommon(g_param, g_val))
    return(true);

  g_val = stripBlankEnds(g_val);

  if(g_param == "period") 
    {
      double dval = atof(g_val.c_str());
      if((dval <= 0) || (!isNumber(g_val)))
	return(false);
      period = dval;
	
      return(true);
    } 
  if(g_param == "speed") 
    {
      double dval = atof(g_val.c_str());
      if((dval <= 0) || (!isNumber(g_val)))
	return(false);
      speed = dval;
	
      return(true);
    } 
  // else if(g_param == "heading")
  //   {
  //     double dval = atof(g_val.c_str());
  //     if((dval < 0) || (!isNumber(g_val)))
	// return(false);
  //     heading = dval;
  //     return(true);
  //   }
  else if(g_param == "amplitude") 
    {
      double dval = atof(g_val.c_str());
      if((dval < 0) || (!isNumber(g_val)))
	return(false);
      amplitude = dval;
      return(true);
    }
  else if(g_param == "zigzag_function") 
    {
      if(g_val == "")
	return(false);
      zigzag_function = g_val;
      return(true);
    } 
  else if(g_param == "current_compensator") 
    {
      g_val = tolower(g_val);
      if((g_val!="on")&&(g_val!="off"))
	return(false);
      m_current_compensator = (g_val == "on");
      return(true);
    }  

  return(false);
}

//-----------------------------------------------------------
// Procedure: onRunState()

IvPFunction *BHV_ZigZag::onRunState()
{
  double curr_ang,head_1,head_2;
 
  bool   ok,ok1,ok2,ok3,ok4,ok5,ok6;
  double curr_x = getBufferDoubleVal("NAV_X", ok);
  double curr_y = getBufferDoubleVal("NAV_Y", ok1);

  double tmp_x = getBufferDoubleVal("CURRENT_X", ok3);
  double tmp_y = getBufferDoubleVal("CURRENT_Y", ok4);


  // STUFF ADDED BY PAAL ------------------------------------------------
  // --------------------------------------------------------------------

 // double opreg_dist = getBufferDoubleVal("OPREG_ABSOLUTE_PERIM_DIST", ok5);
 // double opreg_eta = getBufferDoubleVal("OPREG_TRAJECTORY_PERIM_ETA", ok6);

 // double curr_time = getBufferCurrTime();

 // double dtimestamp = curr_time - m_new_heading_timestamp;

 // //if (opreg_dist <= 25 && dtimestamp <= 20)
 // //if (opreg_eta <= 10)
 // postMessage("Current_time", curr_time );
 // postMessage("Current_time_int", int(curr_time) );

 // if (int(curr_time) % 150 == 0)
 // {
 //   double reversed_heading = angle360(heading + 180);
 //   postMessage("Resetting heading, prev_head=", heading);
 //   heading = reversed_heading;
 //   postMessage("Resetting heading, new_head=", heading);
 //   m_new_heading_timestamp = getBufferCurrTime();
 // }

  m_curr_time = getBufferCurrTime();

  double next_wpt_index = getBufferDoubleVal("WPT_INDEX", ok5);
  string next_wpt       = getBufferStringVal("VIEW_POINT", ok6);

  if (ok6)
  {
    m_next_wpt.set_vx(stof(tokStringParse(next_wpt, "x", ',', '=')));
    m_next_wpt.set_vy(stof(tokStringParse(next_wpt, "y", ',', '=')));
  }

  if (next_wpt_index != m_wpt_index)
  {
    m_wpt_timestamp = m_curr_time;
    m_wpt_capture_angle = relAng(curr_x, curr_y, m_next_wpt.x(), m_next_wpt.y());
    m_new_wpt = true;
    heading = m_wpt_capture_angle;
  }

  m_wpt_index = next_wpt_index;

  // --------------------------------------------------------------------


  if (ok3 && ok4)
  {
    north_current_spd = tmp_y;
    east_current_spd = tmp_x;
  }
  
  if(first_iteration) 
  {
    first_iteration = false;
    distance_traveled = 0;
    start_x = curr_x;
    start_y = curr_y;
  }
  else
    {
      // Projected distance in desired heading direction
      head_x = cos(headingToRadians(heading));
      head_y = sin(headingToRadians(heading));
      distance_traveled = (curr_x - start_x)*head_x + (curr_y - start_y)*head_y;
    }
  //  postMessage("DIST_TRAVELED",distance_traveled);
  if (distance_traveled < 0)
    new_heading = heading;
  else if (zigzag_function == "linear")
    {
      int half_period_num = (int) (distance_traveled*2.0/period + 0.5);
      //    postMessage("HALF_PERIOD_NUM",(double) half_period_num);
      if ( half_period_num%2 == 1 )
	new_heading = angle360(heading - radToDegrees(atan(2*amplitude/period)));
      else
	new_heading = angle360(heading + radToDegrees(atan(2*amplitude/period)));
    }
  else
    new_heading = angle360(heading + radToDegrees(atan(cos((distance_traveled/period)*2*M_PI)*2*M_PI*amplitude/period)));
  
  if (m_current_compensator)
    {
      new_heading = currentCompensatedHeading(new_heading, speed);
      postMessage("COMPENSATED_HEADING",new_heading);
    }
    
  // postMessage("NEW_HEADING",new_heading);
  
  ZAIC_PEAK zaic_heading(m_domain,"course");
  zaic_heading.setSummit(new_heading);
  zaic_heading.setValueWrap(true);
  zaic_heading.setPeakWidth(30.0);
  zaic_heading.setBaseWidth(120.0);
  zaic_heading.setSummitDelta(60.0);

  IvPFunction *ipf_heading = zaic_heading.extractOF();

  ZAIC_PEAK zaic_speed(m_domain, "speed");
  zaic_speed.setSummit(speed);
  zaic_speed.setSummitDelta(100.0);
  zaic_speed.setPeakWidth(0.5);
  zaic_speed.setMinMaxUtil(0, 100);
 
  IvPFunction *ipf_speed = zaic_speed.extractOF();

  OF_Coupler coupler;
  IvPFunction *new_ipf = coupler.couple(ipf_speed, ipf_heading);

  new_ipf->setPWT(m_priority_wt);

  return(new_ipf);

}

double BHV_ZigZag::currentCompensatedHeading(double desired_heading, double u)
{
  double theta;
  double phi = headingToRadians(desired_heading); 

  double b = -2*(east_current_spd*cos(phi) + north_current_spd*sin(phi));
  double c = east_current_spd * east_current_spd + north_current_spd * north_current_spd - u * u;
  
  double lambda = 0.0;
  
  double D = b*b - 4*c;
  if(D >= 0)
    lambda = (-1*b+sqrt(D))/2;
  else
    lambda = -1;
  
  if(lambda < 0)
    {
      //The flow is too strong for the glider
	theta = atan2(north_current_spd, east_current_spd) + M_PI;
    }
  else
    {
      if(lambda*cos(phi) - east_current_spd != 0.0)
	theta = atan2(lambda*sin(phi)-north_current_spd,lambda*cos(phi)-east_current_spd);
    }
  return(radToHeading(theta));
}
