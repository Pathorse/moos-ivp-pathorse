/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.cpp                                  */
/*    DATE: 02.24.2020                                      */
/************************************************************/

#include "PrimeEntry.h"
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <math.h>
#include "MBUtils.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeEntry::PrimeEntry()
{
    m_start_index      = 2;
    m_orig             = 0;
    m_remainder        = 0;
    m_done             = false;
    m_received_index   = 0;
    m_received_time    = 0;
    m_calculated_index = 0;
    m_calculated_time  = 0;
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
    m_remainder = val; // Initialize remainder to original value
}

/*
 * Input:
 *        max_steps    - largest number of steps allowed before termination
 *        max_time     - longest computational time allowed before termination
 *        enable_steps - bool deciding wheter to enforce termination by steps   **DEFAULT = true
 *        enable_time  - bool deciding wheter to enforce termination by time    **DEFAULT = true
 * Output:
 *        true         - If computation finished
 *        false        - If computation terminated by steps or timer
 *
 */
bool PrimeEntry::factor(unsigned long int max_steps, double max_time, bool enable_steps = true, bool enable_timer = true)
{
    unsigned long int num_it = 0; // Counter used for current number of iterations
    double start_time        = MOOSTime(); // Function start time in MOOSTime

    while (m_remainder != 1)
    {
        // Num of iterations termination
        if (enable_steps && num_it >= max_steps) // Quit if max num steps is reached
            return false;

        // Time termination
        if (enable_timer && (MOOSTime() - start_time) >= max_time ) // Quit if max time is reached
            return false;


        // Factorization --------------------------------------------------
        if (m_remainder % m_start_index == 0)
        {
            m_factors.push_back(m_start_index); // Add factor to factors
            m_remainder /= m_start_index; // Calculate and set remainder
        }
        else
        {
            m_start_index++; // Increment divisor, kept over multiple callbacks
        }
        // -----------------------------------------------------------------

        num_it++;

    }



    return true;
}


/*
 * Output:
 *        factors - string consisting of prime factors separated by :
 */
string PrimeEntry::getFactors()
{
    string factors;

    vector<uint64_t>::iterator p;
    for (p = m_factors.begin(); p < m_factors.end(); p++)
        factors += to_string(*p) + ":";

    return factors;
}

/*
 * Output:
 *        report - String of desired object information
 */
string PrimeEntry::getReport()
{
    string report;

    report = "orig=" +  to_string(m_orig) +
        ",received=" + to_string(m_received_index) +
        ",calculated=" + to_string(m_calculated_index) +
        ",solve_time=" + to_string(m_calculated_time - m_received_time) +
        ",primes=" + getFactors() +
        ",username=pathorse";


        /* Additional info, used for debugging purposes
       
        + ",done=" + to_string(m_done) +
        ",run_time=" + to_string(m_received_time - MOOSTime()) +
        ",remainder=" + to_string(m_remainder) +
        ",current_running_index=" + to_string(m_start_index);

        */
   
    return report;
}
