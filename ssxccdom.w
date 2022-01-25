@s mod and
\let\Xmod=\bmod % this is CWEB magic for using "mod" instead of "%"

\datethis
@*Intro. This program is an ``{\mc XCC} solver'' that I'm writing
as an experiment in the use of so-called sparse-set data structures
instead of the dancing links structures I've played with for thirty years.
I plan to write it as if I live on a planet where the sparse-set
ideas are well known, but doubly linked links are almost unheard-of.
As I begin, I know that the similar program {\mc SSXCC2} works fine.

It's an experimental extension of {\mc SSXCC2}, designed to work
{\it only\/} on input from the program {\mc GRACEFUL-DLX}. The idea
is to keep track of the {\it domains\/} of all vertices in a graceful
labeling problem, because this can significantly prune the search tree.

As in {\mc SSXCC2}, I'm following ideas pioneered by Christine Solnon.
She wrote a program \.{xcc-dom.c} in October, 2020, which gave impressive
results; and I'm writing this because I can't resist adding some
refinements that occurred to me while reading it. Hopefully my program
won't be too much more difficult to understand than hers is.

(I apologize for writing this in a hurry.)

@ After this program finds all solutions, it normally prints their total
number on |stderr|, together with statistics about how many
nodes were in the search tree, and how many ``updates'' were made.
The running time in ``mems'' is also reported, together with the approximate
number of bytes needed for data storage.
(An ``update'' is the removal of an option from its item list,
or the removal of a satisfied color constraint from its option.
One ``mem'' essentially means a memory access to a 64-bit word.
The reported totals don't include the time or space needed to parse the
input or to format the output.)

@d o mems++ /* count one mem */
@d oo mems+=2 /* count two mems */
@d ooo mems+=3 /* count three mems */
@d O "%" /* used for percent signs in format strings */
@d mod % /* used for percent signs denoting remainder in \CEE/ */

@d max_verts 100 /* at most this many vertices */
@d max_edges 156 /* at most this many edges, based on |encode| */
@d max_level max_edges /* at most this many options in a solution */
@d max_cols (3*max_edges+max_verts+1) /* at most this many items */
@d max_nodes 10000000 /* at most this many nonzero elements in the matrix */
@d memsize (1<<27) /* main memory for counters; must not exceed $2^{32}$ */
@d bufsize (9*max_cols+3) /* a buffer big enough to hold all item names */

@ Here is the overall structure:

@c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef unsigned int uint; /* a convenient abbreviation */
typedef unsigned long long ullng; /* ditto */
@<Type definitions@>;
@<Global variables@>;
@<Subroutines@>;
main (int argc, char *argv[]) {
  register int c,cc,i,j,k,p,pp,q,qq,r,s,t,cur_choice,cur_node,best_itm;
  @<Process the command line@>;
  @<Input the item names@>;@+@<Input the options@>;
  if (vbose&show_basics)
    @<Report the successful completion of the input phase@>;
  if (vbose&show_tots)
    @<Report the item totals@>;
  imems=mems, mems=0;
  if (baditem) @<Report an uncoverable item@>@;@+else @<Solve the problem@>;
done:@+if (vbose&show_profile) @<Print the profile@>;
  if (vbose&show_final_weights) print_weights();
  if (vbose&show_max_deg)
    fprintf(stderr,"The maximum branching degree was "O"d.\n",maxdeg);
  if (vbose&show_basics) {
    fprintf(stderr,"Altogether "O"llu solution"O"s, "O"llu+"O"llu mems,",
                                count,count==1?"":"s",imems,mems);
    bytes=(itemlength+memlength)*sizeof(tetrabyte)+last_node*sizeof(node)
        +2*maxl*sizeof(int)+(memsize-minsaveptr)*sizeof(tetrabyte);
    fprintf(stderr," "O"llu updates, "O"llu bytes, "O"llu nodes.\n",
                                updates,bytes,nodes);
  }
  if (sanity_checking) fprintf(stderr,"sanity_checking was on!\n");
  @<Close the files@>;
}

@ You can control the amount of output, as well as certain properties
of the algorithm, by specifying options on the command line:
\smallskip\item{$\bullet$}
`\.v$\langle\,$integer$\,\rangle$' enables or disables various kinds of verbose
 output on |stderr|, given by binary codes such as |show_choices|;
\item{$\bullet$}
`\.m$\langle\,$integer$\,\rangle$' causes every $m$th solution
to be output (the default is \.{m0}, which merely counts them);
\item{$\bullet$}
`\.d$\langle\,$integer$\,\rangle$' sets |delta|, which causes periodic
state reports on |stderr| after the algorithm has performed approximately
|delta| mems since the previous report (default 10000000000);
\item{$\bullet$}
`\.c$\langle\,$positive integer$\,\rangle$' limits the levels on which
choices are shown during verbose tracing;
\item{$\bullet$}
`\.C$\langle\,$positive integer$\,\rangle$' limits the levels on which
choices are shown in the periodic state reports;
\item{$\bullet$}
`\.l$\langle\,$nonnegative integer$\,\rangle$' gives a {\it lower\/} limit,
relative to the maximum level so far achieved, to the levels on which
choices are shown during verbose tracing;
\item{$\bullet$}
`\.t$\langle\,$positive integer$\,\rangle$' causes the program to
stop after this many solutions have been found;
\item{$\bullet$}
`\.T$\langle\,$integer$\,\rangle$' sets |timeout| (which causes abrupt
termination if |mems>timeout| at the beginning of a level);
\item{$\bullet$}
`\.w$\langle\,$float$\,\rangle$' is the initial increment |dw| added to
an item's weight (default 1.0);
\item{$\bullet$}
`\.W$\langle\,$float$\,\rangle$' is the factor by which |dw| changes
dynamically (default 1.0);
\item{$\bullet$}
`\.S$\langle\,$filename$\,\rangle$' to output a ``shape file'' that encodes
the search tree.

@<Glob...@>=
int vbose=show_basics+show_warnings; /* level of verbosity */
int spacing; /* solution $k$ is output if $k$ is a multiple of |spacing| */
int show_choices_max=1000000; /* above this level, |show_choices| is ignored */
int show_choices_gap=1000000; /* below level |maxl-show_choices_gap|,
    |show_details| is ignored */
int show_levels_max=1000000; /* above this level, state reports stop */
int maxl; /* maximum level actually reached */
int minsaveptr; /* maximum extent of |savestack| */
char buf[bufsize]; /* input buffer */
ullng count; /* solutions found so far */
ullng options; /* options seen so far */
ullng imems,mems; /* mem counts */
ullng updates; /* update counts */
ullng bytes; /* memory used by main data structures */
ullng nodes; /* total number of branch nodes initiated */
ullng thresh=10000000000; /* report when |mems| exceeds this, if |delta!=0| */
ullng delta=10000000000; /* report every |delta| or so mems */
ullng maxcount=0xffffffffffffffff; /* stop after finding this many solutions */
ullng timeout=0x1fffffffffffffff; /* give up after this many mems */
float w0=1.0,dw=1.0,dwfactor=1.0; /* initial weight, increment, and growth */
FILE *shape_file; /* file for optional output of search tree shape */
char *shape_name; /* its name */
int maxdeg; /* the largest branching degree seen so far */

@ Here are the various flavors of verbosity:

@d show_basics 1 /* |vbose| code for basic stats; this is the default */
@d show_choices 2 /* |vbose| code for backtrack logging */
@d show_details 4 /* |vbose| code for further commentary */
@d show_domain_details 8 /* |vbose| code for even more */
@d show_internal_locs 32 /* |vbose| code to show where items live in |mem| */
@d show_final_weights 64 /* |vbose| code to display weights at the end */
@d show_profile 128 /* |vbose| code to show the search tree profile */
@d show_full_state 256 /* |vbose| code for complete state reports */
@d show_tots 512 /* |vbose| code for reporting item totals at start */
@d show_warnings 1024 /* |vbose| code for reporting options without primaries */
@d show_max_deg 2048 /* |vbose| code for reporting maximum branching degree */

@ If an option appears more than once on the command line, the first
appearance takes precedence.

@<Process the command line@>=
for (j=argc-1,k=0;j;j--) switch (argv[j][0]) {
case 'v': k|=(sscanf(argv[j]+1,""O"d",&vbose)-1);@+break;
case 'm': k|=(sscanf(argv[j]+1,""O"d",&spacing)-1);@+break;
case 'd': k|=(sscanf(argv[j]+1,""O"lld",&delta)-1),thresh=delta;@+break;
case 'c': k|=(sscanf(argv[j]+1,""O"d",&show_choices_max)-1);@+break;
case 'C': k|=(sscanf(argv[j]+1,""O"d",&show_levels_max)-1);@+break;
case 'l': k|=(sscanf(argv[j]+1,""O"d",&show_choices_gap)-1);@+break;
case 't': k|=(sscanf(argv[j]+1,""O"lld",&maxcount)-1);@+break;
case 'T': k|=(sscanf(argv[j]+1,""O"lld",&timeout)-1);@+break;
case 'w': k|=(sscanf(argv[j]+1,""O"f",&dw)-1);@+break;
case 'W': k|=(sscanf(argv[j]+1,""O"f",&dwfactor)-1);@+break;
case 'S': shape_name=argv[j]+1, shape_file=fopen(shape_name,"w");
  if (!shape_file)
    fprintf(stderr,"Sorry, I can't open file `"O"s' for writing!\n",
      shape_name);
  break;
