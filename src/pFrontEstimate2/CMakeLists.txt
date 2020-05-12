#--------------------------------------------------------
# The CMakeLists.txt for:                  pFrontEstimate
# Author(s):                               Henrik Schmidt
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pFrontEstimate2 ${SRC})
   
TARGET_LINK_LIBRARIES(pFrontEstimate2
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   henrik_anneal
)
