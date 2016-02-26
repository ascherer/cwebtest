#define deg u.I \

#define taken v.I \

#define vert w.V
#define ark x.A \

/*1:*/
#line 17 "ham.w"

#include "gb_graph.h" 
#include "gb_save.h" 

main(int argc,char*argv[])
{
Graph*g;
Vertex*x,*y,*z,*tmax;
register Vertex*t,*u,*v;
register Arc*a,*aa;
register int d;
Arc*b,*bb;
int count= 0;
int dmin,modulus;
/*2:*/
#line 50 "ham.w"

if(argc> 1)g= restore_graph(argv[1]);else g= NULL;
if(argc<3||sscanf(argv[2],"%d",&modulus)!=1)modulus= 1000000000;
if(!g||modulus<=0){
fprintf(stderr,"Usage: %s foo.gb [modulus]\n",argv[0]);
exit(-1);
}

/*:2*/
#line 31 "ham.w"
;
/*3:*/
#line 63 "ham.w"

dmin= g->n;
for(v= g->vertices;v<g->vertices+g->n;v++){
v->taken= 0;
d= 0;
for(a= v->arcs;a;a= a->next)d++;
v->deg= d;
if(d<dmin)dmin= d,x= v;
}

/*:3*/
#line 32 "ham.w"
;
for(v= g->vertices;v<g->vertices+g->n;v++)printf(" %d",v->deg);
printf("\n");
if(x->deg<2){
printf("The minimum degree is %d (vertex %s)!\n",x->deg,x->name);
return-1;
}
for(b= x->arcs;b->next;b= b->next)for(bb= b->next;bb;bb= bb->next){
v= b->tip;
z= bb->tip;
/*4:*/
#line 90 "ham.w"

t= g->vertices;tmax= t+g->n-1;
x->taken= 1;t->vert= x;
t->ark= NULL;
advance:/*5:*/
#line 104 "ham.w"

t++;
t->vert= v;
v->taken= 1;
if(v==z){
if(t==tmax)/*9:*/
#line 153 "ham.w"

{
count++;
if(count%modulus==0){
printf("%d: ",count);
for(u= g->vertices;u<=tmax;u++)printf("%s ",u->vert->name);
printf("\n");
}
}

/*:9*/
#line 109 "ham.w"
;
goto backtrack;
}
for(aa= v->arcs,y= NULL;aa;aa= aa->next){
u= aa->tip;
d= u->deg-1;
if(d==1&&u->taken==0){
if(y)goto restore;
y= u;
}
u->deg= d;
}
if(y){
t->ark= NULL;
v= y;
goto advance;
}
a= v->arcs;

/*:5*/
#line 96 "ham.w"
;
try:/*7:*/
#line 132 "ham.w"

while(a){
v= a->tip;
if(v->taken==0){
t->ark= a;
goto advance;
}
a= a->next;
}
restore_all:aa= NULL;

/*:7*/
#line 97 "ham.w"
;
restore:/*6:*/
#line 129 "ham.w"

for(a= t->vert->arcs;a!=aa;a= a->next)a->tip->deg++;

/*:6*/
#line 99 "ham.w"
;
backtrack:/*8:*/
#line 143 "ham.w"

t->vert->taken= 0;
t--;
if(t->ark){
a= t->ark->next;
goto try;
}
if(t==g->vertices)goto done;
goto restore_all;

/*:8*/
#line 101 "ham.w"
;
done:

/*:4*/
#line 43 "ham.w"
;
}
printf("Altogether %d solutions.\n",count);
for(v= g->vertices;v<g->vertices+g->n;v++)printf(" %d",v->deg);
printf("\n");
}

/*:1*/
