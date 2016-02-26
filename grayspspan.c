/*1:*/
#line 32 "./grayspspan.w"

#include "gb_graph.h"
#include "gb_save.h"
#define verbose (xargc> 2) 
#define extraverbose (xargc> 3) 
#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define oooo mems+= 4
#define ooooo mems+= 5
#define oooooo mems+= 6 \

#define isleaf(b) ((b) <=topleaf) 
#define action bondbase
#define tree (&treehead)  \

#define deg u.I
#define prev a.A
#define bond b.A
#define mate(e) (edge_trick&(siz_t) (e) ?(e) -1:(e) +1) 
#define bn(b) ((b) -bondbase) 
#define ebn(e) bn((Bond*) ((e) ->bond) ) 
#define delete(e) ee= e,oooo,ee->prev->next= ee->next,ee->next->prev= ee->prev
#define undelete(e) ee= e,oooo,ee->next->prev= ee,ee->prev->next= ee \

#define stack(k) (g->vertices+(k) ) ->z.V
#define mark v.I \

#define bonds(l) (g->vertices+l) ->y.I
#define aa(l) (g->vertices+l) ->x.A \

#define dup w.A \

#define bfs v.V \

#define makeseries(e,f) buildbond(1,e,f) 
#define makeparallel(e,f) buildbond(0,e,f)  \

#define easy(b) o,b->typ==b->val \


#line 35 "./grayspspan.w"

double mems;
double count;
Graph*gg;
int xargc;
/*5:*/
#line 136 "./grayspspan.w"

typedef struct bond_struct{
int typ;
int val;
struct bond_struct*lchild;
struct bond_struct*lsib;
struct bond_struct*rsib;
struct bond_struct*des;
struct bond_struct*done;
struct bond_struct*scope;
struct bond_struct*focus;
struct bond_struct*left;
struct bond_struct*right;
struct bond_struct*up;
struct bond_struct*down;
struct bond_struct*lsave,*rsave;
Arc*lhist,*rhist;
}Bond;

/*:5*/
#line 40 "./grayspspan.w"

/*7:*/
#line 171 "./grayspspan.w"

Bond*bondbase;
int bondcount;
Bond*topleaf;
Bond treehead;
Bond*inbond,*outbond;
Bond James;

/*:7*/
#line 41 "./grayspspan.w"

/*9:*/
#line 202 "./grayspspan.w"

void printgraph(void)
{
register Vertex*v;
register Arc*e;
for(v= gg->vertices;v<gg->vertices+gg->n;v++)if(v->mark>=0){
printf("Bonds from %s:",v->name);
for(e= v->arcs->next;e->tip;e= e->next)
printf(" %s(%d)",e->tip->name,ebn(e));
printf("\n");
}
}

/*:9*//*10:*/
#line 215 "./grayspspan.w"

void printbond(Bond*b)
{
printf("%d:%c",bn(b),isleaf(b)?'l':b->typ==2?'r':b->typ==1?'s':'p');
if(b->typ==2){
printf(" lhist=%d",ebn(b->lhist));
if(b->rhist)printf(" rhist=%d",ebn(b->lhist));
}else{
printf("%c",b->val+'0');
if(b->lsib)printf(" lsib=%d, rsib=%d",bn(b->lsib),bn(b->rsib));
if(!isleaf(b)){
if(b->focus!=b)printf(" focus=%d",bn(b->focus));
printf(" lchild=%d, scope=%d, des=%d, done=%d",
bn(b->lchild),bn(b->scope),bn(b->des),bn(b->done));
}
}
printf("\n");
}

/*:10*//*11:*/
#line 234 "./grayspspan.w"

void printaction(void)
{
register Bond*b;
for(b= action->right;b!=action;b= b->right)printbond(b);
}

void printtree(void)
{
register Bond*b;
for(b= tree->down;b!=tree;b= b->down)printbond(b);
}

void printleaves(void)
{
register Bond*b;
for(b= bondbase+1;b<=topleaf;b++)printbond(b);
}

/*:11*//*12:*/
#line 263 "./grayspspan.w"

/*16:*/
#line 338 "./grayspspan.w"

