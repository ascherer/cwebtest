\datethis
\def\adj{\mathrel{\!\mathrel-\mkern-8mu\mathrel-\mkern-8mu\mathrel-\!}}
@i gb_types.w

@*Intro. Given a graph $G$ on vertices $\{1,2,\ldots,n\}$,
this program uses a variant of dynamic programming
to enumerate all of the Hamiltonian paths of the induced subgraphs
$G\mid\{1,2,\ldots,m\}$, for $m=1$, 2, \dots,~$n$.
In particular, when $m=n$, it counts {\it all\/} of the Hamiltonian paths.
And if $G$ is, say, the $8\times20$ knight graph, with vertices numbered
column by column, it will count all
open knight's tours of $8\times3$, $8\times4$, \dots, $8\times20$ boards,
when $m=24$, 32, \dots,~160.

I've hastily hacked this program from {\mc DYNAHAM}, which solves the
analogous problem for Hamiltonian {\it cycles}, not paths. It's best for
you to read and understand that program before you delve into this one. The
main difference is that the present program works with the augmented
graph $G^+=G\adj K_1$, which adds a new vertex `$n+1$' to~$G$.
This new vertex is adjacent to every old vertex; and the Hamiltonian
paths of~$G$ are in obvious one-to-one correspondence with the
Hamiltonian cycles of~$G^+$. Furthermore the Hamiltonian paths
of $G\mid\{1,2,\ldots,m\}$ are in obvious one-to-one correspondence with
the Hamiltonian cycles of~$G^+\mid\{1,2,\ldots,m,n+1\}$.

The idea is kind of simple, once you understand it; but it's not quite
easy to explain. Let's say that an ``$m$-config'' is a subset of the
edges of~$G^+$ such that (i)~every vertex $\le m$ appears in exactly two edges;
(ii)~no edge has both endpoints $>m$;
(iii)~there is no cycle of edges.
As a consequence, the edges of an $m$-config will form disjoint subpaths
of the graph.

The ``$m$-frontier'' $F_m$ of $G$ is the set of vertices $>m$ that are
reachable from $\{1,\ldots,m\}$. Each vertex of~$F_m$ in an $m$-config can
be classified as ``outer'' (an endpoint of a subpath), or ``inner''
(an intermediate vertex of a subpath), or ``bare'' (not in any subpath),
according as its degree in the $m$-config is 1, 2, or~0. (There will be
exactly $2t$ outer cells when there are $t$ subpaths.) Two $m$-configs
are {\it equivalent\/} if they have the same outer, inner, and bare cells,
and if the outer cells are paired~up in the same way.

This program counts the number of $m$-configs in every possible equivalence
class, for $m=1$, 2, \dots, until either $m=n$, or the number of classes
gets too big, or the weight of a class gets too big, or the size of a frontier
gets too big. These subsidiary counts allow it to fulfill its main mission,
which is to count the number of $(m+1)$-cycles on $\{1,\ldots,m,n+1\}$.

To represent an equivalence class in a canonical way, we use a sequence of
integer codes to characterize the elements of the frontier.
A bare cell has code~0;
an outer cell of the $j$th subpath has code~$j$, for $1\le j\le t$;
and an inner cell has code~$-1$. The subpaths
are numbered according to when the endpoints first occur, in
a particular ordering of the frontier vertices.

For example, suppose $G$ is the $4\times3$ knight graph.
Instead of naming its vertices 1 thru~13, let's use the cell names
00, 10, 20, 30, 01, 02, \dots, 22, 32, followed by `!'\ for the new vertex.
There are eighteen 2-configs, obtained from the
edges of $G^+$ by first omitting all edges that don't involve the first
two vertices $\{00,10\}$; that leaves just seven edges, namely
$\{00\adj12,\ 00\adj21,\ 00\adj{!},\ 
10\adj02,\ 10\adj22,\ 10\adj31,\ 10\adj{!}\}$.
We get six 2-configs with $00\adj!\adj10$;
six with ${!}\adj00$, 00 paired with 12 or 21,
and 10 paired with two of its three neighbors;
three with ${!}\adj10$, $12\adj00\adj21$, and
10 paired with a neighbor;
three with $12\adj00\adj21$ and 10 paired with two neighbors.
If, say, we choose the 2-config with $12\adj00\adj{!}\adj10\adj02$,
the codes for vertices
({!},\ 21, 31, 02, 12, 22) of the frontier are
respectively ($-1$, 0, 0, 1, 1, 0).
And the codes for the 2-config with $12\adj00\adj{!}$ and $02\adj10\31$ are
respectively (1, 0, 2, 2, 1, 0).

It isn't difficult to find and count the equivalence classes for all
$m$-configs, if we already know all of the counts for $(m-1)$-configs,
because the ``successors'' of an $(m-1)$-config simply add $(1,0,2)$
new edges when vertex $m$ is (outer, inner, bare).

@ Here's an outline of the program as a whole:

@d precision 100 /* this many decimal digits are supported */
@d progress_mask 0x1fffff /* show progress every $2^{21}$ steps */
@d maxn 300 /* the input graph must have fewer than |maxn| vertices */

@c
#include <stdio.h>
#include <stdlib.h>
#include "gb_graph.h"
#include "gb_save.h"
int m; /* the current flavor of configs */
int n; /* the number of vertices in |g| */
@<Type definitions@>;
@<Global variables@>;
@<Subroutines@>;
@<Recursive subroutines@>;
int main(int argc,char*argv[]) {
  register int c,d,i,j,k,l,p,t,x,ii,ik,iv;
  register Graph *g;
  register Arc *a;
  @<Process the command line@>;
  @<Initialize@>;
  for (m=1;;m++) {
    @<Print the previous frontier@>;
    if (wtptr==0) @<Give a final report and exit@>;
    @<Save the current trie of $m$-classes as the old trie of $(m-1)$-classes@>;
    @<Start up a new trie, which will receive the $m$-classes@>;
    @<Contribute the successors of each old class to the new trie@>;
    report_cycles(m);
  }
}

