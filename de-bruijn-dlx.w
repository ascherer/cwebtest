@*Intro. Given $0\le p<q\le n$, make {\mc DLX} data for a de Bruijn
cycle through all binary $n$-tuples of weight between $p$ and~$q$, inclusive.

@d maxn 20 /* |n| must be less than this */

@c
#include <stdio.h>
#include <stdlib.h>
int p,q,n; /* command-line parameters */
int bico[maxn][maxn]; /* binomial coefficients */
main(int argc,char*argv[]) {
  register i,j,k,m,t,u,v,acc;
  @<Process the command line@>;
  @<Print the item-name line@>;
  for (k=p;k<=q;k++) @<Print the options for weight |k|@>;
}

@ @<Process the command line@>=
if (argc!=4 || sscanf(argv[1],"%d",
                                   &p)!=1 ||
               sscanf(argv[2],"%d",
                                   &q)!=1 ||
               sscanf(argv[3],"%d",
                                   &n)!=1) {
  fprintf(stderr,"Usage: %s p q n\n",
                            argv[0]);
  exit(-1);
}
if (p<0 || p>=q || q>n) {
  fprintf(stderr,"The parameters must satisfy 0<=p<q<=n!\n");
  exit(-2);
}
if (n>=maxn) {
  fprintf(stderr,"Sorry, n must be less than %d!\n",
                               maxn);
  exit(-3);
}
printf("| %s %d %d %d\n",
                argv[0],p,q,n);
  
@ @<Print the item-name line@>=
for (m=0,k=p;k<=q;k++) @<Print the binary strings of weight |k|@>;
for (j=0;j<m;j++) printf("p%d ",
                                 j);
printf("|");
for (j=0;j<m;j++) printf(" x%d",
                                  j);
printf("\n");

@ @<Print the binary strings of weight |k|@>=
for (acc=(1<<k)-1;acc<1<<n;) {
  printf("%x ",
              acc);
  m++;
  if (!acc) break;
  u=acc&-acc;
  v=acc+u;
  acc=v+(((v^acc)/u)>>2); /* Gosper's hack */
}

@ @<Print the options for weight |k|@>=
for (acc=(1<<k)-1;acc<1<<n;) {
  for (j=0;j<m;j++) {
    if (acc<(1<<p) && j) break; /* place the smallest guy first */
    if (acc>=(1<<p) && j==0) continue;
    printf("%x p%d",
                   acc,j);
    for (i=0;i<n;i++) printf(" x%d:%d",
                  (i+j)%m,
                  (acc&(1<<(n-1-i)))?1:0);
    printf("\n");   
  }
  if (!acc) break;
  u=acc&-acc;
  v=acc+u;
  acc=v+(((v^acc)/u)>>2); /* Gosper's hack */
}

@*Index.
