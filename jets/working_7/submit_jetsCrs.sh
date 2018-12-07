#!/bin/bash
#macro=readMyJetPythiaCrs.C
#listdir=lists_pythiajets_ue
#output=/star/u/zchang/data05/Run12JetsCrsPythiaUe
#macro=readMyJetCrsMc.C
macro=readMyJetCrs.C
listdir=lists_jets
output=/star/u/zchang/data05/Run12JetsCrsUe
mkdir -p scripts/
tempdir=/star/u/zchang/jetAnalysis/jets
for runnumber in $(< $1 ) ; do
#for runnumber in 13078009; do
	echo runnumber=$runnumber
	outdir=$output/$runnumber
#	outdir=/star/u/zchang/Run12JetSpin_final/$runnumber
	mkdir -p $outdir
		star-submit-template -template $tempdir/jetsCrs.xml -entities runnumber=$runnumber,outdir=$outdir,tempdir=$tempdir,listdir=$listdir,macro=$macro
done
