#define o mems++
#define oo mems+= 2 \

#define succ u.v
#define pred v.v
#define inst w.a \

/*1:*/
#line 12 "hull.w"

#include "gb_graph.h"
#include "gb_miles.h"
int n= 128;
/*2:*/
#line 34 "hull.w"

int mems;
int ccs;

/*:2*//*5:*/
#line 65 "hull.w"

Arc*first_inst;
Arc*next_inst;
Vertex*rover;
Area working_storage;
int serial_no= 1;

/*:5*/
#line 16 "hull.w"

/*13:*/
#line 184 "hull.w"

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
}/*:13*/
#line 17 "hull.w"


main()
{
/*7:*/
#line 94 "hull.w"

Vertex*u,*v,*vv,*w;
Arc*p,*q,*r,*s;

/*:7*/
#line 21 "hull.w"

Graph*g= miles(128,0,0,0,0,0,0);

mems= ccs= 0;
/*8:*/
#line 104 "hull.w"

/*6:*/
#line 76 "hull.w"

init_area(working_storage);
/*4:*/
#line 60 "hull.w"

first_inst= (Arc*)gb_alloc((4*g->n-2)*sizeof(Arc),working_storage);
if(first_inst==NULL)return(1);
next_inst= first_inst;

/*:4*/
#line 78 "hull.w"
;
o,u= g->vertices;
v= u+1;
u->z.i= 0;v->z.i= 1;
oo,u->succ= u->pred= v;
oo,v->succ= v->pred= u;
oo,first_inst->tip= u;first_inst->next= first_inst;
oo,(++next_inst)->tip= v;next_inst->next= first_inst;
o,u->inst= first_inst;
o,v->inst= next_inst++;
rover= u;
if(n<150)printf("Beginning with (%s; %s)\n",u->name,v->name);

/*:6*/
#line 105 "hull.w"
;
for(oo,vv= g->vertices+2;vv<g->vertices+g->n;vv++){
vv->z.i= ++serial_no;
/*10:*/
#line 125 "hull.w"

p= first_inst;
do{
if(oo,ccw(p->tip,vv,(p+1)->tip))p++;
q= p;
o,p= p->next;
}while(p> first_inst);
if(p==NULL)continue;
o,v= q->tip;
o,u= v->pred;

/*:10*/
#line 108 "hull.w"
;
/*11:*/
#line 137 "hull.w"

o,q->next= next_inst;
while(1){
o,w= u->pred;
if(w==v)break;
if(ccw(vv,w,u))break;
o,u->inst->next= next_inst;
if(rover==u)rover= w;
u= w;
}
while(1){
o,w= v->succ;
if(w==u)break;
if(ccw(w,vv,v))break;
if(rover==v)rover= w;
v= w;
o,v->inst->next= next_inst;
}
oo,u->succ= v->pred= vv;
oo,vv->pred= u;vv->succ= v;
/*12:*/
#line 159 "hull.w"

o,next_inst->tip= vv;
o,next_inst->next= first_inst;
o,vv->inst= next_inst;
next_inst++;
o,next_inst->tip= u;
o,next_inst->next= next_inst+1;
next_inst++;
o,next_inst->tip= v;
o,next_inst->next= first_inst;
o,v->inst= next_inst;
next_inst++;
o,next_inst->tip= vv;
o,next_inst->next= NULL;
next_inst++;
if(n<150)printf("New hull sequence (%s; %s; %s)\n",u->name,vv->name,v->name);

/*:12*/
#line 157 "hull.w"
;

/*:11*/
#line 110 "hull.w"
;
}
/*9:*/
#line 117 "hull.w"

u= rover;
printf("The convex hull is:\n");
do{
printf("  %s\n",u->name);
u= u->succ;
}while(u!=rover);

/*:9*/
#line 112 "hull.w"
;

/*:8*/
#line 25 "hull.w"
;
printf("Total of %d mems and %d calls on ccw.\n",mems,ccs);
}

/*:1*/