default: k=1; /* unrecognized command-line option */
}
if (k) {
  fprintf(stderr, "Usage: "O"s [v<n>] [m<n>] [s<n>] [d<n>]"
       " [c<n>] [C<n>] [l<n>] [t<n>] [T<n>] [w<f>] [W<f>] [S<bar>] < foo.dlx\n",
                            argv[0]);
  exit(-1);
}

@ @<Close the files@>=
if (shape_file) fclose(shape_file);

@*Data structures.
Sparse-set data structures were introduced by Preston Briggs
and Linda Torczon [{\sl ACM Letters on Programming Languages and Systems\/
\bf2} (1993), 59--69], who realized that exercise 2.12 in
Aho, Hopcroft, and Ullman's classic text {\sl The Design and Analysis
of Computer Algorithms\/} (Addison--Wesley, 1974) was much more than
just a slick trick to avoid initializing an array.
(Indeed, {\sl TAOCP\/} exercise 2.2.6--24 calls it the ``sparse array trick.'')

The basic idea is amazingly simple, when specialized to the situations
that we need to deal with: We can represent a subset~$S$ of the universe
$U=\{x_0,x_1,\ldots,x_{n-1}\}$ by maintaining two $n$-element arrays
$p$ and $q$, each of which is a permutation of~$\{0,1,\ldots,n-1\}$,
together with an integer $s$ in the range $0\le s\le n$. In fact, $p$ is
the {\it inverse\/} of~$q$; and $s$ is the number of elements of~$S$.
The current value of the set $S$ is then simply
$\{x_{p_0},\ldots,x_{p_{s-1}}\}$. (Notice that every $s$-element subset can be
represented in $s!\,(n-s)!$ ways.)

It's easy to test if $x_k\in S$, because that's true if and only if $q_k<s$.
It's easy to insert a new element $x_k$ into~$S$: Swap indices
so that $p_s=k$, $q_k=s$, then increase $s$ by~1.
It's easy to delete an element $x_k$ that belongs to~$S$: Decrease $s$
by~1, then swap indices so that $p_s=k$ and $q_k=s$.
And so on.

Briggs and Torczon were interested in applications where $s$ begins at
zero and tends to remain small. In such cases, $p$ and $q$ need not
be permutations: The values of $p_s$, $p_{s+1}$, \dots, $p_{n-1}$ can
be garbage, and the values of $q_k$ need be defined only when $x_k\in S$.
(Such situations correspond to Aho, Hopcroft, and Ullman, who started
with an array full of garbage and used a sparse-set structure to remember
the set of nongarbage cells.) Our applications are different: Each set
begins equal to its intended universe, and gradually shrinks. In such
cases, we might as well maintain inverse permutations.
The basic operations go faster when we know in advance that we
aren't inserting an element that's already present (nor deleting an element
that isn't).

Many variations are possible. For example, $p$ could be a permutation
of $\{x_0,x_1,\ldots,x_{n-1}\}$ instead of permutation of
$\{0,1,\ldots,n-1\}$. The arrays that play the role of $q$
in the following routines don't have indices that are consecutive;
they live inside of other structures.

@ This program has an
array called |item|, with one entry for each item. The value of |item[k]|
is an index~|x| into a much larger array called |mem|. The set of all
options that involve the $k$th item appears in that array beginning
at |mem[x]|; and it continues for $s$ consecutive entries, where |s=size(x)|
is an abbreviation for |mem[x-1]|. If |item[k]=x|, we maintain the
relation |pos(x)=k|, where |pos(x)| is an abbreviation for
|mem[x-2]|. Thus |item| plays the role of array~$p$, in a
sparse-set data structure for the set of all currently active items;
and |pos| plays the role of~$q$.

A primary item $x$ also has a |wt| field, |mem[x-3]|, initially~1.
The weight is a floating point number. It's increased by |dw| whenever we
have to backtrack because |x| cannot be covered.

A secondary item $x$ that represents a vertex uses |mem[x-3]| for another
purpose, namely to hold the current size of its domain. So we call it
|mem[x-3]| the |dsize| field, in that context.

Suppose the |k|th item |x| currently appears in |s| options. Those options
are indices into |nd|, which is an array of ``nodes.'' Each node
has four fields: |itm|, |loc|, |clr|, and |aux|. If |x<=q<x+s|, let |y=mem[q]|.
This is essentially a pointer to a node, and we have
|nd[y].itm=x|, |nd[y].loc=q|. In other words,
the sequential list of |s| elements that begins at
|x=item[k]| in the |mem| array is the sparse-set representation of the
currently active options that contain the |k|th item.
The |clr| field contains |x|'s color for this option.
We'll describe |aux| later.
The |itm| and |clr| and |aux| fields remain constant,
once we've initialized everything, but the |loc| fields will change.

The given options are stored sequentially in the |nd| array, with one node
per item, separated by ``spacer'' nodes. If |y| is the spacer node
following an option with $t$ items, we have |nd[y].itm=-t|.
If |y| is the spacer node {\it preceding\/} an option with $t$ items,
we have |nd[y].loc=t|.

This probably sounds confusing, until you can see some code.
Meanwhile, let's take note of the invariant relations that hold
whenever |k|, |q|, |x|, and |y| have appropriate values:
$$\hbox{|pos(item[k])=k|;\quad
|nd[mem[q]].loc=q|;\quad
|item[pos(x)]=x|;\quad
|mem[nd[y].loc]=y|.}$$
(These are the analogs of the invariant relations |p[q[k]]=q[p[k]]=k| in
the simple sparse-set scheme that we started with.)

The |mem| array contains also the item names, as strings packed into
integers in a somewhat peculiar machine-oriented way. When the items
and options are being input, |mem| has only four entries per item;
it's expanded later, after we've absorbed all the options and recorded
them in the |nd| array.

We count one mem for a simultaneous access to the |itm| and |loc| fields
of a node, and one mem for a simultaneous access to |clr| and |aux|.

@d size(x) mem[(x)-1].i /* number of active options of the |k|th item, |x| */
@d pos(x) mem[(x)-2].i /* where that item is found in the |item| array */
@d wt(x) mem[(x)-3].f /* the weight of a primary item */
@d dsize(x) mem[(x)-3].i /* the domain size of a secondary variable item */
@d rnam(x) mem[(x)-3].i /* the last four bytes of |x|'s name, initially */
@d rname(x) mem[(x)-5].i /* those last four bytes after expansion */
@d lname(x) mem[(x)-4].i /* the last four bytes of |x|'s name */

@<Type...@>=
typedef struct node_struct {
  int itm; /* the item |x| corresponding to this node */
  int loc; /* where this node resides in |x|'s active set */
  int clr; /* color associated with item |x| in this option, if any */
  int aux; /* a spare field inserted only to maintain 16-byte alignment */
} node;
typedef union {
  unsigned int i; /* a nonnegative integer (32 bits) */
  float f; /* a floating point value (fits in 4 bytes) */
} tetrabyte;

@ @<Glob...@>=
node nd[max_nodes]; /* the master list of nodes */
int last_node; /* the first node in |nd| that's not yet used */
int item[max_cols]; /* the master list of items */
int second=max_cols; /* boundary between primary and secondary items */
int third; /* boundary between vertex and label secondaries */
int last_itm; /* items seen so far during input, plus 1 */
tetrabyte *mem; /* the big memory for backtrack data */
int itemlength; /* number of elements used in |item| */
int memlength; /* number of elements used in |mem| */
int active; /* current number of active items */
int oactive; /* value of active before swapping out current-choice items */
int baditem; /* an item with no options, plus 1 */
int osecond; /* setting of |second| just after initial input */
int verts,edges; /* vertices and edges in the user's graph */
int sbase; /* where the support counts begin in |mem| */

@ We're going to store string data (an item's name) in the midst of
the integer array |mem|. So we've got to do some type coercion using
low-level \CEE/-ness.

