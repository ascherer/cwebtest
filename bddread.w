\datethis
@*Intro. This program reads output from an {\mc o} command in  {\mc BDD14}
into internal arrays, by brute force.

With change files I can adapt it to other uses.

The output from an {\mc o} command in {\mc BDD15}
can be read in an identical fashion,
so {\mc zddread.w} is identical to {\mc bddread.w}.
However, the interpretations are different
and the correct program must be applied to each output.

Note that if a variable is not present in the input to {\mc bddread}
then it is ignored.
If there is a variable that is not present in the BDD 
(because it is allowed to be true or false in all solutions), 
then you need to add this possibility yourself.
For example, you must multiply the BDD solution count by a factor of 2
for every such variable.
This is typically not a problem in ZDDs,
because a variable not present in a ZDD is forced to be false.

@d memsize 40000000 /* this many nodes */
@d varsize 8192 /* this many variables */
@d bdds 1 /* this many BDDs */
@d bufsize 100 /* buffer size; 100 is plenty big */

@c
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  int v;
  int lo;
  int hi;
  int mark;
} node;
int present[varsize];
node* mem[bdds];
int root[bdds];
FILE *infile;
char buf[bufsize];
unsigned int i1,i2,i3,i4;
int memmax;
@<Subroutines@>@;

int
main(int argc,char *argv[]) {
  register int j,k,r,minv;
  for (r=0;r<bdds;r++) {
    mem[r]=(node*)malloc(memsize*sizeof(node));
    if (!mem[r]) {
      printf("Sorry, I can't allocate mem[%d]!\n",r);
      exit(-99);
    }
    for (k=0;k<memsize;k++) mem[r][k].lo=mem[r][k].hi=0;
    if (!(infile=fopen(argv[r+1],"r"))) {
      printf("Sorry, I can't open `%s' for reading!\n",argv[r+1]);
      exit(-1);
    }
    for (k=0,minv=varsize;;) {
      if (!fgets(buf,bufsize,infile)) break;
      j=sscanf(buf,"%x: (~%u?%x:%x)\n",&i1,&i2,&i3,&i4);
      if (j!=4)
        printf("! I got only %d inputs from the line %s",j,buf);
      else {
        if (i1>memmax) memmax=i1;
        if (i3>memmax) memmax=i3;
        if (i4>memmax) memmax=i4;
        if (i1>=memsize || i2>=varsize || i3>=memsize || i4>=memsize) {
          printf("! address out of range in the line %s",buf);
          exit(-69);
        }@+else if (mem[r][i1].lo || mem[r][i1].hi)
          printf("! clobbered node in the line %s",buf);
        else {
          if (i2<minv) minv=i2,root[r]=i1;
          k++,mem[r][i1].v=i2, mem[r][i1].lo=i3, mem[r][i1].hi=i4;
          present[i2]=1;
        }
      }
    }
    fprintf(stderr,"%d nodes input into mem%d\n",k,r);
    fprintf(stderr,"(memmax=%d)\n",memmax);
  }
  for (j=k=0;j<varsize;j++) if (present[j]) k++;
  fprintf(stderr,"There are %d variables.\n",k);
  @<Do our thing@>;
}

@ @<Sub...@>=

@ @<Do our thing@>=

@*Index.