int bondsanity(Bond*b)
{
int bugs= 0;
register Bond*a,*extent;
register int j,k;
extent= b;
if(b->left->right!=b||b->right->left!=b)
bugs++,printf("Link failure at bond %d\n",bn(b));
for(a= b->lchild,j= k= 0;a!=b->lchild||k==0;a= a->rsib,k++){
if(a<=bondbase||a> bondbase+bondcount){
bugs++,printf("Bond %d has a child out of range\n",bn(b));
break;
}
if(a->lsib->rsib!=a||a->rsib->lsib!=a)
bugs++,printf("Sibling link failure at bond %d\n",bn(a));
if(a==b->des)j= 1;
else if(a->val!=b->typ)
bugs++,printf("Bond %d should have value %d\n",bn(a),b->typ);
if(!isleaf(a)){
if(a->left!=extent)
bugs++,printf("Preorder failure: bond %d doesn't follow %d\n",
bn(a),bn(extent));
bugs+= bondsanity(a);
extent= a->scope;
}
}
if(!j)
bugs++,printf("Bond %d doesn't designate any of its children\n",bn(b));
else if(b->done!=b->des->lsib)
bugs++,printf("Bond %d should be done at %d\n",bn(b),bn(b->des->lsib));
if(b->scope!=extent)
bugs++,printf("Bond %d should have scope %d\n",bn(b),bn(extent));
return bugs;
}

/*:16*/
#line 264 "./grayspspan.w"
;
int sanitycheck(int flags)
{
register Vertex*u,*v;
register Arc*e;
register Bond*b;
register int k,n;
int bugs= 0;
if(flags&1)/*13:*/
#line 278 "./grayspspan.w"

{
for(n= 0,v= gg->vertices;v<gg->vertices+gg->n;v++)
if(v->mark>=0)/*14:*/
#line 288 "./grayspspan.w"

{
n++;
if(v->mark)bugs++,printf("Vertex %s is marked\n",v->name);
if((v->deg<3&&v->deg!=0)||v->deg>=gg->n)
bugs++,printf("Vertex %s has degree %d\n",v->name,v->deg);
for(k= 0,e= v->arcs;k<=v->deg;k++,e= e->next){
if(e->prev->next!=e||e->next->prev!=e)
bugs++,printf("Link failure at vertex %s, bond %d\n",v->name,k);
if(k> 0){
b= (Bond*)e->bond;
if(b<=bondbase||b> bondbase+bondcount)
bugs++,printf("Vertex %s has bad bond %d\n",v->name,k);
else if(b->lhist!=e&&b->lhist!=mate(e))
bugs++,printf("Bond %d has bad lhist pointer\n",bn(b));
if(mate(e)->bond!=e->bond)
bugs++,printf("Vertex %s has unmated bond %d\n",v->name,k);
if(mate(e)->tip!=v)
bugs++,printf("Vertex %s's bond %d has wrong mate tip\n",v->name,k);
u= e->tip;
if(!u)bugs++,printf("Vertex %s has bad tip %d\n",v->name,k);
else if(u->mark<0)
bugs++,printf("Vertex %s points to deleted vertex %s\n",
v->name,u->name);
}
}
if(e!=v->arcs)
bugs++,printf("Vertex %s has more than %d bonds\n",v->name,v->deg);
}

/*:14*/
#line 281 "./grayspspan.w"
;
}

/*:13*/
#line 272 "./grayspspan.w"
;
if(flags&2)/*15:*/
#line 320 "./grayspspan.w"

{
if(action->left->right!=action||action->right->left!=action)
bugs++,printf("Link failure at head of action list\n");
for(b= action->right;b!=action;b= b->right){
if(b->val==1&&(!b->up||b->up->down!=b))
bugs++,printf("Bond %d isn't properly in the tree list\n",bn(b));
if(b->lsib||b->rsib)
bugs++,printf("Top level bond %d has siblings\n",bn(b));
if(isleaf(b))
bugs++,printf("Leaf %d is in the action list\n",bn(b));
else{
bugs+= bondsanity(b);
b= b->scope;
}
}
}

/*:15*/
#line 273 "./grayspspan.w"
;
if(flags&4)/*17:*/
#line 376 "./grayspspan.w"

{
if(tree->up->down!=tree||tree->down->up!=tree)
bugs++,printf("Link failure at head of tree list\n");
for(b= tree->down,k= 0;b!=tree;k++,b= b->down){
if(b->up->down!=b||b->down->up!=b)
bugs++,printf("Link failure in tree list at bond %d\n",bn(b));
if(b->val!=1)
bugs++,printf("Bond %d in the tree list has value 0\n",bn(b));
}
if((flags&1)&&(k!=n-1-(inbond==NULL)))
bugs++,printf("The tree list holds %d bonds, not %d\n",
k,n-1-(inbond==NULL));
}

/*:17*/
#line 274 "./grayspspan.w"
;
return bugs;
}

