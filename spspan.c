#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define oooo mems+= 4
#define call oo \

#define verbose (argc> 2) 
#define extraverbose (argc> 3)  \

#define abort(mess) {fprintf(stderr,"Parsing error: %.*s|%s, %s!\n", \
p-argv[1],argv[1],p,mess) ;exit(-1) ;} \

#define maxn 1000 \

#define isleaf(p) ((p) <nodelist+maxn)  \

#define leafname(p) ('a'+((p) -nodelist) ) 
#define branchname(p) ('A'+((p) -root) ) 
#define nodename(p) (isleaf(p) ?leafname(p) :branchname(p) )  \

#define easy(p) o,p->typ==p->val \

#define done rchild \

/*1:*/
#line 37 "spspan.w"

#include <stdio.h> 
/*6:*/
#line 101 "spspan.w"

typedef struct node_struct{
int typ;
struct node_struct*lchild;
struct node_struct*rchild;
struct node_struct*rsib;
/*13:*/
#line 229 "spspan.w"

int val;
struct node_struct*des;

/*:13*//*22:*/
#line 376 "spspan.w"

struct node_struct*leaf;
struct node_struct*parent;

/*:22*//*25:*/
#line 464 "spspan.w"

struct node_struct*focus;

/*:25*/
#line 107 "spspan.w"

}node;

/*:6*/
#line 39 "spspan.w"

/*3:*/
#line 79 "spspan.w"

int stack[maxn];
int llink[maxn],rlink[maxn];

/*:3*//*7:*/
#line 113 "spspan.w"

node nodelist[maxn+maxn];
node*curleaf;
node*curnode;
node*root,*topnode;

/*:7*/
#line 40 "spspan.w"

unsigned int trees,mems;
/*9:*/
#line 139 "spspan.w"

node*build(int stackitem,node*par)
{
register node*p,*l,*r,*lc,*rc;
register int t,j;
if(stackitem==0)return curleaf++;
t= stackitem>>8,j= stackitem&0xff;
if(t!=par->typ)p= ++curnode,p->typ= t;
else p= par;
l= build(llink[j],p),lc= l->lchild,rc= l->rchild,r= build(rlink[j],p);
if(l==p)/*11:*/
#line 158 "spspan.w"

if(r==p)/*12:*/
#line 162 "spspan.w"

rc->rsib= p->lchild,p->lchild= lc,p->rchild->rsib= lc;

/*:12*/
#line 159 "spspan.w"

else p->rchild= r,rc->rsib= r,r->rsib= lc;

/*:11*/
#line 149 "spspan.w"

else if(r==p)/*10:*/
#line 155 "spspan.w"

r= p->lchild,p->lchild= l,l->rsib= r,p->rchild->rsib= l;

/*:10*/
#line 150 "spspan.w"

else p->lchild= l,p->rchild= r,l->rsib= r,r->rsib= l;
return p;
}

/*:9*//*15:*/
#line 247 "spspan.w"

void init_tree(node*p,node*par)
{
register node*q;
ooo,p->val= (par->des==p?par->val:par->typ);
if(isleaf(p))/*26:*/
#line 472 "spspan.w"

p->leaf= p,p->parent= par;

/*:26*/
#line 252 "spspan.w"

else{
oo,p->des= p->lchild;
for(q= p->lchild;;q= q->rsib){
call,init_tree(q,p);
if(o,q->rsib==p->lchild)break;
}
/*27:*/
#line 475 "spspan.w"

p->leaf= p->des->leaf,p->parent= par;
o,p->focus= p;

/*:27*/
#line 259 "spspan.w"
;
}
}

/*:15*//*16:*/
#line 277 "spspan.w"

node*xx(char c)
{
if(c>='a')return nodelist+(c-'a');
return nodelist+maxn+(c-'@');
}

/*:16*//*17:*/
#line 284 "spspan.w"

void printleaf(node*p)
{
printf("%c:%c rsib=%c\n",
leafname(p),p->val+'0',nodename(p->rsib));
}

