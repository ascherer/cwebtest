#define n1 4
#define n2 4
#define n3 4
#define verbose (argc> 1) 
#define very_verbose (argc> 2) 
#define very_very_verbose (argc> 3) 
#define new_node(ii,jj,kk) { \
cur_node->right= cur_node+1;cur_node->left= cur_node-1; \
p= &headers[ii][jj][kk];q= p->down; \
cur_node->name= p->name; \
cur_node->up= p;cur_node->down= q;p->down= cur_node;q->up= cur_node; \
cur_node->tag= cur_tag; \
cur_node->clist= cur_clist; \
cur_node++; \
} \

#define start_con *cur_clist= cur_con
#define new_con(ii,jj,kk) *cur_con++= &headers[ii][jj][kk]
#define wrap_con cur_con++,cur_clist++ \

#define move_stack_size 1000
#define constr_stack_size 1000
#define max_level (((n1*n2*n3) >>2) -2)  \

#define panic(s) {printf("s stack overflow!\n") ;exit(-1) ;} \

/*1:*/
#line 21 "./antislide.w"

#include <stdio.h> 

/*2:*/
#line 64 "./antislide.w"

typedef struct node_struct{
struct node_struct*left,*right;
struct node_struct*up,*down;
char*name;
struct node_struct***clist;
int*tag;
}node;

/*:2*/
#line 24 "./antislide.w"

/*3:*/
#line 76 "./antislide.w"

node headers[n1][n2][n3];
node nodes[432];
node*constraints[1674];
node**clists[558];
char names[n1*n2*n3*4];
int tags[108];

/*:3*//*10:*/
#line 219 "./antislide.w"

node*move_stack[move_stack_size];
node**constr_stack[constr_stack_size];
node**first[max_level];
node**move[max_level];
node***constr[max_level];
int totsols[max_level];

/*:10*/
#line 25 "./antislide.w"

tmp(){
printf("tmp");
}