@<Type def...@>=
typedef struct {
  int l,r;
} twoints;
typedef union {
  unsigned char str[8]; /* eight one-byte characters */
  twoints lr; /* two four-byte integers */
} stringbuf;
stringbuf namebuf;

@ @<Subroutines@>=
void print_item_name(int k,FILE *stream) {
  namebuf.lr.l=lname(k),namebuf.lr.r=rname(k);
  fprintf(stream," "O".8s",namebuf.str);
}

@ An option is identified not by name but by the names of the items it contains.
Here is a routine that prints an option, given a pointer to any of its
nodes. It also prints the position of the option in its item list.

@<Sub...@>=
void print_option(int p,FILE *stream) {
  register int k,q,x;
  x=nd[p].itm;
  if (p>=last_node || x<=0) {
    fprintf(stderr,"Illegal option "O"d!\n",p);
    return;
  }
  for (q=p;;) {
    print_item_name(x,stream);
    if (nd[q].clr)
      fprintf(stream,":"O"c",encode(nd[q].clr-1));
    q++;
    x=nd[q].itm;
    if (x<0) q+=x,x=nd[q].itm;
    if (q==p) break;
  }
  k=nd[q].loc;
  fprintf(stream," ("O"d of "O"d)\n",k-x+1,size(x));
}
@#
void prow(int p) {
  print_option(p,stderr);
}

@ When I'm debugging, I might want to look at one of the current item lists.

@<Sub...@>=
void print_itm(int c) {
  register int p;
  if (c<4 || c>=memlength ||
         pos(c)<0 || pos(c)>=itemlength || item[pos(c)]!=c) {
    fprintf(stderr,"Illegal item "O"d!\n",c);
    return;
  }
  fprintf(stderr,"Item");
  print_item_name(c,stderr);
  if (c<second)
    fprintf(stderr," ("O"d of "O"d), length "O"d, weight "O"f:\n",
         pos(c)+1,active,size(c),wt(c));
  else if (pos(c)>=active)
    fprintf(stderr," (secondary "O"d, purified), length "O"d:\n",
         pos(c)+1,size(c));
  else fprintf(stderr," (secondary "O"d), length "O"d:\n",
         pos(c)+1,size(c));
  for (p=c;p<c+size(c);p++) prow(mem[p].i);
}

@ Speaking of debugging, here's a routine to check if redundant parts of our
data structure have gone awry.

@d sanity_checking 0 /* set this to 1 if you suspect a bug */

@<Sub...@>=
void sanity(void) {
  register int k,x,i,l,r,q,qq;
  for (k=0;k<itemlength;k++) {
    x=item[k];
    if (pos(x)!=k) {
      fprintf(stderr,"Bad pos field of item");
      print_item_name(x,stderr);
      fprintf(stderr," ("O"d,"O"d)!\n",k,x);
    }
  }
  for (i=0;i<last_node;i++) {
    l=nd[i].itm,r=nd[i].loc;
    if (l<=0) {
      if (nd[i+r+1].itm!=-r)
        fprintf(stderr,"Bad spacer in nodes "O"d, "O"d!\n",i,i+r+1);
      qq=0;
    }@+else {
      if (l>r) fprintf(stderr,"itm>loc in node "O"d!\n",i);
      else {
        if (mem[r].i!=i) {
          fprintf(stderr,"Bad loc field for option "O"d of item",r-l+1);
          print_item_name(l,stderr);
          fprintf(stderr," in node "O"d!\n",i);
        }
        if (pos(l)<active) {
          if (r<l+size(l)) q=+1;@+else q=-1; /* in or out? */
          if (q*qq<0) {
            fprintf(stderr,"Flipped status at option "O"d of item",r-l+1);
            print_item_name(l,stderr);
            fprintf(stderr," in node "O"d!\n",i);
          }
          qq=q;         
        }
      }
    }
  }
}

@ And here's a routine that should never be invoked.

@<Sub...@>=
void confusion(char*m) {
  fprintf(stderr,"This can't happen: "O"s!\n",m);
}

@*Inputting the matrix. Brute force is the rule in this part of the code,
whose goal is to parse and store the input data and to check its validity.

We use only four entries of |mem| per item while reading the item-name line.

@d panic(m) {@+fprintf(stderr,""O"s!\n"O"d: "O".99s\n",m,p,buf);@+exit(-666);@+}

