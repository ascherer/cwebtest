#define o mems++
#define oo mems+= 2
#define ooo mems+= 3 \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_gory_details 8
#define show_histogram 16
#define show_pis 32 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

#define cl(p) mem[p].eta.u \

#define removed (uint) (-1)  \

/*3:*/
#line 79 "./sat9.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*6:*/
#line 207 "./sat9.w"

typedef union{
char ch8[8];
uint u2[2];
long long lng;
}octa;
typedef struct tmp_var_struct{
octa name;
uint serial;
int stamp;
struct tmp_var_struct*next;
}tmp_var;

typedef struct vchunk_struct{
struct vchunk_struct*prev;
tmp_var var[vars_per_vchunk];
}vchunk;

/*:6*//*7:*/
#line 234 "./sat9.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:7*//*24:*/
#line 515 "./sat9.w"

typedef struct{
double eta;
double pi;
uint zf;
uint link;
int rating;
}literal;
typedef struct{
uint start;
uint size;
}clause;
typedef struct{
union{double d;ullng u;}eta;
uint lit;
uint next;
}mem_item;

/*:24*/
#line 86 "./sat9.w"
;
/*4:*/
#line 111 "./sat9.w"

int random_seed= 0;
int verbose= show_basics;
int hbits= 8;
int buf_size= 1024;
int max_iter= 1000;
int min_iter= 5;
int confidence= 50;
double damper= 0.99;
double threshold= 0.01;
ullng imems,mems;
ullng thresh= 0;
ullng delta= 0;
ullng bytes;

/*:4*//*8:*/
#line 240 "./sat9.w"

char*buf;
tmp_var**hash;
uint hash_bits[93][8];
vchunk*cur_vchunk;
tmp_var*cur_tmp_var;
tmp_var*bad_tmp_var;
chunk*cur_chunk;
tmp_var**cur_cell;
tmp_var**bad_cell;
ullng vars;
ullng clauses;
ullng nullclauses;
ullng cells;

/*:8*//*25:*/
#line 533 "./sat9.w"

clause*cmem;
literal*lmem;
mem_item*mem;
mem_item*cur_mcell;
octa*nmem;
double*gam;

/*:25*//*36:*/
#line 690 "./sat9.w"

int iter;
double acc,etabar,pi0,pi1,old_eta,new_eta,new_gam,factor,rein,diff;

double max_diff;
double factor;
int azf;
int max_iter;

/*:36*//*54:*/
#line 1099 "./sat9.w"

int bucket[101],unit;
int fixcount,unitcount;
char name_buf[32];
FILE*out_file;

/*:54*/
#line 87 "./sat9.w"
;
/*26:*/
#line 545 "./sat9.w"

void print_clause(uint c){
register uint l,ll;
fprintf(stderr,"%d:\n",c);
for(l= cmem[c-1].start;l<cmem[c].start;l++){
ll= mem[l].lit;
fprintf(stderr," %s%.8s(%d), eta=%.15g\n",
ll&1?"~":"",nmem[ll>>1].ch8,ll>>1,mem[l].eta.d);
}
}

/*:26*//*27:*/
#line 559 "./sat9.w"

void print_var(uint k){
register uint l= k<<1;
fprintf(stderr,"pi(%.8s)=%.15g(%d), eta(%.8s)=%.15g, ",
nmem[k].ch8,lmem[l].pi,lmem[l].zf,nmem[k].ch8,lmem[l].eta);
fprintf(stderr,"pi(~%.8s)=%.15g(%d), eta(~%.8s)=%.15g\n",
nmem[k].ch8,lmem[l+1].pi,lmem[l+1].zf,nmem[k].ch8,lmem[l+1].eta);
}

/*:27*//*47:*/
#line 974 "./sat9.w"

