/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.h                                    */
/*    DATE: 02.24.2020                                      */
/************************************************************/

 #include <string>
 #include <vector>
 #include <cstdint>
 #include "MOOS/libMOOS/MOOSLib.h"

 #ifndef PRIME_ENTRY_HEADER
 #define PRIME_ENTRY_HEADER

 class PrimeEntry
 {
     public:

         PrimeEntry();
         ~PrimeEntry();

         void setOriginalVal(unsigned long int val);
         void setReceivedIndex(unsigned int val)                     {m_received_index=val;};
         void setReceivedTime(double val)                            {m_received_time=val;};
         void setCalculatedIndex(unsigned int val)                   {m_calculated_index=val;};
         void setCalculatedTime(double val)                          {m_calculated_time=val;};
         void setDone(bool done)                                     {m_done= done;};

         bool done()                                                 {return(m_done);};
         bool   factor(unsigned long int max_steps, double max_time, bool enable_step, bool enable_timer);

         std::string  getFactors();
         std::string  getReport();

     protected:
         uint64_t         m_start_index;
         uint64_t         m_orig;
         uint64_t         m_remainder;
         bool             m_done;
         unsigned int     m_received_index;
         unsigned int     m_calculated_index;
         double           m_received_time;       // MOOSTime
         double           m_calculated_time;     // MOOSTime

         std::vector<uint64_t> m_factors;
 };
#endif
