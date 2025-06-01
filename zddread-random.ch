@x
With change files I can adapt it to other uses.
@y
Here I generate a random solution.
@z
@x
#include <stdlib.h>
@y
#include <stdlib.h>
#include "gb_flip.h"
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
long long count[3][memsize];
int sols,seed; /* command-line parameters */
@z
@x
  register int j,k,r,minv;
@y
  register int j,k,r,minv;
  if (argc!=4 || sscanf(argv[2],"%d",&sols)!=1 || 
                 sscanf(argv[3],"%d",&seed)!=1) {
    fprintf(stderr,"Usage: %s foo.zdd sols seed\n",argv[0]);
    exit(-1);
  }
  gb_init_rand(seed);
@z
@x
@ @<Sub...@>=

@ @<Do our thing@>=
@y
@ First, a recursive subroutine.

@d two_to_the_62 0x4000000000000000

@<Sub...@>=
void countsols(int p) {
  register int q;
  long long c0=0,c1=0,c2=0;
  q=mem[0][p].lo;
  if (q) {
    if (mem[0][q].mark==0) countsols(q);
    c0=count[0][q],c1=count[1][q],c2=count[2][q];
  }
  q=mem[0][p].hi;
  if (q) {
    if (mem[0][q].mark==0) countsols(q);
    c0+=count[0][q],c1+=count[1][q],c2+=count[2][q];
    if (c2>=two_to_the_62) c1++,c2-=two_to_the_62;
    if (c1>=two_to_the_62) c0++,c1-=two_to_the_62;
    if (c0>=two_to_the_62) {
      fprintf(stderr,"Overflow (186 bits or more)!\n");
      printf("Overflow (186 bits or more)!\n");
      exit(-6);
    }
  }
  mem[0][p].mark=1;
  count[0][p]=c0,count[1][p]=c1,count[2][p]=c2;
}

@ I also need an extralong version of |gb_unif_rand|.

@<Sub...@>=
long long long_unif_rand(long long m) {
  register long long t=two_to_the_62-
                 (two_to_the_62%m);
  register long long r;
  do {
    r=(((long long)gb_next_rand())<<31)+gb_next_rand();
  } while (t<=r);
  return r%m;
}
@#
long long in0,in1,in2,out0,out1,out2;
void triply_longlong_unif_rand(void) {
  register long long t;
  if (!in0) {
    out0=0;
    if (!in1) out1=0,out2=long_unif_rand(in2);
    else {
      for (t=1;t<=in1;t<<=1) ;
      while (1) {
        out1=(((long long)gb_next_rand())<<31)+gb_next_rand();
        out1&=t-1;
        if (out1<=in1) {
          out2=(((long long)gb_next_rand())<<31)+gb_next_rand();
          if (out1<in1 || out2<in2) break;
        }
      }
    }
  }
  else {
    for (t=1;t<=in0;t<<=1) ;
    while (1) {
      out0=(((long long)gb_next_rand())<<31)+gb_next_rand();
      out0&=t-1;
      if (out0<=in0) {
        out1=(((long long)gb_next_rand())<<31)+gb_next_rand();
        out2=(((long long)gb_next_rand())<<31)+gb_next_rand();
        if (out0<in0 || out1<in1 || (out1==in1 && out2<in2)) break;
      }
    }
  }
}

@ @<Do our thing@>=
count[2][1]=1, mem[0][1].mark=1, mem[0][1].v=varsize;
countsols(root[0]);
for (r=0;r<sols;r++) {
  for (k=root[0];k>1;) {
    j=mem[0][k].lo;
    in0=count[0][k],in1=count[1][k],in2=count[2][k];
    triply_longlong_unif_rand();
    if (count[0][j]<out0 || (count[0][j]==out0 && count[1][j]<out1) ||
         (count[0][j]==out0 && count[1][j]==out1 && count[2][j]<=out2)) {
      printf(" %d",mem[0][k].v);
      k=mem[0][k].hi;
    }@+else k=j;
  }  
  printf("\n");
}
@z