@ @<Process the command line@>=
if (argc!=2) {
  fprintf(stderr,"Usage: %s foo.gb\n",argv[0]);
  exit(-1);
}
g=restore_graph(argv[1]);
if (!g) {
  fprintf(stderr,"I couldn't reconstruct graph %s!\n",argv[1]);
  exit(-2);
}
n=g->n;
if (n>=maxn) {
  fprintf(stderr,"Recompile me: I allow at most %d vertices!\n",
                              maxn-1);
  exit(-3);
}
(g->vertices+n)->name=gb_save_string("!"); /* name the new vertex */
printf("Dynamic Hamiltonian paths of the graph %s",
                        g->id);
printf(" (%d vertices, %ld edges):\n",
                        n,g->m/2);
fflush(stdout);

@*Low-level arithmetic.
I'll build all the basic tools from scratch, because they're easy
and I don't need to optimize.

First I need routines to add and print high-precision numbers.
I work with radix $10^{18}$, noting that $2^{63}$ exceeds $9\cdot10^{18}$.

@d radix 1000000000000000000LL
@d maxprec ((precision+17)/18) /* this many octabytes for each bignum */

@<Type def...@>=
typedef struct bignum_struct {
  long long val[maxprec];
} bignum;

@ @<Glob...@>=
bignum zero; /* the constant 0 */
bignum one; /* the constant 1 */
bignum infty; /* the largest bignum */
int prec; /* the largest precision needed so far */

@ @<Init...@>=
one.val[0]=1;
for (k=0;k<maxprec;k++) infty.val[k]=radix-1;
prec=1;

@ @<Sub...@>=
void add_to_bignum(bignum *x,bignum delta) {
  register int c,k;
  register long long s;
  for (c=k=0;k<prec;k++) {
    s=x->val[k]+delta.val[k]+c;
    if (s>=radix) c=1,x->val[k]=s-radix;
    else c=0,x->val[k]=s;
  }
  if (c) {
    if (prec==maxprec) @<Abort, because bignum overflow has occurred@>;
    prec++;
    x->val[k]=c;
  }
}

@ @<Sub...@>=
int bignum_comp(bignum x, bignum y) {
  register int k,r=0;
  for (k=0;k<prec;k++) if (x.val[k]!=y.val[k])
    r=(x.val[k]<y.val[k]? -1: +1);
  return r;
}

@ @<Sub...@>=
void print_bignum(FILE *stream,bignum x) {
  register int k;
  for (k=prec-1;k>=0 && x.val[k]==0;k--) ;
  if (k<0) fprintf(stream,"0");
  else {
    fprintf(stream,"%lld",
              x.val[k]);
    while (k>0) {
      k--;
      fprintf(stream,"%018lld",
              x.val[k]);
    }
  }
}

@ @<Abort, because bignum overflow has occurred@>=
{
  fprintf(stderr,"\nSorry, I can't handle numbers bigger than 10^%d!\n",
                       18*maxprec);
  fprintf(stderr,"I had found %lld classes of %d-configs so far.\n",
                         wtptr,m);
  exit(-999);
}

@*Trie structure.
Our main data structure is a big dictionary, with one entry for every
equivalence class. We implement it by building a simple ``trie'' structure.

The trie grows dynamically, using intervals of a giant
array of pointers called |mem| to store the nodes, and using elements
of a giant array of \&{bignum}s called |weight| to store the leaves.

Each leaf (or ``lief'') is identified by a key
$a_0a_1\ldots a_{q-1}$, where we have
$-1\le a_l<|deg|$ for $0\le l<q$. There is a node in the trie
for each prefix $a_0\ldots a_{l-1}$ of the key, for $0\le l<q$.
The root node, at level $l=0$, represents the empty prefix.

Earlier versions of this program simply represented each node as a
an array of approximately ten pointers, each of which was either zero or the
index of the child node or leaf. That idea worked, and many experiments
were successully completed. But in retrospect we can now see that most
of those pointers were actually zero. For example, it turns out that
when all but the last code of the key are known, there are at most
two choices for the final code $a_{q-1}$; consequently at most two of the
pointers in the nodes at level~$q-1$ were nonzero!
Memory is a scarce resource for this program. Hence
we're now adopting a more complex scheme, which is considerably tighter.

The keys in our tries have special properties
that significantly limit the possible values of $a_l$ when
the previous codes $a_0\ldots a_{l-1}$ are known.
For instance, $a_0$ itself can only be $-1$, 0, or~1.
And the key prefix 1021, say, can only be followed by $-1$, 0, 2, or~3.

We can govern the set of possibilities for code $a_l$ by maintaining
a sparse-set structure: The positive codes that have already appeared
once but not twice are listed in the first |tms| elements of the array
|tmap|; and |tmx| is the smallest positive code that has not yet appeared.
Furthermore there's a partial inverse map: If $c$ has occurred once
but not twice, we have |c=tmap[itmap[c]]|.

The node for prefix $a_0\ldots a_{l-1}$ at level~$l$ is identified by
a pointer~$p=p(a_0\ldots a_{l-1})$, which is an index into |mem|. If the possible
values of $a_l$ are respectively $-1$, 0, |tmap[0]|, \dots, |tmap[tms-1]|,
and |tmx|, the child node $p'=p(a_0\ldots p_l)$ will be respectively
|mem[p-1]|, |mem[p]|, |mem[p+1]|, \dots, |mem[p+tms]|, |mem[p+tms+1]|.
Those |mem| locations will be zero if that particular code~$a_l$ hasn't
yet appeared with the given prefix.
(Note that |tms| might be~0. Furthermore, there can be cases where
$-1$ and~0 cannot occur, as well as cases where |tmx| cannot occur.)

(Once you understand this scheme, and see how much memory it saves,
you might come to think that it's rather beautiful despite its complexity ---
especially because it is unlikely to be discovered by AI at any time
in the near future!)

