\datethis
@*ZDD for combinations.

This program accepts $n$ and $r$ as arguments,
and it prints a ZDD network to \.{/tmp/binomial,n,r.zdd}.
The network defines $Choose(n,r)$
-- that is, exactly $r$ of the $n$ variables must be true.

There will be ${n\choose r}$ solutions.
Text is printed to \.{stdout} at the end that can calculate this number
if piped to a bash shell (etc).
When $r=n/2$, this could be a useful test of overflow
for approximately $(n-3)$-bit storage.

The principle of the network is that
we arrive at \.{node(v,i)} when variable \.{v} is to be decided and
exactly \.{i} of the lesser variables have been selected.

@d node(vvv,iii) (2+(n+1-(vvv))*(r+1)+(r-(iii)))

@p
#include <stdio.h>
#include <stdlib.h>
@#
int
main(int argc, char* argv[])
{
  int n,r;
  int v,i,lo,hi;
  char buf[100];
  FILE *outfile;
  if (argc!=3 || sscanf(argv[1],"%d",&n)!=1 ||
                 sscanf(argv[2],"%d",&r)!=1 ||
                 n<r || r<1 || r>=n ) {
    fprintf(stderr,"Usage: %s n r\n",argv[0]);
    exit(-1);
  }
  sprintf(buf,"/tmp/binomial,%d,%d.zdd",n,r);
  outfile=fopen(buf,"w");
  if (!outfile) {
    fprintf(stderr,"I can't open file %s for writing!\n",buf);
    exit(-71);
  }

  for(v=n;v>=1;--v){
    for(i=r-1;i>=0;--i){
      if(i>=v) continue;
      if(n-v+i-r<=-2) continue;
      if(n-v+i-r==-1)
        lo = 0;
      else
        lo = node(v+1,i);
      if(i==r-1)
        hi = 1;
      else
        hi = node(v+1,i+1);
      fprintf(outfile,"%x: (~%d?%x:%x)\n",node(v,i),v,lo,hi);
    }
  }
  fclose(outfile);
  if(r>n-r)r=n-r;
  /* Write an expression for Choose(n,r), piped to the bc calculator: */
  printf("echo '");
  for(v=n,i=r;i>0;--v,--i) printf("%d*",v);
  printf("1");
  for(v=n,i=r;i>0;--v,--i) printf("/%d",i);
  printf("' | bc\n");

  return 0; /* normal exit */
}

@* Index.

