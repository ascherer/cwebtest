#define abort(m,i) {fprintf(stderr,m,argv[i]) ;return-1;}
#define stacksize 100
#define forestsize 100 \

#define cocall(q,s) {cur_cnode->state= s; \
if(q) q->caller= cur_cnode,cur_cnode= q; \
else coresult= false; \
goto cogo;}
#define bitchange(b,s) {cur_cnode->bit= b,coresult= true;coreturn(s) ;}
#define coreturn(s) {cur_cnode->state= s,cur_cnode= cur_cnode->caller; \
goto cogo;} \

#define root_cnode cnode_table[nn].child \

#define head (lnode_table+nn)  \

#define rel(f) (lnode_table[k].f?lnode_table[k].f-lnode_table:-1)  \

/*1:*/
#line 37 "./koda-ruskey.w"

#include <stdio.h> 
/*4:*/
#line 130 "./koda-ruskey.w"

typedef enum{false,true}boolean;

typedef struct cnode_struct{
char bit;
char state;
struct cnode_struct*child;
struct cnode_struct*sib;
struct cnode_struct*caller;
}cnode;

/*:4*//*14:*/
#line 310 "./koda-ruskey.w"

typedef struct lnode_struct{
char bit;
struct lnode_struct*left,*right;
struct lnode_struct*lchild;
struct lnode_struct*focus;
}lnode;

/*:14*/
#line 39 "./koda-ruskey.w"

/*3:*/
#line 79 "./koda-ruskey.w"

int stack[stacksize];
int scope[forestsize];
int nn;

/*:3*//*11:*/
#line 237 "./koda-ruskey.w"

cnode cnode_table[forestsize+1];
boolean coresult;

/*:11*//*17:*/
#line 364 "./koda-ruskey.w"

lnode lnode_table[forestsize+1];
boolean been_there_and_done_that;

/*:17*/
#line 40 "./koda-ruskey.w"


int main(int argc,char*argv[])
{
register int j,k,l;
/*2:*/
#line 61 "./koda-ruskey.w"

if(argc!=2||argv[1][0]!='(')
abort("Usage: %s \"nestedparens\"\n",0);
for(j= k= l= 0;argv[1][k];k++)
if(argv[1][k]=='('){
stack[l++]= j;
if(l==stacksize)
abort("Stack overflow --- \"%s\" is too deep for me!\n",1);
}else if(argv[1][k]==')'){
if(--l<0)
abort("Extra right parenthesis in \"%s\"!\n",1);
scope[j++]= stack[l];
if(j==forestsize)
abort("Memory overflow --- \"%s\" is too big!\n",1);
}else abort("The forest spec \"%s\" should contain only parentheses!\n",1);
if(l)abort("Missing right parenthesis in \"%s\"!\n",1);
nn= j;

/*:2*/
#line 45 "./koda-ruskey.w"
;
printf("Bitstrings generated from \"%s\":\n",argv[1]);
/*9:*/
#line 215 "./koda-ruskey.w"

{
register cnode*cur_cnode;
/*10:*/
#line 227 "./koda-ruskey.w"

scope[nn]= 0;
for(k= 0;k<=nn;k++)if(scope[k]<k){
cnode_table[k].child= cnode_table+k-1;
for(j= k-1;scope[j]> scope[k];j= scope[j]-1)
cnode_table[j].sib= cnode_table+scope[j]-1;
}
cur_cnode= cnode_table+nn;
goto upward_step;

/*:10*/
#line 218 "./koda-ruskey.w"
;
/*5:*/
#line 166 "./koda-ruskey.w"

cogo:switch(cur_cnode->state){
/*6:*/
#line 175 "./koda-ruskey.w"

case 0:bitchange(1,1);

/*:6*//*7:*/
#line 190 "./koda-ruskey.w"

case 1:cocall(cur_cnode->child,2);
case 2:if(coresult)coreturn(1);
cocall(cur_cnode->sib,3);
case 3:coreturn(4);

/*:7*//*8:*/
#line 205 "./koda-ruskey.w"

case 4:cocall(cur_cnode->child,5);
case 5:if(coresult)coreturn(4);
bitchange(0,6);
case 6:cocall(cur_cnode->sib,7);
case 7:coreturn(0);

/*:8*//*12:*/
#line 246 "./koda-ruskey.w"

case 8:if(coresult){
upward_step:/*13:*/
#line 259 "./koda-ruskey.w"

for(k= 0;k<nn;k++)putchar('0'+cnode_table[k].bit);
putchar('\n');

/*:13*/
#line 248 "./koda-ruskey.w"
;
cocall(root_cnode,8);
}
printf("... and now we generate them in reverse:\n");
goto downward_step;
case 9:if(coresult){
downward_step:/*13:*/
#line 259 "./koda-ruskey.w"

for(k= 0;k<nn;k++)putchar('0'+cnode_table[k].bit);
putchar('\n');

/*:13*/
#line 254 "./koda-ruskey.w"
;
cocall(root_cnode,9);
}
break;

/*:12*/
#line 168 "./koda-ruskey.w"
;
default:abort("%s: Unknown state code (this can't happen)!\n",0);
}

/*:5*/
#line 219 "./koda-ruskey.w"
;
}

/*:9*/
#line 47 "./koda-ruskey.w"
;
printf("\nTrying again, looplessly:\n");
/*15:*/
#line 320 "./koda-ruskey.w"

{
register lnode*p,*q,*r;
/*16:*/
#line 350 "./koda-ruskey.w"

for(k= 0;k<=nn;k++){
lnode_table[k].focus= lnode_table+k;
if(scope[k]<k){
for(j= k-1;scope[j]> scope[k];j= scope[j]-1){
lnode_table[j].left= lnode_table+scope[j]-1;
lnode_table[scope[j]-1].right= lnode_table+j;
}
lnode_table[k].lchild= lnode_table+j;
}
}
head->left= head-1,(head-1)->right= head;
head->right= head->lchild,head->lchild->left= head;

/*:16*/
#line 323 "./koda-ruskey.w"
;
while(1){
/*22:*/
#line 392 "./koda-ruskey.w"

for(k= 0;k<nn;k++)putchar('0'+lnode_table[k].bit);
putchar('\n');

/*:22*/
#line 325 "./koda-ruskey.w"
;
/*18:*/
#line 368 "./koda-ruskey.w"

q= head->left;
p= q->focus;
q->focus= q;

/*:18*/
#line 327 "./koda-ruskey.w"
;
if(p!=head){
if(p->bit==0){
p->bit= 1;
/*19:*/
#line 373 "./koda-ruskey.w"

if(p->lchild){
q= p->right;
q->left= p-1,(p-1)->right= q;
p->right= p->lchild,p->lchild->left= p;
}

/*:19*/
#line 331 "./koda-ruskey.w"
;
}else{
p->bit= 0;
/*20:*/
#line 380 "./koda-ruskey.w"

if(p->lchild){
q= (p-1)->right;
p->right= q,q->left= p;
}

/*:20*/
#line 334 "./koda-ruskey.w"
;
}
}else if(been_there_and_done_that)break;
else{
printf("... and now we generate them in reverse:\n");
been_there_and_done_that= true;continue;
}
/*21:*/
#line 388 "./koda-ruskey.w"

p->focus= p->left->focus;
p->left->focus= p->left;

/*:21*/
#line 341 "./koda-ruskey.w"
;
}
}

/*:15*/
#line 49 "./koda-ruskey.w"
;
return 0;
}

/*:1*/
