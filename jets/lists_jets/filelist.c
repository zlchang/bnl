#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(argc=!2)
    {
      printf("The command line is *.exe *.list\n");
      return 1;
    }

  const int N = atoi(argv[2]);

  char prefix[64];
  if(argv[3]) sprintf(prefix, "%s", argv[3]);
  else sprintf(prefix, "%s", "run12.subset");
  int cntr;
  int index;
  int i;

  char path[200];
  char outfile[100];
  FILE *fp,*fout[N];

  if((fp=fopen(argv[1],"r"))==NULL)
    {
      printf("Can't open %s \n",argv[1]);
      return 1;
    }
  cntr=0;

  for(i=0;i<N;i++)
    {
      sprintf(outfile,"%s.%d.list", prefix, i);
      fout[i]=fopen(outfile,"w");
    }

  while(fscanf(fp,"%s",path)==1)
    {
  //    sscanf(path,"/home/starsink/raw/daq/2012/%d/%d/%s",&day,&run,flnm);
      index=cntr%N;
      printf("%d, %d, %s\n",cntr,index,path);
      //    printf("%s,%d\n",path,index);
      cntr++;
      fprintf(fout[index],"%s\n",path);

    }
  fclose(fp);

  for(i=0;i<N;i++)
    {
      fclose(fout[i]);
    }

  return 0;
}
