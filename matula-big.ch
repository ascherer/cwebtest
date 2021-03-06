@x
The trees are given on the command line, each as a string of
``parent pointers'';
this string has one character per node. The first character is always
`\..', standing for the (nonexistent) parent of the root; the next character is
always `\.0', standing for the parent of node~1; and the $(k+1)$st character
stands for the parent of node~$k$, which can be any number less than~$k$.
Numbers larger than~\.9 are encoded by lowercase letters;
numbers larger than~\.z (which represents 35) are encoded by uppercase letters.
Numbers larger than~\.Z (which represents 61) are presently
disallowed; but some day I'll make another version of this program, with
different conventions for the input.

The root of $S$ is assumed to have degree~1. Thus it is actually both a
root and a leaf, and the string for~$S$ will have only one occurrence of~\.0.

For example, here are some trees used in an early test:
$$\eqalign{S&=\.{.0111444759a488cfch};\cr
T&=\.{.011345676965cc5ffh5cklfn55qjstuuwxxwwuCCuFCpppqrtGOHJRLMNO};\cr}$$
can you find $S$ within $T$?
@y
The trees are given in two text files, using the ``rectree format''
defined below. I decided to use this somewhat strange format because
I wanted to test this program on large random trees. The calculations
needed to compute random $n$-node trees involve very large numbers
($\Omega(2.9^n)$), so I wanted to do the calculations with Mathematica.
The program \.{randomfreetree.m} can be used to generate random trees in the
the required format.

For example, here are the rectree specifications of
some trees that I used in early tests:
$$\vtop{\halign{\tt#\cr
{\char`\%} example pattern tree S\cr
T19\_0.\cr
T19\_0=+1T18\_1.\cr
T18\_1=+2T1\_2+1T15\_4.\cr
T15\_4=+1T4\_5+1T4\_9+1T5\_13+1T1\_18.\cr
T5\_13=+2T2\_14.\cr
T4\_9=+1T3\_10.\cr
T3\_10=+2T1\_11.\cr
T4\_5=+1T3\_6.\cr
T3\_6=+1T2\_7.\cr
}}\qquad\vtop{\halign{\tt#\cr
{\char`\%} example target tree T\cr
T59\_0.\cr
T59\_0=+2T1\_1+1T56\_3.\cr
T56\_3=+1T55\_4.\cr
T55\_4=+1T54\_5.\cr
T54\_5=+1T6\_6+1T6\_12+1T6\_18+1T23\_24+1T6\_47+1T6\_53.\cr
T6\_53=+1T3\_54+1T2\_57.\cr
T3\_54=+1T2\_55.\cr
T6\_47=+2T1\_48+1T3\_50.\cr
T3\_50=+1T2\_51.\cr
T23\_24=+1T22\_25.\cr
T22\_25=+1T21\_26.\cr
T21\_26=+1T18\_27+1T2\_45.\cr
T18\_27=+1T1\_28+1T6\_29+1T5\_35+1T5\_40.\cr
T5\_40=+1T4\_41.\cr
T4\_41=+1T3\_42.\cr
T3\_42=+2T1\_43.\cr
T5\_35=+2T1\_36+1T2\_38.\cr
T6\_29=+1T3\_30+2T1\_33.\cr
T3\_30=+2T1\_31.\cr
T6\_18=+1T1\_19+2T2\_20.\cr
T6\_12=+2T1\_13+1T3\_15.\cr
T3\_15=+1T2\_16.\cr
T6\_6=+2T2\_7+1T1\_11.\cr
}}$$
(These were produced by hand, {\it not\/} by \.{randomfreetree.m}.)
Can you find $S$ within $T$? (The node numbers illustrated below are
a permutation of the node numbers in these rectree specifications.)
@z
@x
@d maxn 62 /* could be greatly increased if I had another input convention */
@y
@d maxn 2000
@z
@x
  fprintf(stderr,"Usage: %s S_parents T_parents\n",
@y
  fprintf(stderr,"Usage: %s S.rectree T.rectree\n",
@z
@x
@*Data structures for the trees.
@y
@*Recursive tree format.
The definition of a free tree in rectree format has many redundancy checks
to keep you honest; so it is probably best to let a computer prepare it.

The opening lines of that file may contain optional comments, indicated
by a `\.{\char`\%}' sign at the very left. Then comes the ``main line,''
which defines the overall context. The main line can have one of three
forms:
\smallskip
\item{a)} `\.{T$n$\_0.}' This means an $n$-node tree, beginning with node~0.

\item{b)} `\.{T$m$\_0,T$m$\_$m$.}' This means a $2m$-node tree, formed from
the $m$-node trees \.{T$m$\_0} and \.{T$m$\_$m$}, with their nodes
made adjacent.

