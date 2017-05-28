#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define O "%"
#define mod % \

#define max_level 500
#define max_cols 100000
#define max_nodes 10000000
#define bufsize (9*max_cols+3)  \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_profile 128
#define show_full_state 256
#define show_tots 512
#define show_warnings 1024 \

#define len col
#define aux color \

#define root 0 \

#define sanity_checking 0 \

#define panic(m) {fprintf(stderr,""O"s!\n"O"d: "O".99s\n",m,p,buf) ;exit(-666) ;} \

/*2:*/
#line 80 "./dlx2.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*5:*/
#line 262 "./dlx2.w"

typedef struct node_struct{
int up,down;
int col;
int color;
}node;

/*:5*//*6:*/
#line 282 "./dlx2.w"

typedef struct col_struct{
char name[8];
int prev,next;
}column;

/*:6*/
#line 88 "./dlx2.w"
;
/*3:*/
#line 159 "./dlx2.w"

int random_seed= 0;
int randomizing;
int vbose= show_basics+show_warnings;
int spacing;
int show_choices_max= 1000000;
int show_choices_gap= 1000000;

int show_levels_max= 1000000;
int maxl= 0;
char buf[bufsize];
ullng count;
ullng rows;
ullng imems,mems,lmems;
ullng updates;
ullng cleansings;
ullng bytes;
ullng nodes;
ullng thresh= 0;
ullng delta= 0;
ullng maxcount= 0xffffffffffffffff;
ullng timeout= 0x1fffffffffffffff;

/*:3*//*7:*/
#line 288 "./dlx2.w"

node nd[max_nodes];
int last_node;
column cl[max_cols+2];
int second= max_cols;
int last_col;

/*:7*//*23:*/
#line 578 "./dlx2.w"

int level;
int choice[max_level];
ullng profile[max_level];

/*:23*/
#line 89 "./dlx2.w"
;
/*9:*/
#line 305 "./dlx2.w"

void print_row(int p,FILE*stream){
register int k,q;
if(p<last_col||p>=last_node||nd[p].col<=0){
fprintf(stderr,"Illegal row "O"d!\n",p);
return;
}
for(q= p;;){
fprintf(stream," "O".8s",cl[nd[q].col].name);
if(nd[q].color)
fprintf(stream,":"O"c",nd[q].color> 0?nd[q].color:nd[nd[q].col].color);
q++;
if(nd[q].col<=0)q= nd[q].up;
if(q==p)break;
}
for(q= nd[nd[p].col].down,k= 1;q!=p;k++){
if(q==nd[p].col){
fprintf(stream," (?)\n");return;
}else q= nd[q].down;
}
fprintf(stream," ("O"d of "O"d)\n",k,nd[nd[p].col].len);
}

void prow(int p){
print_row(p,stderr);
}

/*:9*//*10:*/
#line 334 "./dlx2.w"

void print_col(int c){
register int p;
if(c<root||c>=last_col){
fprintf(stderr,"Illegal column "O"d!\n",c);
return;
}
if(c<second)
fprintf(stderr,"Column "O".8s, length "O"d, neighbors "O".8s and "O".8s:\n",
cl[c].name,nd[c].len,cl[cl[c].prev].name,cl[cl[c].next].name);
else fprintf(stderr,"Column "O".8s, length "O"d:\n",cl[c].name,nd[c].len);
for(p= nd[c].down;p>=last_col;p= nd[p].down)prow(p);
}

/*:10*//*11:*/
#line 353 "./dlx2.w"

void sanity(void){
register int k,p,q,pp,qq,t;
for(q= root,p= cl[q].next;;q= p,p= cl[p].next){
if(cl[p].prev!=q)fprintf(stderr,"Bad prev field at col "O".8s!\n",
cl[p].name);
if(p==root)break;
/*12:*/
#line 364 "./dlx2.w"

for(qq= p,pp= nd[qq].down,k= 0;;qq= pp,pp= nd[pp].down,k++){
if(nd[pp].up!=qq)fprintf(stderr,"Bad up field at node "O"d!\n",pp);
if(pp==p)break;
if(nd[pp].col!=p)fprintf(stderr,"Bad col field at node "O"d!\n",pp);
}
if(nd[p].len!=k)fprintf(stderr,"Bad len field in column "O".8s!\n",
cl[p].name);

/*:12*/
#line 360 "./dlx2.w"
;
}
}

