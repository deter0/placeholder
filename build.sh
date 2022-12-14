#!/bin/bash

STARTING_DIR=`pwd`

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

if [ "$2" == "-d"] || [ "$1" == "-d" ]; then
	if [ -d "build" ]; then
    rm -r build
	fi
fi


chmod +x ./prebuild/prebuild.sh

cd prebuild/
./prebuild.sh

cd $STARTING_DIR

mkdir include/la -p
cp prebuild/la/la.h include/la/la.h

if [ -d "build" ] 
then
    echo "Build directory exists" 
else
    echo "$red Build Dir Doesn't exist"
		mkdir build -p
		
		cd build
		meson ..
		cd ..
fi

mkdir build -p
cd build

cmd="meson compile"
$cmd

status=$?
if [ $status -eq 0 ]; then
	echo "$green> Command \`$cmd\` success!$reset";
	echo ""
else
	echo "$red$cmd failed$reset";
	exit -1
fi

if [ "$1" = "-r" ] || [ "$2" = "-r" ]; then
	echo "Running placeholder game.."
	./placeholder
fi

cd $STARTING_DIR