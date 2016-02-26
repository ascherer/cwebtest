#define max_level 100
#define max_degree 6000
#define max_cols 6000
#define max_nodes 1000000 \

#define root col_array[0] \

#define buf_size 8*max_cols+3 \

#define panic(m) {fprintf(stderr,"%s!\n%s",m,buf) ;exit(-1) ;} \

/*1:*/
#line 39 "gdance.w"

#include <stdio.h> 
#include <ctype.h> 
#include <string.h> 
/*3:*/
#line 93 "gdance.w"

typedef struct node_struct{
struct node_struct*left,*right;
struct node_struct*up,*down;
struct col_struct*col;
int color;
}node;

/*:3*//*4:*/
#line 114 "gdance.w"

typedef struct col_struct{
node head;
int len;
char name[8];
struct col_struct*prev,*next;
}column;

/*:4*/
#line 43 "gdance.w"

/*2:*/
#line 65 "gdance.w"

int verbose;
int count= 0;
unsigned int hupdates,updates;
unsigned int hpurifs,purifs;
int spacing= 1;
int profile[max_level][max_degree];
unsigned int upd_prof[max_level];
unsigned int pur_prof[max_level];
int maxb= 0;
int maxl= 0;

/*:2*//*8:*/
#line 158 "gdance.w"

column col_array[max_cols+2];
node node_array[max_nodes];
char buf[buf_size];
column*first_nonprim_col;
column*last_nonprim_col;

/*:8*//*14:*/
#line 280 "gdance.w"

int level;
node*choice[max_level];

/*:14*/
#line 44 "gdance.w"

/*6:*/
#line 132 "gdance.w"

print_row(p)
node*p;
{register node*q= p;
register int k;
do{
printf(" %s",q->col->name);
if(q->color)/*27:*/
#line 463 "gdance.w"

printf(":%c",q->color> 0?q->color:q->col->head.color);

/*:27*/
#line 139 "gdance.w"
;
q= q->right;
}while(q!=p);
for(q= p->col->head.down,k= 1;q!=p;k++)
if(q==&(p->col->head)){
printf("\n");return;
}else q= q->down;
printf(" (%d of %d)\n",k,p->col->len);
}

/*:6*//*15:*/
#line 288 "gdance.w"

cover(c)
column*c;
{register column*l,*r;
register node*rr,*nn,*uu,*dd;
register int k= 1;
l= c->prev;r= c->next;
l->next= r;r->prev= l;
for(rr= c->head.down;rr!=&(c->head);rr= rr->down)
for(nn= rr->right;nn!=rr;nn= nn->right){
uu= nn->up;dd= nn->down;
uu->down= dd;dd->up= uu;
k++;
nn->col->len--;
}
updates+= k;
if(updates>=1000000000)hupdates++,updates-= 1000000000;
upd_prof[level]+= k;
}

/*:15*//*16:*/
#line 312 "gdance.w"

uncover(c)
column*c;
{register column*l,*r;
register node*rr,*nn,*uu,*dd;
for(rr= c->head.up;rr!=&(c->head);rr= rr->up)
for(nn= rr->left;nn!=rr;nn= nn->left){
uu= nn->up;dd= nn->down;
uu->down= dd->up= nn;
nn->col->len++;
}
l= c->prev;r= c->next;
l->next= r->prev= c;
}

/*:16*//*25:*/
#line 419 "gdance.w"

purify(p)
node*p;
{register column*c= p->col;
register int x= p->color;
register node*rr,*nn,*uu,*dd;
register int k= 0,kk= 1;
c->head.color= x;
for(rr= c->head.down;rr!=&(c->head);rr= rr->down)
if(rr->color!=x){
for(nn= rr->right;nn!=rr;nn= nn->right){
uu= nn->up;dd= nn->down;
uu->down= dd;dd->up= uu;
k++;
nn->col->len--;
}
}else if(rr!=p)kk++,rr->color= -1;
updates+= k,purifs+= kk;
if(updates>=1000000000)hupdates++,updates-= 1000000000;
if(purifs>=1000000000)hpurifs++,purifs-= 1000000000;
upd_prof[level]+= k,pur_prof[level]+= kk;
}

