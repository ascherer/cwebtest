@s ullng int

@*Intro. Here I've cobbled together bits and pieces of five programs that
I've used to decipher the secret 125-character message in
Section 7.2.2.8 of {\sl The Art of Computer Programming}.

I started with the basic program {\mc ENIGMA-BOMBE}. But instead of
outputting its equivalence classes, I transform them into a
set of {\mc SAT} constraints, and use a built-in {\mc SAT} solver
(taken from {\mc SAT0W-ALLSOLS}) to get all of their solutions.
Those solutions can be converted, in turn, to a plugboard description.
Meanwhile, the {\mc ENIGMA-BOMBE} algorithm has given me a
set of rotors, a delta path, and a root position. I can feed those
into the method of {\mc ENIGMA-SETUP}, to get a set of suitable
starting points and ring settings. For each of those, and each plugboard,
I can now use the method of {\mc ENIGMA-TAOCP} to determine the
plaintext corresponding to the secret ciphertext. And finally,
I can use the method of {\mc QUINGRAM-RATING} to decide which
of the plaintexts looks most like English.

If you want to understand all this, please look first at the programs
{\mc SAT0W-ALLSOLS}, {\mc ENIGMA-BOMBE}, {\mc ENIGMA-SETUP}, {\mc ENIGMA-TAOCP},
and {\mc QUINGRAM-RATING}, each of which has been incorporated below
without as much documentation or care as was put into the originals.

The command line consists of a crib and a ciphertext. To solve the
puzzle in the book, the crib is \.{ENIGMATICALLY} and the
the ciphertext is \.{WMGQRYVGT...ZQAEGI}.

@d loc(i,j) ((i)<=(j)? rowadd[i]+(j): rowadd[j]+(i))
@d nn 351 /* number of vertices */
@d maxm 25 /* maximum length of plaintext */
@d maxmc 150 /* maximum length of ciphertext */
@d alftonum(a) ((a)-'A')
@d numtoalf(x) ((x)+'A')

@c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
@<Type definitions@>;
@<Global variables@>;
@<Subroutines@>;
main (int argc,char*argv[]) {
  register int c,i,j,k,kk,m,mc,mk,mp,n,s,u,v;
  register unsigned long long bb;
  register node *p,*q;
  @<Process the command line@>;
  @<Initialize the main data structures@>;
  @<Run through the million possible wheel configurations@>;
  @<Print statistics and say adieu@>;
}

@ @<Global variables@>=
char *rotorname[5]={"I","II","III","IV","V"};
char *rotorperm[5]={@/
 "DBYJRKALSNTVOUPMZEIWCXFHQG",@/
 "DMPSWGCROHXLBUIKTAQJZVEYFN",@/
 "YWUSFHJLNPGTVXBZDRCIMAKEOQ",@/
 "KYHXNBDVJWATSCMRUIELFPZQOG",@/
 "VZBRGITYUPSDNHLXAWMJQOFECK"};
char *reflector="YRUHQSLDPXNGOKMIEBFZCWVJAT"; /* the B reflector */
int perm[3][26]; /* the selected rotor permutations */
int iperm[3][26]; /* their inverses */
int refl[26]; /* the reflector permutation */
int pos[3]; /* positions of slow, middle, and fast rotor */
int off[3]; /* ring rotations of slow, middle, and fast rotor */
int mod26[26+26]; /* maybe this will save time, not dividing by 26 */
int enc[26][26][26][26]; /* encodings for the current rotors */
int rootenc[26];
int rowadd[26]={0,25,49,72,94,115,135,154,172,189,205,220,234,@|
            247,259,270,280,289,297,304,310,315,319,322,324,325};
int plaintext[maxm]; /* the given plaintext, with $\.A\mapsto0$ etc. */
int ciphertext[maxmc]; /* the given ciphertext, with $\.A\mapsto0$ etc. */
int fullswap[maxm][26]; /* the current fullswaps */
int pused[26]; /* how often does this letter occur in the plaintext? */
int used[26]; /* how often does this letter occur in the current texts? */
int rep[nn]; /* representative of equivalence class containing this element */
int size[nn]; /* size of the equivalence class represented by this element */
int bits[nn]; /* the distinct letters used, or $-1$ if not distinct */
int link[nn]; /* link to next element in the same equivalence class */
char name[nn][4]; /* \.{AB} or \.{AC} or \dots\ or \.{YZ} */
int klass[nn],kbits[nn];
ullng konstraint[26];
ullng count; /* this many plaintexts examined */
int r0,r1,r2,p0,p1,p2;

