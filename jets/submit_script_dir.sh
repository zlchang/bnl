#!/bin/bash

script=submit_jetsCrs.sh
iDir=0
dir=working
while [ "$iDir" -lt 10 ]
	do
		mkdir -p $dir\_$iDir/scripts
		cp $script $dir\_$iDir/
		cp cleanup.sh $dir\_$iDir/
#		cp runlists/run12.subset.$iDir.list $dir\_$iDir/run12.subset.list
		cd $dir\_$iDir/
		/usr/bin/nohup $script ../runlists/run12.subset.$iDir.list > nohup.out &
		cd ../
		((iDir +=1))
	done
