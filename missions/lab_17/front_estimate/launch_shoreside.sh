#!/bin/bash -e 
#--------------------------------------------------------------  
#  Part 1: Declare global var defaults
#--------------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
HAZARD_FILE="hazards.txt"

IP_ADDR="localhost"
#IP_ADDR="213.128.163.41"
PSHARE_PORT="9300"
#SHORE="localhost:9300"
#SHORE="213.128.163.41:9300"

#-------------------------------------------------------
#  Part 2: Check for and handle command-line arguments
#-------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ]; then
	echo "launch_shoreside.sh [SWITCHES] [time_warp] "
	echo "  --just_make, -j                          "
	echo "  --help, -h                               " 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_make" -o "${ARGI}" = "-j" ]; then
	JUST_MAKE="yes"
    else 
	echo "launch_shoreside.sh: Bad Arg:" $ARGI
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 3: Create the .moos file(s)
#-------------------------------------------------------
nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
       VNAME="shoreside" SHARE_LISTEN=$SHORE_LISTEN               \
       IP_ADDR=$IP_ADDR  PSHARE_PORT=$PSHARE_PORT

if [ ${JUST_MAKE} = "yes" ]; then
    exit 0
fi

#-------------------------------------------------------
#  Part 4: Launch the processes
#-------------------------------------------------------
echo "Launching Shoreside MOOS Community with WARP:" $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
echo "Done "

uMAC targ_shoreside.moos

echo "Killing all processes ... "
kill -- -$$
echo "Done killing processes.   "


