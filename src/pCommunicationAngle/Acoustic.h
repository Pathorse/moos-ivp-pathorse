/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: Acoustic.h                                      */
/*    DATE: 02.25.2020                                      */
/************************************************************/


#ifndef Acoustic_HEADER
#define Acoustic_HEADER

#include <math.h>


using namespace std;

double pressureFieldAmplitude(double s, double theta0, double z0);
double crossSectionalArea(double s, double r, double theta, double theta0);

#endif
