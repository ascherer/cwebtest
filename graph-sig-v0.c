#define maxn 100 \

#define sig w.I
#define link u.V
#define tag v.I \

#define oldsig z.I \

/*1:*/
#line 31 "graph-sig-v0.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_graph.h"
#include "gb_save.h"
#include "gb_flip.h"
long sg[maxn];
Vertex*hd[maxn],*tl[maxn];
main(int argc,char*argv[]){
register int i,j,k,r,change;
register Graph*g;
register Vertex*u,*v;
register Arc*a,*b;
register long x,s;
Vertex*v0,*prev,*head;
/*2:*/
#line 55 "graph-sig-v0.w"

if(argc!=3){
fprintf(stderr,"Usage: %s foo.gb v0\n",
argv[0]);
exit(-1);
}
g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,"I couldn't reconstruct graph %s!\n",
argv[1]);
exit(-2);
}
if(g->n> maxn){
fprintf(stderr,"Recompile me: g->n=%ld, maxn=%d!\n",
g->n,maxn);
exit(-3);
}
gb_init_rand(0);
for(v= g->vertices;v<g->vertices+g->n;v++)
if(strcmp(v->name,argv[2])==0)break;
if(v==g->vertices+g->n){
fprintf(stderr,"I can't find a vertex named `%s'!\n",
argv[2]);
exit(-9);
}
v0= v;

/*:2*/
#line 47 "graph-sig-v0.w"
;
/*3:*/
#line 89 "graph-sig-v0.w"

printf("Initial round:\n");
for(v= g->vertices;v<g->vertices+g->n;v++)v->sig= -1,v->tag= 0;
v0->sig= 0,v0->link= v0,k= 1,v= v0;
while(v){
prev= head= NULL;
while(1){
printf(" %s dist %ld\n",
v->name,v->sig);
/*4:*/
#line 108 "graph-sig-v0.w"

for(a= v->arcs;a;a= a->next){
u= a->tip;
if(u->sig<0){
u->sig= k;
if(prev==NULL)head= u;
else u->link= prev;
prev= u;
}
}

/*:4*/
#line 98 "graph-sig-v0.w"
;
v->tag= k;
v= v->link;
if(v->tag)break;
}
if(prev==NULL)break;
head->link= prev;
v= prev,k++;
}

/*:3*/
#line 48 "graph-sig-v0.w"
;
for(change= 1,r= 1;change;r++){
change= 0;
/*5:*/
#line 124 "graph-sig-v0.w"

printf("Round %d:\n",
r);
for(v= g->vertices;v<g->vertices+g->n;v++)v->oldsig= v->sig;
k++;
x= (gb_next_rand()<<1)+1;
for(v= g->vertices;v<g->vertices+g->n;v++)if(v->tag> 0){
if(v->tag==k)continue;
if(v->link==v){
printf(" %s is fixed\n",
v->name);
v->tag= -k;
continue;
}
for(j= 0;v->tag!=k;v= u){
u= v->link;
/*6:*/
#line 153 "graph-sig-v0.w"

for(s= 1,a= v->arcs;a;a= a->next)
s*= x-a->tip->oldsig;

/*:6*/
#line 140 "graph-sig-v0.w"
;
printf(" %s %lx\n",
v->name,s);
v->sig= s;
for(i= 0,sg[j]= s;sg[i]!=s;i++);
if(i==j)hd[j]= tl[j]= v,j++;
else v->link= tl[i],tl[i]= v;
v->tag= k;
}
for(i= 0;i<j;i++)hd[i]->link= tl[i];
if(j> 1)change= 1;
}

/*:5*/
#line 51 "graph-sig-v0.w"
;
}
}

/*:1*/