@ @<Process the command line@>=
if (argc!=3) {
  fprintf(stderr,"Usage: %s plaintext ciphertext\n",
                         argv[0]);
  exit(-1);
}
m=strlen(argv[1]),mc=strlen(argv[2]);
plainsize=m,ciphersize=mc;
if (m>maxm || mc>maxmc || m>mc) {
  fprintf(stderr,
   "Sorry: I require |plaintext|<=%d, |plaintext|<=|ciphertext|<=%d!\n",
                 maxm,maxmc);
  exit(-2);
}
for (i=0;i<m;i++) {
  if (argv[1][i]<'A' || argv[1][i]>'Z') {
    fprintf(stderr,"Letters of the plaintext must all be uppercase!\n");
    exit(-3);
  }
  plaintext[i]=alftonum(argv[1][i]);
  pused[plaintext[i]]++;
}
for (i=0;i<mc;i++) {
  if (argv[2][i]<'A' || argv[2][i]>'Z') {
    fprintf(stderr,"Letters of the ciphertext must all be uppercase!\n");
    exit(-3);
  }
  ciphertext[i]=alftonum(argv[2][i]);
}
fprintf(stderr,
  "OK, I'm working on plaintext of length %d and ciphertext of length %d.\n",
      m,mc);

@ @<Initialize the main data structures@>=
for (i=0;i<26;i++) mod26[i]=mod26[i+26]=i;
for (i=0;i<26;i++) refl[i]=reflector[i]-'A';
@<Set up the delta tree for the first 25 moves@>;
for (i=0;i<26;i++) for (j=i;j<26;j++)
  sprintf(name[loc(i,j)],"%c%c",
                  numtoalf(i),numtoalf(j));
@<Input the quingram data@>;

@ We select three of the five rotor wheels in $5\cdot4\cdot3$ ways,
then rotate them into their starting positions in $26\cdot26\cdot26$ ways.
That makes 1054560 ways altogether---roughly a million.

@<Run through the million possible wheel configurations@>=
for (r0=0;r0<5;r0++) {
  k=0,j=r0;
  @<Initialize rotor |k| of type |j|@>;
  for (r1=0;r1<5;r1++) if (r1!=r0) {
    k=1,j=r1;
    @<Initialize rotor |k| of type |j|@>;
    for (r2=0;r2<5;r2++) if (r2!=r0 && r2!=r1) {
      k=2,j=r2;
      @<Initialize rotor |k| of type |j|@>;
      @<Compute the encodings for the current rotors@>;
      for (p0=0;p0<26;p0++)
       for (p1=0;p1<26;p1++)
        for (p2=0;p2<26;p2++) {
          @<Look at variant $(p_0,p_1,p_2,r_0,r_1,r_2)$@>;
        }
    }
  }
}

@ @<Initialize rotor |k| of type |j|@>=
for (i=0;i<26;i++) perm[k][i]=rotorperm[j][i]-'A';
for (i=0;i<26;i++) iperm[k][perm[k][i]]=i;

@ Now we get to the soul of the machine.

@<Encipher |c|@>=
{
  c=mod26[perm[2][mod26[c+off[2]]]+26-off[2]]; /* apply fast rotor perm */
  c=mod26[perm[1][mod26[c+off[1]]]+26-off[1]]; /* apply middle rotor perm */
  c=mod26[perm[0][mod26[c+off[0]]]+26-off[0]]; /* apply slow rotor perm */
  c=refl[c]; /* apply the reflector+26 */
  c=mod26[iperm[0][mod26[c+off[0]]]+26-off[0]]; /* apply inverse of slow rotor perm */
  c=mod26[iperm[1][mod26[c+off[1]]]+26-off[1]]; /* apply inverse of middle rotor perm */
  c=mod26[iperm[2][mod26[c+off[2]]]+26-off[2]]; /* apply inverse of fast rotor perm */
}

@*The delta tree. The first step advances the initial offsets
|(pos[0],pos[1],pos[2])| by $(0,0,1)$ or $(0,1,1)$ or $(1,1,1)$.
The second step divides each of those first two scenarios into
two further possibilities, for a total of five. And in general, exactly
$2k+1$ sequences of ``deltas'' to the initial offsets can arise
during the first $k<26$ steps, based on where we start in the
machine's cycle of length $26\cdot26\cdot25$, because of the
built-in logic. These sequences are conveniently represented in
a tree, whose nodes are \&{node} structs.

@<Type def...@>=
typedef unsigned int uint; /* a convenient abbreviation */
typedef unsigned long long ullng; /* ditto */
typedef struct node_struct {
  struct node_struct *parent,*sibling;
  int del[3];
  int mask,tag; /* utility fields */
} node;

@ @<Global...@>=
node delta[676]; /* $1+3+5+7+\cdots+51=676=26^2$ */
node *level[26];

@ It turns out that exactly one of the nodes on level $k$ has the
deltas $(0,0,k)$; another has $(1,1,k)$; and $k$ of them have $(0,1,k)$.
The deltas of the remaining $k-1$ are all equal to $(1,2,k)$.

