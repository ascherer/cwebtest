@x
With change files I can adapt it to other uses.
@y
Here I compute all the solutions of maximum weight.
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
int stack[varsize];
int stackptr;
@z
@x
    for (k=0;k<memsize;k++) mem[r][k].lo=mem[r][k].hi=0;
@y
    for(k=0;k<memsize;k++)mem[r][k].lo=mem[r][k].hi=mem[r][k].mark=0;
@z
@x
@ @<Sub...@>=

@ @<Do our thing@>=
@y
@ First, a recursive subroutine to find the length of the longest solutions.
It also zeros out all links that don't lead to longest solutions.
(I could negate them instead of clobbering them.)

@<Sub...@>=
void prep(int p) {
  register int q,lomark,himark,k;
  q=mem[0][p].lo, lomark=0;
  if (q) {
    if (mem[0][q].mark==0) prep(q);
    lomark=mem[0][q].mark;
    for (k=mem[0][p].v+1;k<mem[0][q].v;k++)
      if (present[k]) lomark++;
  }
  q=mem[0][p].hi, himark=0;
  if (q) {
    if (mem[0][q].mark==0) prep(q);
    himark=mem[0][q].mark+1;
    for (k=mem[0][p].v+1;k<mem[0][q].v;k++)
      if (present[k]) himark++;
  }
  if (lomark<himark) mem[0][p].lo=0,mem[0][p].mark=himark;
  else if (lomark>himark) mem[0][p].hi=0,mem[0][p].mark=lomark;  
  else mem[0][p].mark=lomark;
}

@ Next, a recursive subroutine to print out all paths that remain.

@<Sub...@>=
void printpaths(int p) {
  register int k,q,s=stackptr;
  if (p<=1) {
    for (q=0;q<stackptr;q++) printf(" %d",stack[q]);
    printf("\n");
    return;
  }
  q=mem[0][p].lo;
  if (q) {
    for (k=mem[0][p].v+1;k<mem[0][q].v;k++)
      if (present[k]) stack[stackptr++]=k;
    printpaths(q);
    stackptr=s;
  }
  q=mem[0][p].hi;
  if (q) {
    stack[stackptr++]=mem[0][p].v;
    for (k=mem[0][p].v+1;k<mem[0][q].v;k++)
      if (present[k]) stack[stackptr++]=k;
    printpaths(q);
    stackptr=s;
  }
}

@ @<Do our thing@>=
mem[0][1].mark=1, mem[0][1].v=varsize;
prep(root[0]);
fprintf(stderr,"Maximal solutions have length %d.\n",mem[0][root[0]].mark-1);
printpaths(root[0]);
@z
