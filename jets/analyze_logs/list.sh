#!/bin/sh
dir=~/data_embed
runlist=$2
cur=$runlist\_log
mkdir -p $cur
for ptbin in $( < $1);
do
	touch $cur/$ptbin.list
	rm $cur/$ptbin.list
	touch $cur/$ptbin.list
#	for file in $(<../../jets/r050_no_filter/lists/$ptbin.list);do
        for run in $( < $runlist); do

		for file in `ls $dir/$run/log/$ptbin\_*.log`;do
		echo $file
		echo $file >> $cur/$ptbin.list
#	base=`basename $file .jets.root`
#	base=`basename $file .MuDst.root`
#	if [ -s $dir/$ptbin\_round1/$base.log ]; then
#		echo $dir/$ptbin\_round1/$base.log >> $cur/$ptbin.list
#		elif [ -s $dir/$ptbin/$base.log ]; then
#		echo $dir/$ptbin/$base.log >> $cur/$ptbin.list
#	fi
		done
	done
done
