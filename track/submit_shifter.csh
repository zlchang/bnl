#!/bin/tcsh

set file = $argv[1]

foreach line (`cat $file`)
  set text = $line.B.txt
  if ( -f $text ) then
       rm $text
  endif
  touch $text
  foreach i (`seq 0 9`)
    echo "$line.mudst.subset.$i.list" >> $text
  end
  echo "sbatch --array 0-9 batch.slr $line"
  sbatch --array 0-9 batch.slr $line
end