int fixlist(register int k,int b){
register int c,j,l,ll,p,q;
for(;k&1;o,k= lmem[k].rating){
if(o,lmem[k-1].rating<0)l= k;
else l= k-1;
printf(" %s%.8s",l&1?"~":"",nmem[l>>1].ch8);
/*48:*/
#line 987 "./sat9.w"

for(o,p= lmem[l].link;p;o,p= mem[p-1].next){
oo,c= cl(p-1),j= cmem[c].size;
if(j)o,cmem[c].size= 0;
}

/*:48*/
#line 981 "./sat9.w"
;
/*49:*/
#line 997 "./sat9.w"

for(o,p= lmem[l^1].link;p;p= q){
o,q= mem[p-1].next;
oo,c= cl(p-1),j= cmem[c].size;
if(j==0)continue;
oo,mem[p-1].next= removed,cmem[c].size= j-1;
if(j==2){
for(o,p= cmem[c].start;o,mem[p].next==removed;p++);
/*50:*/
#line 1013 "./sat9.w"

ll= mem[p].lit;
if(ll&1){
if(o,lmem[ll].rating){
if(o,lmem[ll-1].rating> 0)goto contra;
}else{
o,lmem[ll-1].rating= -1;
o,lmem[ll].rating= unit,unit= ll,unitcount++;
}
}else{
if(o,lmem[ll+1].rating){
if(o,lmem[ll].rating<0){
contra:printf("\n");
fprintf(stderr,"Oops, clause %d is contradicted",c);
if(b>=0)fprintf(stderr," in bucket %d!\n",b);
else fprintf(stderr," while propagating unit literals!\n");
return 0;
}
}else{
o,lmem[ll].rating= +1;
o,lmem[ll+1].rating= unit,unit= ll+1,unitcount++;
}
}

/*:50*/
#line 1005 "./sat9.w"
;
}
}

/*:49*/
#line 982 "./sat9.w"
;
}
return 1;
}