/*:12*//*31:*/
#line 733 "./grayspspan.w"

void buildbond(int t,Arc*aa,Arc*bb)
{
register Bond*a,*b,*c,*d;
register int at,av,bt,bv;
register Vertex*u,*v;
register Arc*ee;
bondcount++,c= bondbase+bondcount;
oooo,c->typ= t,c->lhist= aa,c->rhist= bb,c->focus= c;
oo,a= (Bond*)aa->bond,b= (Bond*)bb->bond;
oo,av= a->val,at= a->typ,bv= b->val,bt= b->typ;
if(t)/*32:*/
#line 767 "./grayspspan.w"

{
if(extraverbose)
printf(" %d=series(%d,%d) between %s and %s\n",
bn(c),bn(a),bn(b),aa->tip->name,bb->tip->name);
oooo,ee= mate(bb),aa->prev= ee->prev,aa->next= ee->next;
oo,aa->prev->next= aa->next->prev= aa;
oo,mate(aa)->tip= bb->tip;
}

/*:32*/
#line 744 "./grayspspan.w"

else/*33:*/
#line 777 "./grayspspan.w"

{
oo,u= aa->tip,v= mate(aa)->tip;
if(extraverbose)
printf(" %d=parallel(%d,%d) between %s and %s\n",
bn(c),bn(a),bn(b),u->name,v->name);
oooo,delete(bb),delete(mate(bb)),u->deg--,v->deg--;
}

/*:33*/
#line 745 "./grayspspan.w"
;
oo,mate(bb)->bond= aa->bond;
oo,aa->bond= mate(aa)->bond= (Arc*)c;
if(isleaf(a))
if(isleaf(b))/*34:*/
#line 786 "./grayspspan.w"

{
oooo,a->lsib= a->rsib= b,b->lsib= b->rsib= a;
o,c->lchild= a;
ooo,c->right= action->right,c->right->left= c;
oo,c->left= action,action->right= c;
o,c->scope= c;
}

/*:34*/
#line 749 "./grayspspan.w"

else/*35:*/
#line 795 "./grayspspan.w"

{
if(bt!=t){
oooo,a->lsib= a->rsib= b,b->lsib= b->rsib= a;
ooo,c->right= b,c->scope= b->scope;
}else{
oooo,a->rsib= b->lchild,a->lsib= b->lchild->lsib;
oo,a->rsib->lsib= a->lsib->rsib= a;
oooo,c->right= b->right,c->scope= (b->scope==b?c:b->scope);
}
o,c->lchild= a;
oo,c->left= b->left;
oo,c->left->right= c->right->left= c;
}

/*:35*/
#line 750 "./grayspspan.w"

else if(isleaf(b))/*36:*/
#line 810 "./grayspspan.w"

{
if(at!=t){
oooo,a->lsib= a->rsib= b,b->lsib= b->rsib= a;
oooo,c->right= c->lchild= a,c->scope= a->scope;
}else{
oooo,b->rsib= a->lchild,b->lsib= a->lchild->lsib;
oo,b->rsib->lsib= b->lsib->rsib= b;
oooo,c->right= a->right,c->lchild= a->lchild;
oo,c->scope= (a->scope==a?c:a->scope);
}
oo,c->left= a->left;
oo,c->left->right= c->right->left= c;
}

/*:36*/
#line 751 "./grayspspan.w"

else/*37:*/
#line 825 "./grayspspan.w"

{
ooo,d= a->scope,c->rsave= d->right;
oooo,a->left->right= d->right,d->right->left= a->left;
ooo,c->left= b->left,c->left->right= c;
if(at!=t){
oo,c->lsave= a->left;
ooo,c->lchild= a,c->right= a,a->left= c;
if(bt!=t){
oooo,a->lsib= a->rsib= b,b->lsib= b->rsib= a;
oo,d->right= b,b->left= d;
ooo,c->scope= b->scope;
}else{
oooo,a->rsib= b->lchild,a->lsib= b->lchild->lsib;
oo,a->lsib->rsib= a->rsib->lsib= a;
ooo,d->right= b->right,d->right->left= d;
oo,c->scope= (b->scope==b?d:b->scope);
}
}else if(bt!=t){
ooo,c->lchild= b->rsib= a->lchild;
oo,b->lsib= a->lchild->lsib;
oo,b->lsib->rsib= b->rsib->lsib= b;
if(d==a)o,c->right= b;
else oooo,c->right= a->right,d->right= b,b->left= d;
o,c->right->left= c;
oo,c->scope= b->scope;
}else{
if(d==a)oo,c->right= b->right;
else ooooo,c->right= a->right,d->right= b->right,b->right->left= d;
o,c->right->left= c;
oo,c->lchild= a->lchild;
oooo,d= a->lchild->lsib,a->lchild->lsib= b->lchild->lsib;
ooo,b->lchild->lsib->rsib= a->lchild,b->lchild->lsib= d,d->rsib= b->lchild;
oo,c->scope= (b->scope==b?(a->scope==a?c:a->scope):b->scope);
}
}

/*:37*/
#line 752 "./grayspspan.w"
;
if(av)oooo,a->up->down= a->down,a->down->up= a->up;
if(bv)oooo,b->up->down= b->down,b->down->up= b->up;
if(av==bv||bv==t)
if(isleaf(a)||at!=t)o,c->des= a;
else oo,c->des= a->des;
else if(isleaf(b)||bt!=t)o,c->des= b;
else oo,c->des= b->des;
oooo,c->val= c->des->val,c->done= c->des->lsib;
if(c->val)ooooo,c->up= tree->up,tree->up= c->up->down= c,c->down= tree;
}