main(argc,argv)
int argc;char*argv[];
{
register node*p,*q,*r;
register int stamp= 0;
/*4:*/
#line 86 "./antislide.w"

{
register node*cur_node= &nodes[0],
**cur_con= &constraints[0],
***cur_clist= &clists[0];
register char*cur_name= &names[0];
register int*cur_tag= &tags[0];
register int i,j,k;
/*5:*/
#line 104 "./antislide.w"

for(i= 0;i<n1;i++)for(j= 0;j<n2;j++)for(k= 0;k<n3;k++){
*cur_name= i+'0';*(cur_name+1)= j+'0';*(cur_name+2)= k+'0';
headers[i][j][k].name= cur_name;
cur_name+= 4;
headers[i][j][k].up= headers[i][j][k].down= &headers[i][j][k];
}

/*:5*/
#line 94 "./antislide.w"
;
/*6:*/
#line 126 "./antislide.w"

for(i= 0;i<n1;i++)for(j= 0;j+1<n2;j++)for(k= 0;k+1<n3;k++){
register node*first_node= cur_node;
new_node(i,j,k);
new_node(i,j,k+1);
new_node(i,j+1,k);
new_node(i,j+1,k+1);
first_node->left= cur_node-1;
(cur_node-1)->right= first_node;
if(i> 0){start_con;
new_con(i-1,j,k);new_con(i-1,j,k+1);
new_con(i-1,j+1,k);new_con(i-1,j+1,k+1);wrap_con;}
if(i+1<n1){start_con;
new_con(i+1,j,k);new_con(i+1,j,k+1);
new_con(i+1,j+1,k);new_con(i+1,j+1,k+1);wrap_con;}
if(j> 0){start_con;new_con(i,j-1,k);new_con(i,j-1,k+1);wrap_con;}
if(j+2<n2){start_con;new_con(i,j+2,k);new_con(i,j+2,k+1);wrap_con;}
if(k> 0){start_con;new_con(i,j,k-1);new_con(i,j+1,k-1);wrap_con;}
if(k+2<n3){start_con;new_con(i,j,k+2);new_con(i,j+1,k+2);wrap_con;}
cur_clist++;cur_tag++;
if(very_very_verbose)/*9:*/
#line 195 "./antislide.w"

{node**p1,***c1;
for(p= first_node;;p= p->right){
printf("%s ",p->name);
if(p->right==first_node)break;
}
printf("=>");
for(c1= p->clist;*c1;c1++){
for(p1= *c1;*p1;p1++)printf("%s,",(*p1)->name);
printf(" ");
}
printf("\n");
}

/*:9*/
#line 146 "./antislide.w"
;
}

/*:6*/
#line 95 "./antislide.w"
;
/*7:*/
#line 149 "./antislide.w"

for(i= 0;i+1<n1;i++)for(j= 0;j<n2;j++)for(k= 0;k+1<n3;k++){
register node*first_node= cur_node;
new_node(i,j,k);
new_node(i,j,k+1);
new_node(i+1,j,k);
new_node(i+1,j,k+1);
first_node->left= cur_node-1;
(cur_node-1)->right= first_node;
if(j> 0){start_con;
new_con(i,j-1,k);new_con(i,j-1,k+1);
new_con(i+1,j-1,k);new_con(i+1,j-1,k+1);wrap_con;}
if(j+1<n2){start_con;
new_con(i,j+1,k);new_con(i,j+1,k+1);
new_con(i+1,j+1,k);new_con(i+1,j+1,k+1);wrap_con;}
if(i> 0){start_con;new_con(i-1,j,k);new_con(i-1,j,k+1);wrap_con;}
if(i+2<n1){start_con;new_con(i+2,j,k);new_con(i+2,j,k+1);wrap_con;}
if(k> 0){start_con;new_con(i,j,k-1);new_con(i+1,j,k-1);wrap_con;}
if(k+2<n3){start_con;new_con(i,j,k+2);new_con(i+1,j,k+2);wrap_con;}
cur_clist++;cur_tag++;
if(very_very_verbose)/*9:*/
#line 195 "./antislide.w"

{node**p1,***c1;
for(p= first_node;;p= p->right){
printf("%s ",p->name);
if(p->right==first_node)break;
}
printf("=>");
for(c1= p->clist;*c1;c1++){
for(p1= *c1;*p1;p1++)printf("%s,",(*p1)->name);
printf(" ");
}
printf("\n");
}

/*:9*/
#line 169 "./antislide.w"
;
}

/*:7*/
#line 96 "./antislide.w"
;
/*8:*/
#line 172 "./antislide.w"

for(i= 0;i+1<n1;i++)for(j= 0;j+1<n2;j++)for(k= 0;k<n3;k++){
register node*first_node= cur_node;
new_node(i,j,k);
new_node(i+1,j,k);
new_node(i,j+1,k);
new_node(i+1,j+1,k);
first_node->left= cur_node-1;
(cur_node-1)->right= first_node;
if(k> 0){start_con;
new_con(i,j,k-1);new_con(i+1,j,k-1);
new_con(i,j+1,k-1);new_con(i+1,j+1,k-1);wrap_con;}
if(k+1<n3){start_con;
new_con(i,j,k+1);new_con(i+1,j,k+1);
new_con(i,j+1,k+1);new_con(i+1,j+1,k+1);wrap_con;}
if(j> 0){start_con;new_con(i,j-1,k);new_con(i+1,j-1,k);wrap_con;}
if(j+2<n2){start_con;new_con(i,j+2,k);new_con(i+1,j+2,k);wrap_con;}
if(i> 0){start_con;new_con(i-1,j,k);new_con(i-1,j+1,k);wrap_con;}
if(i+2<n1){start_con;new_con(i+2,j,k);new_con(i+2,j+1,k);wrap_con;}
cur_clist++;cur_tag++;
if(very_very_verbose)/*9:*/
#line 195 "./antislide.w"

{node**p1,***c1;
for(p= first_node;;p= p->right){
printf("%s ",p->name);
if(p->right==first_node)break;
}
printf("=>");
for(c1= p->clist;*c1;c1++){
for(p1= *c1;*p1;p1++)printf("%s,",(*p1)->name);
printf(" ");
}
printf("\n");
}

/*:9*/
#line 192 "./antislide.w"
;
}

/*:8*/
#line 97 "./antislide.w"
;
printf("This problem involves %d namechars, %d moves, %d nodes,\n",
(cur_name-&names[0])/4,cur_tag-&tags[0],cur_node-&nodes[0]);
printf(" %d constraint elements, %d clist elements.\n",
cur_con-&constraints[0],cur_clist-&clists[0]);
}

/*:4*/
#line 35 "./antislide.w"
;
/*11:*/
#line 229 "./antislide.w"

{
register node**mtop= &move_stack[0];
register node***ctop= &constr_stack[0];
register node**pp,***cc;
register int l= 0;
constr[0]= ctop;
/*15:*/
#line 312 "./antislide.w"

pp= first[0]= mtop;
*mtop++= &nodes[0];
goto advance;

/*:15*/
#line 236 "./antislide.w"
;
newlevel:first[l]= mtop;
if(constr[l]==ctop){
/*25:*/
#line 420 "./antislide.w"

totsols[l]++;
if(verbose){
int ii,jj,kk;
printf("%d.%d:",l,totsols[l]);
for(ii= 0;ii<n1;ii++){
printf(" ");
for(jj= 0;jj<n2;jj++)for(kk= 0;kk<n3;kk++){
register int c= (int)headers[ii][jj][kk].right;
printf("%c",c> 9?c-10+'a':c+'0');
}
}
printf("\n");
}

/*:25*/
#line 239 "./antislide.w"
;
if(l==max_level-1)goto backtrack;
/*23:*/
#line 402 "./antislide.w"

{
stamp++;
for(p= &headers[0][0][0];p<&headers[n1][0][0];p++)if(!p->right)
for(q= p->down;q!=p;q= q->down)
if(*(q->tag)!=stamp)*mtop++= q,*(q->tag)= stamp;
}

/*:23*/
#line 241 "./antislide.w"
;
}
else if(l==max_level-1)goto backtrack;
else/*12:*/
#line 263 "./antislide.w"

{
register int count;
node**cbest;int best_count= 100000;
for(cc= constr[l];cc<ctop;cc++){
/*13:*/
#line 283 "./antislide.w"

count= 0;
stamp++;
for(pp= *cc;*pp;pp++)
for(p= (*pp)->down;p->left;p= p->down)
if(*(p->tag)!=stamp)count++,*(p->tag)= stamp;
if(very_verbose){
printf("Constraint ");
for(pp= *cc;*pp;pp++)printf("%s,",(*pp)->name);
printf(" %d\n",count);
}
if(count<best_count)best_count= count,cbest= *cc;

/*:13*/
#line 269 "./antislide.w"
;
}
/*14:*/
#line 298 "./antislide.w"

stamp++;
for(pp= cbest;*pp;pp++)
for(p= (*pp)->down;p->left;p= p->down)
if(*(p->tag)!=stamp)*mtop++= p,*(p->tag)= stamp;
if(mtop>=&move_stack[move_stack_size])panic(move);

/*:14*/
#line 271 "./antislide.w"
;
}

/*:12*/
#line 244 "./antislide.w"
;
pp= first[l];
goto advance;
backtrack:/*22:*/
#line 394 "./antislide.w"

for(pp= mtop-1;pp>=first[l];pp--)
for(p= (*pp)->right;;p= p->right){
q= p->down;r= p->up;
q->up= r->down= p;
if(p==*pp)break;
}

/*:22*/
#line 247 "./antislide.w"
;
mtop= first[l];
if(l==0)goto done;
l--;pp= move[l];
/*19:*/
#line 369 "./antislide.w"

for(p= (*pp)->left;;p= p->left){
/*20:*/
#line 378 "./antislide.w"

for(q= p->up;q!=p;q= q->up){
if(q->left==NULL)q->right= NULL;
else for(r= q->right;r!=q;r= r->right){
r->up->down= r;
r->down->up= r;
}
}

/*:20*/
#line 372 "./antislide.w"
;
if(p==*pp)break;
}
ctop= constr[l+1];

/*:19*/
#line 251 "./antislide.w"
;
/*21:*/
#line 387 "./antislide.w"

for(p= (*pp)->right;;p= p->right){
q= p->down;r= p->up;
q->up= r;r->down= q;
if(p==*pp)break;
}

/*:21*/
#line 252 "./antislide.w"
;
pp++;
advance:if(pp==mtop)goto backtrack;
move[l]= pp;
/*16:*/
#line 321 "./antislide.w"

if(stamp==1620)tmp();
for(p= *pp;;p= p->right){
/*17:*/
#line 345 "./antislide.w"

for(q= p->down;q!=p;q= q->down){
if(q->left==NULL)q->right= (node*)(l+1);
else for(r= q->left;r!=q;r= r->left){
r->up->down= r->down;
r->down->up= r->up;
}
}

/*:17*/
#line 325 "./antislide.w"
;
if(p->right==*pp)break;
}
constr[l+1]= ctop;
for(cc= constr[l];cc<constr[l+1];cc++)
/*18:*/
#line 354 "./antislide.w"

{
for(pp= *cc;*pp;pp++)if((*pp)->right)break;
if(!*pp)*ctop++= *cc;
}

/*:18*/
#line 330 "./antislide.w"
;
for(cc= p->clist;*cc;cc++)
/*18:*/
#line 354 "./antislide.w"

{
for(pp= *cc;*pp;pp++)if((*pp)->right)break;
if(!*pp)*ctop++= *cc;
}

/*:18*/
#line 332 "./antislide.w"
;
if(ctop>=&constr_stack[constr_stack_size])panic(constraint);

/*:16*/
#line 256 "./antislide.w"
;
if(very_verbose)/*24:*/
#line 410 "./antislide.w"

{
printf("Move %d:",l+1);
for(p= (*move[l])->right;;p= p->right){
printf(" %s",p->name);
if(p==*move[l])break;
}
printf(" (%d)\n",stamp);
}

/*:24*/
#line 257 "./antislide.w"
;
l++;
goto newlevel;
done:;
}

/*:11*/
#line 36 "./antislide.w"
;
/*26:*/
#line 435 "./antislide.w"

printf("Total solutions found:\n");
{register int lev;
for(lev= 0;lev<max_level;lev++)if(totsols[lev])
printf("  level %d, %d\n",lev,totsols[lev]);
}

/*:26*/
#line 37 "./antislide.w"
;
}

/*:1*/
