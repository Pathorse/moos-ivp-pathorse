#--------------------------------------------------------
# The CMakeLists.txt for:                       pFrontEstimateComms
# Author(s):                              Paal Arthur S. Thorseth
#--------------------------------------------------------

SET(SRC
  FrontEstimateComms.cpp
  FrontEstimateComms_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pFrontEstimateComms ${SRC})

TARGET_LINK_LIBRARIES(pFrontEstimateComms
   ${MOOS_LIBRARIES}
   geometry
   mbutil
   ufield
   m
   pthread)

