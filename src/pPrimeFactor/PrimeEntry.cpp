/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "PrimeEntry.h"
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeEntry::PrimeEntry()
{
    m_start_index      = 0;
    m_orig             = 0;
    m_done             = false;
    m_received_index   = 0;
    m_calculated_index = 0;

    m_factors          = {};
}

//---------------------------------------------------------
// Destructor

PrimeEntry::~PrimeEntry()
{

}

//---------------------------------------------------------

void PrimeEntry::setOriginalVal(unsigned long int val)
{
    m_orig = val;
}
