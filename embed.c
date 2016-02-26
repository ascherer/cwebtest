/*1:*/
#line 39 "./embed.w"

#include "gb_graph.h"
#include "gb_save.h"
#define start_vertex g->vertices
#define nmax 100
#define cmax 32 \

#define the_arc(u,v) adjmat[u-g->vertices][v-g->vertices] \

#define prev a.A \

#define dist z.I \

#define link y.V \

#define edge_trick (sizeof(Arc) &-sizeof(Arc) ) 
#define lower(a) (edge_trick&(unsigned long) (a) ?(a) -1:(a) ) 
#define upper(a) (edge_trick&(unsigned long) (a) ?(a) :(a) +1) 
#define other(a) (edge_trick&(unsigned long) (a) ?(a) -1:(a) +1) 
#define elink(a) upper(a) ->b.A
#define leader(a) lower(a) ->b.A
#define size(a) lower(a) ->len
#define serial(a) upper(a) ->len \

#define code y.I \

#define mate x.V
#define mark w.V
#define rank v.I \


#line 42 "./embed.w"

Graph*g;
/*2:*/
#line 81 "./embed.w"

Arc*adjmat[nmax][nmax];

/*:2*//*5:*/
#line 165 "./embed.w"

int classes;

/*:5*//*12:*/
#line 274 "./embed.w"

Vertex*rtab[nmax],*stab[nmax];
int tabptr;

/*:12*/
#line 44 "./embed.w"

/*6:*/
#line 170 "./embed.w"

void print_edge(Arc*e)
{
register Arc*f;
if(!e->tip)printf("(header)");
else{
f= leader(e);
printf("Edge %s -- %s",e->tip->name,other(e)->tip->name);
if(serial(f))printf(", class %d",serial(f));
else printf(" (0x%x)",(unsigned long)f);
printf(", size %d\n",size(f));
}
}

/*:6*//*7:*/
#line 186 "./embed.w"

void serialize(Arc*e)
{
e= leader(e);
if(serial(e)==0){
serial(e)= ++classes;
if(classes> cmax){
fprintf(stderr,"Overflow: more than %d classes!\n",cmax);
exit(-5);
}
}
}

/*:7*//*8:*/
#line 199 "./embed.w"

void unionize(Arc*e,Arc*f)
{
register Arc*g;
e= leader(e),f= leader(f);
if(e!=f){
if(serial(e)&&serial(f)&&serial(e)!=serial(f)){
fprintf(stderr,"I goofed (merging two serialize classes)!\n");
exit(-69);
}
if(size(e)> size(f))g= e,e= f,f= g;
leader(e)= f,size(f)+= size(e);
if(serial(e))serial(f)= serial(e);
for(g= elink(e);g!=e;g= elink(g))leader(g)= f;
g= elink(e),elink(e)= elink(f),elink(f)= g;
}
}

/*:8*//*11:*/
#line 242 "./embed.w"

