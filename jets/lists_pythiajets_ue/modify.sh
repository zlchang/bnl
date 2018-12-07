#!/bin/sh

for pt in $(< $1);
do
        #mv $pt.jets.list $pt.list
	echo "sed -i \'s/\global\/homes\/z\/zchang\/data_embed/\/star\/u\/zchang\/data03/g\' $pt.list"
#	sed -i -e 's/\global\/homes\/z\/zchang\/data_embed/star\/u\/zchang\/data03/g' $pt.list
	sed -i -e 's/\/\/star/\/star/g' $pt.list
done
