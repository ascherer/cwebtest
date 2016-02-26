/*1:*/
#line 29 "./grayspan.w"

#include "gb_graph.h"
#include "gb_save.h"
#define verbose (argc> 2) 
#define extraverbose (argc> 3) 
#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define oooo mems+= 4
#define ooooo mems+= 5 \

#define deg u.I
#define prev a.A
#define mate(e) (edge_trick&(siz_t) (e) ?(e) -1:(e) +1)  \

#define aa(l) (g->vertices+l) ->z.A
#define del(l) (g->vertices+l) ->y.A
#define link b.A \

#define delete(e) ee= e,oooo,ee->prev->next= ee->next,ee->next->prev= ee->prev \

#define undelete(e) ee= e,oooo,ee->next->prev= ee,ee->prev->next= ee \

#define bfs v.V \

#define sentinel (g->vertices)  \


#line 32 "./grayspan.w"

double mems;
double count;
/*5:*/
#line 119 "./grayspan.w"

void print_arcs(Vertex*v)
{
register Arc*a;
printf("Arcs leading from %s:\n",v->name);
for(a= v->arcs->next;a->tip;a= a->next)
printf(" %d (to %s)\n",a->len,a->tip->name);
}

/*:5*//*17:*/
#line 342 "./grayspan.w"

void print_a(register Graph*g)
{
register int k;
for(k= 1;k<g->n;k++)
printf(" a%d=%d (%s -- %s)\n",
k,aa(k)->len,aa(k)->tip->name,mate(aa(k))->tip->name);
}

/*:17*/
#line 35 "./grayspan.w"

main(int argc,char*argv[])
{
/*3:*/
#line 61 "./grayspan.w"

register Graph*g;
register int n;
register int k;
register Vertex*u,*v,*w;
register Arc*e,*ee,*f,*ff;

/*:3*//*8:*/
#line 196 "./grayspan.w"

register int l;
Arc*change_e;

/*:8*/
#line 38 "./grayspan.w"
;
/*2:*/
#line 46 "./grayspan.w"

if(argc<2){
fprintf(stderr,"Usage: %s foo.gb [[gory] details]\n",argv[0]);
exit(1);
}
g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,
"Sorry, can't create the graph from file %s! (error code %d)\n",
argv[1],panic_code);
exit(-1);
}
n= g->n;
/*4:*/
#line 88 "./grayspan.w"

if(verbose)printf("Graph %s has the following edges:\n",g->id);
for(v= g->vertices,k= 0;v<g->vertices+n;v++){
f= gb_virgin_arc();f->next= v->arcs;
for(v->deg= 0,e= v->arcs,v->arcs= f;e;v->deg++,f= e,e= e->next){
e->prev= f;
u= e->tip;
if(u==v){
fprintf(stderr,"Oops, there's a loop from %s to itself!\n",v->name);
exit(-3);
}
if(mate(e)->tip!=v){
fprintf(stderr,"Oops: There's an arc from %s to %s,\n",u->name,v->name);
fprintf(stderr," but the edge trick doesn't find the opposite arc!\n");
exit(-4);
}
if(u> v){
e->len= mate(e)->len= ++k;
if(verbose)printf(" %d: %s -- %s\n",k,v->name,u->name);
}
}
v->arcs->prev= f,f->next= v->arcs;
if(v->deg==0){
fprintf(stderr,"Graph %s has an isolated vertex %s!\n",
g->id,v->name);
exit(-5);
}
}

/*:4*/
#line 59 "./grayspan.w"
;

/*:2*/
#line 39 "./grayspan.w"
;
/*16:*/
#line 321 "./grayspan.w"

for(v= g->vertices+1;v<g->vertices+n;v++)v->bfs= NULL;
for(k= n-1,o,w= v= g->vertices,w->bfs= sentinel;;o,v= w,w= w->bfs){
for(oo,e= v->arcs->next;o,u= e->tip;o,e= e->next)
if(o,u->bfs==NULL){
o,aa(k)= e,k--;
if(k==0)goto connected;
o,u->bfs= w,w= u;
}
if(w==sentinel)break;
}
printf("Oops, the graph isn't connected!\n");exit(0);
connected:for(u= g->vertices;u<g->vertices+n;u++)o,u->bfs= NULL;
if(extraverbose){
printf("Depth-first search yields the following spanning tree:\n");
print_a(g);
}
if(verbose)printf("(%.15g mems for initialization)\n",mems);

/*:16*/
#line 40 "./grayspan.w"
;
/*7:*/
#line 174 "./grayspan.w"

