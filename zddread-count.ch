@x
With change files I can adapt it to other uses.
@y
Here I compute the total number of solutions.
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
typedef struct {
  long long c2,c1; /* upper and lower halves */
} dlong;
dlong dlong0={0,0};
dlong dlong1={0,1};
dlong count[memsize];
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
  z.c1=x.c1+y.c1;
  if (z.c1>ten_to_18th) z.c1-=ten_to_18th, z.c2=x.c2+y.c2+1;
  else z.c2=x.c2+y.c2;
  if (z.c2>ten_to_18th) {
    fprintf(stderr,"Possible integer overflow!\n");
    printf("Possible integer overflow!\n");
    exit(-666);
  }
  return z;
}
void print_dlong(FILE *f,dlong x) {
  if (x.c2) fprintf(f,"%lld%018lld",x.c2,x.c1);
  else printf("%lld",x.c1);
}

@ Next, a recursive subroutine.

@<Sub...@>=
void countsols(int p) {
  register int q;
  dlong c=dlong0;
  q=mem[0][p].lo;
  if (q) {
    if (mem[0][q].mark==0) countsols(q);
    c=count[q];
  }
  q=mem[0][p].hi;
  if (q) {
    if (mem[0][q].mark==0) countsols(q);
    c=dadd(c,count[q]);
  }
  mem[0][p].mark=1;
  count[p]=c;
}

@ @<Do our thing@>=
count[1]=dlong1, mem[0][1].mark=1, mem[0][1].v=varsize;
countsols(root[0]);
print_dlong(stdout,count[root[0]]);
printf(" solutions.\n");
@z
