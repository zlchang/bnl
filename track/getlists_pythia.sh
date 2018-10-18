#!/bin/sh

dir=$HOME/data_embed/
out=lists_pythia
mkdir -p $out
for run in $( < $1);
do
	ls -d $dir/$run/*.pythia.root > $out/$run.list
done
