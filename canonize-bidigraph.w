\let\swap=\leftrightarrow
@i gb_types.w

@*Intro. Given a bidirected graph~$B$, this program outputs an equivalent
bidirected graph~$[B]$ with the property that $[B]=[B']$ if and only if
$B'$ is obtained from $B$ by ``complementing'' some subset of the
vertices. Furthermore $[B]$ is directed whenever $B$ is directed.

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
$a[v][w]=1000$ means that $v\0<<w$ but we don't have
$v\0>>w$ or $v\0<>w$ or $v\0><w$;
$a[v][w]=0101$ means that the only edges between $v$ and~$w$ are
$v\0>>w$ and $v\0><w$.

Suppose the four bits of $a[v][w]$ are $\it pqrs$. The operation of
complementing $v$ changes those bits to $\it srqp$; the operation of
complementing $w$ changes them to $\it rspq$; and the operation of
complementing both changes them to $\it qpsr$.

Our canonical form complements the variables in such a way that
the lexicographic order of the bitstrings on and above the diagonal
of the adjacency matrix, namely the sequence of $(n+1)n/2$ four-bit codes
$$(a[1][1],\ldots, a[1][n]; a[2][2],\ldots,a[2][n]; \ldots, a[n][n]),$$
is maximized.

@d maxn 100

@c
#include <stdio.h>
#include <stdlib.h>
#include "gb_graph.h"
#include "gb_save.h"
int a[maxn][maxn]; /* adjacency matrix */
int sign[maxn]; /* the sign of |vert(k)|, or zero */
int fixed[maxn]; /* is the sign of |vert(k)| known? */
int depend[maxn]; /* does |sign[k]| depend on the current row sign? */
int dptr; /* this many dependencies in the current row */
int indep; /* this many vertices are independent */
int unknown; /* this many vertices still haven't been fixed */
Graph *g,*gg;
@<Subroutines@>;
int main(int argc,char*argv[]) {
  register int i,j,k,n,c00,c01,c10,c11,cmax,code,edoc,exo;
  register Arc *aa;
  register Vertex *v, *w;
  @<Process the command line@>;
  @<Set up the adjacency matrix@>;
  @<Determine canonical signs@>;
  @<Output the canonical graph@>;
  fprintf(stderr,"(%d vertices are independent.)\n",
                      indep@q)@>);
  if (!exo) fprintf(stderr,"This graph is actually directed!\n");
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

@ @<Determine canonical signs@>=
for (unknown=n,k=0;unknown;k++) {
  for (j=k;j<n && unknown;j++) {
    if (!fixed[j]) {
      if (!fixed[k]) @<Try four possibilities for |j| and |k|@>@;
      else @<Try two possibilities for |j|@>;
    }@+else if (!fixed[k]) @<Try two possibilities for |k|@>;
  }
  if (!fixed[k]) {
    fprintf(stderr," %s is free\n",
                        vert(k)->name);
    indep++;
    fix(k,0);
  }
  while (dptr) {
    j=depend[--dptr];
    if (j>=0) fix(j,sign[k]);
    else fix(~j,sign[k]^1);
  }
}

@ @<Sub...@>=
void fix(int k,int c) {
  fprintf(stderr,"%s=%d\n",
                    vert(k)->name,c);
  fixed[k]=1,sign[k]=c;
  unknown--;
}

@ Here's the heart of the computation. We're trying to find a
sequence of complementations that maximizes the codes.

The case |j==k| is tricky; I think I've got it right (knock on wood).
For example, only cases |0x2|, |0x8|, |0xa|, and |0xf| are
possible in the big switch when |j=k|.

@<Try four possibilities for |j| and |k|@>=
{
  c00=cmax=a[k][j];
  c01=ccode(a[k][j],0,1);@+if (j!=k && cmax<c01) cmax=c01;
  c11=ccode(a[k][j],1,1);@+if (cmax<c11) cmax=c11;
  c10=ccode(a[k][j],1,0);@+if (j!=k && cmax<c10) cmax=c10;
  switch ((c00==cmax?0x8:0)+
         (c01==cmax?0x4:0)+
         (c11==cmax?0x2:0)+
         (c10==cmax?0x1:0)) {
case 0x1:@+fix(j,0),fix(k,1);@+break;
case 0x2:@+if (j==k) fix(k,1);@+else fix(j,1),fix(k,1);@+break;
case 0x4:@+fix(j,1),fix(k,0);@+break;
case 0x8:@+if (j==k) fix(k,0);@+else fix(j,0),fix(k,0);@+break;
case 0x3:@+fix(j,1);@+break;
case 0x5:@+fprintf(stderr," %s!=%s\n",
                          vert(j)->name,vert(k)->name),
          depend[dptr++]=~j;@+break;
case 0x6:@+fix(k,1);@+break;
case 0x9:@+fix(k,0);@+break;
case 0xa:@+if (j!=k) fprintf(stderr," %s==%s\n",
                          vert(j)->name,vert(k)->name),
    depend[dptr++]=j;@+break;
case 0xc:@+fix(j,0);@+break;
case 0xf:@+break; /* complementation has no effect */
default:@+fprintf(stderr,"I'm confused!\n");
    exit(-666);
  }
}
  
@ We always have |j!=k| here.

@<Try two possibilities for |j|@>=
{
  c00=cmax=ccode(a[k][j],sign[k],0);
  c01=ccode(a[k][j],sign[k],1);@+if (cmax<c01) cmax=c01;
  switch ((c00==cmax?0x2:0)+(c01==cmax?0x1:0)) {
case 0x1: fix(j,1);@+break;
case 0x2: fix(j,0);@+break;
case 0x3: break; /* complementation has no effect */
  }
}

@ Again, |j!=k| here.

@<Try two possibilities for |k|@>=
{
  c00=cmax=ccode(a[k][j],0,sign[j]);
  c10=ccode(a[k][j],1,sign[j]);@+if (cmax<c10) cmax=c10;
  switch ((c00==cmax?0x2:0)+(c10==cmax?0x1:0)) {
case 0x1: fix(k,1);@+break;
case 0x2: fix(k,0);@+break;
case 0x3: break; /* complementation has no effect */
  }
}

@ @d vvert(k) (gg->vertices+(k))

@<Output the canonical graph@>=
gg=gb_new_graph(n);
for (k=0;k<n;k++)
  vvert(k)->name=gb_save_string(vert(k)->name);
for (exo=j=0;j<n;j++) for (k=0;k<n;k++) if (a[j][k]) {
  cmax=ccode(a[j][k],sign[j],sign[k]);
  if (cmax&0x8) gb_new_arc(vvert(j),vvert(k),1);
  if (cmax&0x4) gb_new_arc(vvert(j),vvert(k),3);
  if (cmax&0x2) exo=1,gb_new_arc(vvert(j),vvert(k),0);
  if (cmax&0x1) exo=1,gb_new_arc(vvert(j),vvert(k),2);
}
sprintf(gg->id,"canonical %.*s",
                     ID_FIELD_SIZE-1,g->id);
save_graph(gg,"/tmp/makegraph.gb");
fprintf(stderr,"OK, I've created /tmp/makegraph.gb, with %d vertices",
                                                 n);
fprintf(stderr," and %ld edges\n",
                  gg->m/2);

@*Index.