The trie leaf that corresponds to the full key $a_0\ldots a_{q-1}$
is found in $|weight|[p(a_0\ldots a_{q-1})-1]$, using the same
definition of~$p$. It's a bignum, which represents the current weight of class
$a_0\ldots a_{q-1}$.

@ For example, suppose $q=4$ and assume
that the trie contains only one leaf, whose key is $\bar1101$,
where $\bar1$ stands for~$-1$. The root node is always~1, and its pointers
are |mem[0]|, |mem[1]|, and |mem[2]|. In this case, |mem[0]=4|
points to the node for prefix~$\bar1$, whose pointers are
|mem[3]|, |mem[4]|, and |mem[5]|. So |mem[5]=7| points to the
node for prefix~$\bar11$, whose pointers are |mem[6]|, |mem[7]|,~|mem[8]|.
(The prefix $\bar112$ is impossible when $q=4$, so we don't allocate
|mem[9]| at this time.)
And |mem[7]| points to the node for prefix $\bar110$.
This prefix can only be followed by `1', again since $q=4$. Hence
|mem[7]=8|. (Think about it.) And |mem[9]=1| points to the leaf
represented in |weight[0]|.

Adding another leaf $\bar1110$ would cause |mem[8]=11| to point
to a new node for prefix $\bar111$, whose pointers are |mem[10]| and
|mem[11]|. So |mem[11]=2| tells us that the new leaf is |weight[1]|.

And if we add a third leaf $1212$, we'd set |mem[2]=13|, a new
node for prefix 1, with pointers |mem[12]| thru |mem[15]|. Also |mem[15]=15|
will be a new node for prefix 12, with pointers |mem[16]| and |mem[17]|,
because the only valid keys with prefix 12 are 1212 and 1221.
Finally |mem[16]=17| is the node for prefix 121; and |mem[18]=3|
points to the third leaf, |weight[2]|.

Those three leaves might have entered the trie in the opposite order.
In that case we'd still have nodes for the same prefixes, but the memory
allocation would be quite different: The nonzero |mem| values would be
|mem[0]=11|,
|mem[2]=4|,
|mem[6]=6|,
|mem[7]=8|,
|mem[9]=1|,
|mem[12]=14|,
|mem[14]=17|,
|mem[15]=17|,
|mem[17]=2|,
|mem[18]=3|.

@ That same trie can also be represented in a highly compressed form,
once it has been fully built:
The nonleaf nodes with $d$ pointers are compressed into just $d$ bits,
which are
concatenated in preorder. A 1-bit in position |j| of
the bitstring for |mem[k]| means that the |j|th pointer of node~$p$ is nonzero.

In our example, regardless of the order in which the leaves were
introduced, 
the bitstrings for prefixes $\epsilon$, $\bar1$, 1, $\bar11$, 12,
$\bar110$, $\bar111$, and 121 are respectively
101, 001, 0001, 011, 10, 1, 01, and 1. When we concatenate them in
preorder we get the compressed representation 1010010111010001101.
The leaf weights in the compressed representation also appear in preorder,
which is precisely defined and somewhat similar to lexicographic
order, but not always the same; in this example it does happen
to be lexicographic.

The long trie format is used in the program below to build the
trie of $m$-classes from the trie of $(m-1)$-classes. The latter trie,
by contrast, appears in the compressed form.

The |oldmemsize| should be a little bit larger than |memsize/8|. Why? Well,
the reason is a bit tricky: |memsize/8| bytes give approximately |memsize| bits,
which is what we need. Yet some bits are wasted, because |oldmem|
is composed of 64-bit words, and we typically fill only about 60 of those
bits (in order to avoid splitting a small bitstring between words).
In any case, |oldmem| is small potatoes, since |mem| itself usually
occupies |8*memsize| or |16*memsize| bytes.

@d memsize 3000000000LL /* max mem size; must be sufficiently large */
@d wtsize  1000000000LL /* ditto for the max number of classes */
@d oldmemsize 450000000 /* see above; this is generous */
@d deg 10 /* the codes $a_l$ in keys must be less than this */

@<Initialize@>=
mem=(unsigned long long*)malloc(memsize*sizeof(unsigned long long)); /* nonleaf pointers */
oldmem=(unsigned long long*)malloc(oldmemsize); /* compressed nonleaf bits */
weight=(bignum*)malloc(wtsize*sizeof(bignum));
oldweight=(bignum*)malloc(wtsize*sizeof(bignum));
if (!mem || !oldmem || !weight || !oldweight) {
  fprintf(stderr,"I can't allocate the big tables!\n");
  exit(-6);
}

@ @<Glob...@>=
long long oldp; /* which old weight are we working on? */
long long contribs; /* how many contributions were made by the old classes? */
unsigned long long *mem; /* the array where the big trie lives */
unsigned long long *oldmem; /* the array where compressed bitstrings live */
long long memptr,maxmemptr; /* the first unused slot in |mem| */
long long wtptr,maxwtptr; /* the first unused slot in |weight| */
int q,oldq; /* the length of trie keys */
int code[maxn]; /* the key being looked up in the new trie */
int oldcode[maxn]; /* the current key in the old trie */
bignum *weight,*oldweight; /* the big arrays where their leaves live */
bignum minweight,maxweight; /* extreme weights */
bignum count[maxn+1]; /* how many Hamiltonian $m$-paths have we counted */
int maxdeg; /* largest observed branching factor so far */
unsigned long long pack; /* bits being assembled to append to |oldmem| */
int spack; /* the number of unsaved bits currently in |pack| */
long long owp,omp; /* weight and memory pointers when compressing/uncompressing */
int maxomp; /* the largest |omp| seen so far */
int tmap[deg],itmap[deg],omap[deg],iomap[deg]; /* sparse-sets for tries */
int tms,tmx,oms,omx; /* and their pointers */

@ Here's the subroutine that traverses a trie to find the address of
the leaf that corresponds to a given key, which is a sequence of |code|
values, inserting that key if necessary.

At level |l|, when we branch on $a_l=|code[l]|$, the legal positive
values of $a_l$ are usually in the first |tms| entries of |tmap| together
with |tmx|, as mentioned above.
The yet-unused value |tmx| is sometimes impossible.

@<Sub...@>=
long long trielookup(void) {
  register int j,l,k,kk;
  register unsigned long long p,pp;
  tms=0,tmx=p=1;
  for (l=0;l<q;l++,p=mem[pp]) {
    j=code[l];
    if (j<=0) pp=p+j;
    else if (j==tmx) { /* a higher code than seen before */
      @<Swap |tmx| into the |tmap| and branch to it@>;
    }@+else { /* code |j| matches a previous code */
      @<Remove |j| from the |tmap| and branch to it@>;
    }
    if (mem[pp]==0) { /* $a_0\ldots a_l$ is new */
      if (l+1<q) @<Allocate a new node for level |l+1|@>@;
      else @<Allocate a new weight@>;
    }
  }
  return p-1;
}
    
@ @<Swap |tmx| into the |tmap| and branch to it@>=
if (tmx>maxdeg) {
  maxdeg=tmx;
  if (tmx==deg) {
    fprintf(stderr,"Overflow: code digits must be less than %d!\n",
                           deg);
    exit(-66);
  }
}
tmap[tms]=tmx, itmap[tmx]=tms;
tms++,tmx++;
pp=p+tms;

@ @<Remove |j| from the |tmap| and branch to it@>=      
k=tmap[--tms],kk=itmap[j];
tmap[kk]=k,itmap[k]=kk;
pp=p+1+kk;

@ Some subtlety here: We don't provide a slot for |tmx| when |l+2+tms=q|,
because the value of |tmx| will have to occur twice.

@<Allocate a new node for level |l+1|@>=
{
  register int slots;
  if (l+1+tms<q) { /* allocate space for $\bar1$ and 0 */
    mem[memptr]=mem[memptr+1]=0,memptr+=2; /* this is safe, see below */
    slots=tms+(l+2+tms==q? 0: 1); /* see above */
  }@+else slots=tms; /* allocate space for existing positive codes only */
  if (memptr+slots+1>=memsize) {
    fprintf(stderr,"Oops: Dictionary overflow (more than %lld pointers)!\n",
                                         memsize@t)@>);
    exit(-666);
  }
  mem[pp]=memptr-1;
  for (j=0;j<slots;j++) mem[memptr+j]=0;
  memptr+=slots; /* now |memptr+1| is still less than |memsize| */
}

@ @<Allocate a new weight@>=
{
  mem[pp]=++wtptr;
  if (wtptr>=wtsize) {
    fprintf(stderr,"Oops: Dictionary overflow (more than %lld classes)!\n",
                                         wtsize@t)@>);
    exit(-6666);
  }
  weight[wtptr-1]=zero;
}
  
@ Now here's the recursive subroutine that's used to compress the
trie in |mem|, putting its equivalent into |oldmem|. The packed
bitstrings in |oldmem| will appear in a curious mixture of big-endian
and little-endian order, based on convenience rather than readability.

There's another sparse-set, with |omap|, |iomap|, |oms|, |omx|
analogous to |tmap|, |itmap|, |tms|, |tmx|. (All of these are global variables.)

This subroutine is invoked with |l=0|, |p=1|, |d=3|
and with |pack=spack=omp=owp=oms=0|, |omx=1|.

@<Recursive sub...@>=
void compress(int l,unsigned long long p,int d) {
         /* |d| is degree of node |p| at level |l| */
  register int j,k,kk,kkk;
  register unsigned long long bits;
  if (l==q) @<Save |weight[p-1]|@>@;
  else {
    for (j=(l+oms==q? 1:-1),k=0,bits=0;k<d;j++,k++)
      if (mem[p+j]) bits+=1LL<<k;
    @<Append |bits| to |oldmem|@>;
    for (j=(l+oms==q? 1:-1),k=0;k<d;j++,k++) if (bits&(1LL<<k)) {
      if (j>0) {
        if (j>oms) omap[oms]=omx, iomap[omx]=oms, oms++, omx++, kk=0;
        else kk=omap[--oms],kkk=omap[j-1],omap[j-1]=kk,iomap[kk]=j-1;
      }
      compress(l+1,mem[p+j],oms+(l+1+oms==q? 0:
                               l+2+oms==q? 2: 3));
      if (j>0) { /* we must undo our changes to the sparse-set map */
        if (!kk) oms--,omx--;
        else omap[j-1]=kkk,omap[oms]=kk,iomap[kk]=oms++;
      }
    }
  }
}

@ @d bitsperword 8*sizeof(unsigned long long)

@<Append |bits| to |oldmem|@>=
if (spack+d>bitsperword) {
  oldmem[omp++]=pack,pack=bits,spack=d;
  if (omp>=oldmemsize) {
    fprintf(stderr,"Oops: oldmem overflow (more than %d bytes)!\n",
                                         oldmemsize@t)@>);
    exit(-666666);
  }
}@+else pack+=bits<<spack,spack+=d; 

@ @<Save |weight[p-1]|@>=
{
  for (k=0;k<prec;k++)
    oldweight[owp].val[k]=weight[p-1].val[k];
  owp++;
}

@ @<Save the current trie of $m$-classes as the old trie of $(m-1)$-classes@>=
fprintf(stderr,"There are %lld %d-classes,\n",
                         wtptr,m-1);
fprintf(stderr," resulting from %lld contributions and filling %lld pointers.\n",
                         contribs,memptr);
spack=omp=owp=oms=0, omx=1;
pack=0;
oldq=q;
compress(0,1,3);
oldmem[omp]=pack; /* save the final bits */
if (omp>maxomp) maxomp=omp;

@ The old classes are retrieved from the compressed form by
another recursive procedure, which sort of mirrors the |compress| routine.

The main action of this program actually takes place when this
procedure reaches level |oldq| and ``visits'' an old class.
The old classes are visited in a special order vaguely related to
lexicographic order, and we reconstruct the |oldcode| values that
prefix each node.

To launch this routine, we'll call |uncompress(0,3)| with
|spack=oldp=omp=oms=0|, |omx=1|, and |pack=oldmem[0]|.

@<Recursive sub...@>=
void uncompress(int l,int d) { /* uncompress a node of degree |d| at level |l| */
  register int i,j,k,ii,kk,kkk;
  register unsigned long long bits;
  if (l==oldq) {
    @<Visit the class |oldcode| with weight |oldweight[oldp]|@>@;
    oldp++;
  }@+else {
    @<Set |bits| to the next |d| bits of |oldmem|@>;
    for (j=(l+oms==oldq? 1:-1),k=0;k<d;j++,k++) if (bits&(1LL<<k)) {
      if (j<=0) oldcode[l]=j;
      else if (j>oms)
        oldcode[l]=omx,omap[oms]=omx,iomap[omx]=oms,omx++,oms++,kk=0;
      else oldcode[l]=omap[j-1],kk=omap[--oms],
                     kkk=omap[j-1],omap[j-1]=kk,iomap[kk]=j-1;
      uncompress(l+1,oms+(l+1+oms==oldq? 0:
                          l+2+oms==oldq? 2: 3));
      if (j>0) { /* we must undo our changes to the sparse-set map */
        if (!kk) oms--,omx--;
        else omap[j-1]=kkk,omap[oms]=kk,iomap[kk]=oms++;
      }
    }
  }
}

@ @<Set |bits| to the next |d| bits of |oldmem|@>=
if (spack+d>bitsperword)
  pack=oldmem[++omp],spack=0;
bits=pack>>spack,spack+=d;

@*The frontiers.
We've defined $F_m$ as the set of all vertices $>m$ that are adjacent to
at least one vertex $\le m$. Thus $F_0=\emptyset$, and
$F_m=\bigl(F_{m-1}\cup\{v\mid m\adj v$ and $m<v\}\bigr)\setminus m$.
In particular, $F_1=\{v\mid 1\adj v\}$ is the set of neighbors of vertex~1.
Notice that $\vert F_m\vert\ge\vert F_{m-1}\vert-1$, and
$\vert F_m\vert\le n+1-m$.

For our purposes it turns out to be better to work with the {\it extended\/}
frontiers $\widehat F_m=F_m\cup\{m+1,n+1\}$, for $0\le m<n$, so that $m+1$
and $n+1$ are always elements of~$\widehat F_m$.
In particular, $\widehat F_0=\{1,n+1\}$.
This change doesn't change the definition
of equivalence classes, because vertex $m+1$ will always be bare if it
wasn't already in $F_m$; furthermore, $n+1$ was already present in every
frontier except~$F_0$. And we'll see that extended frontiers make
the program simpler.

A sparse-set data structure is ideal for maintaining the frontiers as $m$
grows. There's an array |fr|, which contains a permutation of the vertices,
and a companion array |ifr| for the inverse permutation. The first
|q| elements of |fr| are the current frontier.

More precisely, we have |fr[ifr[v]]=v|, for $1\le v\le n+1$, and
$1\le|fr[k]|\le n+1$ for $0\le k\le n$.
Vertex $v$ is part of $\widehat F_m$ if and
only if |ifr[v]<q|. (These conventions intentionally mix 0-origin and
1-origin indexing, because I've chosen for expository purposes to
use the names $\{1,\ldots,n+1\}$ for the vertices of $G^+$,
instead of calling them $\{0,\ldots,n\}$.)

The ordering of the frontier elements is important when we form the keys
for the trie. In this implementation we put them into a canonical order:
Let $q$ be the size of $\widehat F_m$, and let $q_0$ be the size of
$\bigl(\widehat F_{m-1}\cup\{m+1\}\bigr)\cap\widehat F_m$.
Then we will arrange things so that |fr[0]=n+1|, and
|fr[k-1]<fr[k]| for $1<k<q_0$ and for $q_0<k<q$. In particular, we'll always
have |fr[1]=m+1|, except in the trivial case $m=n$.

@<Glob...@>=
int fr[maxn], ifr[maxn+1]; /* the current frontier permutations */
int ofr[maxn]; /* copy of the previous frontier */
int q0; /* an intermediate frontier size (see below) */

@ @<Init...@>=
fr[0]=n+1,ifr[n+1]=0;
for (k=1;k<=n;k++) fr[k]=ifr[k]=k;
q=2;
mem[0]=mem[2]=0,mem[1]=1,memptr=3;
weight[0]=one,wtptr=1;

@ @<Change from $\widehat F_{m-1}$ to $\widehat F_m$@>=
for (j=1;j<q;j++) ofr[j]=fr[j];
oldq=q;
iv=ifr[m+1]; /* where is `$m+1$' in |fr|? */
if (iv<--q) { /* it's in $F_{m-1}$ and not the last guy */
  x=fr[q]; /* this is the last guy of $F_{m-1}$ */
  fr[1]=m+1,ifr[m+1]=1;
  fr[q]=m,ifr[m]=q;
  fr[iv]=x,ifr[x]=iv; /* do a three-way switcheroo */
}@+else {
  fr[1]=m+1,ifr[m+1]=1,fr[iv]=m,ifr[m]=iv; /* swap |m| with |m+1| */
  if (iv!=q) q++; /* restore the last guy */
}
q0=q;
@<Include elements of $\widehat F_m\setminus\widehat F_{m-1}$@>;
@<Sort the frontier@>;
@<Set up the mapping tables needed for transitions@>;