@ @d newnode(a,b,c) {
      delta[j].parent=p;
      delta[j].sibling=level[k];
      delta[j].del[0]=a;
      delta[j].del[1]=b;
      delta[j].del[2]=c;
      level[k]=&delta[j];
      j++;
    }

@<Set up the delta tree for the first 25 moves@>=
{
  j=1,k=1,p=&delta[0];
  newnode(0,0,1); newnode(0,1,1); newnode(1,1,1);
  for (k=2;k<26;k++) {
    for (p=level[k-1];p;p=p->sibling) {
      if (p->del[0]==0) {
        if (p->del[1]==0) {
          newnode(0,0,k); newnode(0,1,k);
        }@+else {
          newnode(0,1,k);
          if (p->parent->del[1]==0) newnode(1,2,k);
        }
      }@+else newnode(1,p->del[1],k);
    }
  }
}

@ @<Compute the encodings for the current rotors@>=
for (off[0]=0;off[0]<26;off[0]++)
 for (off[1]=0;off[1]<26;off[1]++)
  for (off[2]=0;off[2]<26;off[2]++)
   for (j=0;j<26;j++) {
     c=j;
     @<Encipher |c|@>;
     enc[off[0]][off[1]][off[2]][j]=c;
   }

@*The problem.
Given the settings of $(p_0,p_1,p_2,r_0,r_1,r_2)$ and a node at level $m-1$
in the delta tree, we can generate $m$ fullswaps that correspond to a
possible Enigma setup. The Bombe algorithm is then run on each of the
length-$m$ substrings of the given ciphertex.

@<Look at variant $(p_0,p_1,p_2,r_0,r_1,r_2)$@>=
{
  for (p=level[m-1];p;p=p->sibling) {
    for (k=m-1,q=p;k>=0;k--,q=q->parent)
      for (j=0;j<26;j++) fullswap[k][j]=
                       enc[mod26[p0+q->del[0]]][mod26[p1+q->del[1]]]
                                     [mod26[p2+q->del[2]]][j];
    for (kk=0;kk+m<=mc;kk++) {
      @<If |plaintext| overlaps substring |kk| of |ciphertext|, |continue|@>;
      for (i=0;i<26;i++) used[i]=pused[i];
      for (i=0;i<m;i++) used[ciphertext[kk+i]]++;
      @<Apply the Bombe algorithm to the current texts and fullswaps@>;
    }
  }
}

@ @<If |plaintext| overlaps substring |kk| of |ciphertext|, |continue|@>=
for (i=0;i<m;i++) if (plaintext[i]==ciphertext[kk+i]) break;
if (i<m) continue;

@ @<Apply the Bombe algorithm to the current texts and fullswaps@>=
@<Make the graph initially empty@>;
for (k=0;k<m;k++) @<Add edges for the $k$th step of encipherment@>;
@<If a tentative solution was found, find its giant forced class@>;

@ In my first draft of this program, which is now called
{\mc ENIGMA-BOMBE-TOY}, I set |size[loc(i,j)]=0| when
both |i| and |j| were unused. However, I can't justify that in general;
the plugboard is used for the whole ciphertext, not just for the crib.

@<Make the graph initially empty@>=
for (v=0;v<nn;v++) rep[v]=link[v]=v,size[v]=1;
for (i=0;i<26;i++) for (j=i;j<26;j++) {
  bits[loc(i,j)]=(1<<i)|(1<<j);
}

@ @<Add edges for the $k$th step of encipherment@>=
for (i=0;i<26;i++) {
  u=loc(plaintext[k],i);
  v=loc(ciphertext[kk+k],fullswap[k][i]);
  yewnion(u,v);
}

@ @<Sub...@>=
int yewnion(int u,int v) {
  register int s,t,p;
  s=rep[u],t=rep[v];
  if (s!=t) {
    if (size[s]<size[t]) s=rep[v],t=rep[u];
    rep[t]=s;
    for (p=link[t];p!=t;p=link[p]) rep[p]=s;
    size[s]+=size[t];
    if (bits[s]&bits[t]) bits[s]=-1;
    else bits[s]+=bits[t];
    p=link[s],link[s]=link[t],link[t]=p;
  }
  return s;
}

@ @<Sub...@>=
void printclass(int s,FILE *stream) {
  register int p;
  fprintf(stream,"{%s",
                   name[s]);
  for (p=link[s];p!=s;p=link[p]) fprintf(stream,",%s",
                                           name[p]);
  fprintf(stream,"}");
}
@#
void pclass(int s) { /* for use in debugging */
  printclass(s,stderr);
  if (bits[rep[s]]>=0) fprintf(stderr,"\n");
  else fprintf(stderr,"##\n");
}
@#
void printclasses(void) { /* ditto */
  register int k;
  for (k=0;k<nn;k++) if (rep[k]==k && bits[k]>=0)
    printclass(k,stderr);
  fprintf(stderr,"\n");
}