@<Input the item names@>=
mem=(tetrabyte*)calloc(memsize,sizeof(tetrabyte));
if (!mem) {
  fprintf(stderr,
    "I couldn't allocate the mem array! (memsize="O"d)!\n",memsize);
  exit(-2);
}
while (1) {
  if (!fgets(buf,bufsize,stdin)) break;
  if (o,buf[p=strlen(buf)-1]!='\n') panic("Input line way too long");
  for (p=0;o,isspace(buf[p]);p++) ;
  if (buf[p]=='|' || !buf[p]) continue; /* bypass comment or blank line */
  last_itm=1;
  break;
}
if (!last_itm) panic("No items");
for (;o,buf[p];) {
  o,namebuf.lr.l=namebuf.lr.r=0;
  for (j=0;j<8 && (o,!isspace(buf[p+j]));j++) {
    if (buf[p+j]==':' || buf[p+j]=='|')
              panic("Illegal character in item name");
    o,namebuf.str[j]=buf[p+j];
  }
  if (j==8 && !isspace(buf[p+j])) panic("Item name too long");
  oo,lname(last_itm<<2)=namebuf.lr.l,rnam(last_itm<<2)=namebuf.lr.r;
  @<Check for duplicate item name@>;
  last_itm++;
  if (last_itm>max_cols) panic("Too many items");
  for (p+=j+1;o,isspace(buf[p]);p++) ;
  if (buf[p]=='|') {
    if (second!=max_cols) panic("Item name line contains | twice");
    second=last_itm;
    for (p++;o,isspace(buf[p]);p++) ;
  }
}

@ @<Check for duplicate item name@>=
for (k=last_itm-1;k;k--) {
  if (o,lname(k<<2)!=namebuf.lr.l) continue;
  if (rnam(k<<2)==namebuf.lr.r) break;
}
if (k) panic("Duplicate item name");

@ I'm putting the option number into the |spr| field of the
spacer that follows it, as a
possible debugging aid. But the program doesn't currently use that information.

These options are supposed to have been produced by {\mc GRACEFUL-DLX},
which uses a human-oriented encoding scheme for colors --- which must be
one character long, representing integers between 0 and 156.
Such colors are converted here into integers between 1 and 157
(but the |print_option| routine hides this from the user).

@<Input the options@>=
while (1) {
  if (!fgets(buf,bufsize,stdin)) break;
  if (o,buf[p=strlen(buf)-1]!='\n') panic("Option line too long");
  for (p=0;o,isspace(buf[p]);p++) ;
  if (buf[p]=='|' || !buf[p]) continue; /* bypass comment or blank line */
  i=last_node; /* remember the spacer at the left of this option */
  for (pp=0;buf[p];) {
    o,namebuf.lr.l=namebuf.lr.r=0;
    for (j=0;j<8 && (o,!isspace(buf[p+j])) && buf[p+j]!=':';j++)
      o,namebuf.str[j]=buf[p+j];
    if (!j) panic("Empty item name");
    if (j==8 && !isspace(buf[p+j]) && buf[p+j]!=':')
          panic("Item name too long");
    @<Create a node for the item named in |buf[p]|@>;
    if (buf[p+j]!=':') o,nd[last_node].clr=0;
    else if (k>=second) {
      if ((o,isspace(buf[p+j+1])) || (o,!isspace(buf[p+j+2])))
        panic("Color must be a single character");
      o,nd[last_node].clr=1+decode((unsigned char)buf[p+j+1]);
      p+=2;
    }@+else panic("Primary item must be uncolored");
    for (p+=j+1;o,isspace(buf[p]);p++) ;
  }
  if (!pp) {
    if (vbose&show_warnings)
      fprintf(stderr,"Option ignored (no primary items): "O"s",buf);
    while (last_node>i) {
      @<Remove |last_node| from its item list@>;
      last_node--;
    }
  }@+else {
    o,nd[i].loc=last_node-i; /* complete the previous spacer */
    last_node++; /* create the next spacer */
    if (last_node==max_nodes) panic("Too many nodes");
    options++;
    o,nd[last_node].itm=i+1-last_node;
    nd[last_node].aux=options; /* option number, for debugging only */
  }
}
@<Initialize |item|@>;
@<Parse the options, making sure that they conform to {\mc GRACEFUL-DLX}@>;
@<Expand |mem|@>;
@<Adjust |nd|@>;
for (k=0;k<verts;k++)
  @<Initialize the domain of vertex |k|@>;
@<Initialize the stack@>;

@ We temporarily use |pos| to recognize duplicate items in an option.

@<Create a node for the item named in |buf[p]|@>=
for (k=(last_itm-1)<<2;k;k-=4) {
  if (o,lname(k)!=namebuf.lr.l) continue;
  if (rnam(k)==namebuf.lr.r) break;
} 
if (!k) panic("Unknown item name");
if (o,pos(k)>i) panic("Duplicate item name in this option");
last_node++;
if (last_node==max_nodes) panic("Too many nodes");
o,t=size(k); /* how many previous options have used this item? */
o,nd[last_node].itm=k>>2,nd[last_node].loc=t;
if ((k>>2)<second) pp=1;
o,size(k)=t+1, pos(k)=last_node;

@ @<Remove |last_node| from its item list@>=
o,k=nd[last_node].itm<<2;
oo,size(k)--,pos(k)=i-1;

@ @<Initialize |item|@>=
active=itemlength=last_itm-1;
for (k=0,j=5;k<itemlength;k++)
  oo,item[k]=j,j+=5+size((k+1)<<2);
memlength=j-5;
if (second==max_cols) osecond=active,second=j;
else osecond=second-1;

@ The input to this program must follow a very strict format, corresponding
to the way {\mc GRACEFUL-DLX} produces options from a given graph.
If the graph has $m$ edges and $n$ edges, the first $m$ items
are single characters, representing the encoded numbers 1 thru~$m$.
The next $m$ items have the form \.{$u$-$v$}, where $u$ and $v$ are
vertex names. The first $n$ primary items have the form \.{.$v$},
where $v$ is a vertex name. And the final $m+1$ primary items have
the form \.{+$c$}, where $c$ is an encoded character between 0 and~$m$.

