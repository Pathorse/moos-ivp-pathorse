/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchAlongLine.h                           */
/*    DATE:                                                 */
/************************************************************/

#ifndef BHV_SearchAlongLine_HEADER
#define BHV_SearchAlongLine_HEADER

#include <string>
#include "IvPBehavior.h"

class BHV_SearchAlongLine : public IvPBehavior {
public:
  BHV_SearchAlongLine(IvPDomain);
  ~BHV_SearchAlongLine() {};
  
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
    double m_desired_speed;

    double m_offset_dist;

    double m_px; // x value of Point [x, y] on the line
    double m_py; // y value of Point [x, y] on the line
    double m_alpha; // Line angle

    double m_kp; // Proportional gain

protected: // State variables
    double m_nav_x;
    double m_nav_y;
    double m_nav_heading;

    double m_cross_track_error;
    double m_desired_heading;
   
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SearchAlongLine(domain);}
}
#endif