/*:47*/
#line 88 "./sat9.w"
;
main(int argc,char*argv[]){
register uint c,g,h,i,j,k,l,p,q,r,ii,kk,ll,fcount;
/*5:*/
#line 159 "./sat9.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,"%d",&verbose)-1);break;
case'h':k|= (sscanf(argv[j]+1,"%d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,"%d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,"%d",&random_seed)-1);break;
case'd':k|= (sscanf(argv[j]+1,"%lld",&delta)-1);thresh= delta;break;
case't':k|= (sscanf(argv[j]+1,"%d",&max_iter)-1);break;
case'l':k|= (sscanf(argv[j]+1,"%d",&min_iter)-1);break;
case'c':k|= (sscanf(argv[j]+1,"%d",&confidence)-1);break;
case'p':k|= (sscanf(argv[j]+1,"%lf",&damper)-1);break;
case'e':k|= (sscanf(argv[j]+1,"%lf",&threshold)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0){
fprintf(stderr,
"Usage: %s [v<n>] [h<n>] [b<n>] [s<n>] [d<n>] [t<n>] [l<n>] [c<n>] [p<f>] [e<f>]\n",
argv[0]);
exit(-1);
}
if(damper<0.0||damper> 1.0){
fprintf(stderr,"Parameter p should be between 0.0 and 1.0!\n");
exit(-666);
}
if(confidence<0||confidence> 100){
fprintf(stderr,"Parameter c should be between 0 and 100!\n");
exit(-667);
}

/*:5*/
#line 91 "./sat9.w"
;
/*9:*/
#line 255 "./sat9.w"

gb_init_rand(random_seed);
buf= (char*)malloc(buf_size*sizeof(char));
if(!buf){
fprintf(stderr,"Couldn't allocate the input buffer (buf_size=%d)!\n",
buf_size);
exit(-2);
}
hash= (tmp_var**)malloc(sizeof(tmp_var)<<hbits);
if(!hash){
fprintf(stderr,"Couldn't allocate %d hash list heads (hbits=%d)!\n",
1<<hbits,hbits);
exit(-3);
}
for(h= 0;h<1<<hbits;h++)hash[h]= NULL;

/*:9*//*15:*/
#line 393 "./sat9.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:15*/
#line 92 "./sat9.w"
;
/*10:*/
#line 284 "./sat9.w"

while(1){
if(!fgets(buf,buf_size,stdin))break;
clauses++;
if(buf[strlen(buf)-1]!='\n'){
fprintf(stderr,
"The clause on line %d (%.20s...) is too long for me;\n",clauses,buf);
fprintf(stderr," my buf_size is only %d!\n",buf_size);
fprintf(stderr,"Please use the command-line option b<newsize>.\n");
exit(-4);
}
/*11:*/
#line 321 "./sat9.w"

for(j= k= 0;;){
while(buf[j]==' ')j++;
if(buf[j]=='\n')break;
if(buf[j]<' '||buf[j]> '~'){
fprintf(stderr,"Illegal character (code #%x) in the clause on line %d!\n",
buf[j],clauses);
exit(-5);
}
if(buf[j]=='~')i= 1,j++;
else i= 0;
/*12:*/
#line 346 "./sat9.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 364 "./sat9.w"

{
register vchunk*new_vchunk;
new_vchunk= (vchunk*)malloc(sizeof(vchunk));
if(!new_vchunk){
fprintf(stderr,"Can't allocate a new vchunk!\n");
exit(-6);
}
new_vchunk->prev= cur_vchunk,cur_vchunk= new_vchunk;
cur_tmp_var= &new_vchunk->var[0];
bad_tmp_var= &new_vchunk->var[vars_per_vchunk];
}

/*:13*/
#line 349 "./sat9.w"
;
/*16:*/
#line 397 "./sat9.w"

cur_tmp_var->name.lng= 0;
for(h= l= 0;buf[j+l]> ' '&&buf[j+l]<='~';l++){
if(l> 7){
fprintf(stderr,
"Variable name %.9s... in the clause on line %d is too long!\n",
buf+j,clauses);
exit(-8);
}
h^= hash_bits[buf[j+l]-'!'][l];
cur_tmp_var->name.ch8[l]= buf[j+l];
}
if(l==0)goto empty_clause;
j+= l;
h&= (1<<hbits)-1;

/*:16*/
#line 351 "./sat9.w"
;
/*17:*/
#line 413 "./sat9.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 352 "./sat9.w"
;
if(p->stamp==clauses||p->stamp==-clauses)/*18:*/
#line 427 "./sat9.w"

{
if((p->stamp> 0)==(i> 0))goto empty_clause;
}

/*:18*/
#line 353 "./sat9.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 377 "./sat9.w"

{
register chunk*new_chunk;
new_chunk= (chunk*)malloc(sizeof(chunk));
if(!new_chunk){
fprintf(stderr,"Can't allocate a new chunk!\n");
exit(-7);
}
new_chunk->prev= cur_chunk,cur_chunk= new_chunk;
cur_cell= &new_chunk->cell[0];
bad_cell= &new_chunk->cell[cells_per_chunk];
}

/*:14*/
#line 356 "./sat9.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:12*/
#line 332 "./sat9.w"
;
}
if(k==0){
fprintf(stderr,"(Empty line %d is being ignored)\n",clauses);
nullclauses++;
}
goto clause_done;
empty_clause:/*19:*/
#line 437 "./sat9.w"

while(k){
/*20:*/
#line 446 "./sat9.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 439 "./sat9.w"
;
k--;
}
if((buf[0]!='~')||(buf[1]!=' '))
fprintf(stderr,"(The clause on line %d is always satisfied)\n",clauses);
nullclauses++;

/*:19*/
#line 339 "./sat9.w"
;
clause_done:cells+= k;

/*:11*/
#line 295 "./sat9.w"
;
}
if((vars>>hbits)>=10){
fprintf(stderr,"There are %d variables but only %d hash tables;\n",
vars,1<<hbits);
while((vars>>hbits)>=10)hbits++;
fprintf(stderr," maybe you should use command-line option h%d?\n",hbits);
}
clauses-= nullclauses;
if(clauses==0){
fprintf(stderr,"No clauses were input!\n");
exit(-77);
}
if(vars>=0x80000000){
fprintf(stderr,"Whoa, the input had %llu variables!\n",cells);
exit(-664);
}
if(clauses>=0x80000000){
fprintf(stderr,"Whoa, the input had %llu clauses!\n",cells);
exit(-665);
}
if(cells>=0x100000000){
fprintf(stderr,"Whoa, the input had %llu occurrences of literals!\n",cells);
exit(-666);
}

/*:10*/
#line 93 "./sat9.w"
;
if(verbose&show_basics)
/*22:*/
#line 464 "./sat9.w"

fprintf(stderr,"(%d variables, %d clauses, %llu literals successfully read)\n",
vars,clauses,cells);

/*:22*/
#line 95 "./sat9.w"
;
/*28:*/
#line 572 "./sat9.w"

/*29:*/
#line 580 "./sat9.w"

free(buf);free(hash);
lmem= (literal*)malloc((vars+vars+1)*sizeof(literal));
if(!lmem){
fprintf(stderr,"Oops, I can't allocate the lmem array!\n");
exit(-12);
}
bytes= (vars+vars+1)*sizeof(literal);
nmem= (octa*)malloc(vars*sizeof(octa));
if(!nmem){
fprintf(stderr,"Oops, I can't allocate the nmem array!\n");
exit(-13);
}
bytes+= vars*sizeof(octa);
mem= (mem_item*)malloc(cells*sizeof(mem_item));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the big mem array!\n");
exit(-10);
}
bytes+= cells*sizeof(mem_item);
cmem= (clause*)malloc((clauses+1)*sizeof(clause));
if(!cmem){
fprintf(stderr,"Oops, I can't allocate the cmem array!\n");
exit(-11);
}
bytes+= (clauses+1)*sizeof(clause);