void printbranch(node*p)
{
printf("%c:%c rsib=%c lchild=%c des=%c rchild=%c",
branchname(p),p->val+'0',nodename(p->rsib),
nodename(p->lchild),nodename(p->des),nodename(p->rchild));
/*28:*/
#line 479 "spspan.w"

printf(" leaf=%c",leafname(p->leaf));
if(p->focus!=p)printf(" focus=%c",branchname(p->focus));

/*:28*/
#line 296 "spspan.w"
;
printf("\n");
}

void printnode(node*p)
{
if(isleaf(p))printleaf(p);
else printbranch(p);
}

/*:17*//*18:*/
#line 306 "spspan.w"

void printtree(node*p,int indent)
{
register node*q;
register int k;
for(k= 0;k<indent;k++)printf(" ");
printnode(p);
if(!isleaf(p))for(q= p->lchild;;q= q->rsib){
printtree(q,indent+1);
if(q->rsib==p->lchild)break;
}
}

/*:18*//*19:*/
#line 319 "spspan.w"

void printedges(node*p)
{
register node*q;
if(isleaf(p)){
if(p->val)printf("%c",leafname(p));
}else for(q= p->lchild;;q= q->rsib){
printedges(q);
if(q->rsib==p->lchild)break;
}
}

/*:19*/
#line 42 "spspan.w"

main(int argc,char*argv[])
{
register int j,k;
if(argc==1){
fprintf(stderr,"Usage: %s SPformula [[gory] details]\n",argv[0]);
exit(0);
}
/*2:*/
#line 66 "spspan.w"

{
register char*p= argv[1];
for(j= k= 0;*p;p++)
if(*p=='-')/*4:*/
#line 87 "spspan.w"

stack[k++]= 0;

/*:4*/
#line 70 "spspan.w"

else if(*p=='s'||*p=='p')/*5:*/
#line 90 "spspan.w"

{
if(k<2)abort("missing operand");
rlink[++j]= stack[--k];
llink[j]= stack[k-1];
stack[k-1]= (*p=='s'?0x100:0)+j;
}

/*:5*/
#line 71 "spspan.w"

else abort("bad symbol");
if(k!=1)abort("disconnected graph");
/*8:*/
#line 123 "spspan.w"

curleaf= nodelist;
topnode= curnode= nodelist+maxn;
curnode->typ= 2;
root= build(stack[0],curnode);
root->rsib= root;

/*:8*/
#line 74 "spspan.w"
;
}

/*:2*/
#line 50 "spspan.w"
;
/*14:*/
#line 238 "spspan.w"

o,topnode->typ= 1;
call,init_tree(root,topnode);
trees= 1;
if(verbose)/*20:*/
#line 331 "spspan.w"

{
if(extraverbose)printtree(root,0);
printf("The first spanning tree is ");
printedges(root);
printf(".\n");
}

/*:20*/
#line 242 "spspan.w"
;

/*:14*/
#line 51 "spspan.w"
;
printf(" (%u mems to get started)\n",mems);mems= 0;
/*29:*/
#line 485 "spspan.w"

topnode->focus= topnode;
while(1){
register node*p,*q,*l,*r;
for(r= curnode;easy(r);r--);
oo,p= r->focus,r->focus= r;
if(p==topnode)break;
/*31:*/
#line 509 "spspan.w"

oo,l= p->des,r= l->rsib;
o,k= p->val;
for(q= l;;o,q= q->des){
o,q->val= k^1;
if(isleaf(q))break;
}
if(verbose)printf(" %c%c",k?'-':'+',leafname(q));
for(q= r;;o,q= q->des){
o,q->val= k;
if(isleaf(q))break;
}
if(verbose)printf("%c%c\n",k?'+':'-',leafname(q));
o,p->des= r,trees++;
for(q= p;q->des==r;r= q,q= q->parent)q->leaf= r->leaf;

if(extraverbose){
printedges(root);
printf("; now %c->leaf=%c\n",branchname(r),leafname(r->leaf));
}

/*:31*/
#line 492 "spspan.w"
;
if(o,p->des==p->done)/*30:*/
#line 499 "spspan.w"

{
o,p->done= l;
for(l= p-1;easy(l);l--);
ooo,p->focus= l->focus,l->focus= l;
}

/*:30*/
#line 493 "spspan.w"
;
}

/*:29*/
#line 53 "spspan.w"
;
printf("Altogether %u spanning trees, %u additional mems.\n",trees,mems);
}

/*:1*/
