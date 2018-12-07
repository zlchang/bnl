#!/bin/sh
list=$1
cur=info_$list\_log
mkdir -p $cur
for pt in $( < ptbin.list);
do
	getInfo.sh $pt $list > $cur/$pt.info.dat &
done
