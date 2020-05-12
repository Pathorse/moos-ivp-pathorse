/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg2.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef Pulse_HEADER
#define Pulse_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYPoint.h"

class BHV_ZigLeg2 : public IvPBehavior {
public:
  BHV_ZigLeg2(IvPDomain);
  ~BHV_ZigLeg2() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected: // Local Utility functions
    IvPFunction* buildFunctionWithZAIC();

protected: // Configuration parameters
    double m_zig_angle;
    double m_zig_duration;


protected: // State variables
    double  m_nav_x;
    double  m_nav_y;
    double  m_nav_heading;

    double  m_wpt_index;
    double  m_wpt_capture_angle;
    XYPoint m_next_wpt;

    double m_curr_time;
    double m_wpt_timestamp;
    double m_timestamp;

    bool m_new_wpt;

    double m_desired_heading;
    bool m_zig_left;

};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_ZigLeg2(domain);}
}
#endif