void embed(Vertex*r)
{
register Vertex*s,*u,*v,*w,*vv;
register Arc*aa,*a,*b;
if(verbose)printf("Beginning to embed subgraph %s:\n",r->name);
for(aa= r->arcs->next;(s= aa->tip)!=NULL;aa= aa->next){
/*13:*/
#line 278 "./embed.w"

rtab[tabptr]= r,stab[tabptr]= s,tabptr++;

/*:13*/
#line 249 "./embed.w"
;
/*15:*/
#line 345 "./embed.w"

s->mark= s,s->rank= 1,s->link= NULL;
for(v= vv= s;v;v= v->link){
/*16:*/
#line 365 "./embed.w"

for(a= v->arcs->next;;a= a->next){
u= a->tip;
if(!(u->mark==s&&u->rank==1))break;
}
v->mate= u;
a->next->prev= a->prev,a->prev->next= a->next;
b= other(a);
b->next->prev= b->prev,b->prev->next= b->next;
unionize(a,aa);

/*:16*/
#line 348 "./embed.w"
;
for(a= v->arcs->next;a->tip;a= a->next){
u= a->tip;
if(u->dist> v->dist)/*17:*/
#line 388 "./embed.w"

{
if(u->mark==s&&u->rank==1)continue;
u->mark= s,u->rank= 2;
b= u->arcs->next,w= b->tip;
if(w==v){
w= b->next->tip;
if(w==NULL||w->dist> u->dist)continue;
}
if(!(w->mark==s&&w->rank==2))
u->rank= 1,u->link= 0,vv->link= u,vv= u;
}

/*:17*/
#line 351 "./embed.w"

else/*18:*/
#line 406 "./embed.w"

if(u->mark==s&&u->rank==1)
unionize(a,the_arc(v->mate,u->mate));

/*:18*/
#line 352 "./embed.w"
;
}
}

/*:15*/
#line 250 "./embed.w"
;
embed(s);
serialize(aa);
if(verbose)
printf(" Edge %s -- %s is in class %d\n",
r->name,s->name,serial(leader(aa)));
}
if(verbose)printf("Done with %s.\n",r->name);
}

/*:11*/
#line 45 "./embed.w"

main(int argc,char*argv[])
{
register int j,k;
register Vertex*u,*v,*w;
register Arc*a,*b,*c;
if(argc<2){
fprintf(stderr,"Usage: %s foo.gb [verbose]\n",argv[0]);
exit(-1);
}
verbose= argc-2;
g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,"Sorry, I couldn't input the graph %s!\n",argv[1]);
exit(-2);
}
if(g->n> nmax){
fprintf(stderr,
"Sorry, the graph has more vertices (%d) than I can handle (%d)!\n",
g->n,nmax);
exit(-3);
}
/*10:*/
#line 226 "./embed.w"

/*3:*/
#line 92 "./embed.w"

for(u= g->vertices;u<g->vertices+g->n;u++){
b= gb_virgin_arc(),b->tip= NULL;
for(a= u->arcs,u->arcs= b,b->next= a;a;b= a,a= b->next)
the_arc(u,a->tip)= a,a->prev= b;
b->next= u->arcs,u->arcs->prev= b;
}

/*:3*/
#line 227 "./embed.w"
;
/*4:*/
#line 120 "./embed.w"

for(u= g->vertices;u<g->vertices+g->n;u++)u->dist= -1,u->link= NULL;
start_vertex->dist= 0;
for(u= v= start_vertex;u;u= u->link){
for(b= u->arcs,a= b->next;a->tip;a= a->next)
if(a->tip->dist<0)a->tip->dist= u->dist+1,v->link= a->tip,v= a->tip;
else if(a->tip->dist<u->dist){
c= a->prev;
if(c!=b){
c->next= a->next,c->next->prev= c;
a->next= b->next,a->next->prev= a;
b->next= a,a->prev= b;
a= c;
}
}
}

/*:4*/
#line 228 "./embed.w"
;
/*9:*/
#line 217 "./embed.w"

for(u= g->vertices;u<g->vertices+g->n;u++)
for(a= u->arcs->next;a->tip;a= a->next)
if(a==lower(a))elink(a)= leader(a)= a,size(a)= 1,serial(a)= 0;

/*:9*/
#line 229 "./embed.w"
;

/*:10*/
#line 67 "./embed.w"
;
embed(start_vertex);
/*14:*/
#line 281 "./embed.w"

printf("The codewords are:\n %s = 00000000\n",start_vertex->name);
start_vertex->code= 0x0;
for(k= 0;k<tabptr;k++){
u= rtab[k],v= stab[k],a= the_arc(u,v);
j= 1<<(serial(leader(a))-1);
if(u->code&j){
fprintf(stderr,"I goofed (class %d used twice)!\n",serial(leader(a)));
exit(-8);
}
v->code= u->code|j;
printf(" %s = %08x\n",v->name,v->code);
}

/*:14*/
#line 69 "./embed.w"
;
}

/*:1*/