\item{c)} `\.{2T$m$\_0.}' This means a $2m$-node tree, formed from
two identical copies of the $m$-node tree \.{T$m$\_0}, with their nodes
made adjacent.

\smallskip\noindent
Options (b) and (c) are created by the Mathematica program
\.{randomfreetree.m} when it's creating a free tree with two centroids. (But
the trees in rectree format are allowed to have their centroids anywhere.)

All lines after the main line contain definitions of the subtrees of size
3~or more, {\it always proceeding in strictly last-in-first-out order.}
Subtree names have the form \.{T$k$\_$o$}, where $k$ is the number of nodes
and $o$ is the offset (the number of the root node). Each definition of a
$k$-node subtree appears on a separate line, beginning with the subtree
name immediately followed by `\.=', and a sum of terms that collectively
define subtrees totalling $k-1$ nodes (and followed by `\..').
Each subtree name in these terms is preceded by an integer~$j$, meaning
that $j$ copies are to appear. The offset after a term `\.{$j$T$k$\_$o$}'
should therefore by $o+jk$.

@*Data structures for the trees.
@z
@x
node snode[maxn]; /* the $m$ nodes of $S$ */
node tnode[maxn]; /* the $n$ nodes of $T$ */
@y
node snode[maxn+1]; /* the $m$ nodes of $S$, and one more */
node tnode[maxn+1]; /* the $n$ nodes of $T$, and one more */
@z
@x
@ @<Input the tree $S$@>=
if (o,argv[1][0]!='.') {
  fprintf(stderr,"The root of S should have `.' as its parent!\n");
  exit(-10);
}
for (m=1;o,argv[1][m];m++) {
  if (m==maxn) {
    fprintf(stderr,"Sorry, S must have at most %d nodes!\n",
                 maxn);
    exit(-11);
  }
  p=decode(argv[1][m]);
  if (p<0) {
    fprintf(stderr,"Illegal character `%c' in S!\n",
                        argv[1][m]);
    exit(-12);
  }
  if (p>=m) {
    fprintf(stderr,"The parent of %c must be less than %c!\n",
                            encode(m),encode(m));
    exit(-13);
  }
  if (p==0 && m>1) {
    fprintf(stderr,"The root of S must have only one child!\n");
    exit(-13);
  }
  oo,q=snode[p].child,snode[p].child=m; /* |m| becomes the first child */
  o,snode[m].sib=q;
}

@ @<Input the tree $T$@>=
if (o,argv[2][0]!='.') {
  fprintf(stderr,"The root of T should have `.' as its parent!\n");
  exit(-20);
}
for (n=1;o,argv[2][n];n++) {
  if (n==maxn) {
    fprintf(stderr,"Sorry, T must have at most %d nodes!\n",
                 maxn);
    exit(-21);
  }
  p=decode(argv[2][n]);
  if (p<0) {
    fprintf(stderr,"Illegal character `%c' in T!\n",
                        argv[2][n]);
    exit(-22);
  }
  if (p>=n) {
    fprintf(stderr,"The parent of %c must be less than %c!\n",
                            encode(n),encode(n));
    exit(-23);
  }
  oo,q=tnode[p].child,tnode[p].child=n; /* |n| becomes the first child */
  o,tnode[n].sib=q;
}
@<Allocate the arcs@>;
fprintf(stderr,
  "OK, I've got %d nodes for S and %d nodes for T, max degree %d.\n",
                   m,n,maxdeg);
@y
@ Here's a subroutine that reads a rectree file and puts the associated tree
into the |tnode| array.

@d bufsize 1<<12

@<Sub...@>=
FILE *infile;
char buf[bufsize];
int read_rectree(char*filename) {
  register i,j,k,p,q,r,s,typ,stack,n,size,off,rightoff,rep;
  mems+=suboverhead;
  infile=fopen(filename,"r");
  if (!infile) {
    fprintf(stderr,"I can't open `%s' for reading!\n",
                     filename);
    exit(-99);
  }
  while (1) {
    if (!fgets(buf,bufsize,infile)) {
      fprintf(stderr,"Rectree file `%s' ended before the main line\n",
            filename);
      exit(-98);
    }
    if (o,buf[0]!=@t)@>'%') break;
  }
  @<Process the main line@>;
  while (stack>=0) @<Process the top subtree definition on the stack@>;
  @<Bring on the clones@>;
  @<Adjust the tree if bicentroidal@>;
  return o,tnode[0].deg;
}

