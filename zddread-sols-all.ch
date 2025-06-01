@x THIS IS A CHANGE FILE FOR "bddread.w"
With change files I can adapt it to other uses.
@y
Here I compute all the solutions, using ZDD conventions.
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
@ @<Sub...@>=

@ @<Do our thing@>=
@y
@ First, a recursive subroutine.

@d var(p) mem[0][p].v

@<Sub...@>=
void printpaths(int p) {
  register int q;
  if (p<=1) {
    printf("%d:",serial);
    for (q=0;q<stackptr;q++) printf(" %d",stack[q]);
    printf("\n");
    serial++;
    return;
  }
  q=mem[0][p].lo;
  if (q) printpaths(q);
  q=mem[0][p].hi;
  if (q) {
    stack[stackptr++]=mem[0][p].v;
    printpaths(q);
    stackptr--;
  }
}

@ @<Do our thing@>=
mem[0][0].v=mem[0][1].v=varsize;
if (root[0]) printpaths(root[0]);
@z
