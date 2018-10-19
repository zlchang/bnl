#!/bin/sh
dir=$HOME/data_embed/pp500_production_2012
list=lists_mudst

mkdir -p $list
for pt in $( < $1);
#for pt in pt2_3;
do
	if [ -e $list/$pt.good.list ];
	then
		rm $list/$pt.good.list
	else
		touch $list/$pt.good.list
	fi
	#echo $day
	while read line
	do
		#echo $line
                base=`basename $line .jets.root`
		echo $base
		index=`expr index $base, c`
		#echo $index
  		run=${base:$((index+1)):8}
		#echo $run
		day=${run:2:3}
 		#echo $pt
		file=$dir/$pt\_100_20160201/P13ib.SL13b/2012/$day/$run/$base.MuDst.root
		ls -d $file >> $list/$pt.good.list
	#done < lists_pythia/$run.good.list
	done < lists_jets/$pt.jets.list
done
