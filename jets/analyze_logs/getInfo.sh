#!/bin/sh
cur=$2\_log
#for pt in pt2_3;
#do
#	sum=0
	pt=$1
	for file in $(< $cur/$pt.list);
	do
		crs=$(awk '/included/ {print $10}' $file)
                crs=${crs/D/E}	
		nevents=$(awk '/NUMBER OF EVENTS PROCESSED/ {print $8}' $file)
#		echo "$nevents"
		base=`basename $file .log`
#		echo "$base"
		length=${#pt}
#		echo "$length"
		run=${base:((length+17)):8}
#		((sum += nevents))
	        echo $run $crs $nevents
#		echo $sum
	done
#done