@ While the tree is being installed, we use the |child| field to link items
in the stack of nodes to be finished. We also use the |deg| field to
record the subtree size, and the |arc| field to record the number of clones
that should be made.

@<Process the main line@>=
if (buf[0]=='2') o,typ=tnode[0].arc=2,p=1;@+else o,typ=p=tnode[0].arc=0;
@<Scan a subtree name@>;
if (off) {
  fprintf(stderr,"The main subtree must start at 0!\n...%s",
               buf+q);
  exit(-104);
}
oo,n=tnode[0].deg=size,tnode[0].child=-1,tnode[0].sib=0,stack=0;
if (typ==2) n+=n;
else if (o,buf[p]==',') {
  typ=1,p++;
  @<Scan a subtree name@>;
  if (off!=n) {
    fprintf(stderr,"The second main subtree should start at %d!\n...%s",
                             off,buf+q);
    exit(-105);
  }
  o,tnode[0].sib=n,stack=n,n+=size;
}
if (n>maxn) {
  fprintf(stderr,"Tree too big, because maxn=%d!\n...%s",
                 maxn,buf+q);
  exit(-102);
}
for (k=1;k<=n;k++) oo,tnode[k].child=tnode[k].sib=tnode[k].deg=tnode[k].arc=0;
if (typ==1) o,tnode[stack].deg=n-stack; /* |tnode[stack].child=0| */
if (o,buf[p]!='.') {
  fprintf(stderr,"The main line didn't end with `.'!\n%s",
             buf);
  exit(-106);
}

@ @<Scan a subtree name@>=
if (o,buf[p]!='T') {
  fprintf(stderr,"Subtree name doesn't start with T!\n...%s",
                buf+p);
  exit(-100);
}
for (q=p++,size=0;o,(buf[p]>='0' && buf[p]<='9');p++) size=10*size+buf[p]-'0';
if (size==0) {
  fprintf(stderr,"Subtree size is missing or zero!\n...%s",
                 buf+q);
  exit(-101);
}
if (buf[p++]!='_') {
  fprintf(stderr,"Subtree name missing `_'!\n...%s",
                    buf+q);
  exit(-103);
}
for (off=0;o,(buf[p]>='0' && buf[p]<='9');p++) off=10*off+buf[p]-'0';

