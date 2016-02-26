#define buf_size 200 \

#define large 1000000000
#define larg 100000000 \

#define nodes_per_block 1000 \

#define stack_size 20
#define save_size 1000 \

#define operand(n) stack[stack_ptr-(n) ] \

#define check_stack(k) if(stack_ptr<k) { \
printf("Not enough items on the stack for operator %c!\n",op) ; \
goto dump_stack; \
} \

#define max_tree 1000 \

#define lg_large 29 \

#define align_to(c) while(col<c) {col++;putchar(' ') ;}
#define print_digs(n) {align_to(c) ;printf("%d",n) ;col+= dwidth(n) ;}
#define print_char(n) {align_to(c) ;putchar(n) ;col++;} \

/*1:*/
#line 85 "tcalc.w"

#include <stdio.h> 

/*11:*/
#line 202 "tcalc.w"

typedef struct node_struct{
int val;
struct node_struct*l,*r;
}node;

/*:11*/
#line 88 "tcalc.w"

/*2:*/
#line 118 "tcalc.w"

char*helps[128];
char buf[buf_size];
char*loc;
char op;
int param;

/*:2*//*12:*/
#line 211 "tcalc.w"

int used;
node*cur_node;
node*bad_node;
node*avail;
int mems;

/*:12*//*24:*/
#line 451 "tcalc.w"

int easy;

/*:24*//*28:*/
#line 524 "tcalc.w"

node*saved[save_size];
int save_ptr;
node*stack[stack_size+1];
int stack_ptr;
int showing_mems;
int showing_size;
int showing_usage;
int old_mems;

/*:28*//*45:*/
#line 749 "tcalc.w"

int cat[20];

/*:45*//*50:*/
#line 844 "tcalc.w"

int threshold;
int max_display_size= max_tree;


/*:50*//*53:*/
#line 885 "tcalc.w"

int width[max_tree]= {1};
int height[max_tree];
int code[max_tree];
int lcode[max_tree];
int count;

/*:53*//*58:*/
#line 991 "tcalc.w"

int h;
int col;

/*:58*//*63:*/
#line 1060 "tcalc.w"

int time_stamp= large;

/*:63*/
#line 89 "tcalc.w"

/*13:*/
#line 220 "tcalc.w"

node*get_avail()
{
register node*p;
if(avail){
p= avail;avail= p->r;
}else{
if(cur_node==bad_node){
cur_node= (node*)calloc(nodes_per_block,sizeof(node));
if(!cur_node){
printf("Omigosh, the memory is all gone!\n");exit(-1);
}
bad_node= cur_node+nodes_per_block;
}
p= cur_node++;
}
p->l= p->r= NULL;
mems++;
used++;
return p;
}

/*:13*//*14:*/
#line 242 "tcalc.w"

void free_node(p)
node*p;
{
p->r= avail;
avail= p;
used--;
mems++;
}

/*:14*//*15:*/
#line 254 "tcalc.w"

void recycle(p)
node*p;
{
if(!p)return;
recycle(p->l);
recycle(p->r);
free_node(p);
}

/*:15*//*16:*/
#line 272 "tcalc.w"

node*left(p)
node*p;
{
mems++;
return p->l;
}

node*right(p)
node*p;
{
mems++;
return p->r;
}

void change(p,q)
node**p;
node*q;
{
*p= q;
mems++;
}

/*:16*//*17:*/
#line 300 "tcalc.w"

node*copy(p)
node*p;
{
register node*q;
if(!p)return NULL;
q= get_avail();
change(&q->l,copy(left(p)));
change(&q->r,copy(right(p)));
return q;
}

/*:17*//*18:*/
#line 315 "tcalc.w"

node*cheap_copy(p)
node*p;
{
register node*q;
register int m= mems;
q= copy(p);
mems= m;
return q;
}

/*:18*//*19:*/
#line 330 "tcalc.w"

int compare(p,q)

node*p,*q;
{
register int k;
if(!p){
if(!q)return 0;
return-1;
}
if(!q)return 1;
k= compare(left(p),left(q));
if(k!=0)return k;
return compare(right(p),right(q));
}

