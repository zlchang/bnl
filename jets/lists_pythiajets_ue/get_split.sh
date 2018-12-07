#!/bin/sh

for run in $( < $1); do
	echo $run
	#./filelist $run.list 3 $run
	$HOME/software/local/bin/filelist $run.list 5 $run
done