@ @d vert(k) (g->vertices+(k)-1)
@d vertnum(v) ((v)-g->vertices+1)

@<Include elements of $\widehat F_m\setminus\widehat F_{m-1}$@>=
for (a=vert(m)->arcs;a;a=a->next) {
  k=vertnum(a->tip); /* the number of a neighbor of |m| */
  if (k<m) continue;
  ik=ifr[k]; /* where does |k| live in |fr|? */
  if (ik>=q) { /* we should put |k| into the frontier */
    x=fr[q];
    fr[q]=k,ifr[k]=q,fr[ik]=x,ifr[x]=ik;
    q++;
  }
}

@ @<Sort the frontier@>=
for (k=3;k<q0;k++) if (fr[k]<fr[k-1]) {
  for (t=fr[k],j=k-1;;j--) {
    fr[j+1]=fr[j],ifr[fr[j]]=j+1;
    if (j==1 || fr[j-1]<t) break;
  }
  fr[j]=t,ifr[t]=j;
}
for (k=q0+1;k<q;k++) if (fr[k]<fr[k-1]) {
  for (t=fr[k],j=k-1;;j--) {
    fr[j+1]=fr[j],ifr[fr[j]]=j+1;
    if (j==q0 || fr[j-1]<t) break;
  }
  fr[j]=t,ifr[t]=j;
}