All options have exactly 6 items, and they have the general form
$$ l\quad u\.-v\quad \..u{:}i\quad \..v{:}j\quad \.+i{:}c(u)\quad \.+j{:}c(v)$$
where $l$ is an encoded edge label, $u$ and $v$ are vertex names,
$i$ and $j$ are encoded vertex labels, $c(u)$ and $c(v)$ are encoded
vertex numbers. It means that edge label |l| is formed by the edge
between $u$ and $v$, because $u$ and $v$ are labeled $i$ and $j$.
(Furthermore $l=\vert i-j\vert$, but I don't check that.)

This program analyzes the graph structure in order to build
internal tables that enable it to keep track of vertex domains.
The testing in this section doesn't try to be 100\% accurate, but it
does try to detect input that is wildly wrong.

@d encode(x) ((x)<10? (x)+'0': (x)<36? (x)-10+'a': (x)<62? (x)-36+'A': (x)+99)
@d decode(x) ((x)<='9'? (x)-'0': (x)<='Z'? (x)-'A'+36:
                                   (x)<='z'? (x)-'a'+10: (x)-99)

@<Parse the options, making sure that they conform to {\mc GRACEFUL-DLX}@>=
@<Check the items for conformity, calculating |verts| and |edges|@>;
@<Check the options for conformity@>;
fprintf(stderr,"(the graph has "O"d vertices, "O"d edges)\n",verts,edges);

@ @<Check the items for conformity, calculating |verts| and |edges|@>=
for (k=0;;k++) {
  o,namebuf.lr.l=lname((k+1)<<2);
  if (namebuf.str[1]) break;
  if (namebuf.str[0]!=encode(k+1)) {
    fprintf(stderr,"Item "O"d should be named `"O"c', not `"O"c'!\n",
                 k+1,encode(k+1),namebuf.str[0]);
    exit(-10);
  }
}
edges=k,k+=edges;
if (second!=k+1) {
  fprintf(stderr,"There should be "O"d primary items, not "O"d!\n",k,second-1);
  exit(-11);
}
for (p=0;;p++) {
  o,namebuf.lr.l=lname((k+p+1)<<2);
  if (namebuf.str[0]!='.') break;
  o,vert[p]=item[k+p];
}  
verts=p,k+=verts;
if (itemlength!=3*edges+verts+1) {
  fprintf(stderr,"There should be "O"d items, not "O"d!\n",
           3*edges+verts+1,itemlength);
  exit(-12);
}

@ Each edge has two endpoints, and a vertex of degree $d$ is associated
with $d$ of those $2m$ endpoints. We sneakily include the information
about which endpoints correspond to~$v$ in the |aux| fields of the
first option where the endpoint is used.

@<Check the options for conformity@>=
for (k=0;k<last_node;k+=7) {
  if (o,nd[k].loc!=6) {
    fprintf(stderr,
      "Option "O"d should have length 6, not "O"d!\n",1+k/7,nd[k].loc);
    exit(-13);
  }
  ooo,p=nd[k+2].itm-edges-1,q=nd[k+3].itm-second,r=nd[k+4].itm-second;
  if (p<0 || p>=edges || q<0 || q>=verts || r<0 || r>=verts) {
    fprintf(stderr,"Option "O"d is malformed!\n",1+k/7);
    exit(-14);
  }
  if ((o,q!=nd[k+5].clr-1) || (o,r!=nd[k+6].clr-1)) {
    fprintf(stderr,"Option "O"d is screwed up!\n",1+k/7);
    exit(-15);
  }
  if (o,!endpt[p]) {
    o,endpt[p]=item[nd[k+3].itm-1]; /* another mem is charged later */
    ooo,nd[k+1].aux=first[q],first[q]=k+1;
  }
  if (o,!endpt[p+edges]) {
    o,endpt[p+edges]=item[nd[k+4].itm-1]; /* another mem is charged later */
    ooo,nd[k+2].aux=first[r],first[r]=k+2;
  }
  if ((o,endpt[p]!=item[nd[k+3].itm-1]) ||
      (o,endpt[p+edges]!=item[nd[k+4].itm-1])) {
    fprintf(stderr,"Option "O"d's edge is inconsistent!\n",1+k/7);
    exit(-16);
  }
  oo,nd[k+3].aux=p,nd[k+4].aux=p+edges;
  oo,nd[k+5].aux=p,nd[k+6].aux=p+edges;
}

@ Going from high to low, we now move the item names and sizes
to their final positions (leaving room for the pointers into |nb|).

@<Expand |mem|@>=
for (o,t=mem[0].i,k=itemlength;k;k--) {
  o,j=item[k-1];
  oo,size(j)=size(k<<2);
  if (size(j)==0 && k<osecond) baditem=k;
  o,pos(j)=k-1;
  oo,rname(j)=rnam(k<<2),lname(j)=lname(k<<2);
  if (k<=osecond) o,wt(j)=w0;
}
o,lname(j)=t; /* this kludge patches a weird bug (go figure) */
second=item[2*edges]; /* |second| is now an index into |mem| */
third=item[2*edges+verts]; /* and |third| ends the vertex secondaries */
if (vbose&show_internal_locs) {
  for (k=0;k<itemlength;k++) {
    print_item_name(item[k],stderr);
    fprintf(stderr,"("O"d)\n",item[k]);
  }
}

@ For each of the $2m$ endpoints, we maintain an array of $m+1$
``support pointers,'' which indicate how many options currently assign
a given color to that endpoint. These counters appear en masse in |mem|,
just after the items, so that they can be saved and restored with the
same mechanism that's used for sparse-set counts.

The |aux| field of a node that represents a vertex is set to the |mem|
location of its corresponding support counter. (In previous processing,
we've set |aux| to the endpoint number.)

@d supp(ept) sbase+ept*(edges+1)

@<Adjust |nd|@>=
sbase=memlength;
memlength+=2*edges*(edges+1);
if (memlength>=memsize) {
  fprintf(stderr,"Not nearly enough memory (memsize="O"d)!\n",memsize);
  exit(-6);
}
for (k=1;k<last_node;k++) {
  o,p=nd[k].itm;
  if (p<0) continue; /* skip over a spacer */
  o,j=item[p-1];
  i=j+nd[k].loc; /* no mem charged because we just read |nd[k].itm| */
  o,nd[k].itm=j,nd[k].loc=i;
  o,mem[i].i=k;
  if (p>osecond && p<=osecond+verts) { /* a vertex secondary */
    o,q=supp(nd[k].aux)+nd[k].clr-1;
    o,nd[k].aux=q;
    oo,mem[q].i++; /* increase the support counter */
  }
}

@ The domain of the $k$th vertex is represented as a sparse set
(what else?), in the arrays |dom[k]| and |idom[k]|.

@<Initialize the domain of vertex |k|@>=
{
  for (q=0;q<=edges;q++) oo,dom[k][q]=idom[k][q]=q;
  for (o,t=first[k],s=edges+1;t;o,t=nd[t].aux) {
    o,p=supp(nd[t+4].aux); /* |vert[k]| is endpoint number |nd[t+4].aux| */
    for (q=0;q<=edges;q++) if (o,mem[p+q].i==0) {
      o,qq=idom[k][q];
      if (qq<s) { /* we want to remove |q| from |dom[k]| */
        s--;
        o,r=dom[k][s];
        oo,dom[k][qq]=r,idom[k][r]=qq;
        oo,dom[k][s]=q,idom[k][q]=s;
      }
    }
  }
  oo,dsize(vert[k])=s;
  if (vbose&show_domain_details) {
    fprintf(stderr,"Initial domain of");
    print_item_name(vert[k],stderr);
    fprintf(stderr,": ");
    print_domain(k);
  }
}

@ @<Sub...@>=
void print_domain(int k) {
  register int q;
  for (q=0;q<dsize(vert[k]);q++)
    fprintf(stderr,""O"c",encode(dom[k][q]));
  fprintf(stderr,"\n");
}

@ There's a stack of vertices whose domain has changed, but whose
options have not yet been culled to reflect the change. This stack
is maintained as a sparse set, instead of using a ``stamping'' mechanism,
because a vertex might go on and off the stack repeatedly while we're
propagating the changes.

@<Initialize the stack@>=
for (k=0;k<verts;k++) oo,stack[k]=istack[k]=k;

@ @<Report an uncoverable item@>=
{
  if (vbose&show_choices) {
    fprintf(stderr,"Item");
    print_item_name(item[baditem-1],stderr);
    fprintf(stderr," has no options!\n");
  }
}

@ The ``number of entries'' includes spacers (because {\mc DLX2}
includes spacers in its reports). If you want to know the
sum of the option lengths, just subtract the number of options.

@<Report the successful completion of the input phase@>=
fprintf(stderr,
  "("O"lld options, "O"d+"O"d items, "O"d entries successfully read)\n",
                       options,osecond,itemlength-osecond,last_node);

@ The item lengths after input are shown (on request).
But there's little use trying to show them after the process is done,
since they are restored somewhat blindly.
(Failures of the linked-list implementation in {\mc DLX2} could sometimes be
detected by showing the final lengths; but that reasoning no longer applies.)

@<Report the item totals@>=
{
  fprintf(stderr,"Item totals:");
  for (k=0;k<itemlength;k++) {
    if (k==second) fprintf(stderr," |");
    fprintf(stderr," "O"d",size(item[k]));
  }
  fprintf(stderr,"\n");
}

@*The dancing.
Our strategy for generating all exact covers will be to repeatedly
choose always an item that appears to be hardest to cover, namely the
item with smallest set, from all items that still need to be covered.
And we explore all possibilities via depth-first search.

The neat part of this algorithm is the way the sets are maintained.
Depth-first search means last-in-first-out maintenance of data structures;
and the sparse-set representations make it particularly easy to undo
what we've done at less-deep levels.

The basic operation is ``covering an item.'' This means removing it
from the set of items needing to be covered, and ``hiding'' its
options: removing them from the sets of the other items they contain.

@<Solve the problem@>=
{
  o,level=0,saved[0]=saveptr=minsaveptr=memsize;
forward: nodes++;
  if (vbose&show_profile) profile[level]++;
  if (sanity_checking) sanity();
  @<Do special things if enough |mems| have accumulated@>;
  @<Set |best_itm| to the best item for branching@>;
  if (t==max_nodes) @<Visit a solution and |goto backup|@>;
  cur_choice=tough_itm=best_itm; 
  if (t==0) goto abort; /* |t=size(best_itm)| */
  @<Swap |best_itm| out of the active list@>;
  oactive=active,hide(best_itm,0,0); /* hide its options */
  @<Save the currently active set sizes and support counts@>;
advance: oo,cur_node=choice[level]=mem[cur_choice].i;
tryit:@+if ((vbose&show_choices) && level<show_choices_max) {
    fprintf(stderr,"L"O"d:",level);
    print_option(cur_node,stderr);
  }
  @<Swap out all other items of |cur_node|@>;
  stackptr=0;@+@<Hide the other options of those items, or |goto abort|@>;
  if (stackptr && (o,propagate()==0)) goto abort;
  @<Increase |level| and |goto forward|@>;
backup:@+if (level==0) goto done;
  level--;
  o,cur_node=choice[level];
  o,best_itm=nd[cur_node].itm,cur_choice=nd[cur_node].loc;
  goto reconsider;
abort:@+@<Increase the weight of |tough_itm|@>;
reconsider:@+if (o,cur_choice+1>=best_itm+size(best_itm)) goto backup;
  @<Restore the current set sizes and support counts@>;
  cur_choice++;@+goto advance;
}

@ @<Increase |level| and |goto forward|@>=
if (++level>maxl) {
  if (level>=max_level) {
    fprintf(stderr,"Too many levels!\n");
    exit(-4);
  }
  maxl=level;
}
goto forward;

@ We save the sizes of active items on |savestack|, whose entries
have two fields |itm| and |siz|. This stack makes it easy to undo
all deletions, by simply restoring the former sizes and support counts.

@<Glob...@>=
int level; /* number of choices in current partial solution */
int choice[max_level]; /* the node chosen on each level */
int saveptr; /* |savestack| is in |mem[saveptr]| thru |mem[memsize-1]| */
int saved[max_level+1]; /* value of |saveptr| on each level */
int wasactive[max_level+1]; /* value of |active| on each level */
ullng profile[max_level]; /* number of search tree nodes on each level */
int tough_itm; /* an item that led to a conflict */
int vert[max_verts]; /* the vertex items */
int endpt[2*max_edges]; /* the endpoints */
int first[max_verts]; /* the first endpoint of each vertex */
int dom[max_verts][max_edges+1],
   idom[max_verts][max_edges+1]; /* the domains */
int stack[max_verts],istack[max_verts]; /* vertices with unsettled domains */
int stackptr; /* the number of such vertices */

@ @<Do special things if enough |mems| have accumulated@>=
if (delta && (mems>=thresh)) {
  thresh+=delta;
  if (vbose&show_full_state) print_state();
  else print_progress();
}
if (mems>=timeout) {
  fprintf(stderr,"TIMEOUT!\n");@+goto done;
}

@ @<Swap |best_itm| out of the active list@>=
p=active-1,active=p;
o,pp=pos(best_itm);
o,cc=item[p];
oo,item[p]=best_itm,item[pp]=cc;
oo,pos(cc)=pp,pos(best_itm)=p;
updates++;

@ Note that a colored secondary item might have already been purified,
in which case it has already been swapped out. We don't want to
tamper with any of the inactive items.

@<Swap out all other items of |cur_node|@>=
p=oactive=active;
for (q=cur_node+1;q!=cur_node;) {
  o,c=nd[q].itm;
  if (c<0) q+=c;
  else {
    o,pp=pos(c);
    if (pp<p) {
      o,cc=item[--p];
      oo,item[p]=c,item[pp]=cc;
      oo,pos(cc)=pp,pos(c)=p;
      updates++;
    }
    q++;
  }
}
active=p;

@ A secondary item was purified at lower levels if and only if
its position is |>=oactive|.

@<Hide the other options of those items...@>=
stackptr=0;
for (q=cur_node+1;q!=cur_node;) {
  o,cc=nd[q].itm;
  if (cc<0) q+=cc;
  else {
    if (cc<second) {
      tough_itm=cc;
      if (hide(cc,0,1)==0) goto abort;
    }@+else { /* do nothing if |cc| already purified */
      o,pp=pos(cc),tough_itm=0;
      if (pp<oactive && (o,hide(cc,nd[q].clr,1)==0)) goto abort;
    }
    q++;
  }
}

@ The |hide| routine hides all of the incompatible options remaining in the
set of a given item. If |check| is nonzero, it
returns zero if that would cause a primary item to be uncoverable.

If the |color| parameter is zero, all options are incompatible.
Otherwise, however, the given is secondary, and we retain options
for which that item has a |color| match.

When an option is hidden, it leaves all sets except the set of that
given item. And the given item is inactive.
Thus a node is never removed from a set twice.

@<Sub...@>=
int hide(int c,int color,int check) {
  register int cc,s,rr,ss,nn,tt,uu,vv,nnp,xx,yy;
  for (o,rr=c,s=c+size(c);rr<s;rr++) {
    o,tt=mem[rr].i;
    if (!color || (o,nd[tt].clr!=color))
      @<Remove option |tt| from the other sets it's in@>;
  }
  return 1;
}

@ @<Remove option |tt| from the other sets it's in@>=
{
  for (nn=tt+1;nn!=tt;) {
    o,uu=nd[nn].itm,vv=nd[nn].loc;
    if (uu<0) {@+nn+=uu;@+continue;@+}
    if (o,pos(uu)<oactive) {
      o,ss=size(uu)-1;
      if (ss==0 && check && uu<second && (o,pos(uu)<active)) {
        if ((vbose&show_choices) && level<show_choices_max) {
          fprintf(stderr," can't cover");
          print_item_name(uu,stderr);
          fprintf(stderr,"\n");
        }    
        tough_itm=uu;
        return 0;
      }
      o,nnp=mem[uu+ss].i;
      o,size(uu)=ss;
      oo,mem[uu+ss].i=nn,mem[vv].i=nnp;
      oo,nd[nn].loc=uu+ss,nd[nnp].loc=vv;
      updates++;
      if (uu>=second && uu<third && (o,pos(uu)<active))
        @<Decrease a support counter, possibly reducing a domain@>;
    }
    nn++;
  }
}

@ @<Decrease a support counter, possibly reducing a domain@>=
{
  o,xx=nd[nn].aux; /* location of the support counter */
  o,yy=mem[xx].i-1;
  o,mem[xx].i=yy;
  if (yy==0 && check) {
    cc=nd[nn].clr-1; /* no mem charged, since we've fetched |nd[nn].aux| */
    o,vv=nd[nn+2].clr-1;
    @<Remove color |cc| from the domain of vertex |vv|@>;
  }
}

@ @<Remove color |cc| from the domain of vertex |vv|@>=
{
  register int p,d,r;
  o,p=idom[vv][cc];
  if (uu!=vert[vv]) confusion("bad vertex id");
  o,d=dsize(uu);
  if (p<d) { /* yes, |cc| is in the domain and should be swapped out */
    d--;
    if (vbose&show_domain_details) {
      fprintf(stderr,"Domain of");
      print_item_name(uu,stderr);
      fprintf(stderr," loses "O"c, new size "O"d\n",encode(cc),d);
    }
    if (d==0) return 0; /* |tough_itm| may or may not have been set */
    o,r=dom[vv][d];
    oo,dom[vv][p]=r,idom[vv][r]=p;
    oo,dom[vv][d]=cc,idom[vv][cc]=d;
    o,dsize(uu)=d;
    @<Put |vv| on the stack, unless it's already present there@>;
  }
}  

@ @<Put |vv| on the stack, unless it's already present there@>=
o,p=istack[vv];
if (p>=stackptr) {
  o,r=stack[stackptr];
  oo,stack[p]=r,istack[r]=p;
  oo,stack[stackptr]=vv,istack[vv]=stackptr;
  stackptr++;  
}

@ If a weight becomes dangerously large, we rescale all the weights.

@d dangerous 1e32f
@d wmin 1e-30f

@<Increase the weight of |tough_itm|@>=
if (tough_itm) {
  oo,wt(tough_itm)+=dw;
  dw*=dwfactor;
  if (wt(tough_itm)>=dangerous) {
    register int k;
    register float t;
    for (k=0;k<itemlength;k++) if (o,item[k]<second) {
      o,t=wt(item[k])*1e-20f;
      o,wt(item[k])=(t<wmin?wmin:t);
    }
    dw*=1e-20f;
    if (dw<wmin) dw=wmin;
    w0*=1e-20f;
    if (w0<wmin) w0=wmin;
  }
}

@ @<Sub...@>=
void print_weights(void) {
  register int k;
  for (k=0;k<itemlength;k++) if (item[k]<second && wt(item[k])!=w0) {
    print_item_name(item[k],stderr);
    fprintf(stderr," wt "O"f\n",wt(item[k]));
  }
}

@ Here's the final {\it coup de gr\^ ace\/} as far as domains are concerned.
(Sorry: I couldn't resist a bad pun re gracefulness.)

The idea is to semi-purify the options associated with a vertex,
removing some of the colors but not necessarily all of them.
(My previous program {\mc DLX2-GRACEFUL} was able to do this
with doubly linked lists, but only with a rather awkward control
structure. The sparse sets make it all work better.)

It's not as easy to do this as I originally thought! We don't want to swap
two options that both should be deleted, so it's necessary to work from
both ends of the list.

@<Sub...@>=
int propagate(void) {
  register int cc,d,k,p,q,r,s,ss,nn,t,tt,uu,vv,nnp,xx,yy,check=1;
  while (stackptr) {
    @<Pop an item, |k|, off the stack@>;
    o,p=vert[k];
    if (o,pos(p)<active) {
      if (vbose&show_domain_details) {
        fprintf(stderr,"Propagating reductions of");
        print_item_name(p,stderr);
        fprintf(stderr,"\n");
      }
      o,d=dsize(p);
      for (o,r=p,s=p+size(p)-1;r<=s;r++) {
        o,t=mem[r].i;
        o,cc=nd[t].clr-1;
        o,q=idom[k][cc];
        if (q<d) continue;
        for (;s>r;s--) {
          o,tt=mem[s].i;
          o,cc=nd[tt].clr-1;
          o,q=idom[k][cc];
          if (q<d) break;
          @<Remove option |tt| as before from the other sets it's in@>;
        }
        tt=t;
        @<Remove option |tt| as before from the other sets it's in@>;
        s--;
      }
    }
  }
  return 1;
}

@ The stack contains only secondary items that represent vertices.
Popping does not require swapping.

@<Pop an item, |k|, off the stack@>=
o,k=stack[--stackptr];

@ This section is almost identical to the similarly named section in |hide|.
The main difference is that node |tt| itself is also removed.

@<Remove option |tt| as before from the other sets it's in@>=
{
  for (nn=tt+1;;) {
    o,uu=nd[nn].itm,vv=nd[nn].loc;
    if (uu<0) {@+nn+=uu;@+continue;@+}
    if (o,pos(uu)<active) {
      o,ss=size(uu)-1;
      if (ss==0 && uu<second) {
        if ((vbose&show_choices) && level<show_choices_max) {
          fprintf(stderr," can't cover");
          print_item_name(uu,stderr);
          fprintf(stderr,"\n");
        }    
        tough_itm=uu;
        return 0;
      }
      o,nnp=mem[uu+ss].i;
      o,size(uu)=ss;
      oo,mem[uu+ss].i=nn,mem[vv].i=nnp;
      oo,nd[nn].loc=uu+ss,nd[nnp].loc=vv;
      updates++;
      if (uu>=second && uu<third)
        @<Decrease a support counter, possibly reducing a domain@>;
    }
    if (nn==tt) break;
    nn++;
  }
}

@ The ``best item'' is considered to be an item that minimizes the
number of remaining choices, divided by the item's weight.
If there are several candidates, we choose the leftmost.

When an item has at most one option left, however, we consider it
to be forced, and we stop looking for other possibilities.

Sometimes an item has no remaining options. This couldn't happen
in {\mc SSXCC1}; but the present program might choose to branch on
a heavyweight item whose options strictly include all of the remaining
options of a lightweight item. (The heavyweight's options are removed
when the |check| parameter to~|hide| is~0.)

(This program explores the search space in a different order
from {\mc DLX2}, because the ordering of items in the active list
is no longer fixed. Thus ties are broken in a different way.)

@d infty 2e32f /* twice |dangerous| */

@<Set |best_itm| to the best item for branching@>=
{
  register float score,tscore,w;
  register int force;
  score=infty,force=0;
  if ((vbose&show_details) &&
      level<show_choices_max && level>=maxl-show_choices_gap)
    fprintf(stderr,"Level "O"d:",level);
  for (k=0;!force && k<active;k++) if (o,item[k]<second) {
    oo,s=size(item[k]),w=wt(item[k]);
    if ((vbose&show_details) &&
        level<show_choices_max && level>=maxl-show_choices_gap) {
      print_item_name(item[k],stderr);@+
      fprintf(stderr,"("O"d,"O".2f)",s,w);
    }
    if (s<=1) force=1,t=s,best_itm=item[k];
    else {
      tscore=s/w;
      if (tscore>=infty) tscore=dangerous;
      if (tscore<score) best_itm=item[k],score=tscore;
    }
  }
  if (!force) t=(score==infty? max_nodes: size(best_itm));
  if ((vbose&show_details) &&
      level<show_choices_max && level>=maxl-show_choices_gap) {
    if (t==max_nodes) fprintf(stderr," solution\n");
    else {
      fprintf(stderr," branching on");
      print_item_name(best_itm,stderr);@+
      if (t<=1) fprintf(stderr,"(forced)\n");
      else fprintf(stderr,"("O"d), score "O".4f\n",t,score);
    }
  }
  if (t>maxdeg && t<max_nodes) maxdeg=t;
  if (shape_file) {
    if (t==max_nodes) fprintf(shape_file,"sol\n");
    else {
      fprintf(shape_file,""O"d",t);
      print_item_name(best_itm,shape_file);@+
      fprintf(shape_file,"\n");
    }
    fflush(shape_file);
  }
}

@ @<Visit a solution and |goto backup|@>=
{
  count++;
  if (spacing && (count mod spacing==0)) {
    printf(""O"lld:\n",count);
    for (k=0;k<level;k++) print_option(choice[k],stdout);
    fflush(stdout);
  }
  if (count>=maxcount) goto done;
  goto backup;
}

@ We store a |mem| index and its contents, to remember results that will be
restored later. These two |int|s occupy a single octabyte, so only one
mem unit is charged.

@<Save the currently active set sizes and support counts@>=
o,wasactive[level]=active;
for (p=0;p<active;p++) {
  o,q=item[p];
  oo,mem[saveptr-2].i=q-1,mem[saveptr-1].i=size(q),saveptr-=2;
  if (saveptr<minsaveptr) {
    minsaveptr=saveptr;
    if (saveptr<=memlength) {
      fprintf(stderr,"Stack overflow (memsize="O"d)!\n",memsize);
      exit(-5);
    }
  }
  @<If |q| represents a vertex, save its domain info@>;
}
o,saved[level+1]=saveptr;

@ My fancy data structures have obscured the original number of a
vertex, because items are primarily identified by the position of
their cover sets in |mem|. Fortunately, the options are highly redundant,
so that $v$'s number can be recovered from any option that includes `\.{.$v$}'.

@<If |q| represents a vertex, save its domain info@>=
if (q>=second && q<third) {
  oo,r=nd[mem[q].i+2].clr-1; /* |q| represents |vert[r]|! */
  o,j=dsize(q);
  ooo,mem[saveptr-2].i=q-3,mem[saveptr-1].i=j,saveptr-=2;
  for (o,t=first[r];t;o,t=nd[t].aux) {
    o,pp=supp(nd[t+4].aux);
    for (i=0;i<j;i++) {
      o,qq=pp+dom[r][i];
      ooo,mem[saveptr-2].i=qq,mem[saveptr-1].i=mem[qq].i,saveptr-=2;
    }
  }
  if (saveptr<minsaveptr) {
    minsaveptr=saveptr;
    if (saveptr<=memlength) {
      fprintf(stderr,"Stack overflow (memsize="O"d)!\n",memsize);
      exit(-6);
    }
  }
}

@ @<Restore the current set sizes and support counts@>=
o,active=wasactive[level];
oo,saveptr=saved[level+1],p=saved[level];
for (;p>saveptr;p-=2)
  oo,mem[mem[p-2].i].i=mem[p-1].i;

@ @<Sub...@>=
void print_savestack(int start,int stop) {
  register k,p,q,r,v;
  for (k=start&-2;k<=stop;k+=2) {
    p=mem[k].i,v=mem[k+1].i; /* |mem[p]| is to be restored to |v| */
    if (p<sbase) {
      if (mem[p-1].i<itemlength && item[mem[p-1].i]==p+1) {
        print_item_name(p+1,stderr); /* |v| is |size(p+1)| */
        fprintf(stderr,"("O"d)",p+1); /* print also the internal number */
      }@+else if (mem[p+1].i<itemlength && item[mem[p+1].i]==p+3) {
        print_item_name(p+3,stderr); /* |v| is |dsize(p+3)| */
        fprintf(stderr,"("O"d)dom",p+3); /* print also the internal number */
      }@+else fprintf(stderr,""O"d??",p); /* |p| doesn't make sense */
    }@+else {
      q=(p-sbase)/(edges+1);  /* |v| is support count for endpoint |q| */
      r=(p-sbase)mod(edges+1); /* and color |r| */
      fprintf(stderr,""O"d"O"c",q,encode(r));
    }
    fprintf(stderr,", "O"d\n",v);
  }
}

@ @<Sub...@>=
void print_state(void) {
  register int l;
  fprintf(stderr,"Current state (level "O"d):\n",level);
  for (l=0;l<level;l++) {
    print_option(choice[l],stderr);
    if (l>=show_levels_max) {
      fprintf(stderr," ...\n");
      break;
    }
  }
  fprintf(stderr," "O"lld solutions, "O"lld mems, and max level "O"d so far.\n",
                              count,mems,maxl);
}

@ During a long run, it's helpful to have some way to measure progress.
The following routine prints a string that indicates roughly where we
are in the search tree. The string consists of character pairs, separated
by blanks, where each character pair represents a branch of the search
tree. When a node has $d$ descendants and we are working on the $k$th,
the two characters respectively represent $k$ and~$d$ in a simple code;
namely, the values 0, 1, \dots, 61 are denoted by
$$\.0,\ \.1,\ \dots,\ \.9,\ \.a,\ \.b,\ \dots,\ \.z,\ \.A,\ \.B,\ \dots,\.Z.$$
All values greater than 61 are shown as `\.*'. Notice that as computation
proceeds, this string will increase lexicographically.

Following that string, a fractional estimate of total progress is computed,
based on the na{\"\i}ve assumption that the search tree has a uniform
branching structure. If the tree consists
of a single node, this estimate is~.5; otherwise, if the first choice
is `$k$ of~$d$', the estimate is $(k-1)/d$ plus $1/d$ times the
recursively evaluated estimate for the $k$th subtree. (This estimate
might obviously be very misleading, in some cases, but at least it
grows monotonically.)

@<Sub...@>=
void print_progress(void) {
  register int l,k,d,c,p;
  register double f,fd;
  fprintf(stderr," after "O"lld mems: "O"lld sols,",mems,count);
  for (f=0.0,fd=1.0,l=0;l<level;l++) {
    c=nd[choice[l]].itm,d=size(c),k=nd[choice[l]].loc-c+1;
    fd*=d,f+=(k-1)/fd; /* choice |l| is |k| of |d| */
    fprintf(stderr," "O"c"O"c",
      k<10? '0'+k: k<36? 'a'+k-10: k<62? 'A'+k-36: '*',
      d<10? '0'+d: d<36? 'a'+d-10: d<62? 'A'+d-36: '*');
    if (l>=show_levels_max) {
      fprintf(stderr,"...");
      break;
    }
  }
  fprintf(stderr," "O".5f\n",f+0.5/fd);
}

@ @<Print the profile@>=
{
  fprintf(stderr,"Profile:\n");
  for (level=0;level<=maxl;level++)
    fprintf(stderr,""O"3d: "O"lld\n",
                              level,profile[level]);
}

@*Index.
