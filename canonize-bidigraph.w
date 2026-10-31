\let\swap=\leftrightarrow
@i gb_types.w

@*Intro. Given a bidirected graph~$B$, this program outputs an equivalent
bidirected graph~$[B]$ with the property that $[B]=[B']$ if and only if
$B'$ is obtained from $B$ by ``complementing'' some subset of the
vertices.

(A bidirected graph is represented as in other programs such as
{\mc SSBIDIHAM}; please see that program for the basic definitions.
Complementing a vertex means to swap $v^-\!\swap v^+$
in the undirected graph $G(B)$ that corresponds to~$B$.
Complementation is of interest because it preserves the essential path
structure of~$B$.)

\def\0#1#2{\mathrel{\.{#1#2}}}
Every pair of distinct vertices $v$ and $w$ in a bidirected graph
is related in one of 16 different ways, depending on which of the
edges $v\0<<w$, $v\0>>w$, $v\0<>w$, $v\0><w$ are present.
(And there are 8 possible ``loops'' between a vertex and itself,
because $v\0<<v$ is the same as $v\0>>v$.)

Internally we represent a bidirected graph on $n$ vertices by an $n\times n$
adjacency matrix $a$, where $a[v][w]$ is a four-bit code. For example,
$a[v][w]=(1000)_2$ means that $v\0<<w$ but we don't have
$v\0>>w$ or $v\0<>w$ or $v\0><w$;
$a[v][w]=(0101)_2$ means that the only edges between $v$ and~$w$ are
$v\0>>w$ and $v\0><w$.

Suppose the four bits of $a[v][w]$ are $\it pqrs$. The operation of
complementing $v$ changes those bits to $\it srqp$; the operation of
complementing $w$ changes them to $\it rspq$; and the operation of
complementing both changes them to $\it qpsr$.

Our canonical form complements the variables in a way that maximizes
the bitstrings on and above the diagonal of the adjacency matrix.
More precisely, it maximizes the lexicographic order of the
sequence of $(n+1)n/2$ four-bit codes
$$(a[1][1],\ldots, a[1][n]; a[2][2],\ldots,a[2][n]; \ldots, a[n][n]).$$

@d maxn 100

@c
#include <stdio.h>
#include <stdlib.h>
#include "gb_graph.h"
#include "gb_save.h"
int a[maxn][maxn]; /* adjacency matrix */
int link[maxn]; /* |j|, if |vert(k)|'s sign depends on |vert(j)|; else |k| */
int sign[maxn]; /* the sign of |vert(k)|, or zero, if |link[k]=k|;
                        else multiplier of the sign */
Graph *g,*gg; /* the input and output graphs */
@<Subroutines@>;
int main(int argc,char*argv[]) {
  register int i,j,k,jj,kk,n,t,c00,c01,c10,c11,cmax,code,edoc,cands;
  register Arc *aa;
  register Vertex *v, *w;
  @<Process the command line@>;
  @<Set up the adjacency matrix@>;
  @<Determine canonical signs@>;
  @<Output the canonical graph@>;
}

@ I'm not concerned about efficiency today. So I'll try to check my assumptions
in order to avoid careless errors.

@<Sub...@>=
void check(int assertion) {
  if (!assertion) {
    fprintf(stderr,"Oops, I goofed.\n");
    exit(-66);
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
  fprintf(stderr,"Sorry, graph %s has too many vertices (%d>=%d)!\n",
                              argv[1],n,maxn);
  exit(-2);
}

@ @d vert(k) (g->vertices+(k))
@d inx(v) ((v)-g->vertices)

@<Set up the adjacency matrix@>=
for (k=0;k<n;k++) {
  v=vert(k);
  for (aa=v->arcs;aa;aa=aa->next) {
    w=aa->tip, j=inx(w);
    switch (aa->len & 0x3) {
case 0: code=edoc=0x2;@+break;      /* $v\0<>w$ and $w\0<>v$ */
case 1: code=0x8,edoc=0x4;@+break;  /* $v\0<<w$ and $w\0>>v$ */
case 2: code=edoc=0x1;@+break;      /* $v\0><w$ and $w\0><v$ */
case 3: code=0x4,edoc=0x8;@+break;  /* $v\0>>w$ and $w\0<<v$ */   
    }
    a[k][j]|=code, a[j][k]|=edoc;
  }
}

@ The |ccode| routine changes a 4-bit |code| for |a[v][w]| by complementing~|v|
if $l=1$, complementing~|w| if $r=1$.

A little bit of bitwise fun.

@<Sub...@>=
int ccode(int code,int l,int r) {
  register int c=code;
  if (l) c=c^(((c^(c>>1))&0x5)*0x3);
  if (l!=r) c=c^(((c^(c>>2))&0x3)*0x5);
  return c;
}

@ As we march through the adjacency matrix, we determine the lexicographically
largest equivalent at each step, accumulating constraints on the signs as we go.
Each constraint is simple: Either it adds no restrictions at all, or
it forces a vertex to have a certain sign, or it forces a vertex to
have $\pm$ the sign of another vertex. Thus, a simple variant of the
well-known ``union-find'' data structure, represented by the |link|
and |sign| arrays defined above, suffices to represent the
current set of constraints.

@ @<Initialize the data structures@>=
for (k=0;k<n;k++) link[k]=k, sign[k]=0;

@ Let |k| be the representative of the current equivalence class
for |vert(k)|. The |lookup| subroutine returns |k| if the current sign of
|vert(j)| is |sign[k]|; otherwise it returns |~k|.

@<Sub...@>=
int lookup(int j) {
  register int k,s;
  for (k=j,s=+1; link[k]!=k; s*=sign[k],k=link[k]) ;
  return (s>0? k: ~k);
}

@ The |compat| subroutine tells us whether the current constraints allow
the sign of |vert(j)| to be~|s| and, simultaneously,
the sign of |vert(k)| to be~|t|.

@<Sub...@>=
int compat(int j, int s, int k, int t) {
  register int jj=lookup(j), kk=lookup(k);
  if (jj<0) s=-s, jj=~jj;
  if (kk<0) t=-t, kk=~kk;
  if (jj==kk) return (s==t) && (s!=-sign[jj]);
      /* |vert(j)| and |vert(k)| are dependent */
  else return (s!=-sign[jj]) && (t!=-sign[kk]);
}

@ Here's how we freeze the sign of |vert(j)|.

@<Sub...@>=
void fix(int j,int s) {
  register int jj=lookup(j);
  if (jj>=0) {
    if (!sign[jj]) {
      fprintf(stderr,"sign(%s)=%c\n",
                         vert(jj)->name,s>0?'+':'-'@t)@>);
      sign[jj]=s;
    }@+else check(sign[jj]==s);
  }@+else {
    if (!sign[~jj]) {
      fprintf(stderr,"sign(%s)=%c\n",
                         vert(~jj)->name,s>0?'-':'+'@t)@>);
      sign[~jj]=-s;
    }@+else check(sign[~jj]==-s);
  }
}

