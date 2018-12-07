#!/bin/bash

iDir=0
while [ "$iDir" -lt 10 ]
	do
		cd working_$iDir/
		cp ../cleanup.sh .
		./cleanup.sh &
		rm nohup*.out &
		cd ../
		((iDir +=1))
	done
