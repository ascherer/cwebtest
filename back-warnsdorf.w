@i gb_types.w
\def\given{\!\mid\!}

@*Intro. Given a graph $G$ on the vertex set $V$, and a vertex $s\in V$,
this program finds all possible simple paths from $s$ that could be
traversed by Warnsdorf's (nondeterministic) rule.
It reports their lengths and their final vertices.

Warndorf's rule is simply this: At a given stage, we have covered
a set of vertices~$U$, with a path from $s$ to some $v\in U$.
If $v$ has no neighbors $\notin U$, stop. Otherwise
cover a neighbor $u\notin U$ whose degree in $G\given(V-U)$ is minimum,
and set $v\gets u$, and repeat.

An optional extension to that rule is also implemented: One or more
vertices can be mentioned as preferred candidates for the end of the path.
(For example, we might want to mention a neighbor of $s$, because
that would imply a Hamiltonian cycle.)

This implementation has an interesting artifact: If the same vertex is
mentioned more than once as a target, it essentially disappears from the
graph. Therefore we could, say, try for a Hamiltonian path that
ends with $a$, $b$, $c$ by listing $a$, $b$, $b$, $c$, $c$ on the
command line.

@d maxn 1000

@c
#include <stdio.h>
#include <stdlib.h>
#include "gb_graph.h"
#include "gb_save.h"
unsigned long long nodes,lnodes[maxn];
unsigned long long count,hcount,lcount[maxn],vcount[maxn];
double pr[maxn],vpr[maxn],lpr[maxn],totalpr;
Vertex *start,*vstack[maxn];
int sptr,bestl;
int mv[maxn],ht[maxn];
@<Subroutines@>@;
main(int argc, char*argv[]) {
  register int d,k,l,x;
  register Arc *a;
  register Graph *g;
  register Vertex *u,*v;
  register double p;
  @<Process the command line@>;
  @<Backtrack through the Warnsdorf tree@>;
  @<Output the results@>;
}

@ @<Process the command line@>=
if (argc<3) {
  fprintf(stderr,"Usage: %s foo.gb start [dest]*\n",argv[0]);
  exit(-1);
}
g=restore_graph(argv[1]);
if (!g) {
  fprintf(stderr,"I couldn't reconstruct graph %s!\n",argv[1]);
  exit(-2);
}
if (g->n>=maxn) {
  fprintf(stderr,"Recompile me --- I can't deal with more than %d vertices!\n",
                            maxn-1);
  exit(-6);
}
start=findvert(g,argv[2]);

@ @<Sub...@>=
Vertex *findvert(Graph *g, char*str) {
  register Vertex *v;
  for (v=g->vertices;v<g->vertices+g->n;v++)
    if (strcmp(v->name,str)==0) break;
  if (v<g->vertices+g->n) return v;
  fprintf(stderr,"Vertex %s isn't in the graph!\n",
                                        str);
  exit(-3);
}

@ Again I follow ye olde structure of Algorithm 7.2.2B.

@d deg u.I
@d covered v.I

@<Backtrack through the Warnsdorf tree@>=
b1:@+for (v=g->vertices;v<g->vertices+g->n;v++) {
    for (d=0,a=v->arcs;a;a=a->next) d++;
    v->deg=d,v->covered=0;
  }
  for (k=3;argv[k];k++)
    findvert(g,argv[k])->deg+=g->n; /* make this vertex undesirable */
  l=0,v=start,ht[0]=0;
  vstack[0]=v,sptr=1,x=0;
  p=1.0;
b2: nodes++, lnodes[l]++; /* at this point |v=vstack[x]| */
  mv[l]=x, x=ht[++l]=sptr;
  p=p/(double)(ht[l]-ht[l-1]);
  pr[l]=p; /* the  probability that a random tree walk would get here */
  v->covered=1;
  for (d=g->n+g->n,a=v->arcs;a;a=a->next) {
    u=a->tip;
    if (u->covered) continue;
    k=u->deg-1, u->deg=k;
    if (k>d) continue;
    if (k<d) d=k,sptr=x;
    vstack[sptr++]=u;
  }
  if (x==sptr) @<Report end of path and |goto b5|@>;
b3: v=vstack[x];
  goto b2;
b4:@+if (++x<sptr) goto b3;
b5:@+if (--l) {
    x=mv[l],p=pr[l],v=vstack[x],sptr=ht[l+1];
    v->covered=0;
    for (a=v->arcs;a;a=a->next) {
      u=a->tip;
      if (u->covered) continue;
      u->deg++;
    }
    goto b4;
  }

@ @<Report end of path and |goto b5|@>=
{
  count++, lcount[l]++, lpr[l]+=p, totalpr+=p;
  nodes++, lnodes[l]++; /* count the solution nodes with the branch nodes */
  if (l>=bestl) {
    bestl=l;
    @<Publish the current path@>;
    if (l==g->n) hcount++,vcount[v-g->vertices]++,vpr[v-g->vertices]+=p;
  }
  goto b5;    
}

@ @<Publish the current path@>=
{
  for (k=0;k<l;k++) printf("%s ",
                           vstack[mv[k]]->name);
  printf("(%d,%g) #%llu, %g\n",
                     l,p,count,totalpr); @q)@>;
}

@ @<Output the results@>=
fprintf(stderr,"Altogether %llu nodes, %llu paths, %llu hpaths.\n",
                                     nodes,count,hcount);
for (l=0;l<=bestl;l++) {
  fprintf(stderr," Level %d: %12llu node%s",
                              l,lnodes[l],lnodes[l]==1?"":"s");
  if (lcount[l])
    fprintf(stderr,", %llu paths (%g)",
                              lcount[l],lpr[l]/totalpr);
  fprintf(stderr,"\n");
}
for (k=0;k<g->n;k++) if (vcount[k])
  fprintf(stderr," hamiltonian to %s: %12llu, %g\n",
                           (g->vertices+k)->name,vcount[k],vpr[k]/totalpr);

@ Here's a routine that I might use while debugging.

@<Sub...@>=
void print_vert(Vertex *v) {
  fprintf(stderr," %s %s%ld%s\n",
                   v->name,v->covered?"(":"",v->deg,v->covered?")":"");
}

@ And another, to display the state of {\it all\/} the vertices.

@<Sub...@>=
void print_state(Graph *g) {
  register int k;
  for (k=0;k<g->n;k++) print_vert(g->vertices+k);
}

@ Yet another, to show the neighbors of a given vertex and their
current degrees.

@<Sub...@>=
void print_arcs(Vertex *v) {
  register Arc *a;
  register Vertex *u;
  fprintf(stderr,"%s ->",
                          v->name);
  for (a=v->arcs;a;a=a->next) {
    u=a->tip;
    if (u->covered) fprintf(stderr," (%s)",
                               u->name)@q)@>;
    else fprintf(stderr," %s(%ld)",
                               u->name,u->deg);
  }
  fprintf(stderr,"\n");
}

@*Index.
