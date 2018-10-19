#!/bin/tcsh

set file = $argv[1]
set dir = output/tmp
foreach line (`cat $file`)
  foreach i (`seq 0 9`)
    if ( ! -f $dir/$line/$line.mudst.subset.$i.list.emcGeant.root) then
      echo "sbatch --array $i batch.slr $line"
      sbatch --array $i batch.slr $line
    endif
  end
end