@ @<Process the top subtree definition on the stack@>=
{
  oo,k=stack,stack=tnode[k].child,s=tnode[k].deg;
  o,tnode[k].child=(s>=2? k+1: 0);
  if (s>2) {
    if (!fgets(buf,bufsize,infile)) {
      fprintf(stderr,"Rectree file `%s' ended before defining T%d_%d!\n",
                          filename,s,k);
      exit(-107);
    }
    p=0;@+@<Scan a subtree name@>;
    if (size!=s || off!=k) {
      fprintf(stderr,"Rectree file `%s' doesn't define T%d_%d!\n %s",
                          filename,s,k,buf);
      exit(-108);
    }
    if (o,buf[p++]!='=') {
      fprintf(stderr,"Missing `=' in definition of T%d_%d!\n %s",
                         s,k,buf);
      exit(-109);
    }
    rightoff=k+1;
    @<Define the subtrees of node |k|@>;
    if (buf[p]!='.') {
      fprintf(stderr,"Missing `.' after definition of T%d_%d!\n %s",
                         s,k,buf);
      exit(-112);
    }
    if (rightoff!=k+s) {
      fprintf(stderr,"The definition of T%d_%d has %d nodes!\n %s",
                        s,k,rightoff-k,buf);
      exit(-113);
    }
  }
}

@ @<Define the subtrees of node |k|@>=
while (o,buf[p]=='+') {
  for (q=p++,rep=0;o,(buf[p]>='0' && buf[p]<='9');p++) rep=10*rep+buf[p]-'0';
  if (rep==0) {
    fprintf(stderr,"Replication count missing or zero!\n...%s",
                     buf+q);
    exit(-110);
  }
  @<Scan a subtree name@>;
  if (off!=rightoff) {
    fprintf(stderr,"That subtree should start at %d!\n...%s",
                         rightoff,buf+q);
    exit(-111);
  }
  oo,j=rightoff,tnode[j].deg=size,tnode[j].child=stack;
  if (rep>1) tnode[j].arc=rep; /* that mem was already charged */
  stack=j;
  rightoff+=rep*size;
  if (buf[p]=='+') tnode[j].sib=rightoff;
}

@ At this point the entire tree is in place, except that no cloning
has yet been done to copy the subtrees that are supposed to be repeated.

Clones can appear inside of clones. But everything is easily
patched up, if we look at the tree from bottom up when finding
work to do, then clone from top down. (Kind of cute.) And we know
that the total work will take linear time, because nothing is done twice.

@<Bring on the clones@>=
for (p=n-1;p>=0;p--) if (o,tnode[p].arc) {
  s=tnode[p].deg, j=s*tnode[p].arc;
  o,tnode[p].arc=0; /* erase tracks */
  oo,i=tnode[p].sib,tnode[p].sib=p+s; /* the clone will be a sibling */
  for (k=p+s;k<p+j;k++) {
    o,q=tnode[k-s].child,r=tnode[k-s].sib;
    if (q) o,tnode[k].child=q+s;
    if (r) o,tnode[k].sib=r+s;
  }
  o,tnode[k-s].sib=i; /* the rightmost sibling is original sibling of |p| */
}

@ @<Adjust the tree if bicentroidal@>=
if (typ) {
  o,p=tnode[0].sib; /* sibling of the root will become its child */
  oo,tnode[p].sib=tnode[0].child,tnode[0].child=p,tnode[0].sib=0;
  o,tnode[0].deg=n;
}

@ This program wants the root of $S$ to be a leaf. That usually means that
all the nodes must be renumbered---at least internally---although the
results should be reported to the user as if no renumbering had occurred.

So we first input tree $S$ into the |tnode| array. Then we transform
the |tnode| array so that the root is a leaf. Then we copy |tnode| to |snode|,
remapping all node numbers as we go. The new number for node~|k| is
kept in |snode[k].arc|, which is otherwise unused.

@<Input the tree $S$@>=
m=read_rectree(argv[1]);
@<Make the root of |tnode| into a leaf@>;
@<Copy and remap |tnode| into |snode|@>;