@ Here's a routine that shows what letters are currently
feasible partners for a given letter.

@<Sub...@>=
void printmates(int i,FILE *stream) {
  register int j;
  for (j=0;j<26;j++) if (bits[rep[loc(i,j)]]>=0)
    fprintf(stream,"%c",
                   numtoalf(j));
  fprintf(stream,"\n");
}
@#
void mates(int i) {
  printmates(i,stderr);
}

@*An improved filter.
At this point, we've found all the equivalence classes defined by the Bombe
algorithm. Every letter~$i$ used in the plaintext or in the subciphertext
must now match at least one vertex~$ij$ that's in a ``good'' class,
namely a class whose pairs are disjoint. Such classes are distinguished
by having |bits>=0|, and they're rather rare in practice. Thus we
usually find that at least one of the used letters has been wiped out
of all the good classes. And in such cases, we're done, because the
Enigma cannot produce the desired ciphertext from the current
|(p0,p1,p2,r0,r1,r2,p)|; no suitable plugboard pairs exist.

But we can do better than that. (Indeed, we must; otherwise there will be
too many spurious ``solutions.'') Fortunately, the very first tentative solution
acceptable to the Bombe logic on the author's first draft of this program
could, in fact, be ruled out easily: The used letter \.A belonged to only
one good class, and that class contained the pair \.{AU}; therefore
\.A and \.U had to be paired in the Enigma's plugboard. Another used
letter, \.B, also belonged to only one good class; and that class contained
the pair \.{BU}, a contradiction.

Suppose $i$ is a used letter for which exactly one good class contains
a pair~$ij$; then all pairs in that class must be plugged together.
We call this a {\it forced class}.
All forced classes can be merged together into a single class, and
the pairs in this giant forced class must be distinct.

@<If a tentative solution was found, find its giant forced class@>=
{
  register int giant,hit;
  for (giant=-1,i=0;i<26;i++) if (used[i]) {
    for (c=j=0;j<26;j++) if (bits[rep[loc(i,j)]]>=0) c++,hit=rep[loc(i,j)];
    if (c==0) goto done; /* not a solution because $i$ is wiped out */
    if (c==1) { /* |hit| is a forced class */
      if (giant<0) giant=hit;
      else if (giant!=hit) {
        giant=yewnion(giant,hit);
        if (bits[giant]<0) goto done;
      }
    }
  }
  if (giant>=0) @<Use the giant class for further pruning, if possible@>;
  @<Visit the solution@>;
}
done:

@ Every class other than the giant is viable only if its pairs
are disjoint from the ones that are forced. Hence we can kill
any class that intersects the giant; and that might cause other
classes to become forced. We continue this process until we've
either found a contradiction or achieved stability.

@<Use the giant class for further pruning, if possible@>=
while (1) {
  register int change;
  for (change=k=0;k<nn;k++)
    if (rep[k]==k &&  bits[k]>=0 && k!=giant && (bits[k]&bits[giant]))
      change=1,bits[k]=-1;
  if (!change) break;
  for (i=0;i<26;i++) if (used[i] && ((1<<i)&bits[giant])==0) {
    for (c=j=0;j<26;j++) if (bits[rep[loc(i,j)]]>=0) c++,hit=rep[loc(i,j)];
    if (c==0) goto done; /* not a solution because $i$ is wiped out */
    if (c==1) { /* |hit| is a forced class */
      change=1,giant=yewnion(giant,hit);
      if (bits[giant]<0) goto done;
    }
  }
  if (!change) break;
}

@*Visitation. Hey, we may have found a configuration for which a plugboard
exists. We'll feed it to a {\mc SAT} solver to make sure.

@<Visit the solution@>=
cases++;
for (n=k=0;k<nn;k++) if (rep[k]==k && bits[k]>=0) {
  klass[n]=k,kbits[n]=bits[k],n++;
  if (n>=varsmax) varsmax=n;
}
@<Prepare the {\mc SAT} constraints@>@;
@<Launch the solver@>;
if (!sols) {
  fails++;
  if (n>64) hardfails++;
}@+else @<Investigate each candidate plaintext
                for each plugboard and ring setting@>;

@ The {\mc SAT} constraints are based on a simple idea. We've got
viable equivalence classes $C_0$, \dots, $C_{n-1}$, and they correspond
to Boolean variables $x_0$, \dots, $x_{n-1}$. There's an at-most-one constraint
$(\bar x_j\lor\bar x_k)$ whenever $C_j$ and $C_k$ have a letter in
common (i.e., whenever |kbits[j]&kbits[k]| is nonzero).
And for each letter $l$, there's an at-least-one constraint
$\bigl(\bigvee\{x_j\mid l\in C_j\}\bigr)$.