@ And here's how we make |vert(j)| depend on |vert(k)|.

@<Sub...@>=
void depend(int j,int k,int s) {
  register int jj=lookup(j),kk=lookup(k);
  if (jj==kk) { /* already dependent */
    check(s>0);
    return;
  }
  if (jj==~kk) { /* already antidependent */
    check(s<0);
    return;
  }
  if (jj<0) jj=~jj, s=-s;
  if (kk<0) kk=~kk, s=-s;
  if (!sign[jj]) {
    if (!sign[kk]) {
      link[jj]=kk, sign[jj]=s;
      fprintf(stderr,"sign(%s)=%csign(%s)\n",
                        vert(jj)->name,s>0?'+':'-',vert(kk)->name@t)@>);
    }@+else fix(jj,s*sign[kk]);
  }@+else if (!sign[kk]) fix(kk,s*sign[jj]);
  else check(sign[jj]*sign[kk]==s);
}

@ @<Determine canonical signs@>=
@<Initialize the data structures@>;
for (k=0;k<n;k++) {
  for (j=k;j<n;j++) {
    if (a[k][j]>0x0 && a[k][j]<0xf)
      @<Canonize position $[k,j]$ of the adjacency matrix@>;
  }
}
@<Make the remaining free signs all positive@>;

@ Complementation of $j$ and/or $k$ can produce up to 16 different outcomes
from the 4-bit code |a[k][j]|. We want to achieve the maximum of the outcomes
that are possible. And we'll enforce any new constraints that are necessary
to guarantee that the maximum is achieved.