@ After $m$ has advanced and we're ready to run through all of the
$(m-1)$-classes in order, it's a good time to print the specs of the
relevant frontier.

@<Print the previous frontier@>=
fprintf(stderr,"\nThe frontier for %d-classes is",
                                m-1);
for (k=0;k<q;k++)
  fprintf(stderr," %s",
           vert(fr[k])->name);
fprintf(stderr,".\n");

@*The canonical key of an equivalence class.
When the current extended frontier has |q| vertices $v_1\ldots v_q$,
we represent each of its classes internally by a table
|mate[1]|\dots|mate[q]|,
where |mate[j]=0| when $v_j$ is bare; |mate[j]=-1| when $v_j$ is
inner; and |mate[j]=k| when $v_j$ and $v_k$ are the endpoints
of a subpath.

This representation is different from the sequence of codes described earlier,
where the endpoints of a path were vertices with the same code number.

The keys in our tries are sequences of code numbers, |code[0]| thru
|code[q-1]|. So we need to convert
from the |mate| representation to the |code| representation.

@<Convert the |mate| table to a key@>=
for (t=0,k=1;k<=q;k++) {
  j=mate[k];
  if (j<=0) code[k-1]=j;
  else if (j>k) code[k-1]=code[j-1]=++t;
}

@ The inverse operation is cute too:

@<Convert an old key to an |oldmate| table@>=
for (k=1;k+k<=oldq;k++) path[k]=0;
for (k=1;k<=oldq;k++) {
  j=oldcode[k-1];
  if (j<=0) oldmate[k]=j;
  else {
    l=path[j];
    if (!l) path[j]=k;
    else oldmate[k]=l,oldmate[l]=k;
  }
}

@ @<Glob...@>=
int path[maxn]; /* working storage for path numbers or endpoints */
int mate[maxn],oldmate[maxn]; /* tables that characterize classes */
int bmate[maxn]; /* a basic mate table in transitions */
int mp[maxn+1]; /* the |map| function: |map[x]=mp[1+x]| */
int imap[maxn]; /* inverse (roughly) of the |map| function */
int r; /* the number of neighbors of vertex $m$ that exceed $m$ */
int nbr[maxn]; /* list of those neighbors, as indices to the frontier */
int steps; /* the number of old classes we have processed */

@*The transitions.
The basic idea in this program is that we can compute all of the
class weights of $m$-configs when we know all of the class weights
of $(m-1)$-configs. In order to understand this computation, it
will be helpful to consider a nontrivial example that exhibits most of
the features of the general case.

Consider therefore the case when $m=5$, $n=19$, and the 4-frontier of the
graph~$G^+$ is the sequence of vertices $(20,5,6,13,7,8)$.
Furthermore we shall assume
that the neighbors of vertex~5 that exceed~5 are 6, 8, 11, 12, and 20.

It follows from the program above that the 5-frontier of the graph
will be the sequence of vertices $(20,6,7,8,13,11,12)$. (First the
|fr| array is changed so that its first six elements are $(20,6,8,13,7,5)$,
and $q$ is reduced from |oldq=6| to~$q_0=5$;
then 11 and 12 are swapped into place while $q$ increases to~7;
then we sort.)

In the trie of 4-classes (the equivalence classes for 4-configs),
the vertices $u_1u_2u_3u_4u_5u_6$ are the vertices 20,~5, 6, 13, 7, 8 of $G^+$,
in that order;
and the |oldmate| table uses indices $\{1,2,3,4,5,6\}$ for those vertices.
For example, if vertices 6 and 7 are endpoints of a subpath in some
equivalence class, they are vertices $u_3$ and $u_5$; hence
|oldmate[3]=5| and |oldmate[5]=3|.

The vertices $v_1v_2v_3v_4v_5v_6v_7$ of the 5-frontier are, similarly,
vertices 20, 6, 7, 8, 13, 11, 12 of $G^+$,
in that order. Vertices 6 and 7 of the graph
are now $v_2$ and $v_3$; so the relations |oldmate[3]=5| and |oldmate[5]=3|
in the 4-frontier 
are equivalent to |mate[2]=3| and |mate[3]=2| in the 5-frontier.

It's natural therefore to construct a |map| array that characterizes the
relation between the two labelings: |map[1]=1|, |map[2]=0|, |map[3]=2|,
|map[4]=5|, |map[5]=3|, |map[6]=4|, so that |map[1]| is always~1,
|map[2]| is always~0, and
$u_j=v_{map[j]}$ for $2<j\le|oldq|$.
We also set |map[-1]=-1| and |map[0]=0|.

Furthermore we can make an inverse array |imap|, with |imap[1]=1|,
|imap[2]=3|, |imap[3]=5|, |imap[4]=6|, |imap[5]=4|; in general
$v_j=u_{imap[j]}$ for $1\le j\le q_0$, where $q_0$ is the number
of vertices in the 5-frontier that were also in the 4-frontier
(because in this example the $(m{-}1)$-frontier happens to contain $m+1=6$).
It follows that, most of the time, |mate[j]=map[oldmate[imap[j]]]|,
for $1\le j\le q_0$; and |mate[j]=0| for $q_0<j\le q$.
We also set |oldmate[0]=mate[0]=0|.