/*:29*/
#line 573 "./sat9.w"
;
/*30:*/
#line 607 "./sat9.w"

for(l= vars+vars;l;l--)o,lmem[l-1].link= 0;

/*:30*/
#line 574 "./sat9.w"
;
/*31:*/
#line 610 "./sat9.w"

for(c= clauses,cur_mcell= mem+cells,kk= 0;c;c--){
o,cmem[c].start= cur_mcell-mem;
k= 0;
/*32:*/
#line 635 "./sat9.w"

for(i= 0;i<2;k++){
/*20:*/
#line 446 "./sat9.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 637 "./sat9.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
cur_mcell--;
o,cur_mcell->lit= l= p+p+(i&1);
oo,cur_mcell->next= lmem[l].link;
o,lmem[l].link= cur_mcell-mem+1;
}

/*:32*/
#line 614 "./sat9.w"
;
if(k> kk)kk= k;
}
if(cur_mcell!=mem){
fprintf(stderr,"Confusion about the number of cells!\n");
exit(-99);
}
o,cmem[0].start= 0;
gam= (double*)malloc(kk*sizeof(double));
if(!gam){
fprintf(stderr,"Oops, I can't allocate the gamma array!\n");
exit(-16);
}
bytes+= kk*sizeof(double);

/*:31*/
#line 576 "./sat9.w"
;
/*33:*/
#line 646 "./sat9.w"

for(c= vars;c;c--){
/*21:*/
#line 455 "./sat9.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:21*/
#line 648 "./sat9.w"
;
o,nmem[c-1].lng= cur_tmp_var->name.lng;
}

/*:33*/
#line 577 "./sat9.w"
;
/*34:*/
#line 655 "./sat9.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL){
fprintf(stderr,"This can't happen (consistency check failure)!\n");
exit(-14);
}
free(cur_chunk);free(cur_vchunk);

/*:34*/
#line 578 "./sat9.w"
;