/*:25*//*26:*/
#line 445 "gdance.w"

unpurify(p)
node*p;
{register column*c= p->col;
register int x= p->color;
register node*rr,*nn,*uu,*dd;
for(rr= c->head.up;rr!=&(c->head);rr= rr->up)
if(rr->color<0)rr->color= x;
else if(rr!=p){
for(nn= rr->left;nn!=rr;nn= nn->left){
uu= nn->up;dd= nn->down;
uu->down= dd->up= nn;
nn->col->len++;
}
}
c->head.color= 0;
}

/*:26*//*28:*/
#line 469 "gdance.w"

void show_state()
{
register int k;
printf("Current state (level %d):\n",level);
for(k= 0;k<level;k++)print_row(choice[k]);
printf("Max level so far: %d\n",maxl);
printf("Max branching so far: %d\n",maxb);
printf("Solutions so far: %d\n",count);
}

/*:28*/
#line 45 "gdance.w"
;

main(argc,argv)
int argc;
char*argv[];
{
/*10:*/
#line 198 "gdance.w"

register column*cur_col;
register char*p,*q;
register node*cur_node;
int primary;

/*:10*//*13:*/
#line 276 "gdance.w"

register column*best_col;
register node*pp;

/*:13*//*20:*/
#line 362 "gdance.w"

register int minlen;
register int j,k,x;

/*:20*/
#line 51 "gdance.w"
;
verbose= argc-1;
if(verbose)sscanf(argv[1],"%d",&spacing);
/*7:*/
#line 152 "gdance.w"

/*9:*/
#line 167 "gdance.w"

cur_col= col_array+1;
fgets(buf,buf_size,stdin);
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
for(p= buf,primary= 1;*p;p++){
while(isspace(*p))p++;
if(!*p)break;
if(*p=='|'){
primary= 0;
if(cur_col==col_array+1)panic("No primary columns");
(cur_col-1)->next= &root,root.prev= cur_col-1;
first_nonprim_col= cur_col;
continue;
}
for(q= p+1;!isspace(*q);q++);
if(q> p+7)panic("Column name too long");
if(cur_col>=&col_array[max_cols])panic("Too many columns");
for(q= cur_col->name;!isspace(*p);q++,p++)*q= *p;
cur_col->head.up= cur_col->head.down= &cur_col->head;
cur_col->len= 0;
if(primary)cur_col->prev= cur_col-1,(cur_col-1)->next= cur_col;
else cur_col->prev= cur_col->next= cur_col;
cur_col++;
}
if(primary){
if(cur_col==col_array+1)panic("No primary columns");
(cur_col-1)->next= &root,root.prev= cur_col-1;
first_nonprim_col= cur_col;
}
last_nonprim_col= cur_col;

/*:9*/
#line 153 "gdance.w"
;
/*11:*/
#line 204 "gdance.w"

cur_node= node_array;
while(fgets(buf,buf_size,stdin)){
register column*ccol;
register node*row_start,*x;
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
row_start= NULL;
for(p= buf;*p;p++){
while(isspace(*p))p++;
if(!*p)break;
for(q= p+1;!isspace(*q)&&*q!=':';q++);
if(q> p+7)panic("Column name too long");
for(q= cur_col->name;!isspace(*p)&&*p!=':';q++,p++)*q= *p;
*q= '\0';
for(ccol= col_array;strcmp(ccol->name,cur_col->name);ccol++);
if(ccol==cur_col)panic("Unknown column name");
if(cur_node==&node_array[max_nodes])panic("Too many nodes");
if(!row_start)row_start= cur_node;
else cur_node->left= cur_node-1,(cur_node-1)->right= cur_node;
for(x= row_start;x!=cur_node;x++)
if(x->col==ccol)panic("A row can't use a column twice");
cur_node->col= ccol;
cur_node->up= ccol->head.up,ccol->head.up->down= cur_node;
ccol->head.up= cur_node,cur_node->down= &ccol->head;
ccol->len++;
if(*p==':')/*24:*/
#line 405 "gdance.w"

{
if(ccol<first_nonprim_col)panic("Color isn't allowed in a primary column");
if(isspace(*(p+1))||!isspace(*(p+2)))
panic("Color should be a single character");
cur_node->color= *(p+1);
p+= 2;
}

/*:24*/
#line 229 "gdance.w"
;
cur_node++;
}
if(!row_start)panic("Empty row");
row_start->left= cur_node-1,(cur_node-1)->right= row_start;
}

/*:11*/
#line 154 "gdance.w"
;

/*:7*/
#line 54 "gdance.w"
;
/*12:*/
#line 253 "gdance.w"

level= 0;
forward:/*19:*/
#line 342 "gdance.w"

minlen= max_nodes;
if(verbose> 2)printf("Level %d:",level);
for(cur_col= root.next;cur_col!=&root;cur_col= cur_col->next){
if(verbose> 2)printf(" %s(%d)",cur_col->name,cur_col->len);
if(cur_col->len<minlen)best_col= cur_col,minlen= cur_col->len;
}
if(verbose){
if(level> maxl){
if(level>=max_level)panic("Too many levels");
maxl= level;
}
if(minlen> maxb){
if(minlen>=max_degree)panic("Too many branches");
maxb= minlen;
}
profile[level][minlen]++;
if(verbose> 2)printf(" branching on %s(%d)\n",best_col->name,minlen);
}

/*:19*/
#line 255 "gdance.w"
;
cover(best_col);
cur_node= choice[level]= best_col->head.down;
advance:if(cur_node==&(best_col->head))goto backup;
if(verbose> 1){
printf("L%d:",level);
print_row(cur_node);
}
/*17:*/
#line 327 "gdance.w"

for(pp= cur_node->right;pp!=cur_node;pp= pp->right)
if(!pp->color)cover(pp->col);
else if(pp->color> 0)purify(pp);

/*:17*/
#line 263 "gdance.w"
;
if(root.next==&root)/*21:*/
#line 366 "gdance.w"

{
count++;
if(verbose){
profile[level+1][0]++;
if(count%spacing==0){
printf("%d:\n",count);
for(k= 0;k<=level;k++)print_row(choice[k]);
}
}
goto recover;
}

/*:21*/
#line 264 "gdance.w"
;
level++;
goto forward;
backup:uncover(best_col);
if(level==0)goto done;
level--;
cur_node= choice[level];best_col= cur_node->col;
recover:/*18:*/
#line 337 "gdance.w"

for(pp= cur_node->left;pp!=cur_node;pp= pp->left)
if(!pp->color)uncover(pp->col);
else if(pp->color> 0)unpurify(pp);

/*:18*/
#line 271 "gdance.w"
;
cur_node= choice[level]= cur_node->down;goto advance;
done:if(verbose> 3)
/*22:*/
#line 379 "gdance.w"

{
printf("Final column lengths");
for(cur_col= root.next;cur_col!=&root;cur_col= cur_col->next)
printf(" %s(%d)",cur_col->name,cur_col->len);
printf("\n");
}

/*:22*/
#line 274 "gdance.w"
;

/*:12*/
#line 55 "gdance.w"
;
printf("Altogether %d solutions,",count);
if(hupdates)printf(" after %u%09d updates",hupdates,updates);
else printf(" after %d updates",updates);
if(hpurifs)printf(" and %u%09d cleansings.\n",hpurifs,purifs);
else printf(" and %d cleansings.\n",purifs);
if(verbose)/*23:*/
#line 387 "gdance.w"

{
x= 1;
for(level= 1;level<=maxl+1;level++){
j= 0;
for(k= 0;k<=maxb;k++){
printf("%6d",profile[level][k]);
j+= profile[level][k];
}
printf("%8d nodes, %u updates, %u cleansings\n",
j,upd_prof[level-1],pur_prof[level-1]);
x+= j;
}
printf("Total %d nodes.\n",x);
}

/*:23*/
#line 61 "gdance.w"
;
exit(0);
}

/*:1*/