``Most of the time'' in the previous paragraph means ``except when
|oldmate[imap[j]]=2|,'' because |map[2]=0|. Let's ignore that exceptional
case for the moment.

@<Init...@>=
mp[0]=-1, mp[2]=1; /* |map[-1]=-1|, |map[1]=1| */
imap[1]=1;

@ The simplest case arises in equivalence classes where the vertex~$m$
is ``inner'' in its $(m-1)$-config, namely when |oldmate[2]=-1|.
Then the $(m-1)$-config is already an $m$-config.
So we simply contribute it to the
trie of $m$-configs, after a suitable remapping.
The |mate| table in this case is called |bmate|.

@<Create the |bmate| table from |oldmate|@>=
for (j=1;j<=q0;j++) bmate[j]=mp[1+oldmate[imap[j]]]; /* |mp[x]=map[x-1]| */
for (;j<=q;j++) bmate[j]=0;

@ @<Contribute the |bmate| class@>=
{
  for (j=1;j<=q;j++) mate[j]=bmate[j];
  contribute();
}

@ The |contribute| subroutine just mentioned is our basic mechanism
for updating the trie of $m$-classes.

@<Sub...@>=
void contribute(void) {
  register int j,k,t;
  register long long p;
  @<Convert the |mate| table to a key@>;
  p=trielookup();
  add_to_bignum(&weight[p],oldweight[oldp]);
  contribs++;
}

@ The neighbors of 5 that exceed 5 in our example graph, namely
$(20,6,8,11,12)$, also happen to be $(v_1,v_2,v_4,v_6,v_7)$.
In general when vertex~$m$
has $r$ neighbors in the $m$-frontier, we can list their indices in
the |nbr| array; our example has $r=5$, |nbr[0]=1|,
|nbr[1]=2|, |nbr[2]=4|, |nbr[3]=6|, |nbr[4]=7|.
We'll always have |nbr[0]=1|.

@<Initialize@>=
nbr[0]=1;

@ @<Set up the mapping tables needed for transitions@>=
for (r=1,a=vert(m)->arcs;a;a=a->next) {
  k=vertnum(a->tip); /* the number of a neighbor of |m| */
  if (k<m) continue;
  nbr[r++]=ifr[k]+1;
}

@ Another ``unexceptional'' case arises in equivalence classes where vertex~$m$
is ``bare,'' namely when we have |oldmate[2]=0|.
Then there are $r\choose2$ potential contributions
to $m$-classes, namely one for every pair of neighbors.
In our example, one of the ${5\choose2}=10$ choices of neighbors is
$v_4\adj 5\adj v_7$; we add those two edges to the class for |bmate| if we can.
The effect is the same as adding the derived edge $v_4\adj v_7$ to that class.

@<Contribute the relevant classes when |m| is bare@>=
{
  for (i=0;i<r;i++) for (ii=i+1;ii<r;ii++) {
    for (j=1;j<=q;j++) mate[j]=bmate[j];
    if (add_derived(nbr[i],nbr[ii])) contribute();
  }
}

@ Finally, the exceptional cases arise when |oldmate[2]>0|. Then $r$
contributions to $m$-classes occur, one for each neighbor.
For example, suppose |oldmate[2]=6|, meaning that $u_2$ is the
endpoint of a path whose other endpoint is $u_6$, which is also
$v_{map[6]}=v_4$. (It's an exceptional case because
|oldmate[imap[4]]=oldmate[6]=2|.) Adding an edge from 5 to $v_j$ is
very much like adding an edge between $v_4$ and $v_j$, because
it creates a path from $v_4$ to $v_j$.

@<Contribute the relevant classes when |m| is outer@>=
{
  for (i=0;i<r;i++) {
    for (j=1;j<=q;j++) mate[j]=bmate[j];
    if (add_derived(mp[1+oldmate[2]],nbr[i])) contribute();
  }
}

@ If vertex $m+1$ is not in the old frontier, we want |imap[2]| to be zero,
so that |bmate[2]| will also be zero.
Otherwise |imap[2]| should be set to the former location of $m+1$.

@<Set up the mapping tables needed for transitions@>=
imap[2]=0;
for (j=3;j<=oldq;j++) {
  mp[j+1]=1+ifr[ofr[j-1]]; /* |map[j]=mp[j+1]| */
  imap[mp[j+1]]=j;
}

@*Adding a derived edge.
Given a set of subpaths between pairs of the vertices $v_1\ldots v_q$,
defined by the |mate| table, we can add a connection between $v_i$ and~$v_j$
only when neither $v_i$ nor~$v_j$ is an inner vertex, namely
when |mate[i]>=0| and |mate[j]>=0|.

And in the latter case there are four subcases, depending on whether
$v_i$ and/or $v_j$ are bare.

An unexpected subtlety does arise: If vertex~$m$ is an outer vertex that's
adjacent to its other endpoint, we're closing a derived loop.

@<Sub...@>=
int add_derived(int i,int j) {
  register int k,kk;
  if (mate[i]<0 || mate[j]<0) return 0;
  if (!mate[i]) { /* $v_i$ is bare */
    if (!mate[j]) { /* $v_j$ is bare */
      if (i==j) goto cycle;
      mate[i]=j,mate[j]=i;
      return 1;
    }@+else { /* $v_j$ is outer */
      mate[i]=mate[j],mate[mate[j]]=i,mate[j]=-1; /* $v_j$ becomes inner */
      return 1;
    }
  }@+else if (!mate[j]) { /* $v_j$ is bare */
    mate[j]=mate[i],mate[mate[i]]=j,mate[i]=-1; /* $v_i$ becomes inner */
    return 1;
  }@+else if (mate[i]!=j) { /* join two subpaths */
    mate[mate[i]]=mate[j],mate[mate[j]]=mate[i];
    mate[i]=mate[j]=-1;
    return 1;
  }@+else @<Take note of a cycle and |return 0|@>;
}

@ A cycle is, of course, formed when we join the endpoints of a subpath.
In such a case all vertices of the cycle have become inner.
Cycles aren't legal in an $m$-config; so we won't be contributing the
current class to the trie.