/*:11*//*25:*/
#line 606 "./dlx2.w"

void cover(int c){
register int cc,l,r,rr,nn,uu,dd,t;
o,l= cl[c].prev,r= cl[c].next;
oo,cl[l].next= r,cl[r].prev= l;
updates++;
for(o,rr= nd[c].down;rr>=last_col;o,rr= nd[rr].down)
for(nn= rr+1;nn!=rr;){
if(o,nd[nn].color>=0){
o,uu= nd[nn].up,dd= nd[nn].down;
cc= nd[nn].col;
if(cc<=0){
nn= uu;
continue;
}
oo,nd[uu].down= dd,nd[dd].up= uu;
updates++;
o,t= nd[cc].len-1;
o,nd[cc].len= t;
if(cc>=second)lmems+= 2;
}
nn++;
}
}

/*:25*//*26:*/
#line 640 "./dlx2.w"

void uncover(int c){
register int cc,l,r,rr,nn,uu,dd,t;
for(o,rr= nd[c].down;rr>=last_col;o,rr= nd[rr].down)
for(nn= rr+1;nn!=rr;){
if(o,nd[nn].color>=0){
o,uu= nd[nn].up,dd= nd[nn].down;
cc= nd[nn].col;
if(cc<=0){
nn= uu;
continue;
}
oo,nd[uu].down= nd[dd].up= nn;
o,t= nd[cc].len+1;
o,nd[cc].len= t;
if(cc>=second)lmems+= 2;
}
nn++;
}
o,l= cl[c].prev,r= cl[c].next;
oo,cl[l].next= cl[r].prev= c;
}

/*:26*//*29:*/
#line 693 "./dlx2.w"

void purify(int p){
register int cc,rr,nn,uu,dd,t,x;
o,cc= nd[p].col,x= nd[p].color;
nd[cc].color= x;
cleansings++;
for(o,rr= nd[cc].down;rr>=last_col;o,rr= nd[rr].down){
if(o,nd[rr].color!=x){
for(nn= rr+1;nn!=rr;){
o,uu= nd[nn].up,dd= nd[nn].down;
o,cc= nd[nn].col;
if(cc<=0){
nn= uu;continue;
}
if(nd[nn].color>=0){
oo,nd[uu].down= dd,nd[dd].up= uu;
updates++;
o,t= nd[cc].len-1;
o,nd[cc].len= t;
if(cc>=second)lmems+= 2;
}
nn++;
}
}else if(rr!=p)cleansings++,o,nd[rr].color= -1;
}
}

/*:29*//*30:*/
#line 723 "./dlx2.w"

void unpurify(int p){
register int cc,rr,nn,uu,dd,t,x;
o,cc= nd[p].col,x= nd[p].color;
for(o,rr= nd[cc].up;rr>=last_col;o,rr= nd[rr].up){
if(o,nd[rr].color<0)o,nd[rr].color= x;
else if(rr!=p){
for(nn= rr-1;nn!=rr;){
o,uu= nd[nn].up,dd= nd[nn].down;
o,cc= nd[nn].col;
if(cc<=0){
nn= dd;continue;
}
if(nd[nn].color>=0){
oo,nd[uu].down= nd[dd].up= nn;
o,t= nd[cc].len+1;
o,nd[cc].len= t;
if(cc>=second)lmems+= 2;
}
nn--;
}
}
}
}

/*:30*//*33:*/
#line 786 "./dlx2.w"

void print_state(void){
register int l;
fprintf(stderr,"Current state (level "O"d):\n",level);
for(l= 0;l<level;l++){
print_row(choice[l],stderr);
if(l>=show_levels_max){
fprintf(stderr," ...\n");
break;
}
}
fprintf(stderr," "O"lld solutions, "O"lld mems, and max level "O"d so far.\n",
count,mems,maxl);
}

/*:33*//*34:*/
#line 821 "./dlx2.w"