In practice many of the at-least-one constraints subsume others.
So we attempt to produce only minimal ones.

@ @<Prepare the {\mc SAT} constraints@>=
vars=n,clauses=nonspec=n+n+2,cells=sols=0;
for (j=0;j<n;j++) for (k=j+1;k<n;k++) if (kbits[j]&kbits[k])
  @<Make a binary constraint $(\bar x_j\lor\bar x_k)$@>;
if (n>64) @<Make the at-least-one constraints by brute force@>@;
else {
  for (mk=i=0;i<26;i++) {
    for (k=0,bb=0LL;k<n;k++) if ((1<<i)&kbits[k]) bb+=1LL<<k;
    for (k=0;k<mk;k++) {
      if ((konstraint[k]&bb)==konstraint[k]) break; /* |bb| is subsumed */
      if ((konstraint[k]&bb)==bb) { /* |bb| subsumes a previous one */
        mk--, konstraint[k]=konstraint[mk]; /* delete it, replace by last */
        k--; /* don't advance |k|; test it against the new |mk| */
      }
    }
    if (k==mk) konstraint[mk++]=bb;
  }
  for (j=0;j<mk;j++) @<Make the at-least-one constraint for |konstraint[j]|@>;
}

@ I thought the number of viable classes would never exceed 64,
based on early tests. So it was appealing to economize, removing
subsumptions as above, by using fullword bitstrings as above.

But one out every 1500 or so cases turned out to be an exception.
In such cases we can afford to be inefficient.

@<Make the at-least-one constraints by brute force@>=
{
  hardcases++;
  for (i=0;i<26;i++) {
    @<Get ready for a new at-least-one constraint@>;
    for (k=0;k<n;k++) if ((1<<i)&kbits[k])
      @<Add |k| to that new constraint@>;  
    @<Complete the new at-least-one constraint@>;
  }
}

@*SAT solving. The following code was hacked from my old program
{\mc SAT0W}, but vastly simplified.
(We know, for example, that there are at most 64 variables,
at most $64\choose2$ at-most-one clauses, and at most
26 at-least-one clauses.) That program implements Algorithm 7.2.2.2B.

@ The basic unit in our data structure is called a cell. There's one
cell for each literal in each clause. This stripped-down version
of {\mc SAT0} doesn't really need a special data type for cells,
but I've kept it anyway.

Each link is a 32-bit integer. (I don't use \CEE/ pointers in the main
data structures, because they occupy 64 bits and clutter up the caches.)
The integer is an index into a monolithic array of cells called |mem|.

@<Type...@>=
typedef struct {
  uint litno; /* literal number */
} cell;

@ Each clause is represented by a pointer to its first cell, which
contains its watched literal. There's also a pointer to another clause
that has the same watched literal.

Clauses are stored in natural order: The cells of clause |c| run
from |cmem[c].start| to |cmem[c+1].start-1|.
(By constrast, {\mc SAT0W} did it backwards.)

The first $2n+2$ ``clauses'' are special; they serve as list heads for
watch lists of each literal.

@<Type...@>=
typedef struct {
  uint start; /* the address in |mem| where the cells for this clause start */
  uint wlink; /* link for the watch list */
} clause;

@ @d memsize 4000 /* this should be more than enough */
@d clausesize 6000 /* ditto */
@d maxsols 10000

@<Glob...@>=
cell mem[memsize]; /* the master array of cells */
clause cmem[clausesize]; /* the master array of clauses */
uint nonspec; /* address in |cmem| of the first non-special clause */
char move[64]; /* the stack of choices made so far */
int vars,clauses,cells,sols;
int plugs[maxsols][26];
int solsmax,cellsmax,clausesmax;
int cases,fails,hardcases,hardfails;

@ Here is a subroutine that prints a clause symbolically. It illustrates
some of the conventions of the data structures that have been explained above.
I use it only for debugging. The variables are named \.{x0}, \.{x1}, etc.,
although in the solver they're numbered 1, 2, etc.

@<Sub...@>=
void print_clause(uint c) {
  register uint k,l;
  printf("%d:",
           c); /* show the clause number */
  for (k=cmem[c].start;k<cmem[c+1].start;k++) {
    l=mem[k].litno;
    printf(" %sx%d",
      l&1? "~": "", (l>>1)-1); /* $k$th literal */
  }
  printf("\n");
}

@ Similarly we can print out all of the clauses that watch
a particular literal.

@<Sub...@>=
void print_clauses_watching(int l) {
  register uint p;
  for (p=cmem[l].wlink;p;p=cmem[p].wlink)
    print_clause(p);
}

@ I'm hoping that verbose printing won't be necessary, and that
this simple routine to show the current state of backtracking will
suffice for debugging.

