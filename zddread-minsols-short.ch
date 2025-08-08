@x
With change files I can adapt it to other uses.
@y
Here I compute all the solutions of minimum weight.
If there's more than one, I print only the first, but note
that more are possible.
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
int stack[varsize];
int stackptr;
int serial;
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
@ First, a recursive subroutine to find the lengths of the lightest solutions.
It also zeros out all links that don't lead to lightest solutions.
(I could negate them instead of clobbering them.)

@<Sub...@>=
void prep(int p) {
  register int q,lomark,himark;
  q=mem[0][p].lo, lomark=varsize+2;
  if (q) {
    if (mem[0][q].mark==0) prep(q);
    lomark=mem[0][q].mark;
  }
  q=mem[0][p].hi, himark=varsize+2;
  if (q) {
    if (mem[0][q].mark==0) prep(q);
    himark=mem[0][q].mark+1;
  }
  if (lomark>himark) mem[0][p].lo=0,mem[0][p].mark=himark;
  else if (lomark<himark) mem[0][p].hi=0,mem[0][p].mark=lomark;  
  else mem[0][p].mark=lomark;
}

@ Next, a recursive subroutine to print out all paths that remain.

@<Sub...@>=
void printpaths(int p) {
  register int q,s=stackptr;
  if (p<=1) {
    if (serial) {
      printf(" and more...\n");
      exit(0);
    }@+else {
      serial=1;
      for (q=0;q<stackptr;q++) printf(" %d",stack[q]);
      printf(" (length %d)\n",stackptr);
    }
    return;
  }
  q=mem[0][p].hi;
  if (q) {
    stack[stackptr++]=mem[0][p].v;
    printpaths(q);
  }
  q=mem[0][p].lo;
  if (q) {
    printpaths(q);
  }
  stackptr=s;
}

@ @<Do our thing@>=
mem[0][1].mark=1, mem[0][1].v=varsize;
prep(root[0]);
printpaths(root[0]);
@z
