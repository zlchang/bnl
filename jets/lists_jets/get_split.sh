#!/bin/sh

for run in $( < $1); do
	echo $run
	#./filelist $run.list 3 $run
	./filelist $run.list 10 $run
done
