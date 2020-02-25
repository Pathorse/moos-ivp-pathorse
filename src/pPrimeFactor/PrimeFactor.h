/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                   */
/*    DATE: 02.24.2020                                      */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "PrimeEntry.h"
#include <string.h>
#include <list>

using namespace std;

class PrimeFactor : public CMOOSApp
{
    public:
        PrimeFactor();
        ~PrimeFactor();

    protected: // Standard MOOSApp functions to overload  
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

        protected:
        void RegisterVariables();

    private: // Configuration variables

    private: // State variables
        unsigned int m_total_num_received;
        unsigned int m_total_num_calculated;
        list<PrimeEntry> m_received_numbers;
};

#endif 