The cycle might, however, be important to us, namely when 
it's one of the special cycles that we are enumerating.
That happens if and only if there is an integer $m'$
such that (i)~all vertices $\le m'$ in the graph are inner; and
(ii)~all vertices $>m'$ in the graph are bare. When both conditions
are satisfied, we contribute to the count of all Hamiltonian $m'$-paths.

The frontier vertices |fr[k]| for $q_0\le k<q$ cannot be inner, because their
only neighbor $\le m$ is $m$ itself. A valid cycle must therefore have
|fr[1]=m+1|, |fr[2]=m+2|, \dots, |fr|$[m'-m]=m'$, and $m'-m<q_0$.

@<Take note of a cycle and |return 0|@>=
{
cycle: mate[i]=mate[j]=-1;
  if (mate[1]>=0) return 0; /* $n+1$ is not inner */
  for (k=2;k<=q0;k++) {
    if (mate[k]>=0) break; /* $v_k$ is not inner */
    if (fr[k-1]!=m+k-1) return 0; /* |m+k-1| is not inner */
  }
  for (kk=k;kk<=q;kk++)
    if (mate[kk]) return 0; /* a frontier element |>=m+k-1| isn't bare */
  {
    register int l;
    fprintf(stderr,"Class ");
    for (l=0;l<oldq;l++) fprintf(stderr,"%c",
                         encode(oldcode[l]));
    fprintf(stderr," contributes ");
    print_bignum(stderr,oldweight[oldp]);
    fprintf(stderr," to a %d-path.\n",
                                    m+k-2);
    add_to_bignum(&count[m+k-2],oldweight[oldp]);
  }
  return 0;    
}

@*Traversing the trie.
The main loop of this program consists of visiting every $(m-1)$-class in the
``old'' trie --- the trie defined by |oldmem| and |oldweight| ---
and contributing its successors to the ``new'' trie of $m$-classes.
It takes place at level |oldq| of the |uncompress| routine, at which
time the |oldcode| table has been set up for the current $(m-1)$-class
that's being visited.

@<Contribute the successors of each old class to the new trie@>=
spack=oldp=omp=oms=0, omx=1, pack=oldmem[0];
uncompress(0,3);

@ @<Visit the class |oldcode| with weight |oldweight[oldp]|@>=
{
  @<Convert an old key to an |oldmate| table@>;
  @<Print statistics if this class is noteworthy@>;
  @<Create the |bmate| table from |oldmate|@>;
  if (oldmate[2]<0) @<Contribute the |bmate| class@>@;
  else if (oldmate[2]==0) @<Contribute the relevant classes when |m| is bare@>@;
  else @<Contribute the relevant classes when |m| is outer@>;
}

@ @<Print statistics if this class is noteworthy@>=
if (oldp==0) @<Print statistics for the first class@>@;
else @<Print statistics for a subsequent class@>;

@ @d encode(x) ((x)<0?'#':(x)<10?(x)+'0':(x)-10+'a')

@<Print statistics for the first class@>=
{
  fprintf(stderr,"The first one is ");
  for (l=0;l<oldq;l++) fprintf(stderr,"%c",
                         encode(oldcode[l]));
  fprintf(stderr,"\nand its weight is ");
  print_bignum(stderr,oldweight[oldp]);
  fprintf(stderr,".\n");
  minweight=maxweight=oldweight[oldp];
  steps=contribs=0;
  fflush(stderr);
}

@ @<Print statistics for a subsequent class@>=
{
  if (bignum_comp(oldweight[oldp],minweight)<0) {
    minweight=oldweight[oldp];
    fprintf(stderr,"Class ");
    for (l=0;l<oldq;l++) fprintf(stderr,"%c",
                         encode(oldcode[l]));
    fprintf(stderr," has weight ");
    print_bignum(stderr,oldweight[oldp]);
    fprintf(stderr,".\n");
  }
  if (bignum_comp(oldweight[oldp],maxweight)>0) {
    maxweight=oldweight[oldp];
    fprintf(stderr,"Class ");
    for (l=0;l<oldq;l++) fprintf(stderr,"%c",
                         encode(oldcode[l]));
    fprintf(stderr," has weight ");
    print_bignum(stderr,oldweight[oldp]);
    fprintf(stderr,".\n");
  }
  if ((steps++&progress_mask)==progress_mask) {
    fprintf(stderr,".");
    fflush(stderr);
  }
}

@ @<Start up a new trie, which will receive the $m$-classes@>=
if (memptr>maxmemptr) maxmemptr=memptr;
if (wtptr>maxwtptr) maxwtptr=wtptr;
mem[0]=mem[1]=mem[2]=0,memptr=3,wtptr=0;
   /* root node is always present */
@<Change from $\widehat F_{m-1}$ to $\widehat F_m$@>;

@ If a Hamiltonian $m'$-cycle exists, there is at least one
$\lfloor(m'-1)/2\rfloor$-config. But there might not be any
$\lfloor(m'+1)/2\rfloor$-config. Therefore we might have to list
several cycle counts after the last $m$-config has been found.

@<Give a final report and exit@>=
{
  fprintf(stderr,"\n");
  for (k=m;k<=n;k++) {
    if (k>m+m) break;
    report_cycles(k);
  }
  fprintf(stderr,"\nThat's all; there are no %d-configs!\n",
                                      m-1);
  fprintf(stderr,
          "Storage requirements: %lld memsize, %d deg, %lld wtsize, %d maxprec.\n",
                                 maxmemptr,maxdeg+1,maxwtptr,18*prec);
  exit(0);
}

@ @<Sub...@>=
void report_cycles(int m) { /* report the number of Hamiltonian $m$-paths */
  if (bignum_comp(count[m],one)>=0) { /* if nonzero */
    printf("There are ");
    print_bignum(stdout,count[m]);
    printf(" Hamiltonian %d-paths.\n",
                             m);
    fflush(stdout);
  }
}

@*Index.
