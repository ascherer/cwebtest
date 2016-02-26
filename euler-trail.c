#define parent v.V
#define nav w.A \

/*1:*/
#line 10 "./euler-trail.w"

#include <stdio.h> 
#include <stdlib.h> 
#include "gb_graph.h"
#include "gb_save.h"
/*3:*/
#line 43 "./euler-trail.w"

void dfs(register Vertex*u,register Vertex*v){
register Vertex*w;
register Arc*a;
v->parent= u;
for(a= v->arcs;a;a= a->next){
w= a->tip;
if(w==u)v->nav= a->next;
else if(w->parent==NULL)dfs(v,w);
}
}

/*:3*/
#line 15 "./euler-trail.w"


main(int argc,char*argv[]){
register int k;
Graph*g;
Vertex*u,*v;
Arc*a;
/*2:*/
#line 28 "./euler-trail.w"

if(argc!=2||!(g= restore_graph(argv[1]))){
fprintf(stderr,"Usage: %s foo.gb\n",argv[0]);
exit(-1);
}
fprintf(stderr,"OK, I've input `%s'.\n",argv[1]);
gb_new_edge(g->vertices,g->vertices+g->n,0);

/*:2*/
#line 22 "./euler-trail.w"
;
/*4:*/
#line 55 "./euler-trail.w"

dfs(g->vertices+g->n,g->vertices);

/*:4*/
#line 23 "./euler-trail.w"
;
/*5:*/
#line 60 "./euler-trail.w"

for(v= g->vertices;v!=g->vertices+g->n;){
printf(" %s",v->name);
a= v->nav;
if(!a)a= v->arcs;
v->nav= a->next;
v= a->tip;
}

/*:5*/
#line 24 "./euler-trail.w"
;
printf("\n");
}

/*:1*/
