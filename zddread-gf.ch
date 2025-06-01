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
  register int k,q;
  int c=0,cc=0,ccc,kk,d,dd,l;
  q=mem[0][p].lo;
  if (q) {
    if (mem[0][q].mark==0) findgf(q);
    c=mem[0][q].mark;
  }
  q=mem[0][p].hi;
  if (q) {
    if (mem[0][q].mark==0) findgf(q);
    cc=mem[0][q].mark;
  }@+else {
    fprintf(stderr,"This isn't a ZDD!\n");
    exit(-66);
  }
  @<Set |ccc| to the sum of |c+z*cc|@>;
  mem[0][p].mark=ccc;
}

@ Here I assume the coefficients will not exceed $2^{63}$ in their
higher slot.

@d gfx(j) gf[j].u /* this is the slot that may hold the degree */

@<Set |ccc| to the sum of |c+z*cc|@>=
if (gfptr+varsize>=gfsize) {
  fprintf(stderr,"Memory overflow (gfsize=%d)!\n",gfsize);
  exit(-97);
}
ccc=gfptr+1;
if (c==0) {
  while (gfx(cc)>=0) gf[++gfptr]=gf[cc++];
  gfx(++gfptr)=gfx(cc)-1;
}@+else {
  for (k=0;gfx(c+k)>=0;k++) acc1[k]=gf[c+k];
  d=k,c=-1-gfx(c+k);
  for (k=0;gfx(cc+k)>=0;k++) acc2[k]=gf[cc+k];
  dd=k,cc=-gfx(cc+k);
  l=(c<cc? c: cc);
  for (k=kk=0;k<d || kk<dd;) {
    if (c<cc) {
      if (k<d) gf[++gfptr]=acc1[k++],c++;
      else gf[++gfptr]=dlong0,c++;
    }@+else if (c>cc) {
      if (kk<dd) gf[++gfptr]=acc2[kk++],cc++;
      else gf[++gfptr]=dlong0,cc++;
    }@+else if (k==d) gf[++gfptr]=acc2[kk++];
    else if (kk==dd) gf[++gfptr]=acc1[k++];
    else {
      gf[++gfptr]=dadd(acc1[k++],acc2[kk++]);
    }
  }
  gfx(++gfptr)=-1-l;
}

@ @<Do our thing@>=
gf[1]=dlong1, gfx(2)=-1, gfptr=2;
mem[0][1].mark=1, mem[0][1].v=varsize;
findgf(root[0]);
printf("The generating function coefficients are");
for (k=mem[0][root[0]].mark;gfx(k)>=0;k++) {
  printf(" ");
  print_dlong(stdout,gf[k]);
}
printf(" (* z^%lld..z^%lld).\n",-1-gfx(k),k-mem[0][root[0]].mark-2-gfx(k));
@z
