#define o mems++
#define oo mems+= 2 \

#define succ u.v
#define pred v.v \

/*1:*/
#line 11 "./hulls.w"

#include "gb_graph.h"
#include "gb_miles.h"
int n= 128;
/*2:*/
#line 33 "./hulls.w"

int mems;
int ccs;
int serial_no= 1;

/*:2*//*4:*/
#line 54 "./hulls.w"

Vertex*rover;

/*:4*/
#line 15 "./hulls.w"

/*11:*/
#line 142 "./hulls.w"

int ccw(u,v,w)
Vertex*u,*v,*w;
{register double wx= (double)w->x.i,wy= (double)w->y.i;
register double det= ((double)u->x.i-wx)*((double)v->y.i-wy)
-((double)u->y.i-wy)*((double)v->x.i-wx);
Vertex*uu= u,*vv= v,*ww= w,*t;
if(det==0){
det= 1;
if(u->x.i> v->x.i||(u->x.i==v->x.i&&(u->y.i> v->y.i||
(u->y.i==v->y.i&&u->z.i> v->z.i)))){
t= u;u= v;v= t;det= -det;
}
if(v->x.i> w->x.i||(v->x.i==w->x.i&&(v->y.i> w->y.i||
(v->y.i==w->y.i&&v->z.i> w->z.i)))){
t= v;v= w;w= t;det= -det;
}
if(u->x.i> v->x.i||(u->x.i==v->x.i&&(u->y.i> v->y.i||
(u->y.i==v->y.i&&u->z.i<v->z.i)))){
det= -det;
}
}
if(n<150)printf("cc(%s; %s; %s) is %s\n",uu->name,vv->name,ww->name,
det> 0?"true":"false");
ccs++;
return(det> 0);
}/*:11*/
#line 16 "./hulls.w"


main()
{
/*6:*/
#line 73 "./hulls.w"

Vertex*u,*v,*vv,*w;

/*:6*/
#line 20 "./hulls.w"

Graph*g= miles(128,0,0,0,0,0,0);

mems= ccs= 0;
/*7:*/
#line 82 "./hulls.w"

/*5:*/
#line 61 "./hulls.w"

o,u= g->vertices;
v= u+1;
u->z.i= 0;v->z.i= 1;
oo,u->succ= u->pred= v;
oo,v->succ= v->pred= u;
rover= u;
if(n<150)printf("Beginning with (%s; %s)\n",u->name,v->name);

/*:5*/
#line 83 "./hulls.w"
;
for(oo,vv= g->vertices+2;vv<g->vertices+g->n;vv++){
vv->z.i= ++serial_no;
/*9:*/
#line 103 "./hulls.w"

u= rover;
do{
o,v= u->succ;
if(ccw(u,vv,v))goto found;
u= v;
}while(u!=rover);
continue;
found:;

/*:9*/
#line 86 "./hulls.w"
;
/*10:*/
#line 114 "./hulls.w"

if(u==rover){
while(1){
o,w= u->pred;
if(w==v)break;
if(ccw(vv,w,u))break;
u= w;
}
rover= w;
}
while(1){
if(v==rover)break;
o,w= v->succ;
if(ccw(w,vv,v))break;
v= w;
}
oo,u->succ= v->pred= vv;
oo,vv->pred= u;vv->succ= v;
if(n<150)printf("New hull sequence (%s; %s; %s)\n",u->name,vv->name,v->name);

/*:10*/
#line 88 "./hulls.w"
;
}
/*8:*/
#line 95 "./hulls.w"

u= rover;
printf("The convex hull is:\n");
do{
printf("  %s\n",u->name);
u= u->succ;
}while(u!=rover);

/*:8*/
#line 90 "./hulls.w"
;

/*:7*/
#line 24 "./hulls.w"
;
printf("Total of %d mems and %d calls on ccw.\n",mems,ccs);
}

/*:1*/