void print_progress(void){
register int l,k,d,c,p;
register double f,fd;
fprintf(stderr," after "O"lld mems: "O"lld sols,",mems,count);
for(f= 0.0,fd= 1.0,l= 0;l<level;l++){
c= nd[choice[l]].col,d= nd[c].len;
for(k= 1,p= nd[c].down;p!=choice[l];k++,p= nd[p].down);
fd*= d,f+= (k-1)/fd;
fprintf(stderr," "O"c"O"c",
k<10?'0'+k:k<36?'a'+k-10:k<62?'A'+k-36:'*',
d<10?'0'+d:d<36?'a'+d-10:d<62?'A'+d-36:'*');
if(l>=show_levels_max){
fprintf(stderr,"...");
break;
}
}
fprintf(stderr," "O".5f\n",f+0.5/fd);
}

/*:34*/
#line 90 "./dlx2.w"
;
main(int argc,char*argv[]){
register int cc,i,j,k,p,pp,q,r,t,cur_node,best_col;
/*4:*/
#line 185 "./dlx2.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,""O"d",&vbose)-1);break;
case'm':k|= (sscanf(argv[j]+1,""O"d",&spacing)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1),randomizing= 1;break;
case'd':k|= (sscanf(argv[j]+1,""O"lld",&delta)-1),thresh= delta;break;
case'c':k|= (sscanf(argv[j]+1,""O"d",&show_choices_max)-1);break;
case'C':k|= (sscanf(argv[j]+1,""O"d",&show_levels_max)-1);break;
case'l':k|= (sscanf(argv[j]+1,""O"d",&show_choices_gap)-1);break;
case't':k|= (sscanf(argv[j]+1,""O"lld",&maxcount)-1);break;
case'T':k|= (sscanf(argv[j]+1,""O"lld",&timeout)-1);break;
default:k= 1;
}
if(k){
fprintf(stderr,"Usage: "O"s [v<n>] [m<n>] [s<n>] [d<n>]"
" [c<n>] [C<n>] [l<n>] [t<n>] [T<n>] < foo.dlx\n",
argv[0]);
exit(-1);
}
if(randomizing)gb_init_rand(random_seed);

/*:4*/
#line 93 "./dlx2.w"
;
/*13:*/
#line 378 "./dlx2.w"

if(max_nodes<=2*max_cols){
fprintf(stderr,"Recompile me: max_nodes must exceed twice max_cols!\n");
exit(-999);
}
while(1){
if(!fgets(buf,bufsize,stdin))break;
if(o,buf[p= strlen(buf)-1]!='\n')panic("Input line way too long");
for(p= 0;o,isspace(buf[p]);p++);
if(buf[p]=='|'||!buf[p])continue;
last_col= 1;
break;
}
if(!last_col)panic("No columns");
for(;o,buf[p];){
for(j= 0;j<8&&(o,!isspace(buf[p+j]));j++){
if(buf[p+j]==':'||buf[p+j]=='|')
panic("Illegal character in column name");
o,cl[last_col].name[j]= buf[p+j];
}
if(j==8&&!isspace(buf[p+j]))panic("Column name too long");
/*14:*/
#line 413 "./dlx2.w"

for(k= 1;o,strncmp(cl[k].name,cl[last_col].name,8);k++);
if(k<last_col)panic("Duplicate column name");

/*:14*/
#line 399 "./dlx2.w"
;
/*15:*/
#line 417 "./dlx2.w"

if(last_col> max_cols)panic("Too many columns");
if(second==max_cols)
oo,cl[last_col-1].next= last_col,cl[last_col].prev= last_col-1;
else o,cl[last_col].next= cl[last_col].prev= last_col;

o,nd[last_col].up= nd[last_col].down= last_col;
last_col++;

/*:15*/
#line 400 "./dlx2.w"
;
for(p+= j+1;o,isspace(buf[p]);p++);
if(buf[p]=='|'){
if(second!=max_cols)panic("Column name line contains | twice");
second= last_col;
for(p++;o,isspace(buf[p]);p++);
}
}
if(second==max_cols)second= last_col;
o,cl[root].prev= second-1;
last_node= last_col;
o,nd[last_node].col= 0;

/*:13*/
#line 94 "./dlx2.w"
;
/*16:*/
#line 429 "./dlx2.w"