@<Sub...@>=
void print_state(int l) {
  register int k;
  for (k=1;k<=l;k++) fprintf(stderr,"%c",move[k]+'0');
  fprintf(stderr,"\n");
  fflush(stderr);
}

@ @<Make a binary constraint $(\bar x_j\lor\bar x_k)$@>=
{
  cmem[clauses].wlink=cmem[j+j+3].wlink;
  cmem[j+j+3].wlink=clauses;
  mem[cells++].litno=j+j+3; /* $\bar x_j$ */
  mem[cells++].litno=k+k+3; /* $\bar x_k$ */
  cmem[++clauses].start=cells;
}

@ @<Make the at-least-one constraint for |konstraint[j]|@>=
{
  i=cells;
  for (k=0,bb=1LL;bb && bb<=konstraint[j];k++,bb<<=1)
    if (bb&konstraint[j]) mem[cells++].litno=k+k+2; /* $x_k$ */
  i=mem[i].litno; /* the watched literal was stored there */
  cmem[clauses].wlink=cmem[i].wlink, cmem[i].wlink=clauses;
  cmem[++clauses].start=cells;
}

@ @<Get ready for a new at-least-one constraint@>=
s=cells;

@ @<Add |k| to that new constraint@>=
mem[cells++].litno=k+k+2;
s=mem[s].litno; /* the watched literal was stored there */
cmem[clauses].wlink=cmem[s].wlink, cmem[s].wlink=clauses;
cmem[++clauses].start=cells;

@ @<Complete the new at-least-one constraint@>=


@*Doing it. Now comes ye olde basic backtrack.

A choice is recorded in the |move| array, as the number 0 if we're
trying first to set the current variable true;
it is 3 if that move failed and we're trying the other alternative.

@<Launch the solver@>=
{
  register uint c,h,i,j,k,l,p,q,r,level,parity;
  level=1; /* I used to start at level 0, but Algorithm 7.2.2.2B does this */
newlevel: if (level>vars) {
    @<Visit the {\mc SAT} solution found@>;
    goto backtrack;
  }
  move[level]=(cmem[level+level+1].wlink!=0 || cmem[level+level].wlink==0);
tryit: parity=move[level]&1;
  @<Make variable |level| non-watched by the clauses in the non-chosen list;
    |goto try_again| if that would make a clause empty@>;
  level++;@+goto newlevel;
try_again:@+if (move[level]<2) {
    move[level]=3-move[level];
    goto tryit;
  }
backtrack:@+if (level>1) @<Backtrack to the previous level@>;
  @<Clear |cmem|@>;
}

@ @<Make variable |level|...@>=
for (c=cmem[level+level+1-parity].wlink;c;c=q) {
  i=cmem[c].start,q=cmem[c].wlink,j=cmem[c+1].start;
  for (p=i+1;p<j;p++) {
    k=mem[p].litno;
    if (k>=level+level || (((move[k>>1]^k)&1)==0)) break;
  }
  if (p==j) { /* clause |c| contradicted */
    cmem[level+level+1-parity].wlink=c;
    goto try_again;
  }
  mem[i].litno=k,mem[p].litno=level+level+1-parity;
  cmem[c].wlink=cmem[k].wlink,cmem[k].wlink=c;
     /* clause |c| now watches |k>>1| */
}
cmem[level+level+1-parity].wlink=0;

@ @<Backtrack to the previous level@>=
{
  level--;
  goto try_again;
}

@ When the {\mc SAT} solver finds a solution, the corresponding plugboard
pairs are the variables in classes |klass[k]| for all $k$ with $x_k$ true.
That set of pairs is recorded in the array |plugs[sols]|.

@<Visit the {\mc SAT} solution found@>=
for (j=0,k=1;k<level;k++) if ((move[k]&1)==0) { /* $x_{k-1}$ is true */
  plugs[sols][j++]=klass[k-1];
  for (p=link[klass[k-1]];p!=klass[k-1];p=link[p]) plugs[sols][j++]=p;
  plugs[sols][j]=-1; /* sentinel */
}
if (++sols>solsmax) {
  if (sols>=maxsols) {
    fprintf(stderr,"Overflow: too many SAT solutions!\n");
    exit(-5);
  }
  solsmax=sols;
}
if (cells>cellsmax) cellsmax=cells;
if (clauses>clausesmax) clausesmax=clauses;

@ After finding all the solutions, we must erase our tracks, so that
the solver will be ready for another task later.
This means the |wlink| fields must be zeroed.
And (oops!) also the |start| fields, because the program assumes
that |c[nonspec].start| is zero.

Also, oops oops, it's important to erase also |cmem[clauses]|,
whose |start| field is nonzero even though there's no such clause.