/*:19*//*55:*/
#line 908 "tcalc.w"

int dwidth(n)
int n;
{
register int j,k;
for(j= 1,k= 10;n>=k;j++,k*= 10);
return j;
}

/*:55*//*60:*/
#line 1024 "tcalc.w"

int eval(p)
node*p;
{
register int lv,rv;
if(!p)return 0;
lv= eval(p->l);
rv= eval(p->r);
p->val= (lv<=lg_large?1<<lv:large)+rv;
if(p->val> large)p->val= large;
return p->val;
}

/*:60*//*62:*/
#line 1047 "tcalc.w"

void stamp(p)
node*p;
{
if(!p)return;
stamp(p->l);
stamp(p->r);
if(p->val==time_stamp)
printf("***Node overlap!!\n");
p->val= time_stamp;
count++;
}

/*:62*/
#line 90 "tcalc.w"

/*20:*/
#line 363 "tcalc.w"

void succ(p)
node*p;
{
register node*pr,*pl,*prr,*prl;
pr= right(p);
if(!pr){
pr= get_avail();change(&p->r,pr);
}else succ(pr);
prr= right(pr);
if(!prr){

pl= left(p);
prl= left(pr);
if(compare(pl,prl)==0){
recycle(pr);
change(&p->r,NULL);
if(pl)succ(pl);
else change(&p->l,get_avail());
}
}
}

/*:20*//*21:*/
#line 404 "tcalc.w"

node*sum(p,q)
node*p,*q;
{
register node*pl,*ql;
register int s;
easy= 1;
if(!p)return q;
if(!q)return p;
pl= left(p);ql= left(q);
s= compare(pl,ql);
if(s==0){
/*23:*/
#line 444 "tcalc.w"

recycle(ql);
if(pl)succ(pl);
else change(&p->l,get_avail());
change(&p->r,sum(right(p),right(q)));
free_node(q);

/*:23*/
#line 416 "tcalc.w"

easy= 0;return p;
}else{
if(s<0)/*22:*/
#line 439 "tcalc.w"

{
pl= ql;ql= p;p= q;q= ql;
}

/*:22*/
#line 419 "tcalc.w"
;
q= sum(right(p),q);
if(easy)goto no_sweat;
else{
ql= left(q);
s= compare(pl,ql);
if(s==0){
change(&p->r,right(q));
recycle(ql);free_node(q);
if(pl)succ(pl);
else change(&p->l,get_avail());
return p;
}
else easy= 1;
}
no_sweat:change(&p->r,q);
return p;
}
}

/*:21*//*25:*/
#line 457 "tcalc.w"

node*normalize(p)
node*p;
{
register node*q,*qq;
if(!p)return;
q= qq= left(p);q= normalize(q);
if(q!=qq)change(&p->l,q);
q= qq= right(p);q= normalize(q);
change(&p->r,NULL);
return sum(p,q);
}

/*:25*//*26:*/
#line 474 "tcalc.w"

node*ez_prod(p,q)
node*p,*q;
{
register node*qq,*qqr;
if(!q){
recycle(p);return NULL;
}
for(qq= q;qq;qq= qqr){
qqr= right(qq);
if(qqr)change(&qq->l,sum(left(qq),copy(p)));
else change(&qq->l,sum(left(qq),p));
}
return q;
}

/*:26*//*27:*/
#line 500 "tcalc.w"

node*prod(p,q)
node*p,*q;
{
register node*pp,*ppr,*ss;
if(!p||!q){
recycle(p);recycle(q);return NULL;
}
for(pp= p,ss= NULL;pp;pp= ppr){
ppr= right(pp);
if(ppr)ss= sum(ss,ez_prod(left(pp),copy(q)));
else ss= sum(ss,ez_prod(left(pp),q));
free_node(pp);
}
return ss;
}

/*:27*//*44:*/
#line 729 "tcalc.w"