/*:31*//*38:*/
#line 865 "./grayspspan.w"

void deletebonds(Arc*aa,Arc*bb)
{
register Bond*a,*c;
register Vertex*u,*v;
register Arc*ee;
bondcount++,c= bondbase+bondcount;
ooo,c->typ= 2,c->lhist= aa,c->rhist= bb;
oo,u= mate(aa)->tip,a= (Bond*)aa->bond;
o,v= (bb?mate(bb)->tip:aa->tip);
if(extraverbose)
printf(" %d: deleting bond %d between %s and %s%s\n",
bn(c),bn(a),u->name,bb?"":"endpoint ",v->name);
oo,delete(aa),u->deg--;
if(bb)oo,delete(bb),v->deg--;
}

/*:38*//*39:*/
#line 900 "./grayspspan.w"

void unbuildbond()
{
register Bond*a,*b,*c,*d;
register int t,at,bt;
register Vertex*u,*v;
register Arc*aa,*bb,*ee;
c= bondbase+bondcount,bondcount--;
ooo,t= c->typ,aa= c->lhist,bb= c->rhist;
if(t> 1)/*46:*/
#line 1042 "./grayspspan.w"

{
oo,u= mate(aa)->tip,a= (Bond*)aa->bond;
o,v= (bb?mate(bb)->tip:aa->tip);
if(bb)oo,undelete(bb),v->deg++;
else v->mark= 0;
oo,undelete(aa),u->deg++;
if(extraverbose)
printf(" restoring bond %d between %s and %s%s\n",
bn(a),u->name,bb?"":"endpoint ",v->name);
if(a->val)ooooo,a->up= tree->up,tree->up= a->up->down= a,a->down= tree;
return;
}

/*:46*/
#line 909 "./grayspspan.w"

oo,a= (Bond*)mate(bb)->bond,b= (Bond*)bb->bond;
o,mate(bb)->bond= (Arc*)b;
oo,aa->bond= mate(aa)->bond= (Arc*)a;
oo,at= a->typ,bt= b->typ;
if(t)/*40:*/
#line 926 "./grayspspan.w"

{
undelete(mate(bb));
oo,v= ee->tip,mate(aa)->tip= v,v->mark= 0;
ooo,aa->prev= v->arcs,aa->next= bb;
if(extraverbose)
printf(" removing %d=series(%d,%d) between %s and %s\n",
bn(c),bn(a),bn(b),aa->tip->name,bb->tip->name);
}

/*:40*/
#line 914 "./grayspspan.w"

else/*41:*/
#line 936 "./grayspspan.w"

{
oo,u= aa->tip,v= mate(aa)->tip;
oooo,undelete(mate(bb)),undelete(bb),u->deg++,v->deg++;
if(extraverbose)
printf(" removing %d=parallel(%d,%d) between %s and %s\n",
bn(c),bn(a),bn(b),u->name,v->name);
}

/*:41*/
#line 915 "./grayspspan.w"
;
if(isleaf(a))
if(isleaf(b))/*42:*/
#line 949 "./grayspspan.w"

{
a->lsib= a->rsib= b->lsib= b->rsib= NULL;
ooo,action->right= c->right,action->right->left= action;
}

/*:42*/
#line 917 "./grayspspan.w"

else/*43:*/
#line 958 "./grayspspan.w"

{
if(bt!=t){
a->lsib= a->rsib= b->lsib= b->rsib= NULL;
oo,b->left= c->left;
}else{
if(o,c->des==a)o,b->val= bt;
else oo,b->val= c->val;
if(b->val!=bt)b->des= c->des;
oooo,b->lchild->lsib= a->lsib,a->lsib->rsib= b->lchild;
ooo,b->done= b->des->lsib;
a->lsib= a->rsib= NULL;
oo,b->right->left= b;
}
oo,b->left->right= b;
}

/*:43*/
#line 918 "./grayspspan.w"

else if(isleaf(b))/*44:*/
#line 975 "./grayspspan.w"

{
if(at!=t){
a->lsib= a->rsib= b->lsib= b->rsib= NULL;
oo,a->left= c->left;
}else{
if(o,c->des==b)o,a->val= at;
else oo,a->val= c->val;
if(a->val!=at)a->des= c->des;
oooo,a->lchild->lsib= b->lsib,b->lsib->rsib= a->lchild;
ooo,a->done= a->des->lsib;
b->rsib= b->lsib= NULL;
oo,a->right->left= a;
}
oo,a->left->right= a;
}

/*:44*/
#line 919 "./grayspspan.w"

else/*45:*/
#line 996 "./grayspspan.w"

{
ooo,d= a->scope,d->right= c->rsave;
if(at!=t){
oo,a->left= c->lsave;
if(bt!=t){
a->lsib= a->rsib= b->lsib= b->rsib= NULL;
ooo,b->left= c->left,b->left->right= b;
}else{
if(o,c->des==a)o,b->val= bt;
else oo,b->val= c->val;
if(b->val!=bt)b->des= c->des;
oooo,b->lchild->lsib= a->lsib,a->lsib->rsib= b->lchild;
ooo,b->done= b->des->lsib;
oooo,b->left->right= b->right->left= b;
a->lsib= a->rsib= NULL;
}
}else{
if(d!=a)oo,a->right->left= a;
if(bt!=t){
if(o,c->des==b)o,a->val= at;
else oo,a->val= c->val;
if(a->val!=at)a->des= c->des;
oooo,a->lchild->lsib= b->lsib,b->lsib->rsib= a->lchild;
ooo,a->done= a->des->lsib;
oooo,b->left= c->left,b->left->right= b;
b->lsib= b->rsib= NULL;
}else{
if(c->val==t)oo,a->val= b->val= t;
else for(ooo,d= c->des;;o,d= d->lsib)
if(d==a->lchild){
ooo,a->val= 1-t,b->val= t,a->des= c->des;break;
}else if(d==b->lchild){
ooo,b->val= 1-t,a->val= t,b->des= c->des;break;
}
oooooo,d= a->lchild->lsib,a->lchild->lsib= b->lchild->lsib,
b->lchild->lsib= d;
oo,d->rsib= b->lchild,a->lchild->lsib->rsib= a->lchild;
ooooo,a->done= a->des->lsib,b->done= b->des->lsib;
d= a->scope;
oooo,b->left->right= b->right->left= b;
}
}
oooo,a->left->right= a,d->right->left= d;
}

/*:45*/
#line 920 "./grayspspan.w"
;
if(c->val)oooo,c->up->down= c->down,c->down->up= c->up;
if(a->val)ooooo,a->up= tree->up,tree->up= a->up->down= a,a->down= tree;
if(b->val)ooooo,b->up= tree->up,tree->up= b->up->down= b,b->down= tree;
}