@<Clear |cmem|@>=
for (c=0;c<=clauses;c++) cmem[c].start=cmem[c].wlink=0;

@*Setting up the start positions and ring settings.
Yippee, progress continues: We've found at least one plugboard for the delta
path that leads from $p$ to the Enigma root setting
|(p0,p1,p2)|, at prefix~|kk|.

Our current task is therefore to find all setups for which Enigma will
reach that root setting and follow the reverse of that delta path at the
proper time. The $k$th setup will be stored in
|startpos[k]| and |rings[k]|.

I hacked this code from {\mc ENIGMA-SETUP}, where the logic was the
same but the intermediate representation was totally different.

@d maxsetups 500

@<Glob...@>=
int startpos[maxsetups][4], rings[maxsetups][4];
int start[3],now[3],root[3];
int plainsize,ciphersize,prefix;
int setups;
int setupsmax,solsbysetupsmax,varsmax; /* record highs */

@ @<Make a list of the possible setups@>=
setups=0,prefix=kk;
if (p->del[0]) @<Do the easy case@>@;
else if (p->del[1]) @<Do the medium case@>@;
else @<Do the hard case@>;
if (sols*setups>solsbysetupsmax) solsbysetupsmax=sols*setups;

@ In the easy case, we know the exact position of the big carry
(where all three rotors advance).

@<Do the easy case@>=
{
  if (p->del[1]==1) outbig(prefix+1);
  else {
    for (q=p->parent; q->del[0]; q=q->parent) ;
    outbig(prefix+1+q->del[2]);
  }
}

@ This subroutine sets Enigma to make a big carry at the
correct time.

@d modulo26(x) ((x)%26)

@<Sub...@>=
void outbig(int bigcarry) {
  register int i,carry;
  carry=modulo26(bigcarry-1);
  start[2]=25-carry;
  start[1]=24-(bigcarry-1)/26;
  now[0]=(bigcarry<=prefix);
  now[2]=modulo26(start[2]+prefix+1);
  now[1]=start[1]+(start[2]+prefix+1)/26-25*now[0];
  for (i=0;i<3;i++) {
    startpos[setups][i]=start[i],
    rings[setups][i]=modulo26(now[i]+26-root[i]);
  }
  if (++setups>setupsmax) {
    if (setups>=maxsetups) {
      fprintf(stderr,"Overflow: Too many setups per configuration!\n");
      exit(-777);
    }
    setupsmax=setups;
  }
}

@ This subroutine sets Enigma to make carries at the proper times,
mod~26, and never to make a big carry (unless the ciphertext has
more than 600 characters).

@<Sub...@>=
void outnobig(int carry) {
  register int i;
  start[2]=25-carry;
  start[1]=0;
  now[2]=modulo26(start[2]+prefix+1);
  now[1]=(start[2]+prefix+1)/26;
  now[0]=0;
  for (i=0;i<3;i++) {
    startpos[setups][i]=start[i],
    rings[setups][i]=modulo26(now[i]+26-root[i]);
  }
  if (++setups>setupsmax) {
    if (setups>=maxsetups) {
      fprintf(stderr,"Overflow: Too many setups per configuration!\n");
      exit(-778);
    }
    setupsmax=setups;
  }
}

@ @<Do the medium case@>=
{
  for (q=p->parent;q->del[1];q=q->parent) ;
  outmedium(modulo26(prefix+1+q->del[2]));
}

@ @<Sub...@>=
void outmedium(int carry) {
  register int bigcarry;
  outnobig(carry);
  for (bigcarry=carry+1;bigcarry<ciphersize; bigcarry+=26) 
    if (bigcarry<=prefix || bigcarry>prefix+plainsize) outbig(bigcarry);
}

@ In the remaining case, we want to set up for every case that does
neither a carry nor a big carry while encrypting the crib.

@<Do the hard case@>=
{
  for (k=modulo26(prefix+plainsize);k!=modulo26(prefix+1);k=modulo26(k+1))
    outmedium(k);
}

@ @<Investigate each candidate plaintext for each plugboard and ring setting@>=
{
  root[0]=p0,root[1]=p1,root[2]=p2;
  @<Make a list of the possible setups@>;
  for (i=0;i<setups;i++) for (j=0;j<sols;j++)
    @<Investigate the ciphertext from setup |i| and solution |j|@>;
}

@*Rating a candidate's approximation to English.
The code here has been imported (and hacked) from {\mc ENIGMA-TAOCP}
and {\mc QUINGRAM-RATING}.

@d quincount_file_name "vol1-quingrams.dat"
@d maxscore 10000

@<Glob...@>=
FILE *quinscores;
int i0,i1,i2,i3,i4,rawdata;
int score[26][26][26][26][26];
int bestscore;
int plntxt[maxmc];
int plugboard[26];
int tally[maxscore];