@<Canonize position $[k,j]$ of the adjacency matrix@>=
{
  cmax=cands=t=0;
  if (compat(j,-1,k,-1)) {
    cands++,c11=ccode(a[k][j],1,1);
    if (c11>cmax) cmax=c11;
  }@+else c11=-1;
  if (compat(j,-1,k,+1)) {
    cands++,c10=ccode(a[k][j],0,1);
    if (c10>cmax) cmax=c10;
  }@+else c10=-1;
  if (compat(j,+1,k,-1)) {
    cands++,c01=ccode(a[k][j],1,0);
    if (c01>cmax) cmax=c01;
  }@+else c01=-1;
  if (compat(j,+1,k,+1)) {
    cands++,c00=ccode(a[k][j],0,0);
    if (c00>cmax) cmax=c00;
  }@+else c00=-1;
  if (cands>1) {
    @<Compute |t|, the 5-bit index for switching@>;
    switch (t) {
      @<Cases for the big switch@>;
    }
  }
}

@ @<Compute |t|, the 5-bit index for switching@>=
if (cands==2 && ((c00<0)==(c11<0))) t+=0x10;
if (c11>0 && c11<cmax) t+=0x8;
if (c10>0 && c10<cmax) t+=0x4;
if (c01>0 && c01<cmax) t+=0x2;
if (c00>0 && c00<cmax) t+=0x1;

@ Here's the heart of the computation. We're switching on a 5-bit ``case,''
whose bits tell us which scenarios must be avoided.

For example, case |0x3| means that |c01| and |c00| haven't yet been ruled out,
but they both are less than |cmax|. Thus we must complement |vert(j)|,
if we want to maximize the adjacency matrix.

Some cases can't happen. (More precisely, if they do occur, I~made a
mistake when I convinced myself that the constraints always
have a simple form that can be handled by a union-find data structure.)

@<Cases for the big switch@>=
case 0x0: case 0x10: break; /* no new constraints are needed */
case 0x3: fix(j,-1);@+break;
case 0x5: case 0x11: case 0x14: fix(k,-1);@+break;
case 0xa: case 0x12: case 0x18: fix(k,+1);@+break;
case 0xc: fix(j,+1);@+break;
case 0x2: case 0x4: case 0x6: depend(j,k,+1);@+break;
case 0x1: case 0x8: case 0x9: depend(j,k,-1);@+break;
case 0x7: fix(j,-1);@+fix(k,-1);@+break;
case 0xb: fix(j,-1);@+fix(k,+1);@+break;
case 0xd: fix(j,+1);@+fix(k,-1);@+break;
case 0xe: fix(j,+1);@+fix(k,+1);@+break;
default:@+fprintf(stderr,"I'm confused!\n");
    exit(-666);

@ @<Make the remaining free signs all positive@>=
for (k=0;k<n;k++) if (link[k]==k && !sign[k]) {
  fprintf(stderr,"sign(%s)=+ (free)\n",
                       vert(k)->name@t)@>);
  sign[k]=+1;
}

@ @d vvert(k) (gg->vertices+(k))

@<Output the canonical graph@>=
gg=gb_new_graph(n);
for (k=0;k<n;k++)
  vvert(k)->name=gb_save_string(vert(k)->name);
for (j=0;j<n;j++) for (k=0;k<n;k++) if (a[j][k]) {
  jj=lookup(j), kk=lookup(k);
  cmax=ccode(a[j][k],jj<0?(sign[~jj]>0):(sign[jj]<0),
                     kk<0?(sign[~kk]>0):(sign[kk]<0));
  if (cmax&0x8) gb_new_arc(vvert(j),vvert(k),1);
  if (cmax&0x4) gb_new_arc(vvert(j),vvert(k),3);
  if (cmax&0x2) gb_new_arc(vvert(j),vvert(k),0);
  if (cmax&0x1) gb_new_arc(vvert(j),vvert(k),2);
}
sprintf(gg->id,"canonical %.*s",
                     ID_FIELD_SIZE-11,g->id);
save_graph(gg,"/tmp/makegraph.gb");
fprintf(stderr,"OK, I've created /tmp/makegraph.gb,\n with %d vertices",
                                                 n);
fprintf(stderr," and %ld bidirected edges\n",
                  gg->m/2);

@*Index.
