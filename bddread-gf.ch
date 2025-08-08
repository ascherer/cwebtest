@x
With change files I can adapt it to other uses.
@y
Here I compute the generating function.

The generating function associated with node |p| begins at location
|l=mem[0][p].mark| in the |gf| array. Suppose |gf[l]|, |gf[l+1]|, \dots,
are $c_0$, $c_1$, \dots, $c_k$, $d$, where $d<0$; the associated
generating function is then $c_0z^{-d-1}+c_1z^{-d}+\cdots+c_kz^{k-1-d}$.

@z
@x
@d bdds 1 /* this many BDDs */
@y
@d bdds 1 /* this many BDDs */
@d gfsize 85000000 /* this many entries in the |gf| table (100M OK too) */
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
typedef struct {
  long long u,l; /* upper and lower halves */
} dlong;
dlong dlong0={0,0};
dlong dlong1={0,1};
dlong* gf;
int gfptr;
dlong acc1[varsize+1],acc2[varsize+1];
@z
@x
  register int j,k,r,minv;
@y
  register int j,k,r,minv;
  gf=(dlong*)malloc(gfsize*sizeof(dlong));
  if (!gf) {
    printf("Sorry, I can't allocate %d dlongs for the gf array!\n",gfsize);
    exit(-98);
  }
@z
@x
@ @<Sub...@>=

@ @<Do our thing@>=
@y
@ First, two-longlong fixed point addition on nonnegative integers.
Here and below I assume that the sums will not exceed
$10^{36}$.

@d ten_to_18th 1000000000000000000

@<Sub...@>=
dlong dadd(dlong x,dlong y) {
  dlong z;
  z.l=x.l+y.l;
  if (z.l>ten_to_18th) z.l-=ten_to_18th, z.u=x.u+y.u+1;
  else z.u=x.u+y.u;
  if (z.u>ten_to_18th) {
    fprintf(stderr,"Possible integer overflow!\n");
    printf("Possible integer overflow!\n");
    exit(-666);
  }
  return z;
}
void print_dlong(FILE *f,dlong x) {
  if (x.u) fprintf(f,"%lld%018lld",x.u,x.l);
  else printf("%lld",x.l);
}

@ Next, a recursive subroutine.

@<Sub...@>=
void findgf(int p) {
  register int j,k,q;
  int c=0,cc=0,newleft=0,ccc,kk,d1,d2,l1,l2,l;
  q=mem[0][p].lo;
  if (q) {
    if (mem[0][q].mark==0) findgf(q);
    c=mem[0][q].mark;
  }
  q=mem[0][p].hi;
  if (q) {
    if (mem[0][q].mark==0) findgf(q);
    cc=mem[0][q].mark;
  }
  if (cc) @<Set |acc2| to the right gf@>;
  if (c) @<Set |acc1| to the left gf@>;
  @<Set |ccc| to the sum of the left and right gf's@>;
  mem[0][p].mark=ccc;
}

@ @<Set |acc1| to the left gf@>=
{
  for (k=0;gf[c+k].l>=0;k++) acc1[k]=gf[c+k];
  d1=k, l1=-1-gf[c+k].l;
  q=mem[0][p].lo;
  for (k=mem[0][p].v+1;k<mem[0][q].v;k++) if (present[k]) {
    newleft=1,acc1[d1]=acc1[d1-1],d1++;
    for (j=d1-2; j; j--)
      acc1[j]=dadd(acc1[j],acc1[j-1]); /* multiply |acc1| by $1+z$ */
  }
}

@ @<Set |acc2| to the right gf@>=
{
  for (k=0;gf[cc+k].l>=0;k++) acc2[k]=gf[cc+k];
  d2=k, l2=-gf[cc+k].l;
  for (k=mem[0][p].v+1;k<mem[0][q].v;k++) if (present[k]) {
    acc2[d2]=acc2[d2-1],d2++;
    for (j=d2-2; j; j--) {
      acc2[j]=dadd(acc2[j],acc2[j-1]); /* multiply |acc2| by $1+z$ */
    }
  }
}

@ @<Set |ccc| to the sum...@>=
if (gfptr+varsize>=gfsize) {
  fprintf(stderr,"Memory overflow (gfsize=%d)!\n",gfsize);
  exit(-97);
}
ccc=gfptr+1;
if (c==0) {
  for (k=0;k<d2;k++) gf[++gfptr]=acc2[k];
  gf[++gfptr].l=-1-l2;
}@+else if (cc==0) {
  if (newleft) {
    for (k=0;k<d1;k++) gf[++gfptr]=acc1[k];
    gf[++gfptr].l=-1-l1;
  }@+else ccc=c; /* retain the old left gf if we're only adding 0 */
}@+else {
  l=(l1<l2? l1: l2);
  for (k=kk=0;k<d1 || kk<d2;) {
    if (l1<l2) {
      if (k<d1) gf[++gfptr]=acc1[k++],l1++;
      else gf[++gfptr]=dlong0,l1++;
    }@+else if (l2<l1) {
      if (kk<d2) gf[++gfptr]=acc2[kk++],l2++;
      else gf[++gfptr]=dlong0,l2++;
    }@+else if (k==d1) gf[++gfptr]=acc2[kk++];
    else if (kk==d2) gf[++gfptr]=acc1[k++];
    else {
      gf[++gfptr]=dadd(acc1[k++],acc2[kk++]);
    }
  }
  gf[++gfptr].l=-1-l;
}

@ @<Do our thing@>=
gf[1].u=0, gf[1].l=1, gf[2].u=0, gf[2].l=-1, gfptr=2;
mem[0][1].mark=1, mem[0][1].v=varsize;
findgf(root[0]);
printf("The generating function coefficients are");
for (k=mem[0][root[0]].mark;gf[k].l>=0;k++) {
  printf(" ");
  print_dlong(stdout,gf[k]);
}
printf(" (* z^%lld..z^%lld).\n",-1-gf[k].l,k-mem[0][root[0]].mark-2-gf[k].l);
@z
