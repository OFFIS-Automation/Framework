#!/bin/sh

echo "Staring roscore"
roscore &

if [ "$1" = "debug" ]
then 
	echo "Staring OFFIS Automation Framework Debug"	
	./../../../bin/debug/Toolbox
else
 	echo "Staring OFFIS Automation Framework Release"
	./../../../bin/Toolbox	
fi

echo "Startup done"
exit 0