while(1){
if(!fgets(buf,bufsize,stdin))break;
if(o,buf[p= strlen(buf)-1]!='\n')panic("Row line too long");
for(p= 0;o,isspace(buf[p]);p++);
if(buf[p]=='|'||!buf[p])continue;
i= last_node;
for(pp= 0;buf[p];){
for(j= 0;j<8&&(o,!isspace(buf[p+j]))&&buf[p+j]!=':';j++)
o,cl[last_col].name[j]= buf[p+j];
if(!j)panic("Empty column name");
if(j==8&&!isspace(buf[p+j])&&buf[p+j]!=':')
panic("Column name too long");
if(j<8)o,cl[last_col].name[j]= '\0';
/*17:*/
#line 470 "./dlx2.w"

for(k= 0;o,strncmp(cl[k].name,cl[last_col].name,8);k++);
if(k==last_col)panic("Unknown column name");
if(o,nd[k].aux>=i)panic("Duplicate column name in this row");
last_node++;
if(last_node==max_nodes)panic("Too many nodes");
o,nd[last_node].col= k;
if(k<second)pp= 1;
o,t= nd[k].len+1;
/*18:*/
#line 492 "./dlx2.w"

o,nd[k].len= t;
nd[k].aux= last_node;
if(!randomizing){
o,r= nd[k].up;
ooo,nd[r].down= nd[k].up= last_node,nd[last_node].up= r,nd[last_node].down= k;
}else{
mems+= 4,t= gb_unif_rand(t);
for(o,r= k;t;o,r= nd[r].down,t--);
ooo,q= nd[r].up,nd[q].down= nd[r].up= last_node;
o,nd[last_node].up= q,nd[last_node].down= r;
}

/*:18*/
#line 479 "./dlx2.w"
;

/*:17*/
#line 443 "./dlx2.w"
;
if(buf[p+j]!=':')o,nd[last_node].color= 0;
else if(k>=second){
if((o,isspace(buf[p+j+1]))||(o,!isspace(buf[p+j+2])))
panic("Color must be a single character");
o,nd[last_node].color= buf[p+j+1];
p+= 2;
}else panic("Primary column must be uncolored");
for(p+= j+1;o,isspace(buf[p]);p++);
}
if(!pp){
if(vbose&show_warnings)
fprintf(stderr,"Row ignored (no primary columns): "O"s",buf);
while(last_node> i){
/*19:*/
#line 505 "./dlx2.w"

o,k= nd[last_node].col;
oo,nd[k].len--,nd[k].aux= i-1;
o,q= nd[last_node].up,r= nd[last_node].down;
oo,nd[q].down= r,nd[r].up= q;

/*:19*/
#line 457 "./dlx2.w"
;
last_node--;
}
}else{
o,nd[i].down= last_node;
last_node++;
if(last_node==max_nodes)panic("Too many nodes");
rows++;
o,nd[last_node].up= i+1;
o,nd[last_node].col= -rows;
}
}

/*:16*/
#line 95 "./dlx2.w"
;
if(vbose&show_basics)
/*20:*/
#line 511 "./dlx2.w"

fprintf(stderr,
"("O"lld rows, "O"d+"O"d columns, "O"d entries successfully read)\n",
rows,second-1,last_col-second,last_node-last_col);

/*:20*/
#line 97 "./dlx2.w"
;
if(vbose&show_tots)
/*21:*/
#line 520 "./dlx2.w"

{
fprintf(stderr,"Column totals:");
for(k= 1;k<last_col;k++){
if(k==second)fprintf(stderr," |");
fprintf(stderr," "O"d",nd[k].len);
}
fprintf(stderr,"\n");
}

/*:21*/
#line 99 "./dlx2.w"
;
imems= mems,mems= 0;
/*22:*/
#line 547 "./dlx2.w"

level= 0;
forward:nodes++;
if(vbose&show_profile)profile[level]++;
if(sanity_checking)sanity();
/*24:*/
#line 583 "./dlx2.w"

if(delta&&(mems>=thresh)){
thresh+= delta;
if(vbose&show_full_state)print_state();
else print_progress();
}
if(mems>=timeout){
fprintf(stderr,"TIMEOUT!\n");goto done;
}