@ @<Input the quingram data@>=
quinscores=fopen(quincount_file_name,"r");
if (!quinscores) {
  fprintf(stderr,"I can't open file `%s' for reading!\n",
                        quincount_file_name);
  exit(-1);
}
for (i0=0;i0<26;i0++)
 for (i1=0;i1<26;i1++)
  for (i2=0;i2<26;i2++)
   for (i3=0;i3<26;i3++)
    for (i4=0;i4<26;i4++) {
      if (fscanf(quinscores,"%d\n",
                  &rawdata)!=1) {
      fprintf(stderr,"trouble reading the raw data for %c%c%c%c%c!\n",
                         i0+'A',i1+'A',i2+'A',i3+'A',i4+'A');
      exit(-6);
    }
    score[i0][i1][i2][i3][i4]=rawdata;
  }

@ So here we are at the end of the pipeline, ready to see if that mysterious
ciphertext will be made plain by |startpos[i]| and |rings[i]| and |plugs[j]|,
using Enigma's current rotors. With bated breath and fingers crossed,
we put the candidate plaintext into |plntxt|.

@<Investigate the ciphertext from setup |i| and solution |j|@>=
{
  count++;
  for (k=0;k<3;k++) pos[k]=startpos[i][k], off[k]=mod26[pos[k]+26-rings[i][k]];
  @<Set |plugboard| from |plugs[j]|@>;
  for (k=0;k<ciphersize;k++) {
    c=ciphertext[k];
    @<Advance the rotors@>;
    c=plugboard[c];
    @<Encipher |c|@>;
    c=plugboard[c];
    plntxt[k]=c;
  }    
  @<Doublecheck that |plaintext| is where we thought it would be@>;
  @<Compute the score...@>;
  if (count%250000==0)
    fprintf(stderr,"... %lld plaintexts tried so far; %s %s %s %c%c%c\n",
         count,@)@>
         rotorname[r0],rotorname[r1],rotorname[r2],p0+'A',p1+'A',p2+'A');
}

@ @<Set |plugboard| from |plugs[j]|@>=
for (k=0;plugs[j][k]>=0;k++) {
  u=name[plugs[j][k]][0]-'A';
  v=name[plugs[j][k]][1]-'A';
  plugboard[u]=v;
  plugboard[v]=u;
}

@ @<Advance the rotors@>=
if (pos[1]==25) goto up012;
if (pos[2]==25) goto up12;
goto up2;
up012: pos[0]=mod26[pos[0]+1], off[0]=mod26[off[0]+1];
up12: pos[1]=mod26[pos[1]+1], off[1]=mod26[off[1]+1];
up2: pos[2]=mod26[pos[2]+1], off[2]=mod26[off[2]+1];

@ @<Doublecheck that |plaintext| is where we thought it would be@>=
for (k=0;k<plainsize;k++) if (plntxt[k+prefix]!=plaintext[k])
  fprintf(stderr,"Ouch, I blew it.\n");

@ @<Compute the score of |plntxt| and possibly output it@>=
for (s=0,k=4;k<ciphersize;k++) s+=
  score[plntxt[k-4]][plntxt[k-3]][plntxt[k-2]][plntxt[k-1]][plntxt[k]];
tally[s]++;
if (s>=bestscore || s>=6000) {
  for (k=0;k<ciphersize;k++) printf("%c",
                 plntxt[k]+'A');
  printf(" %s %s %s %c%c%c %c%c%c",
         rotorname[r0],rotorname[r1],rotorname[r2],
         startpos[i][0]+'A',startpos[i][1]+'A',startpos[i][2]+'A',         
            rings[i][0]+'A',   rings[i][1]+'A',   rings[i][2]+'A'); 
  @<Print the plugboard settings...@>;
  printf(" (%c%c%c) %6d %lld\n",
         p0+'A',p1+'A',p2+'A',s,count);
  bestscore=s;
}

@ @<Print the plugboard settings of |plugs[j]|@>=
for (k=0;plugs[j][k]>=0;k++) if (name[plugs[j][k]][0]!=name[plugs[j][k]][1])
  printf(" %s",
      name[plugs[j][k]]);

@ @<Print statistics and say adieu@>=
for (s=0;s<maxscore;s++) if (tally[s])
  printf("%10d:%10d\n",
               s,tally[s]);
fprintf(stderr,"Altogether %lld plaintexts examined;\n",
           count);
fprintf(stderr," %d of %d cases had no solutions, including %d of %d hard;\n",
                    fails,cases,hardfails,hardcases);
fprintf(stderr,"max vars %d, max clauses %d, max cells %d,\n",
                    varsmax,clausesmax,cellsmax);
fprintf(stderr," max sols %d, max setups %d, max sols*setups %d.\n",
                    solsmax,setupsmax,solsbysetupsmax);

@*Index.
