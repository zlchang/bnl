#!/bin/tcsh
@ a = (6 / 2)
echo $a

set dataList = pt9_11.B.txt

setenv PATH_DAQ $HOME/jetAnalysis/track/lists_mudst
setenv SLURM_ARRAY_TASK_ID 0
@ kD = ($SLURM_ARRAY_TASK_ID + 1)
echo "pick data $kD from list $dataList"
set dataName = ${PATH_DAQ}/`head -n $kD  $dataList | tail -n1`
set ptbase = `basename $dataList .B.txt`
echo ${ptbase} $dataName

if ( $a == 1 ) then

echo $a==1

endif

time ls -l *.list; ls -l *.C

set SLURM_CLUSTER_NAME = "pdsf1"
echo $SLURM_CLUSTER_NAME
if ( "$SLURM_CLUSTER_NAME" !~ "pdsf*" ) then
    echo $SLURM_CLUSTER_NAME 
endif

set file = $argv[1]

foreach line (`cat $file`)
  echo $line
end < $file