change_e= NULL;
v= g->vertices;
for(l= 1;l<n-1;l++){
o,del(l)= NULL;
enter:ooo,e= aa(l+1),u= e->tip,v= mate(e)->tip;
if(oo,u->deg> v->deg)v= u,e= mate(e),u= e->tip;
/*10:*/
#line 225 "./grayspan.w"

oo,k= u->deg+v->deg;
for(o,f= u->arcs->next,ff= NULL;o,f->tip;o,f= f->next)
if(f->tip==v)delete(f),delete(mate(f)),k-= 2,o,f->link= ff,ff= f;
else o,mate(f)->tip= v;
oo,e->link= ff,v->deg= k;
if(extraverbose)
printf("level %d: Shrinking %d; now %s has degree %d\n",
l,e->len,v->name,v->deg);
o,ff= v->arcs;
oooo,f->prev->next= ff->next,ff->next->prev= f->prev;
ooo,f->next->prev= ff,ff->next= f->next;

/*:10*/
#line 181 "./grayspan.w"
;
o,aa(l)= e;
}
for(o,e= v->arcs->next;o,e->tip;o,e= e->next){
o,aa(l)= e;
/*9:*/
#line 200 "./grayspan.w"

count++;
if(verbose){
if(!change_e||extraverbose){
printf("%.15g:",count);
for(k= 1;k<n;k++)printf(" %d",aa(k)->len);
if(extraverbose&&change_e)printf(" (-%d+%d)\n",change_e->len,e->len);
else printf("\n");
}else printf("%.15g: -%d+%d\n",count,change_e->len,e->len);
}

/*:9*/
#line 186 "./grayspan.w"
;
change_e= e;
}
for(l--;l;l--){
e= aa(l),u= e->tip,v= mate(e)->tip;
/*11:*/
#line 244 "./grayspan.w"

oo,f= u->arcs,ff= v->arcs;
ooo,ff->next= f->prev->next;o,ff->next->prev= ff;
ooo,f->prev->next= f,f->next->prev= f;
for(f= f->prev;o,f->tip;o,f= f->prev)o,mate(f)->tip= u;
for(oo,f= e->link,k= v->deg;f;o,f= f->link)
k+= 2,undelete(mate(f)),undelete(f);
oo,v->deg= k-u->deg;
if(extraverbose)
printf("level %d: Unshrinking %d; now %s has degree %d\n",
l,e->len,v->name,v->deg);

/*:11*/
#line 191 "./grayspan.w"
;
/*12:*/
#line 269 "./grayspan.w"

if(o,u->deg==1){
if(extraverbose)printf("level %d: %d is a bridge with endpoint %s\n",
l,e->len,u->name);
goto bridge;
}
if(o,e->link->link){
if(extraverbose)printf("level %d: %d is parallel to %d\n",
l,e->len,e->link->len!=e->len?e->link->len:e->link->link->len);
goto nonbridge;
}
for(o,u->bfs= v,w= u;u!=v;o,u= u->bfs){
for(oo,f= u->arcs->next;o,f->tip;o,f= f->next)
if(o,f->tip->bfs==NULL){
if(f->tip==v){
if(f!=mate(e))/*13:*/
#line 295 "./grayspan.w"

{
for(o,u= e->tip;u!=v;o,u->bfs= NULL,u= w)o,w= u->bfs;
goto nonbridge;
}

/*:13*/
#line 284 "./grayspan.w"
;
}else oo,f->tip->bfs= v,w->bfs= f->tip,w= f->tip;
}
}
if(extraverbose)printf("level %d: %d is a bridge\n",l,e->len);
for(o,u= e->tip;u!=v;o,u->bfs= NULL,u= w)o,w= u->bfs;
goto bridge;
nonbridge:change_e= e;
/*14:*/
#line 301 "./grayspan.w"

if(extraverbose)printf("level %d: deleting %d\n",l,e->len);
ooo,e->link= del(l),del(l)= e;
delete(e),delete(mate(e)),oo,e->tip->deg--,v->deg--;
goto enter;

/*:14*/
#line 292 "./grayspan.w"
;
bridge:

/*:12*/
#line 192 "./grayspan.w"
;
/*15:*/
#line 307 "./grayspan.w"

for(o,e= del(l);e;o,e= e->link){
oooo,mate(e)->tip->deg++,e->tip->deg++,undelete(mate(e)),undelete(e);
if(extraverbose)printf("undeleting %d\n",e->len);
}

/*:15*/
#line 193 "./grayspan.w"
;
}

/*:7*/
#line 41 "./grayspan.w"
;
printf("Altogether %.15g spanning trees, using %.15g mems.\n",count,mems);
exit(0);
}

/*:1*/