node*normal_tree(n)
int n;
{
register int k;
register node*p;
if(!n)return NULL;
for(k= 0;(1<<k)<=n;k++);
p= get_avail();mems--;
p->l= normal_tree(k-1);
p->r= normal_tree(n-(1<<(k-1)));
return p;
}

/*:44*//*47:*/
#line 765 "tcalc.w"

node*btree(n,m)
int n,m;
{
register node*p;
register int k;
if(!n)return NULL;
for(k= 0;cat[k]*cat[n-1-k]<=m;k++)m-= cat[k]*cat[n-1-k];
p= get_avail();mems--;
p->l= btree(k,(int)(m/cat[n-1-k]));
p->r= btree(n-1-k,m%cat[n-1-k]);
return p;
}

/*:47*//*54:*/
#line 892 "tcalc.w"

void get_stats(p)
node*p;
{
register int j,jl,jr;
if(!p)return;
get_stats(p->l);get_stats(p->r);
jl= (p->l?p->l->val:0);
jr= (p->r?p->r->val:0);
p->val= j= ++count;
if(count<max_display_size)
/*56:*/
#line 923 "tcalc.w"

{
register int tjl;
tjl= (code[jl]<=lg_large?1<<code[jl]:large);
if(tjl<=threshold){
if(code[jr]<tjl&&tjl+code[jr]<=threshold){
code[j]= tjl+code[jr];
lcode[j]= 0;
width[j]= dwidth(code[j]);
height[j]= 0;
}else{
code[j]= large;
lcode[j]= tjl;
width[j]= dwidth(tjl)+width[jr]+1;
height[j]= height[jr];
}
}else{
code[j]= large;
width[j]= width[jl]+width[jr];
if(p->r==0)lcode[j]= 0;
else lcode[j]= large,width[j]+= 2;
height[j]= 1+height[jl];
if(height[jr]> height[j])height[j]= height[jr];
}
}

/*:56*/
#line 903 "tcalc.w"
;
}

/*:54*//*57:*/
#line 966 "tcalc.w"

void print_rep(p,l,c)
node*p;
int l,c;
{
register int j= (p?p->val:0);
if(code[j]<large){
if(l==h)print_digs(code[j]);
}else if(lcode[j]&&lcode[j]<large){
if(l==h)print_digs(lcode[j]);
}else{
register int jl= (p->l?p->l->val:0);
if(l==h)print_char('2');
if(l<h&&l+1+height[jl]>=h)print_rep(p->l,l+1,c+1);
}
if(lcode[j]){
register jr= p->r->val;
if(l+height[jr]>=h){
c+= width[j]-width[jr]-1;
if(l==h)print_char('+');
print_rep(p->r,l,c+1);
}
}
}

/*:57*/
#line 91 "tcalc.w"