/*:24*/
#line 552 "./dlx2.w"
;
/*31:*/
#line 753 "./dlx2.w"

t= max_nodes;
if((vbose&show_details)&&
level<show_choices_max&&level>=maxl-show_choices_gap)
fprintf(stderr,"Level "O"d:",level);
for(o,k= cl[root].next;k!=root;o,k= cl[k].next){
if((vbose&show_details)&&
level<show_choices_max&&level>=maxl-show_choices_gap)
fprintf(stderr," "O".8s("O"d)",cl[k].name,nd[k].len);
if(o,nd[k].len<=t){
if(nd[k].len<t)best_col= k,t= nd[k].len,p= 1;
else{
p++;
if(randomizing&&(mems+= 4,!gb_unif_rand(p)))best_col= k;
}
}
}
if((vbose&show_details)&&
level<show_choices_max&&level>=maxl-show_choices_gap)
fprintf(stderr," branching on "O".8s("O"d)\n",cl[best_col].name,t);

/*:31*/
#line 553 "./dlx2.w"
;
cover(best_col);
oo,cur_node= choice[level]= nd[best_col].down;
advance:if(cur_node==best_col)goto backup;
if((vbose&show_choices)&&level<show_choices_max){
fprintf(stderr,"L"O"d:",level);
print_row(cur_node,stderr);
}
/*27:*/
#line 663 "./dlx2.w"

for(pp= cur_node+1;pp!=cur_node;){
o,cc= nd[pp].col;
if(cc<=0)o,pp= nd[pp].up;
else{
if(!nd[pp].color)cover(cc);
else if(nd[pp].color> 0)purify(pp);
pp++;
}
}

/*:27*/
#line 561 "./dlx2.w"
;
if(o,cl[root].next==root)/*32:*/
#line 774 "./dlx2.w"

{
count++;
if(spacing&&(count mod spacing==0)){
printf(""O"lld:\n",count);
for(k= 0;k<=level;k++)print_row(choice[k],stdout);
fflush(stdout);
}
if(count>=maxcount)goto done;
goto recover;
}

/*:32*/
#line 562 "./dlx2.w"
;
if(++level> maxl){
if(level>=max_level){
fprintf(stderr,"Too many levels!\n");
exit(-4);
}
maxl= level;
}
goto forward;
backup:uncover(best_col);
if(level==0)goto done;
level--;
oo,cur_node= choice[level],best_col= nd[cur_node].col;
recover:/*28:*/
#line 677 "./dlx2.w"

for(pp= cur_node-1;pp!=cur_node;){
o,cc= nd[pp].col;
if(cc<=0)o,pp= nd[pp].down;
else{
if(!nd[pp].color)uncover(cc);
else if(nd[pp].color> 0)unpurify(pp);
pp--;
}
}

/*:28*/
#line 575 "./dlx2.w"
;
oo,cur_node= choice[level]= nd[cur_node].down;goto advance;

/*:22*/
#line 101 "./dlx2.w"
;
done:if(sanity_checking)sanity();
if(vbose&show_tots)
/*21:*/
#line 520 "./dlx2.w"

{
fprintf(stderr,"Column totals:");
for(k= 1;k<last_col;k++){
if(k==second)fprintf(stderr," |");
fprintf(stderr," "O"d",nd[k].len);
}
fprintf(stderr,"\n");
}

/*:21*/
#line 104 "./dlx2.w"
;
if(vbose&show_profile)/*35:*/
#line 841 "./dlx2.w"

{
fprintf(stderr,"Profile:\n");
for(level= 0;level<=maxl;level++)
fprintf(stderr,""O"3d: "O"lld\n",
level,profile[level]);
}

/*:35*/
#line 105 "./dlx2.w"
;
if(vbose&show_basics){
fprintf(stderr,"Altogether "O"llu solution"O"s, "O"llu+"O"llu-"O"llu mems,",
count,count==1?"":"s",imems,mems,lmems);
bytes= last_col*sizeof(column)+last_node*sizeof(node)+maxl*sizeof(int);
fprintf(stderr," "O"llu updates, "O"llu cleansings,",
updates,cleansings);
fprintf(stderr," "O"llu bytes, "O"llu nodes.\n",
bytes,nodes);
}
}

/*:2*/
