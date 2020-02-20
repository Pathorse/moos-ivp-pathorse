/************************************************************/
/*    NAME: Paal Arthur S. Thorseth                         */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.h                                    */
/*    DATE:                                                 */
/************************************************************/

 #include <string>
 #include <vector>
 #include <cstdint>
#include <algorithm>

 #ifndef PRIME_ENTRY_HEADER
 #define PRIME_ENTRY_HEADER

 class PrimeEntry
 {
     public:

         PrimeEntry();
         ~PrimeEntry();

         void setOriginalVal(unsigned long int val);
         void setReceivedIndex(unsigned int val)                     {m_received_index=val;};
         void setCalculatedIndex(unsigned int val)                   {m_calculated_index=val;};
         void setDone(bool done)                                     {m_done= done;};

         bool   done() {return(m_done);};

         bool   factor(unsigned long int max_steps);

         std::string  getFactorsString();
         std::string  getReport();

     protected:
         uint64_t         m_start_index;
         uint64_t         m_orig;
         bool             m_done;
         unsigned int     m_received_index;
         unsigned int     m_calculated_index;
         double           m_solve_time;

         std::vector<uint64_t> m_factors;
 };
#endif