/*:39*/
#line 42 "./grayspspan.w"

main(int argc,char*argv[])
{
/*3:*/
#line 68 "./grayspspan.w"

register Graph*g;
register int n;
register int m;
register int l;
register int k;
register Vertex*u,*v,*w;
register Arc*e,*ee,*f,*ff;
register Bond*b;

/*:3*/
#line 45 "./grayspspan.w"
;
/*2:*/
#line 52 "./grayspspan.w"

if(argc<2){
fprintf(stderr,"Usage: %s foo.gb [[gory] details]\n",argv[0]);
exit(-11);
}
xargc= argc;
gg= g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,
"Sorry, can't create the graph from file %s! (error code %d)\n",
argv[1],panic_code);
exit(-2);
}
/*6:*/
#line 164 "./grayspspan.w"

m= g->m/2;
bondbase= (Bond*)calloc(3*m,sizeof(Bond));
action->left= action->right= action,action->typ= 2,action->focus= action;

tree->up= tree->down= tree;

/*:6*/
#line 65 "./grayspspan.w"
;
/*18:*/
#line 413 "./grayspspan.w"

for(v= g->vertices+1;v<=g->vertices+g->n;v++)v->mark= 0;
if(verbose)printf("Graph %s has the following edges:\n",g->id);
v= g->vertices,stack(0)= v,k= 1,v->mark= 2,n= 1,m= 0;
while(k){
o,v= stack(--k);
f= gb_virgin_arc();f->next= v->arcs;
for(v->deg= 0,e= v->arcs,v->arcs= f;e;v->deg++,f= e,e= e->next){
looky:u= e->tip;
if(u==v){
f->next= e= e->next;
if(!e)break;
goto looky;
}
e->prev= f;
if(mate(e)->tip!=v){
fprintf(stderr,"Graph %s has an arc from %s to %s,\n",
g->id,u->name,v->name);
fprintf(stderr," but the edge trick doesn't find the opposite arc!\n");
exit(-3);
}
if(o,!e->bond){
ooo,m++,b= bondbase+m,e->bond= mate(e)->bond= (Arc*)b,b->lhist= e;
if(verbose)printf(" %d: %s -- %s\n",m,v->name,u->name);
}
if(o,!u->mark){
ooo,u->mark= 2,stack(k++)= u,b= (Bond*)e->bond,b->val= 1;
ooo,n++,b->up= tree->up,tree->up->down= b,tree->up= b,b->down= tree;
}
}
v->arcs->prev= f,f->next= v->arcs;
}
if(n<g->n){
fprintf(stderr,"Oops, the graph isn't connected!\n");
exit(-4);
}
o,topleaf= bondbase+m,bondcount= m;

