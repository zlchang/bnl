#!/bin/bash

libdir=`pwd`
listdir=`pwd`/lists_mudst
mkdir -p scripts/
#for weight in 0;
#do
	outdir=`pwd`/output/track_rn
	for pt in $( < $1); do
		echo pt=$pt
		mkdir -p $outdir/$pt
	        mkdir -p $outdir/$pt/log
		mkdir -p $outdir/$pt/err
		star-submit-template -template track.xml -entities pt=$pt,outdir=$outdir,listdir=$listdir,libdir=$libdir
	done
#done
