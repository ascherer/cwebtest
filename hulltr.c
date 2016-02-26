#define o mems++
#define oo mems+= 2
#define ooo mems+= 3 \

/*1:*/
#line 12 "./hulltr.w"

#include "gb_graph.h"
#include "gb_miles.h"
#include "gb_rand.h"
/*3:*/
#line 57 "./hulltr.w"

typedef struct node_struct{
struct vertex_struct*vert;
struct node_struct*succ,*pred,*left,*right,*parent;
long prio;
}node;

/*:3*/
#line 16 "./hulltr.w"

int n= 128;
/*2:*/
#line 37 "./hulltr.w"

int mems;
int ccs;

/*:2*//*5:*/
#line 69 "./hulltr.w"

node*head;
node*next_node;
Area working_storage;
int serial_no= 1;

/*:5*/
#line 18 "./hulltr.w"

/*9:*/
#line 123 "./hulltr.w"

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

/*:9*//*14:*/
#line 212 "./hulltr.w"

rotup(p,pp,q,qp)
node*p;
node**pp;
node*q;
long qp;
{node*pr,**ppr;
node*qq;
while(1){
o,pr= p->parent;
if(o,pr->right==p)ppr= &(pr->right);
else ppr= &(pr->left);
if(pp==&(p->right)){
if(n<150)printf("...(rotating left)\n");
o,qq= q->left;
o,q->left= p;o,p->parent= q;
o,p->right= qq;
if(qq!=NULL)o,qq->parent= p;
}else{
if(n<150)printf("...(rotating right)\n");
o,qq= q->right;
o,q->right= p;o,p->parent= q;
o,p->left= qq;
if(qq!=NULL)o,qq->parent= p;
}
if(o,qp>=pr->prio)break;
p= pr;pp= ppr;
}
o,q->parent= pr;o,*ppr= q;
}

/*:14*//*16:*/
#line 268 "./hulltr.w"

deldown(p,pp,ql,qr,qlp,qrp)
node*p;
node**pp;
node*ql,*qr;
int qlp,qrp;
{node*qq;
if(qlp<qrp){
if(n<150)printf("...(moving left child up)\n");
o,ql->parent= p;o,*pp= ql;
o,qq= ql->right;
if(qq!=NULL)
o,deldown(ql,&(ql->right),qq,qr,qq->prio,qrp);
else{
o,ql->right= qr;o,qr->parent= ql;
}
}else{
if(n<150)printf("...(moving right child up)\n");
o,qr->parent= p;o,*pp= qr;
o,qq= qr->left;
if(qq!=NULL)
o,deldown(qr,&(qr->left),ql,qq,qlp,qq->prio);
else{
o,qr->left= ql;o,ql->parent= qr;
}
}
}

/*:16*//*19:*/
#line 363 "./hulltr.w"

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
}/*:19*/
#line 19 "./hulltr.w"