/*:18*/
#line 66 "./grayspspan.w"
;

/*:2*/
#line 46 "./grayspspan.w"
;
if(n> 1)/*25:*/
#line 600 "./grayspspan.w"

{
for(k= 0;k<n;k++)o,stack(k)= g->vertices+k;

o,b= tree->up,b->val= 0;
oo,tree->up= b->up,tree->up->down= tree;
if(verbose){
printf("Start with the near-spanning edges");
for(b= tree->down;b!=tree;b= b->down)printf(" %d",bn(b));
printf("\n");
}
l= 0;
/*19:*/
#line 488 "./grayspspan.w"

reduce:while(k){
o,v= stack(--k);
if(o,v->mark> 1)/*20:*/
#line 506 "./grayspspan.w"

{
for(oo,e= v->arcs->next;o,e->tip;o,e= e->next)o,e->tip->dup= NULL;
for(e= v->arcs->next;o,e->tip;o,e= e->next){
u= e->tip;
if(o,u->dup){
makeparallel(u->dup,e);
if(o,!u->mark)oo,u->mark= 1,stack(k++)= u;
}else o,u->dup= e;
}
}

/*:20*/
#line 491 "./grayspspan.w"
;
if(o,v->deg<3)/*21:*/
#line 520 "./grayspspan.w"

{
v->mark= -1;
if(v->deg==2)/*23:*/
#line 559 "./grayspspan.w"

{
oooooo,e= v->arcs->next,f= e->next,u= e->tip,w= f->tip,b= (Bond*)e->bond;
if(o,!b->val){
o,b= (Bond*)f->bond;
if(o,!b->val){
if(inbond){
fprintf(stderr,"I've doubly goofed (inbond set)!\n");
exit(-6);
}
inbond= b;
}
}
makeseries(e,f);
if(o,u->mark)o,u->mark= 2;
else oo,u->mark= 2,stack(k++)= u;
if(o,w->mark)o,w->mark= 2;
else oo,w->mark= 2,stack(k++)= w;
}

/*:23*/
#line 523 "./grayspspan.w"

else if(v->deg==1)/*22:*/
#line 537 "./grayspspan.w"

{
ooo,e= v->arcs->next,b= (Bond*)e->bond;
o,u= e->tip;
deletebonds(mate(e),NULL);
if(o,b->val)oooo,b->up->down= b->down,b->down->up= b->up;
else{
if(inbond){
fprintf(stderr,"I've goofed (inbond doubly set)!\n");
exit(-5);
}
inbond= b;
}
if(o,!u->mark)oo,u->mark= 1,stack(k++)= u;
}

/*:22*/
#line 524 "./grayspspan.w"

else{
v->mark= 0;
goto trivialgraph;
}
}

/*:21*/
#line 493 "./grayspspan.w"

else o,v->mark= 0;
}

o,l++,bonds(l)= bondcount;
if(extraverbose)printf("Entering level %d\n",l);
oooo,b= tree->down,tree->down= b->down,tree->down->up= tree;
oo,e= b->lhist,aa(l)= e;
/*24:*/
#line 582 "./grayspspan.w"

oo,u= e->tip,v= mate(e)->tip;
for(oo,f= u->arcs->next;o,f->tip;o,f= f->next)
if(f==mate(e))delete(f);
else o,mate(f)->tip= v;
delete(e);
ooo,v->deg+= u->deg-2;
o,ee= v->arcs;
oooo,f->prev->next= ee->next,ee->next->prev= f->prev;
ooo,f->next->prev= ee,ee->next= f->next;
if(extraverbose)printf(" shrinking %d; now %s has degree %d\n",
ebn(e),v->name,v->deg);
oo,u->mark= -1,v->mark= 2,k= 1,stack(0)= v;

/*:24*/
#line 501 "./grayspspan.w"
;
goto reduce;

/*:19*/
#line 612 "./grayspspan.w"
;
trivialgraph:
/*30:*/
#line 686 "./grayspspan.w"

count++;
if(verbose)printf("%.15g:",count);
if(outbond){
for(b= outbond;;o,b= b->des){
o,b->val= 0;
if(isleaf(b))break;
}
if(verbose)printf("-%d",bn(b));
}
if(!inbond){
fprintf(stderr,"Internal error (no inbond)!\n");
exit(-7);
}
for(b= inbond;;o,b= b->des){
o,b->val= 1;
if(isleaf(b))break;
}
if(verbose){
printf("+%d",bn(b));
if(extraverbose){
printf(" (");
for(b= bondbase+1;b<=topleaf;b++)if(b->val)printf(" %d",bn(b));
printf(" )\n");
}else printf("\n");
}
inbond= outbond= NULL;

/*:30*/
#line 614 "./grayspspan.w"
;
/*47:*/
#line 1062 "./grayspspan.w"

while(1){
register Bond*q,*l,*r;
for(o,r= action->left;easy(r);o,r= r->left);

oo,b= r->focus,r->focus= r;
if(b==action)break;
/*49:*/
#line 1087 "./grayspspan.w"

count++;
oo,l= b->des,r= l->rsib;
o,k= b->val;
for(q= l;;o,q= q->des){
o,q->val= k^1;
if(isleaf(q))break;
}
if(verbose)printf("%.15g: %c%d",count,k?'-':'+',bn(q));
for(q= r;;o,q= q->des){
o,q->val= k;
if(isleaf(q))break;
}
if(verbose){
printf("%c%d",k?'+':'-',bn(q));
if(extraverbose){
printf(" (");
for(q= bondbase+1;q<=topleaf;q++)if(q->val)printf(" %d",bn(q));
printf(" )\n");
}else printf("\n");
}
o,b->des= r;

/*:49*/
#line 1069 "./grayspspan.w"
;
if(o,b->des==b->done)/*48:*/
#line 1076 "./grayspspan.w"

{
o,b->done= l;
for(o,l= b->left;easy(l);o,l= l->left);

ooo,b->focus= l->focus,l->focus= l;
}

/*:48*/
#line 1070 "./grayspspan.w"
;
}

/*:47*/
#line 615 "./grayspspan.w"
;
while(l){
/*29:*/
#line 671 "./grayspspan.w"

o;while(bondcount> bonds(l))unbuildbond();

/*:29*/
#line 617 "./grayspspan.w"
;
o,e= aa(l);
/*26:*/
#line 629 "./grayspspan.w"

ooooo,u= e->tip,v= mate(e)->tip,v->deg-= u->deg-2;
oo,ee= v->arcs,f= u->arcs;
oooo,ee->next= f->prev->next,ee->next->prev= ee;
ooo,f->prev->next= f,f->next->prev= f;
for(o,f= f->next;o,f->tip;o,f= f->next)o,mate(f)->tip= u;
undelete(e),undelete(mate(e));
if(extraverbose)printf(" unshrinking %d; now %s has degree %d\n",
ebn(e),v->name,v->deg);
u->mark= 0;

/*:26*/
#line 619 "./grayspspan.w"
;
l--;
/*27:*/
#line 645 "./grayspspan.w"

oo,u= e->tip,v= mate(e)->tip;
for(o,u->bfs= v,w= u;u!=v;o,u= u->bfs){
for(oo,f= u->arcs->next;o,f->tip;o,f= f->next)
if(o,f->tip->bfs==NULL){
if(f->tip==v){
if(f!=mate(e))
/*28:*/
#line 661 "./grayspspan.w"

{
for(o,u= e->tip;u!=v;o,u->bfs= NULL,u= w)o,w= u->bfs;
outbond= (Bond*)(e->bond);
deletebonds(e,mate(e));
oooo,k= 2,stack(0)= e->tip,stack(1)= mate(e)->tip;
oo,e->tip->mark= mate(e)->tip->mark= 1;
goto reduce;
}

/*:28*/
#line 652 "./grayspspan.w"
;
}else oo,f->tip->bfs= v,w->bfs= f->tip,w= f->tip;
}
}
if(extraverbose)printf("Leaving level %d: %d is a bridge\n",l+1,ebn(e));
for(o,u= e->tip;u!=v;o,u->bfs= NULL,u= w)o,w= u->bfs;

/*:27*/
#line 622 "./grayspspan.w"
;
oooooo,b= (Bond*)e->bond,b->up= tree->up,tree->up= b->up->down= b,b->down= tree;
}
}

/*:25*/
#line 47 "./grayspspan.w"
;
printf("Altogether %.15g spanning trees, using %.15g mems.\n",count,mems);
exit(0);
}

/*:1*/
