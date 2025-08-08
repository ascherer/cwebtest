@x
With change files I can adapt it to other uses.
@y
Here I compute the total number of solutions.
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
double count[memsize];
@z
@x
@ @<Sub...@>=

@ @<Do our thing@>=
@y
@ First, a recursive subroutine.

@<Sub...@>=
void countsols(int p) {
  register int k,q;
  double c=0,cc;
  q=mem[0][p].lo;
  if (q) {
    if (mem[0][q].mark==0) countsols(q);
    for (cc=count[q],k=mem[0][p].v+1;k<mem[0][q].v;k++)
      if (present[k]) cc+=cc;
    c+=cc;
  }
  q=mem[0][p].hi;
  if (q) {
    if (mem[0][q].mark==0) countsols(q);
    for (cc=count[q],k=mem[0][p].v+1;k<mem[0][q].v;k++)
      if (present[k]) cc+=cc;
    c+=cc;
  }
  mem[0][p].mark=1;
  count[p]=c;
}

@ @<Do our thing@>=
count[1]=1, mem[0][1].mark=1, mem[0][1].v=varsize;
countsols(root[0]);
if ((count[root[0]]+1.0e0)-count[root[0]] == 1.0e0)
  printf("%.0f solutions.\n",count[root[0]]);
else
  printf("Solution count exceeds precision limit - DO NOT TRUST!\n"),
  printf("Corrupted count %.15g\n",count[root[0]]),
  exit(-1);
@z