main()
{
/*7:*/
#line 100 "./hulltr.w"

Vertex*u,*v,*vv,*w;
node*p,*pp,*q,*qq,*qqq,*r,*rr,*s,*ss,*tt,**par,**ppar,*prepar,*preppar;
int replaced;

/*:7*/
#line 23 "./hulltr.w"

Graph*g= miles(128,0,0,0,0,0,0);

mems= ccs= 0;
/*10:*/
#line 144 "./hulltr.w"

/*6:*/
#line 79 "./hulltr.w"

init_area(working_storage);
/*4:*/
#line 64 "./hulltr.w"

head= (node*)gb_alloc((g->n)*sizeof(node),working_storage);
if(head==NULL)return(1);
next_node= head;

/*:4*/
#line 81 "./hulltr.w"
;
o,u= g->vertices;
v= u+1;
u->z.i= 0;v->z.i= 1;
p= ++next_node;
ooo,head->succ= head->pred= head->right= p;
oo,p->succ= p->pred= head;
o,p->parent= head;
oo,p->left= p->right= NULL;
gb_init_rand(110);
o,p->prio= gb_next_rand();
o,head->vert= u;
o,p->vert= v;
next_node++;
if(n<150)printf("Beginning with (%s; %s)\n",u->name,v->name);

/*:6*/
#line 145 "./hulltr.w"
;
for(oo,vv= g->vertices+2;vv<g->vertices+g->n;vv++){
vv->z.i= ++serial_no;
o,q= head->pred;
replaced= 0;
o,u= head->vert;
if(o,ccw(vv,u,q->vert))/*12:*/
#line 171 "./hulltr.w"

{qqq= head;
while(1){
o,r= qqq->succ;
if(r==q)break;
if(oo,ccw(vv,qqq->vert,r->vert))break;
/*15:*/
#line 245 "./hulltr.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,prepar= qqq->parent;
if(o,prepar->right==qqq)par= &(prepar->right);
else par= &(prepar->left);
o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= prepar;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= prepar;
}else{
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
oo,deldown(prepar,par,pp,ss,pp->prio,ss->prio);
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:15*/
#line 177 "./hulltr.w"
;
qqq= r;
}
qq= qqq;qqq= q;
while(1){
o,r= qqq->pred;
if(r==qq)break;
if(oo,ccw(vv,r->vert,qqq->vert))break;
/*15:*/
#line 245 "./hulltr.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,prepar= qqq->parent;
if(o,prepar->right==qqq)par= &(prepar->right);
else par= &(prepar->left);
o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= prepar;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= prepar;
}else{
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
oo,deldown(prepar,par,pp,ss,pp->prio,ss->prio);
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:15*/
#line 185 "./hulltr.w"
;
qqq= r;
}
q= qqq;
if(!replaced)/*13:*/
#line 196 "./hulltr.w"

{
tt= next_node++;
o,tt->vert= vv;o,tt->succ= head;o,tt->pred= q;o,head->pred= tt;o,q->succ= tt;
oo,tt->left= tt->right= NULL;
o,tt->prio= gb_next_rand();
if(n<150)printf("(Inserting %s at right of tree, prio=%d)\n",vv->name,tt->prio);
if(o,tt->prio<q->prio)rotup(q,&(q->right),tt,tt->prio);
else{
o,tt->parent= q;o,q->right= tt;
}
}

/*:13*/
#line 189 "./hulltr.w"
;
if(n<150)printf("New hull sequence (%s; %s; %s)\n",
q->vert->name,vv->name,qq->vert->name);
}

/*:12*/
#line 151 "./hulltr.w"

else/*17:*/
#line 296 "./hulltr.w"

{o,qq= head->right;
while(1){
if(qq==q||(o,ccw(u,vv,qq->vert))){
o,r= qq->left;
if(r==NULL){
preppar= qq;
o,ppar= &(qq->left);
break;
}
}else{
o,r= qq->right;
if(r==NULL){
preppar= qq;
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
/*15:*/
#line 245 "./hulltr.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,prepar= qqq->parent;
if(o,prepar->right==qqq)par= &(prepar->right);
else par= &(prepar->left);
o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= prepar;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= prepar;
}else{
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
oo,deldown(prepar,par,pp,ss,pp->prio,ss->prio);
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:15*/
#line 324 "./hulltr.w"
;
}
r= qqq;
}
qqq= qq;
while(1){
if(qqq==q)break;
oo,rr= qqq->succ;
if(oo,ccw(vv,qqq->vert,rr->vert))break;
/*15:*/
#line 245 "./hulltr.w"

if(replaced){
o,pp= qqq->pred;o,tt= qqq->succ;o,pp->succ= tt;o,tt->pred= pp;
o,prepar= qqq->parent;
if(o,prepar->right==qqq)par= &(prepar->right);
else par= &(prepar->left);
o,pp= qqq->left;
if(o,(ss= qqq->right)==NULL){
if(n<150)printf("(Deleting %s from tree, case 1)\n",qqq->vert->name);
o,*par= pp;
if(pp!=NULL)o,pp->parent= prepar;
}else if(pp==NULL){
if(n<150)printf("(Deleting %s from tree, case 2)\n",qqq->vert->name);
o,*par= ss;o,ss->parent= prepar;
}else{
if(n<150)printf("(Deleting %s from tree, hard case)\n",qqq->vert->name);
oo,deldown(prepar,par,pp,ss,pp->prio,ss->prio);
}
}else{
o,qqq->vert= vv;
replaced= 1;
}

/*:15*/
#line 333 "./hulltr.w"
;
qqq= rr;
}
if(!replaced)/*18:*/
#line 342 "./hulltr.w"

{
tt= next_node++;
o,tt->vert= vv;o,tt->succ= qq;o,tt->pred= r;o,qq->pred= tt;o,r->succ= tt;
oo,tt->left= tt->right= NULL;
o,tt->prio= gb_next_rand();
if(n<150)printf("(Inserting %s at bottom of tree, prio=%d)\n",vv->name,tt->prio);
if(o,tt->prio<preppar->prio)rotup(preppar,ppar,tt,tt->prio);
else{
o,tt->parent= preppar;o,*ppar= tt;
}
}

/*:18*/
#line 336 "./hulltr.w"
;
if(n<150)printf("New hull sequence (%s; %s; %s)\n",
r->vert->name,vv->name,qqq->vert->name);
}
}

/*:17*/
#line 152 "./hulltr.w"
;
/*8:*/
#line 108 "./hulltr.w"

p= head;count= 0;
do{
count++;
p->prio= (p->prio&0xffff0000)+count;
if(p->succ->pred!=p)
printf("succ/pred failure at %s!\n",p->vert->name);
if(p->left!=NULL&&p->left->parent!=p)
printf("parent/lchild failure at %s!\n",p->vert->name);
if(p->right!=NULL&&p->right->parent!=p)
printf("parent/rchild failure at %s!\n",p->vert->name);
p= p->succ;
}while(p!=head);
count= 1;inorder(head->right);

/*:8*/
#line 153 "./hulltr.w"
;
}
/*11:*/
#line 160 "./hulltr.w"

p= head;
printf("The convex hull is:\n");
do{
printf("  %s\n",p->vert->name);
p= p->succ;
}while(p!=head);

/*:11*/
#line 155 "./hulltr.w"
;

/*:10*/
#line 27 "./hulltr.w"
;
printf("Total of %d mems and %d calls on ccw.\n",mems,ccs);
}

/*:1*/
