#!/bin/bash

STARTING_DIR=`pwd`

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

chmod +x ./prebuild/prebuild.sh

cd prebuild/
./prebuild.sh

cd $STARTING_DIR

mkdir include/la -p
cp prebuild/la/la.h include/la/la.h

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

if [ "$1" = "-r" ]; then
	echo "Running placeholder game.."
	./placeholder
fi

cd $STARTING_DIR