/*:28*/
#line 96 "./sat9.w"
;
imems= mems,mems= 0;
/*35:*/
#line 667 "./sat9.w"

factor= 1.0;
/*37:*/
#line 707 "./sat9.w"

for(k= 0;k<cells;k++)
mems+= 5,mem[k].eta.d= ((double)(gb_next_rand()))/2147483647.0;
for(k= 0;k<vars+vars;k+= 2)
ooo,lmem[k].eta= 0.0,lmem[k+1].eta= 0.0;

/*:37*/
#line 669 "./sat9.w"
;
for(iter= 0;iter<max_iter;iter++){
if((iter&1)&&iter>=min_iter){
/*39:*/
#line 746 "./sat9.w"

{
factor*= damper;
rein= 1.0-factor;
if(verbose&show_details)
fprintf(stderr," (rein=%.15g)\n",rein);
for(l= 0;l<vars+vars;l+= 2){
if(o,lmem[l].zf)pi0= 0.0;
else o,pi0= lmem[l].pi;
if(o,lmem[l+1].zf)pi1= 0.0;
else o,pi1= lmem[l+1].pi;
if(pi0+pi1==0.0){
if(verbose&show_basics)
fprintf(stderr,
"Sorry, a contradiction was found after iteration %d!\n",iter);
goto contradiction;
}
if(pi1> pi0){
o,lmem[l].rating= (pi0>=0.5?0:1);
if((verbose&show_gory_details)&&lmem[l+1].eta)
fprintf(stderr," eta(~%.8s) reset\n",nmem[l>>1].ch8);
oo,lmem[l].eta= rein*(pi1-pi0)/(pi0+pi1-pi0*pi1),lmem[l+1].eta= 0.0;
}else{
o,lmem[l].rating= (pi1>=0.5?0:-1);
if((verbose&show_gory_details)&&lmem[l].eta)
fprintf(stderr," eta(%.8s) reset\n",nmem[l>>1].ch8);
oo,lmem[l+1].eta= rein*(pi0-pi1)/(pi0+pi1-pi0*pi1),lmem[l].eta= 0.0;
}
}
}

/*:39*/
#line 672 "./sat9.w"
;
/*40:*/
#line 784 "./sat9.w"

for(k= c= 0;c<clauses;c++){
for(o;k<cmem[c+1].start;k++){
oo,l= mem[k].lit,p= lmem[l&-2].rating;
if(p==0)goto ok;
if(((int)p<0)==(l&1))goto ok;
}
goto not_ok;
ok:k= cmem[c+1].start;
continue;
}
if(verbose&show_details)
fprintf(stderr,"Clauses pseudo-satisfied on iteration %d\n",iter+1);
break;
not_ok:

/*:40*/
#line 673 "./sat9.w"
;
}
if(verbose&show_choices)
fprintf(stderr,"beginning iteration %d\n",iter+1);
/*38:*/
#line 713 "./sat9.w"

for(l= 0;l<vars+vars;l++){
if(o,lmem[l].eta==1.0)acc= 1.0,azf= 1;
else acc= 1.0-lmem[l].eta,azf= 0;
for(j= lmem[l].link;j;j= mem[j-1].next){
o,etabar= 1.0-mem[j-1].eta.d;
if(etabar==0.0)azf++;
else acc*= etabar;
}
oo,lmem[l].zf= azf,lmem[l].pi= acc;
}

/*:38*/
#line 677 "./sat9.w"
;
/*41:*/
#line 805 "./sat9.w"

max_diff= 0.0;
for(k= c= 0;c<clauses;c++){
acc= 1.0,azf= 0;
for(o,j= 0;k<cmem[c+1].start;j++,k++){
o,l= mem[k].lit;
if(o,lmem[l^1].zf)pi0= 0.0;
else o,pi0= lmem[l^1].pi;
o,old_eta= mem[k].eta.d;
if(old_eta==1.0){
if(o,lmem[l].zf> 1)pi1= 0.0;
else o,pi1= lmem[l].pi;
}else if(o,lmem[l].zf)pi1= 0.0;
else o,pi1= lmem[l].pi/(1.0-old_eta);
pi1= pi1*(1.0-pi0);
if(pi1==0.0)azf++,o,gam[j]= 0.0;
else{
new_gam= pi1/(pi1+pi0);
o,gam[j]= new_gam;
acc*= new_gam;
}
}
for(i= j;i;i--){
if(o,gam[j-i]==0.0){
if(azf> 1)new_eta= 0.0;
else new_eta= acc;
}else if(azf)new_eta= 0.0;
else new_eta= acc/gam[j-i];
o,diff= new_eta-mem[k-i].eta.d;
if(diff> 0){
if(diff> max_diff)max_diff= diff;
}else if(-diff> max_diff)max_diff= -diff;
o,mem[k-i].eta.d= new_eta;
}
}

/*:41*/
#line 678 "./sat9.w"
;
if(verbose&show_details)
fprintf(stderr," (max diff %.15g, %lld mems)\n",max_diff,mems);
if(delta&&(mems>=thresh)){
thresh+= delta;
fprintf(stderr," after %lld mems, iteration %d had max diff %g\n",
mems,iter+1,max_diff);
}
if(max_diff<threshold&&iter>=min_iter)break;
}
/*42:*/
#line 847 "./sat9.w"

