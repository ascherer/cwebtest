#define o mems++
#define oo mems+= 2
#define ooo mems+= 3 \

/*1:*/
#line 13 "./hullt.w"

#include "gb_graph.h"
#include "gb_miles.h"
#include "gb_rand.h"
/*3:*/
#line 62 "./hullt.w"

typedef struct node_struct{
struct vertex_struct*vert;
struct node_struct*succ,*pred,*left,*right,**parent;
long prio;
}node;

/*:3*/
#line 17 "./hullt.w"

int n= 128;
/*2:*/
#line 38 "./hullt.w"

int mems;
int ccs;

/*:2*//*5:*/
#line 74 "./hullt.w"

node*head;
node*next_node;
Area working_storage;
int serial_no= 1;

/*:5*/
#line 19 "./hullt.w"

/*9:*/
#line 126 "./hullt.w"

int count;
inorder(p)
node*p;
{if(p){
inorder(p->left);
if((p->prio&0xffff)!=++count){
printf("tree node %d is missing at %d: %s!\n",count,p->prio&0xffff,
p->vert->name);
count= p->prio&0xffff;
}
inorder(p->right);
}
}

/*:9*//*17:*/
#line 297 "./hullt.w"

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
}/*:17*/
#line 20 "./hullt.w"


main()
{
/*7:*/
#line 105 "./hullt.w"

Vertex*u,*v,*vv,*w;
node*p,*pp,*q,*qq,*qqq,*r,*rr,*s,*ss,*tt,**par,**ppar;
int replaced;

/*:7*/
#line 24 "./hullt.w"

Graph*g= miles(128,0,0,0,0,0,0);

mems= ccs= 0;
/*10:*/
#line 147 "./hullt.w"

/*6:*/
#line 84 "./hullt.w"

init_area(working_storage);
/*4:*/
#line 69 "./hullt.w"

head= (node*)gb_alloc((g->n)*sizeof(node),working_storage);
if(head==NULL)return(1);
next_node= head;

/*:4*/
#line 86 "./hullt.w"
;
o,u= g->vertices;
v= u+1;
u->z.i= 0;v->z.i= 1;
p= ++next_node;
ooo,head->succ= head->pred= head->right= p;
oo,p->succ= p->pred= head;
o,p->parent= &(head->right);
oo,p->left= p->right= NULL;
gb_init_rand();
p->prio= gb_next_rand();
o,head->vert= u;
o,p->vert= v;
next_node++;
if(n<150)printf("Beginning with (%s; %s)\n",u->name,v->name);

/*:6*/
#line 148 "./hullt.w"
;
for(oo,vv= g->vertices+2;vv<g->vertices+g->n;vv++){
vv->z.i= ++serial_no;
o,q= head->pred;
replaced= 0;
o,u= head->vert;
if(o,ccw(vv,u,q->vert))/*12:*/
#line 174 "./hullt.w"

{qqq= head;
while(1){
o,r= qqq->succ;
if(r==q)break;
if(oo,ccw(vv,qqq->vert,r->vert))break;
/*14:*/
#line 209 "./hullt.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,par= qqq->parent;o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= par;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= par;
}else{
o,tt->left= pp;o,pp->parent= &(tt->left);
if(ss!=tt){
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
o,ppar= tt->parent;
o,pp= tt->right;
o,*ppar= pp;
if(pp!=NULL)o,pp->parent= ppar;
o,tt->right= ss;o,ss->parent= &(tt->right);
}else if(n<150)printf("(Deleting %s from tree, case 3)\n",qqq->vert->name);
o,*par= tt;o,tt->parent= par;
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:14*/
#line 180 "./hullt.w"
;
qqq= r;
}
qq= qqq;qqq= q;
while(1){
o,r= qqq->pred;
if(r==qq)break;
if(oo,ccw(vv,r->vert,qqq->vert))break;
/*14:*/
#line 209 "./hullt.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,par= qqq->parent;o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= par;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= par;
}else{
o,tt->left= pp;o,pp->parent= &(tt->left);
if(ss!=tt){
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
o,ppar= tt->parent;
o,pp= tt->right;
o,*ppar= pp;
if(pp!=NULL)o,pp->parent= ppar;
o,tt->right= ss;o,ss->parent= &(tt->right);
}else if(n<150)printf("(Deleting %s from tree, case 3)\n",qqq->vert->name);
o,*par= tt;o,tt->parent= par;
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:14*/
#line 188 "./hullt.w"
;
qqq= r;
}
q= qqq;
if(!replaced)/*13:*/
#line 199 "./hullt.w"

{
tt= next_node++;
o,tt->vert= vv;o,tt->succ= head;o,tt->pred= q;o,head->pred= tt;o,q->succ= tt;
oo,tt->left= tt->right= NULL;o,tt->parent= &(q->right);o,q->right= tt;
tt->prio= gb_next_rand();
}

/*:13*/
#line 192 "./hullt.w"
;
if(n<150)printf("New hull sequence (%s; %s; %s)\n",
q->vert->name,vv->name,qq->vert->name);
}

/*:12*/
#line 154 "./hullt.w"

else/*15:*/
#line 237 "./hullt.w"

{o,qq= head->right;
while(1){
if(qq==q||(o,ccw(u,vv,qq->vert))){
o,r= qq->left;
if(r==NULL){
o,ppar= &(qq->left);
break;
}
}else{
o,r= qq->right;
if(r==NULL){
o,ppar= &(qq->right);
o,qq= qq->succ;
break;
}
}
qq= r;
}
if(o,(r= qq->pred)==head||(oo,ccw(vv,qq->vert,r->vert))){
if(r!=head){
while(1){
qqq= r;
o,r= qqq->pred;
if(r==head)break;
if(oo,ccw(vv,r->vert,qqq->vert))break;
/*14:*/
#line 209 "./hullt.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,par= qqq->parent;o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= par;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= par;
}else{
o,tt->left= pp;o,pp->parent= &(tt->left);
if(ss!=tt){
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
o,ppar= tt->parent;
o,pp= tt->right;
o,*ppar= pp;
if(pp!=NULL)o,pp->parent= ppar;
o,tt->right= ss;o,ss->parent= &(tt->right);
}else if(n<150)printf("(Deleting %s from tree, case 3)\n",qqq->vert->name);
o,*par= tt;o,tt->parent= par;
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:14*/
#line 263 "./hullt.w"
;
}
r= qqq;
}
qqq= qq;
while(1){
if(qqq==q)break;
oo,rr= qqq->succ;
if(oo,ccw(vv,qqq->vert,rr->vert))break;
/*14:*/
#line 209 "./hullt.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,par= qqq->parent;o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= par;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= par;
}else{
o,tt->left= pp;o,pp->parent= &(tt->left);
if(ss!=tt){
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
o,ppar= tt->parent;
o,pp= tt->right;
o,*ppar= pp;
if(pp!=NULL)o,pp->parent= ppar;
o,tt->right= ss;o,ss->parent= &(tt->right);
}else if(n<150)printf("(Deleting %s from tree, case 3)\n",qqq->vert->name);
o,*par= tt;o,tt->parent= par;
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:14*/
#line 272 "./hullt.w"
;
qqq= rr;
}
if(!replaced)/*16:*/
#line 281 "./hullt.w"

{
tt= next_node++;
o,tt->vert= vv;o,tt->succ= qq;o,tt->pred= r;o,qq->pred= tt;o,r->succ= tt;
oo,tt->left= tt->right= NULL;o,tt->parent= ppar;o,*ppar= tt;
tt->prio= gb_next_rand();
}

/*:16*/
#line 275 "./hullt.w"
;
if(n<150)printf("New hull sequence (%s; %s; %s)\n",
r->vert->name,vv->name,qqq->vert->name);
}
}

/*:15*/
#line 155 "./hullt.w"
;
/*8:*/
#line 113 "./hullt.w"

p= head;count= 0;
do{
count++;
p->prio= (p->prio&0xffff0000)+count;
if(p->succ->pred!=p)
printf("succ/pred failure at %s!\n",p->vert->name);
if(p!=head&&*(p->parent)!=p)
printf("parent/child failure at %s!\n",p->vert->name);
p= p->succ;
}while(p!=head);
count= 1;inorder(head->right);

/*:8*/
#line 156 "./hullt.w"
;
}
/*11:*/
#line 163 "./hullt.w"

p= head;
printf("The convex hull is:\n");
do{
printf("  %s\n",p->vert->name);
p= p->succ;
}while(p!=head);

/*:11*/
#line 158 "./hullt.w"
;

/*:10*/
#line 28 "./hullt.w"
;
printf("Total of %d mems and %d calls on ccw.\n",mems,ccs);
}

/*:1*/
