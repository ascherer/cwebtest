#define max_level 150
#define max_degree 1000
#define max_cols 10000
#define max_nodes 1000000 \

#define root col_array[0] \

#define buf_size 4*max_cols+3 \

#define panic(m) {fprintf(stderr,"%s!\n%s",m,buf) ;exit(-1) ;} \

/*1:*/
#line 28 "dance.w"

#include <stdio.h> 
#include <ctype.h> 
#include <string.h> 
/*3:*/
#line 76 "dance.w"

typedef struct node_struct{
struct node_struct*left,*right;
struct node_struct*up,*down;
struct col_struct*col;
}node;

/*:3*//*4:*/
#line 96 "dance.w"

typedef struct col_struct{
node head;
int len;
char name[8];
struct col_struct*prev,*next;
}column;

/*:4*/
#line 32 "dance.w"

/*2:*/
#line 50 "dance.w"

int verbose;
int count= 0;
double updates;
int spacing= 1;
double profile[max_level][max_degree];
double upd_prof[max_level];
int maxb= 0;
int maxl= 0;

/*:2*//*8:*/
#line 145 "dance.w"

column col_array[max_cols+2];
node node_array[max_nodes];
char buf[buf_size];

/*:8*//*14:*/
#line 259 "dance.w"

int level;
node*choice[max_level];

/*:14*/
#line 33 "dance.w"

/*6:*/
#line 114 "dance.w"

print_row(p)
node*p;
{register node*q= p;
register int k;
do{
printf(" %s",q->col->name);
q= q->right;
}while(q!=p);
for(q= p->col->head.down,k= 1;q!=p;k++)
if(q==&(p->col->head)){
printf("\n");return;
}else q= q->down;
printf(" (%d of %d)\n",k,p->col->len);
}

void print_state(int lev)
{
register int l;
for(l= 0;l<=lev;l++)print_row(choice[l]);
}

/*:6*//*15:*/
#line 267 "dance.w"

cover(c)
column*c;
{register column*l,*r;
register node*rr,*nn,*uu,*dd;
register k= 1;
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
upd_prof[level]+= k;
}

/*:15*//*16:*/
#line 290 "dance.w"

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

/*:16*/
#line 34 "dance.w"
;

main(argc,argv)
int argc;
char*argv[];
{
/*10:*/
#line 180 "dance.w"

register column*cur_col;
register char*p,*q;
register node*cur_node;
int primary;

/*:10*//*13:*/
#line 255 "dance.w"

register column*best_col;
register node*pp;

/*:13*//*20:*/
#line 336 "dance.w"

register int minlen;
register int j,k,x;

/*:20*/
#line 40 "dance.w"
;
verbose= argc-1;
if(verbose)sscanf(argv[1],"%d",&spacing);
/*7:*/
#line 139 "dance.w"

/*9:*/
#line 152 "dance.w"

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
}

/*:9*/
#line 140 "dance.w"
;
/*11:*/
#line 186 "dance.w"

cur_node= node_array;
while(fgets(buf,buf_size,stdin)){
register column*ccol;
register node*row_start;
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
row_start= NULL;
for(p= buf;*p;p++){
while(isspace(*p))p++;
if(!*p)break;
for(q= p+1;!isspace(*q);q++);
if(q> p+7)panic("Column name too long");
for(q= cur_col->name;!isspace(*p);q++,p++)*q= *p;
*q= '\0';
for(ccol= col_array;strcmp(ccol->name,cur_col->name);ccol++);
if(ccol==cur_col)panic("Unknown column name");
if(cur_node==&node_array[max_nodes])panic("Too many nodes");
if(!row_start)row_start= cur_node;
else cur_node->left= cur_node-1,(cur_node-1)->right= cur_node;
cur_node->col= ccol;
cur_node->up= ccol->head.up,ccol->head.up->down= cur_node;
ccol->head.up= cur_node,cur_node->down= &ccol->head;
ccol->len++;
cur_node++;
}
if(!row_start)panic("Empty row");
row_start->left= cur_node-1,(cur_node-1)->right= row_start;
}

/*:11*/
#line 141 "dance.w"
;

/*:7*/
#line 43 "dance.w"
;
/*12:*/
#line 232 "dance.w"

level= 0;
forward:/*19:*/
#line 316 "dance.w"

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
#line 234 "dance.w"
;
cover(best_col);
cur_node= choice[level]= best_col->head.down;
advance:if(cur_node==&(best_col->head))goto backup;
if(verbose> 1){
printf("L%d:",level);
print_row(cur_node);
}
/*17:*/
#line 305 "dance.w"

for(pp= cur_node->right;pp!=cur_node;pp= pp->right)cover(pp->col);

/*:17*/
#line 242 "dance.w"
;
if(root.next==&root)/*21:*/
#line 340 "dance.w"

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
#line 243 "dance.w"
;
level++;
goto forward;
backup:uncover(best_col);
if(level==0)goto done;
level--;
cur_node= choice[level];best_col= cur_node->col;
recover:/*18:*/
#line 313 "dance.w"

for(pp= cur_node->left;pp!=cur_node;pp= pp->left)uncover(pp->col);

/*:18*/
#line 250 "dance.w"
;
cur_node= choice[level]= cur_node->down;goto advance;
done:if(verbose> 3)
/*22:*/
#line 353 "dance.w"

{
printf("Final column lengths");
for(cur_col= root.next;cur_col!=&root;cur_col= cur_col->next)
printf(" %s(%d)",cur_col->name,cur_col->len);
printf("\n");
}

/*:22*/
#line 253 "dance.w"
;

/*:12*/
#line 44 "dance.w"
;
printf("Altogether %d solutions, after %.15g updates.\n",count,updates);
if(verbose)/*23:*/
#line 361 "dance.w"

{
double tot,subtot;
tot= 1;
for(level= 1;level<=maxl+1;level++){
subtot= 0;
for(k= 0;k<=maxb;k++){
printf(" %5.6g",profile[level][k]);
subtot+= profile[level][k];
}
printf(" %5.15g nodes, %.15g updates\n",subtot,upd_prof[level-1]);
tot+= subtot;
}
printf("Total %.15g nodes.\n",tot);
}

/*:23*/
#line 46 "dance.w"
;
exit(0);
}

/*:1*/
