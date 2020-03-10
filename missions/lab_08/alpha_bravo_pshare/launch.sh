#!/bin/bash -e
#----------------------------------------------------------
#  Script: launch.sh
#  Author: Michael Benjamin
#  LastEd: May 20th 2019
#----------------------------------------------------------
#  Part 1: Set Exit actions and declare global var defaults
#----------------------------------------------------------
trap "kill -- -$$" EXIT SIGTERM SIGHUP SIGINT SIGKILL
TIME_WARP=1
COMMUNITY1="shoreside"
COMMUNITY2="alpha"
COMMUNITY3="bravo"
GUI="yes"

#----------------------------------------------------------
#  Part 2: Check for and handle command-line arguments
#----------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	echo "launch.sh [SWITCHES] [time_warp]   "
	echo "  --help, -h                       " 
	exit 0;
    elif [ "${ARGI}" = "--nogui" ] ; then
	GUI="no"
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done


#----------------------------------------------------------
#  Part 3: Launch the processes
#----------------------------------------------------------
echo "Launching $COMMUNITY1, $COMMUNITY2 and $COMMUNITY3 MOOS Communities. WARP is" $TIME_WARP
pAntler $COMMUNITY1.moos --MOOSTimeWarp=$TIME_WARP >& /dev/null &
pAntler $COMMUNITY2.moos --MOOSTimeWarp=$TIME_WARP >& /dev/null &
pAntler $COMMUNITY3.moos --MOOSTimeWarp=$TIME_WARP >& /dev/null &

uMAC -t $COMMUNITY1.moos
uMAC -t $COMMUNITY2.moos
uMAC -t $COMMUNITY3.moos
