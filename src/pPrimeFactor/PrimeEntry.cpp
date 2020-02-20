/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.cpp                                  */
/*    DATE:                                                 */
/************************************************************/

#include "PrimeEntry.h"
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <math.h>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeEntry::PrimeEntry()
{
    m_start_index      = 0;
    m_orig             = 0;
    m_remainder        = 0;
    m_done             = false;
    m_received_index   = 0;
    m_calculated_index = 0;
    m_solve_time       = 0;

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
    m_orig      = val;
    m_remainder = val;
}


bool PrimeEntry::factor(unsigned long int max_steps)
{
    int num_it = 0;
    unsigned long int div;
    (m_start_index > 2) ? div = m_start_index : div = 2;

    while (m_remainder != 1 && num_it < max_steps)
    {
        if (m_remainder % div == 0)
        {
            m_factors.push_back(div);
            m_remainder /= div;
        }
        else
        {
            m_start_index++;
        }
    }



    return true;
}

string PrimeEntry::getFactors()
{
    string factors;

    vector<uint64_t>::iterator p;
    for (p = m_factors.begin(); p < m_factors.end(); p++)
        factors += to_string(*p) + ":";

    return factors;
}

string PrimeEntry::getReport()
{
    string report;

    report = "orig=" +  to_string(m_orig) +
        ",received=" + to_string(m_received_index) +
        ",calculated=" + to_string(m_calculated_index) +
        ",solve_time=" + to_string(m_solve_time) +
        ",primes=" + getFactors() +
        ",username=pathorse";
   
    return report;
}