if(iter==max_iter){
if(verbose&show_basics)
fprintf(stderr,"The messages didn't converge.\n");
goto contradiction;
}
if(verbose&show_pis)
/*43:*/
#line 868 "./sat9.w"

{
if(iter<max_iter)
fprintf(stderr,"converged after %d iterations.\n",iter+1);
else fprintf(stderr,"no convergence (diff %g) after %d iterations.\n",
max_diff,max_iter);
fprintf(stderr,"variable      pi(v)        pi(~v)         1    0    *\n");
for(k= 0;k<vars;k++){
double den;
fprintf(stderr,"%8.8s %10.7f(%d) %10.7f(%d)",
nmem[k].ch8,lmem[k+k].pi,lmem[k+k].zf,lmem[k+k+1].pi,lmem[k+k+1].zf);
pi0= lmem[k+k].pi;
if(lmem[k+k].zf)pi0= 0.0;
pi1= lmem[k+k+1].pi;
if(lmem[k+k+1].zf)pi1= 0.0;
den= pi0+pi1-pi0*pi1;
fprintf(stderr,"    %4.2f %4.2f %4.2f\n",
pi1*(1-pi0)/den,pi0*(1-pi1)/den,pi0*pi1/den);
}
}

/*:43*/
#line 854 "./sat9.w"
;
if(verbose&show_histogram)
/*44:*/
#line 889 "./sat9.w"

{
uint hist[10][10];
for(j= 0;j<10;j++)for(k= 0;k<10;k++)hist[j][k]= 0;
for(k= 0;k<vars;k++){
i= (int)(10*lmem[k+k].pi),j= (int)(10*lmem[k+k+1].pi);
if(lmem[k+k].zf)i= 0;
if(lmem[k+k+1].zf)j= 0;
if(i==10)i= 9;
if(j==10)j= 9;
hist[i][j]++;
}
fprintf(stderr,"Histogram of the pi's, after %d iterations:\n",iter+1);
for(j= 10;j;j--){
for(i= 0;i<10;i++)fprintf(stderr,"%7d",hist[i][j-1]);
fprintf(stderr,"\n");
}
}

/*:44*/
#line 856 "./sat9.w"
;
/*45:*/
#line 927 "./sat9.w"

