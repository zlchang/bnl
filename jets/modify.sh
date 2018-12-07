#!/bin/sh

for pt in $(< $1);
do
	echo "sed -i \'s/\global\/homes\/z\/zchang\/data_embed/\/star\/u\/zchang\/data03/g\' $pt.jets.list"
#	sed -i -e 's/\global\/homes\/z\/zchang\/data_embed/\/star\/u\/zchang\/data03/g' $pt.jets.list
	sed -i -e 's/\/\/star/\/star/g' $pt.jets.list
done
