#!/bin/sh

for pt in $( < $1); do
	echo $pt
	$HOME/software/local/bin/filelist $pt.good.list 10 $pt.subset
done