for(k= confidence;k<=100;k++)o,bucket[k]= 2;
unit= 2;
for(l= 0;l<vars+vars;l+= 2){
if(o,lmem[l].zf)pi0= 0.0;
else o,pi0= lmem[l].pi;
if(o,lmem[l+1].zf)pi1= 0.0;
else o,pi1= lmem[l+1].pi;
if(pi0+pi1==0.0){
if(verbose&show_basics)
fprintf(stderr,"Sorry, a contradiction was found!\n");
goto contradiction;
}
acc= (pi1-pi0)/(pi0+pi1-pi0*pi1);
o,lmem[l].rating= acc> 0?+1:acc<0?-1:0;
if(acc<0)acc= -acc;
j= (int)(100.0*acc);
if(j>=confidence){
oo,lmem[l+1].rating= bucket[j];
o,bucket[j]= l+1;
fixcount++;
}
}
if(verbose&show_basics)
fprintf(stderr,"(fixing %d variables after %d iterations, e=%g)\n",
fixcount,iter+1,max_diff);

/*:45*/
#line 857 "./sat9.w"
;
/*46:*/
#line 963 "./sat9.w"

for(k= c= 0;c<clauses;c++){
for(;k<cmem[c+1].start;k++)o,cl(k)= c;
oo,cmem[c].size= k-cmem[c].start;
if(cmem[c].size==1){
/*51:*/
#line 1037 "./sat9.w"

ll= mem[k-1].lit;
if(ll&1){
if(o,lmem[ll].rating){
if(o,lmem[ll-1].rating> 0)goto contra;
}else{
o,lmem[ll-1].rating= -1;
o,lmem[ll].rating= unit,unit= ll,unitcount++;
}
}else{
if(o,lmem[ll+1].rating){
if(o,lmem[ll].rating<0){
contra:printf("\n");
fprintf(stderr,"Oops, clause %d is contradicted!\n",c);
goto contradiction;
}
}else{
o,lmem[ll].rating= +1;
o,lmem[ll+1].rating= unit,unit= ll+1,unitcount++;
}
}

/*:51*/
#line 968 "./sat9.w"
;
}
}

/*:46*/
#line 858 "./sat9.w"
;
/*52:*/
#line 1059 "./sat9.w"

for(k= 100;k>=confidence;k--)
if(ooo,fixlist(bucket[k],k)==0)goto contradiction;
while(unit&1){
p= unit,unit= 2;
if(oo,fixlist(p,-1)==0)goto contradiction;
}
printf("\n");
if(unitcount&&(verbose&show_basics))
fprintf(stderr,"(unit propagation fixed %d more variable%s)\n",
unitcount,unitcount==1?"":"s");

/*:52*/
#line 859 "./sat9.w"
;
/*53:*/
#line 1071 "./sat9.w"

sprintf(name_buf,"/tmp/sat9-%d.dat",random_seed);
out_file= fopen(name_buf,"w");
if(!out_file){
fprintf(stderr,"I can't open `%s' for writing!\n");
exit(-668);
}
for(kk= k= p= c= 0;c<clauses;c++){
o,i= cmem[c].size;
if(i==0){
o,k= cmem[c+1].start;
continue;
}
p++;
while(i> kk)gam[kk++]= 0;
gam[i-1]+= 1;
for(o;k<cmem[c+1].start;k++)if(o,mem[k].next!=removed){
l= mem[k].lit;
fprintf(out_file," %s%.8s",l&1?"~":"",nmem[l>>1].ch8);
}
fprintf(out_file,"\n");
}
fclose(out_file);
fprintf(stderr,"Reduced problem of %d clauses written on file %s\n",
p,name_buf);
for(i= 0;i<kk;i++)if(gam[i])
fprintf(stderr," (%g %d-clauses)\n",gam[i],i+1);

/*:53*/
#line 860 "./sat9.w"
;
goto done;
contradiction:printf("~~?\n");
done:

/*:42*/
#line 688 "./sat9.w"
;

/*:35*/
#line 98 "./sat9.w"
;
if(verbose&show_basics)
fprintf(stderr,"Altogether %llu+%llu mems, %llu bytes.\n",
imems,mems,bytes);
}

/*:3*/
