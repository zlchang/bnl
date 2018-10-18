#!/bin/sh

for pt in $( < $1); do
	echo $pt
	$HOME/bin/filelist $pt.good.list 10 $pt.mudst.subset
done