@ I thought this would be easier than it has turned out to be.
Did I miss something? It's a nice little exercise in datastructurology.

Node 0 moves to node |m|, so that it can become a child or a sibling.

@<Make the root of |tnode| into a leaf@>=
oo,r=m,p=tnode[0].child,tnode[r].child=p;
while (o,q=tnode[p].child) { /* make |p| the root, retaining its child |q| */
  o,k=tnode[p].sib,s=tnode[q].sib;
  o,tnode[p].sib=0;
  o,tnode[q].sib=r;
  o,tnode[r].child=k,tnode[r].sib=s;
  r=p,p=q;
}
ooo,s=tnode[p].sib,tnode[p].sib=0,tnode[p].child=r,tnode[r].child=s;
/* now |p| is the root */

@ @<Copy and remap |tnode| into |snode|@>=
copyremap(p);
if (gg!=m) {
  fprintf(stderr,"I'm confused!\n");
  exit(-666);
}
oo,snode[0].arc=snode[m].arc;

@ This recursion is a bit tricky, and I wonder what's the best way to explain it.
(An exercise for the reader.) 

@<Sub...@>=
int gg; /* global counter for remapping */
void copyremap(int r) {
  register int p,q;
  mems+=suboverhead;
  o,snode[gg].deg=r; /* |deg| is the inverse map to |arc|, which is */
  o,snode[r].arc=gg++; /* the interior ($S$) name of the user's node |r| in $T$ */
  o,p=tnode[r].child;
  if (!p) return;
  o,snode[gg-1].child=gg; /* copy a (remapped) child pointer */
  while (1) {
    q=gg; /* the future interior name of |p| */
    copyremap(p);
    o,p=tnode[p].sib;
    if (!p) return;
    o,snode[q].sib=gg; /* copy a (remapped) sibling pointer */
  }
}

@ @<Input the tree $T$@>=
n=read_rectree(argv[2]);
@<Allocate the arcs@>;
fprintf(stderr,
  "OK, I've got %d nodes for S and %d nodes for T, max degree %d.\n",
                   m,n,maxdeg);
@z
@x
if (m==0) goto yes_sol; /* every boy matches every girl */
@y
if (m==0) goto yes_sol; /* every boy matches every girl */
if (m*n>record) {
  record=m*n;
  fprintf(stderr," ...matching %d boys to %d girls\n",
                               m,n);
}
@z
@x
if (z<0) fprintf(stderr,
         "Failure; We can't even embed node %d and its parent.\n",
                                    encode(-z));
else {
  fprintf(stderr,"There %s %d place%s to anchor an embedding of node 1.\n",
                            z==1?"is":"are", z, z==1?"":"s");
@y
if (z<0) {
  k=-z;
  for (p=k-1;;p--) {
    if (snode[p].child==k) break;
    else if (snode[p].sib==k) k=p;
  }
  fprintf(stderr,
         "Failure; We can't even embed node %d and its parent %d.\n",
                                    snode[-z].deg,snode[p].deg);
}@;
else {
  fprintf(stderr,"There %s %d place%s to anchor an embedding of node %d.\n",
                            z==1?"is":"are", z, z==1?"":"s",snode[1].deg);
@z
@x
  oo,printf("%c",
           encode(uert[solarc[1]]));
  for (p=1;p<m;p++) oo,printf(" %c",
               encode(vert[solarc[p]]));
@y
  for (p=0;p<m;p++) {
    o,q=snode[p].arc; /* the original node named |p| before remapping */
    if (q) oo,printf(" %d",
                   vert[solarc[q]]);
    else oo,printf(" %d",
                   uert[solarc[1]]);
  }
@z
@x
@ Did you solve the puzzle?
@y
@ @<Glob...@>=
int record; /* the largest bipartite matching problem encountered so far */

@ Did you solve the puzzle?
@z
