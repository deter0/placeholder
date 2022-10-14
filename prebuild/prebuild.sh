#!/bin/bash

STARTING_DIR=`pwd`


red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

# LA
cd la
echo "$green> RUNNING MAKE ON LA!$reset"

cmd="make"
$cmd

status=$?
if [ $status -eq 0 ]; then
	echo "$green> Command \`$cmd\` success!$reset";
	echo ""
else
	echo "$red$cmd failed$reset";
	exit -1
fi


if [ ! -f "la.h" ]; then
	echo "LA FAILED TO GENERATE! (probably more information above, this is a bug.)"
	exit -1
fi

cd $STARTING_DIR


