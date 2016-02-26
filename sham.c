#define deg u.I
#define mm 8
#define nn 9 \

#define mate(v) (Vertex*) (((unsigned long) g->vertices) + \
((unsigned long) (g->vertices+g->n-1) ) - \
((unsigned long) v) )  \

#define taken v.I \

#define ark x.A \

/*1:*/
#line 14 "sham.w"

#include "gb_graph.h" 
#include "gb_basic.h" 

main()
{
Graph*g= board(mm,nn,0,0,5,0,0);
Vertex*x,*z,*tmax;
register Vertex*t,*u,*v;
register Arc*a,*aa,*yy;
register int d;
Arc*b,*bb;
int count= 0,dcount= 0;
int dmin;
/*2:*/
#line 57 "sham.w"

for(v= g->vertices;mate(v)> v;v++)
for(a= v->arcs;a;a= a->next){
u= mate(a->tip);
if(u> a->tip)a->len= 0;
else{
a->len= 1;
a->tip= u;
}
}
g->n/= 2;

/*:2*/
#line 28 "sham.w"
;
/*4:*/
#line 86 "sham.w"

/*3:*/
#line 73 "sham.w"

for(v= g->vertices;v<g->vertices+g->n;v++)
for(a= v->arcs,aa= NULL;a;a= a->next)
if(a->tip==v){
if(aa)aa->next= a->next;
else v->arcs= a->next;
}else aa= a;

/*:3*/
#line 87 "sham.w"
;
dmin= g->n;
for(v= g->vertices;v<g->vertices+g->n;v++){
v->taken= 0;
d= 0;
for(a= v->arcs;a;a= a->next)d++;
v->deg= d;
if(d<dmin)dmin= d,x= v;
}

/*:4*/
#line 29 "sham.w"
;
for(v= g->vertices;v<g->vertices+g->n;v++)printf(" %d",v->deg);
printf("\n");
if(x->deg<2){
printf("The minimum degree is %d (vertex %s)!\n",x->deg,x->name);
return-1;
}
for(b= x->arcs;b->next;b= b->next)for(bb= b->next;bb;bb= bb->next){
a= b;
z= bb->tip;
/*5:*/
#line 111 "sham.w"

v= a->tip;
t= g->vertices;tmax= t+g->n-1;
x->taken= 1;
a->len+= 4;
advance:/*6:*/
#line 126 "sham.w"

t->ark= a;
t++;
v= a->tip;
v->taken= 1;
if(v==z){
if(t==tmax&&v->deg==1)/*10:*/
#line 195 "sham.w"

{int s= 0;
for(u= g->vertices;u<tmax;u++)s^= u->ark->len&1;
if(s){
count++;
if(count%100000==0){
printf("%d: %s",count,x->name);
for(u= g->vertices;u<tmax;u++)
printf("%s%s",u->ark->len&1?"*":" ",u->ark->tip->name);
printf("\n");
}
}else{
dcount++;
if(dcount%100000==0){
printf(">%d: %s",dcount,x->name);
for(u= g->vertices;u<tmax;u++)
printf("%s%s",u->ark->len&1?"*":" ",u->ark->tip->name);
printf("\n");
}
}
}

/*:10*/
#line 132 "sham.w"
;
goto backtrack;
}
yy= NULL;
for(aa= v->arcs;aa;aa= aa->next){
u= aa->tip;
d= u->deg-1;
if(d==1&&u->taken==0){
if(yy)goto restore;
yy= aa;
}
u->deg= d;
}
if(yy){
a= yy;
a->len+= 4;
goto advance;
}
a= v->arcs;

/*:6*/
#line 118 "sham.w"
;
try:/*8:*/
#line 156 "sham.w"

while(a){
if(a->tip->taken==0){
a->len+= 2;
goto advance;
}
a= a->next;
}
restore_all:aa= NULL;

/*:8*/
#line 119 "sham.w"
;
restore:/*7:*/
#line 153 "sham.w"

for(a= (t-1)->ark->tip->arcs;a!=aa;a= a->next)a->tip->deg++;

/*:7*/
#line 121 "sham.w"
;
backtrack:/*9:*/
#line 177 "sham.w"

t--;
a= t->ark;
a->tip->taken= 0;
d= a->len;
a->len&= 1;
if(d<4){
a= a->next;
goto try;
}
if(t==g->vertices)goto done;
for(aa= (t-1)->ark->tip->arcs;aa!=a;aa= aa->next)if(aa->tip==a->tip){
aa->len+= 4;
a= aa;
goto advance;
}
goto restore_all;

/*:9*/
#line 123 "sham.w"
;
done:

/*:5*/
#line 40 "sham.w"
;
}
printf("Altogether %d solutions and %d wannabees.\n",count,dcount);
for(v= g->vertices;v<g->vertices+g->n;v++)printf(" %d",v->deg);
printf("\n");
}

/*:1*/