main()
{
register int k;
register node*p;
/*5:*/
#line 148 "tcalc.w"

/*4:*/
#line 144 "tcalc.w"

helps['h']= ".helpful summary of all known operators";


/*:4*//*7:*/
#line 162 "tcalc.w"

helps['q']= ".quit the program";

/*:7*//*31:*/
#line 561 "tcalc.w"

helps['S']= ":show tree sizes, if <n> is nonzero";
helps['T']= ":show computation time in mems, if <n> is nonzero";
helps['U']= ":show node usage, if <n> is nonzero";
helps['k']= ":kill %<n> to conserve memory";

/*:31*//*33:*/
#line 580 "tcalc.w"

helps['%']= ":recall a previously computed tree";
helps['d']= ":duplicate a tree that's already on the stack";

/*:33*//*36:*/
#line 615 "tcalc.w"

helps['p']= ".pop the top tree off the stack";
helps['x']= ".exchange the top two trees";

/*:36*//*38:*/
#line 633 "tcalc.w"

helps['l']= ".replace tree by its log (the left subtree)";
helps['r']= ".replace tree by its remainder (the right subtree)";
helps['s']= ".replace tree by its successor";
helps['n']= ".normalize a tree";
helps['+']= ".replace a,b by a+b";
helps['*']= ".replace a,b by ab";
helps['^']= ".replace a,b by a^b, assuming that a is a power of 2";
helps['j']= ".replace a,b by 2^a+b";
helps['m']= ".replace a,b by 2^a b";

/*:38*//*48:*/
#line 779 "tcalc.w"

helps['t']= ":the standard tree that represents <n>";
helps['b']= ":the binary tree of rank <n> in lexicographic order";

/*:48*//*51:*/
#line 849 "tcalc.w"

helps['M']= ".use maximum possible compression threshold for tree display";
helps['N']= ":compress tree displays only for t0..t<n>";
helps['O']= ":omit display of trees having <n> or more nodes";

/*:51*/
#line 149 "tcalc.w"
;

/*:5*//*46:*/
#line 756 "tcalc.w"

cat[0]= 1;
for(k= 1;k<20;k++){
register int quot= cat[k-1]/(k+1),rem= cat[k-1]%(k+1);
cat[k]= (4*k-2)*quot+(int)(((4*k-2)*rem)/(k+1));
}

/*:46*/
#line 97 "tcalc.w"
;
while(1)/*3:*/
#line 125 "tcalc.w"

{
/*9:*/
#line 171 "tcalc.w"

printf("? ");
if(fgets(buf,buf_size,stdin)==0)return 0;
loc= buf;

/*:9*/
#line 127 "tcalc.w"
;
/*29:*/
#line 534 "tcalc.w"

stack_ptr= 0;
mems= 0;

/*:29*/
#line 128 "tcalc.w"
;
while(1){
/*10:*/
#line 181 "tcalc.w"

while(loc<&buf[buf_size]&&(*loc==' '||*loc<0||*loc>=128))loc++;

param= 0;
if(loc==&buf[buf_size])op= '\n';
else{
op= *loc++;
if(op!='\n')
while(loc<&buf[buf_size]&&(*loc==' '||(*loc<='9'&&*loc>='0'))){
if(*loc!=' '){
if(param>=larg)
printf("(I'm reducing your large parameter mod %d)\n",large);
param= ((param%larg)*10)+*loc-'0';
}
loc++;
}
}

/*:10*/
#line 130 "tcalc.w"
;
switch(op){
case'\n':goto dump_stack;
/*6:*/
#line 153 "tcalc.w"

case'h':printf("The following operators are currently implemented:\n");
for(k= 0;k<128;k++)if(helps[k])
printf("  %c%s %s\n",k,(*helps[k]=='.'?":   ":"<n>:"),helps[k]+1);
break;

/*:6*//*8:*/
#line 165 "tcalc.w"

case'q':printf("Type <return> to confirm quitting:");
if(getchar()=='\n')return 0;
fgets(buf,buf_size,stdin);
goto dump_stack;

/*:8*//*32:*/
#line 567 "tcalc.w"

case'S':showing_size= param;break;
case'T':showing_mems= param;break;
case'U':showing_usage= param;break;
case'k':if(param> save_ptr)
printf("You can't do k%d, because %%%d doesn't exist!\n",param,param);
else{
recycle(saved[param]);saved[param]= NULL;
}
break;

/*:32*//*34:*/
#line 584 "tcalc.w"

case'%':if(param> save_ptr){
printf("(%%%d is unknown; I'm using %%0 instead)\n",param);
param= 0;
}
operand(0)= cheap_copy(saved[param]);
inc_stack:if(stack_ptr<stack_size){
stack_ptr++;
break;
}
printf("Oops---the stack overflowed!\n");
recycle(operand(0));
goto dump_stack;

/*:34*//*35:*/
#line 606 "tcalc.w"

case'd':check_stack(param+1);
operand(0)= cheap_copy(operand(param+1));
goto inc_stack;

/*:35*//*37:*/
#line 621 "tcalc.w"

case'p':check_stack(1);
stack_ptr--;
recycle(operand(0));
break;
case'x':check_stack(2);
p= operand(2);operand(2)= operand(1);operand(1)= p;
break;

/*:37*//*39:*/
#line 646 "tcalc.w"

case'n':check_stack(1);
operand(1)= normalize(operand(1));
break;

/*:39*//*40:*/
#line 653 "tcalc.w"

case's':check_stack(1);
if(operand(1))succ(operand(1));
else operand(1)= get_avail();
break;

/*:40*//*41:*/
#line 662 "tcalc.w"

case'l':check_stack(1);
p= operand(1);
if(!p)printf("(log 0 is undefined; I'm using 0)\n");
else{
operand(1)= left(p);
recycle(right(p));free_node(p);
}
break;
case'r':check_stack(1);
p= operand(1);
if(!p)printf("(rem 0 is undefined; I'm using 0)\n");
else{
operand(1)= right(p);
recycle(left(p));free_node(p);
}
break;

/*:41*//*42:*/
#line 682 "tcalc.w"

case'j':check_stack(2);
stack_ptr--;
p= get_avail();
p->l= operand(1);
p->r= operand(0);
return_p:operand(1)= p;
break;
case'+':check_stack(2);
stack_ptr--;
operand(1)= sum(operand(1),operand(0));
break;
case'*':check_stack(2);
stack_ptr--;
operand(1)= prod(operand(1),operand(0));
break;
case'm':check_stack(2);
stack_ptr--;
operand(1)= ez_prod(operand(1),operand(0));
break;

/*:42*//*43:*/
#line 707 "tcalc.w"

case'^':check_stack(2);
stack_ptr--;
p= operand(1);
if(!p){
if(operand(0))recycle(operand(0));
else p= get_avail();
}else if(right(p)){
printf("Sorry, I don't do a^b unless a is a power of 2!\n");
stack_ptr++;goto dump_stack;
}else change(&p->l,prod(left(p),operand(0)));
goto return_p;

/*:43*//*49:*/
#line 787 "tcalc.w"

case't':operand(0)= normal_tree(param);
goto inc_stack;
case'b':for(k= 0;cat[k]<=param;k++)param-= cat[k];
operand(0)= btree(k,param);
goto inc_stack;

/*:49*//*52:*/
#line 854 "tcalc.w"

case'M':param= large-1;
case'N':threshold= param;
break;
case'O':if(param> max_tree){
printf("(I've changed O%d to the maximum permitted value, O%d)\n",
param,max_tree);
param= max_tree;
}
max_display_size= param;
break;

/*:52*/
#line 133 "tcalc.w"

default:printf("Unknown operator `%c'!\n",op);
}
}
dump_stack:/*30:*/
#line 542 "tcalc.w"

old_mems= mems;
while(stack_ptr){
stack_ptr--;
if(++save_ptr<save_size)k= save_ptr;
else{
k= 0;
recycle(saved[0]);
save_ptr= save_size-1;
}
saved[k]= operand(0);
if(stack_ptr==0&&k> 0){
recycle(saved[0]);saved[0]= copy(saved[k]);
}
/*59:*/
#line 998 "tcalc.w"

count= 0;
p= saved[k];
get_stats(p);
if(count>=max_display_size)printf("%%%d=large",k);
else for(h= (p?height[count]:0);h>=0;h--){
if(h==0)printf("%%%d=",k);
col= (h==0?dwidth(k)+2:0);
print_rep(p,0,dwidth(k)+2);
if(h)printf("\n");
else if(showing_size){
int c= dwidth(k)+2+width[count];
align_to(c);
}
}
if(showing_size)printf(" (%d nodes)\n",count);
else printf("\n");

/*:59*/
#line 556 "tcalc.w"
;
}
if(showing_mems&&old_mems)printf("Operations cost %d mems\n",old_mems);
if(showing_usage)printf("(%d nodes are now in use)\n",used);

/*:30*/
#line 137 "tcalc.w"
;
/*61:*/
#line 1041 "tcalc.w"

++time_stamp;
count= 0;
for(k= 0;k<=save_ptr;k++)stamp(saved[k]);
if(count!=used)printf("We lost track of %d nodes!\n",used-count);

/*:61*/
#line 138 "tcalc.w"
;
}

/*:3*/
#line 98 "tcalc.w"

}

/*:1*/
