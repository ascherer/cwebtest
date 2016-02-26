#define o mems++
#define oo mems+= 2
#define ooo mems+= 3 \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_gory_details 8 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define value(l) (vmem[(l) >>1].val^((l) &1) )  \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

/*3:*/
#line 66 "sat8.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*6:*/
#line 189 "sat8.w"

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
#line 216 "sat8.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:7*//*24:*/
#line 496 "sat8.w"

typedef struct{
uint val;
uint breakcount;
uint pos_start,neg_start;
}variable;
typedef struct{
uint start;
uint tcount;
uint fplace;
uint fslot;
}clause;

/*:24*/
#line 73 "sat8.w"
;
/*4:*/
#line 97 "sat8.w"

int random_seed= 0;
int verbose= show_basics;
int hbits= 8;
int buf_size= 1024;
ullng maxsteps;
unsigned int maxthresh= 50;
int maxtrials= 1000000;
double nongreedprob= 0.4;
unsigned long nongreedthresh;
ullng imems,mems;
ullng thresh= 0;
ullng delta= 0;
ullng timeout= 0x1fffffffffffffff;
ullng bytes;

/*:4*//*8:*/
#line 222 "sat8.w"

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
#line 509 "sat8.w"

clause*cmem;
variable*vmem;
uint*mem;
uint*cur_mcell;
uint*tmem;
octa*nmem;
int trial;
ullng step;
uint*best;

/*:25*/
#line 74 "sat8.w"
;
/*26:*/
#line 524 "sat8.w"

void print_clause(uint c){
register uint l,ll;
fprintf(stderr,"%d:",c);
for(l= cmem[c-1].start;l<cmem[c].start;l++){
ll= mem[l];
fprintf(stderr," %s%.8s(%d)",ll&1?"~":"",nmem[ll>>1].ch8,value(ll));
}
fprintf(stderr,"\n");
}

/*:26*//*27:*/
#line 538 "sat8.w"

void print_unsat_clause(uint c){
register uint l,ll;
fprintf(stderr,"%d:",c);
for(l= cmem[c-1].start;l<cmem[c].start;l++){
ll= mem[l];
fprintf(stderr," %s%.8s(%d)",ll&1?"~":"",nmem[ll>>1].ch8,
vmem[ll>>1].breakcount);
}
fprintf(stderr,"\n");
}

/*:27*//*28:*/
#line 553 "sat8.w"

void print_literal_uses(uint l){
register uint ll,c;
ll= l>>1;
fprintf(stderr,"%s%.8s(%d) is in",l&1?"~":"",nmem[ll].ch8,value(l));
for(c= (l&1?vmem[ll].neg_start:vmem[ll].pos_start);
c<(l&1?vmem[ll+1].pos_start:vmem[ll].neg_start);c++)
fprintf(stderr," %d",tmem[c]);
fprintf(stderr,"\n");
}

/*:28*/
#line 75 "sat8.w"
;
main(int argc,char*argv[]){
register uint c,g,h,i,j,k,l,p,q,r,ii,kk,ll,fcount;
/*5:*/
#line 145 "sat8.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,"%d",&verbose)-1);break;
case'h':k|= (sscanf(argv[j]+1,"%d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,"%d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,"%d",&random_seed)-1);break;
case'd':k|= (sscanf(argv[j]+1,"%lld",&delta)-1);thresh= delta;break;
case't':k|= (sscanf(argv[j]+1,"%llu",&maxsteps)-1);break;
case'c':k|= (sscanf(argv[j]+1,"%u",&maxthresh)-1);break;
case'w':k|= (sscanf(argv[j]+1,"%d",&maxtrials)-1);break;
case'p':k|= (sscanf(argv[j]+1,"%lf",&nongreedprob)-1);break;
case'T':k|= (sscanf(argv[j]+1,"%lld",&timeout)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0){
fprintf(stderr,
"Usage: %s [v<n>] [h<n>] [b<n>] [s<n>] [d<n>]",argv[0]);
fprintf(stderr," [t<n>] [c<n>] [w<n>] [p<f>] [T<n>] < foo.sat\n");
exit(-1);
}
if(nongreedprob<0.0||nongreedprob> 1.0){
fprintf(stderr,"Parameter p should be between 0.0 and 1.0!\n");
exit(-666);
}

/*:5*/
#line 78 "sat8.w"
;
/*9:*/
#line 237 "sat8.w"

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
#line 375 "sat8.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:15*/
#line 79 "sat8.w"
;
/*10:*/
#line 266 "sat8.w"

while(1){
if(!fgets(buf,buf_size,stdin))break;
clauses++;
if(buf[strlen(buf)-1]!='\n'){
fprintf(stderr,
"The clause on line %lld (%.20s...) is too long for me;\n",clauses,buf);
fprintf(stderr," my buf_size is only %d!\n",buf_size);
fprintf(stderr,"Please use the command-line option b<newsize>.\n");
exit(-4);
}
/*11:*/
#line 303 "sat8.w"

for(j= k= 0;;){
while(buf[j]==' ')j++;
if(buf[j]=='\n')break;
if(buf[j]<' '||buf[j]> '~'){
fprintf(stderr,"Illegal character (code #%x) in the clause on line %lld!\n",
buf[j],clauses);
exit(-5);
}
if(buf[j]=='~')i= 1,j++;
else i= 0;
/*12:*/
#line 328 "sat8.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 346 "sat8.w"

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
#line 331 "sat8.w"
;
/*16:*/
#line 379 "sat8.w"

cur_tmp_var->name.lng= 0;
for(h= l= 0;buf[j+l]> ' '&&buf[j+l]<='~';l++){
if(l> 7){
fprintf(stderr,
"Variable name %.9s... in the clause on line %lld is too long!\n",
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
#line 333 "sat8.w"
;
/*17:*/
#line 395 "sat8.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 334 "sat8.w"
;
if(p->stamp==clauses||p->stamp==-clauses)/*18:*/
#line 409 "sat8.w"

{
if((p->stamp> 0)==(i> 0))goto empty_clause;
}

/*:18*/
#line 335 "sat8.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 359 "sat8.w"

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
#line 338 "sat8.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:12*/
#line 314 "sat8.w"
;
}
if(k==0){
fprintf(stderr,"(Empty line %lld is being ignored)\n",clauses);
nullclauses++;
}
goto clause_done;
empty_clause:/*19:*/
#line 419 "sat8.w"

while(k){
/*20:*/
#line 428 "sat8.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 421 "sat8.w"
;
k--;
}
if((buf[0]!='~')||(buf[1]!=' '))
fprintf(stderr,"(The clause on line %lld is always satisfied)\n",clauses);
nullclauses++;

/*:19*/
#line 321 "sat8.w"
;
clause_done:cells+= k;

/*:11*/
#line 277 "sat8.w"
;
}
if((vars>>hbits)>=10){
fprintf(stderr,"There are %llu variables but only %d hash tables;\n",
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
#line 80 "sat8.w"
;
if(verbose&show_basics)
/*22:*/
#line 449 "sat8.w"

fprintf(stderr,
"(%llu variables, %llu clauses, %llu literals successfully read)\n",
vars,clauses,cells);

/*:22*/
#line 82 "sat8.w"
;
/*29:*/
#line 568 "sat8.w"

/*30:*/
#line 577 "sat8.w"

free(buf);free(hash);
vmem= (variable*)malloc((vars+1)*sizeof(variable));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-12);
}
bytes= (vars+1)*sizeof(variable);
nmem= (octa*)malloc(vars*sizeof(octa));
if(!nmem){
fprintf(stderr,"Oops, I can't allocate the nmem array!\n");
exit(-13);
}
bytes+= vars*sizeof(octa);
mem= (uint*)malloc(cells*sizeof(uint));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the big mem array!\n");
exit(-10);
}
bytes+= cells*sizeof(uint);
tmem= (uint*)malloc(cells*sizeof(uint));
if(!tmem){
fprintf(stderr,"Oops, I can't allocate the big tmem array!\n");
exit(-14);
}
bytes+= cells*sizeof(uint);
cmem= (clause*)malloc((clauses+1)*sizeof(clause));
if(!cmem){
fprintf(stderr,"Oops, I can't allocate the cmem array!\n");
exit(-11);
}
bytes+= (clauses+1)*sizeof(clause);

/*:30*/
#line 569 "sat8.w"
;
/*31:*/
#line 610 "sat8.w"

for(c= vars;c;c--)o,vmem[c-1].pos_start= vmem[c-1].neg_start= 0;

/*:31*/
#line 570 "sat8.w"
;
/*32:*/
#line 613 "sat8.w"

for(c= clauses,cur_mcell= mem+cells,kk= 0;c;c--){
o,cmem[c].start= cur_mcell-mem,k= 0;
/*33:*/
#line 637 "sat8.w"

for(i= 0;i<2;k++){
/*20:*/
#line 428 "sat8.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 639 "sat8.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
cur_mcell--;
o,*cur_mcell= l= p+p+(i&1);
if(l&1)oo,vmem[l>>1].neg_start++;
else oo,vmem[l>>1].pos_start++;
}

/*:33*/
#line 616 "sat8.w"
;
if(k> kk)kk= k;
}
if(cur_mcell!=mem){
fprintf(stderr,"Confusion about the number of cells!\n");
exit(-99);
}
cmem[0].start= 0;
best= (uint*)malloc(kk*sizeof(uint));
if(!best){
fprintf(stderr,"Oops, I can't allocate the best array!\n");
exit(-16);
}
bytes+= kk*sizeof(uint);

/*:32*/
#line 572 "sat8.w"
;
/*35:*/
#line 662 "sat8.w"

for(c= vars;c;c--){
/*21:*/
#line 440 "sat8.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:21*/
#line 664 "sat8.w"
;
o,nmem[c-1].lng= cur_tmp_var->name.lng;
}

/*:35*/
#line 573 "sat8.w"
;
/*34:*/
#line 648 "sat8.w"

for(j= k= 0;k<vars;k++){
o,i= vmem[k].pos_start,ii= vmem[k].neg_start;
o,vmem[k].pos_start= j+i,vmem[k].neg_start= j+i+ii;
j= j+i+ii;
}
o,vmem[k].pos_start= j;
for(c= k= 0,o,kk= cmem[1].start;k<cells;k++){
if(k==kk)o,c++,kk= cmem[c+1].start;
l= mem[k];
if(l&1)ooo,i= vmem[l>>1].neg_start-1,tmem[i]= c,vmem[l>>1].neg_start= i;
else ooo,i= vmem[l>>1].pos_start-1,tmem[i]= c,vmem[l>>1].pos_start= i;
}

/*:34*/
#line 574 "sat8.w"
;
/*36:*/
#line 671 "sat8.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL){
fprintf(stderr,"This can't happen (consistency check failure)!\n");
exit(-14);
}
free(cur_chunk);free(cur_vchunk);

/*:36*/
#line 575 "sat8.w"
;

/*:29*/
#line 83 "sat8.w"
;
imems= mems,mems= 0;
/*37:*/
#line 683 "sat8.w"

if(maxsteps==0)maxsteps= maxthresh*vars;
nongreedthresh= nongreedprob*(unsigned long)0x80000000;
for(trial= 0;trial<maxtrials;trial++){
if(delta&&(mems>=thresh)){
thresh+= delta;
fprintf(stderr," after %lld mems, beginning trial %d\n",mems,trial+1);
}else if(verbose&show_choices)
fprintf(stderr,"beginning trial %d\n",trial+1);
/*38:*/
#line 715 "sat8.w"

for(k= 0,r= 1;k<vars;k++){
if(r==1)mems+= 4,r= gb_next_rand()+(1U<<31);
o,vmem[k].val= r&1,r>>= 1;
vmem[k].breakcount= 0;
}

/*:38*/
#line 692 "sat8.w"
;
if(verbose&show_details)/*47:*/
#line 839 "sat8.w"

{
fprintf(stderr," initial guess");
for(k= 0;k<vars;k++)
fprintf(stderr," %s%.8s",vmem[k].val?"":"~",nmem[k].ch8);
fprintf(stderr,"\n");
}

/*:47*/
#line 693 "sat8.w"
;
/*39:*/
#line 722 "sat8.w"

fcount= 0;
for(c= k= 0;c<clauses;c++){
o,kk= cmem[c+1].start;
p= 0;
for(;k<kk;k++){
o,l= mem[k];
if(o,value(l))p++,ll= l;
}
o,cmem[c].tcount= p;
if(p<=1){
if(p)oo,vmem[ll>>1].breakcount++;
else oo,cmem[c].fplace= fcount,cmem[fcount++].fslot= c;
}
}

/*:39*/
#line 694 "sat8.w"
;
for(step= 0;;step++){
if(fcount==0)/*48:*/
#line 847 "sat8.w"

{
for(k= 0;k<vars;k++)
printf(" %s%.8s",vmem[k].val?"":"~",nmem[k].ch8);
printf("\n");
if(verbose&show_basics)fprintf(stderr,"!SAT!\n");
goto done;
}

/*:48*/
#line 696 "sat8.w"
;
if(mems> timeout){
fprintf(stderr,"TIMEOUT!\n");
goto done;
}
if(step==maxsteps)break;
/*40:*/
#line 738 "sat8.w"

if(verbose&show_gory_details){
fprintf(stderr,"currently false:\n");
for(k= 0;k<fcount;k++)print_unsat_clause(cmem[k].fslot+1);
}
mems+= 5,c= cmem[gb_unif_rand(fcount)].fslot;
if(verbose&show_choices)
fprintf(stderr,"in %u(%d)",c+1,fcount);

/*:40*/
#line 702 "sat8.w"
;
/*41:*/
#line 747 "sat8.w"

oo,k= cmem[c].start,kk= cmem[c+1].start,h= kk-k;
ooo,p= mem[k],r= vmem[p>>1].breakcount,best[0]= p,j= 1;
for(k++;k<kk;k++){
oo,p= mem[k],q= vmem[p>>1].breakcount;
if(q<=r){
if(q<r)o,r= q,best[0]= p,j= 1;
else o,best[j++]= p;
}
}
if(r==0)goto greedy;
if(mems+= 4,(gb_next_rand()<nongreedthresh)){
mems+= 5,l= mem[kk-1-gb_unif_rand(h)],g= 0;
goto got_l;
}
greedy:g= 1;
if(j==1)l= best[0];
else mems+= 5,l= best[gb_unif_rand(j)];
got_l:p= l>>1;
if(verbose&show_choices){
if(verbose&show_details)
fprintf(stderr,", %d*%d of %d%s,",r,j,h,g?"":" nongreedy");
fprintf(stderr," flip %s%.8s (cost %d)\n",
vmem[p].val?"":"~",nmem[p].ch8,vmem[p].breakcount);
}

/*:41*/
#line 703 "sat8.w"
;
/*42:*/
#line 775 "sat8.w"

if(l&1){
oo,k= vmem[p].neg_start,kk= vmem[p+1].pos_start;
/*43:*/
#line 790 "sat8.w"

for(h= 0;k<kk;k++){
ooo,c= tmem[k],j= cmem[c].tcount,cmem[c].tcount= j+1;
if(j<=1){
if(j)/*45:*/
#line 816 "sat8.w"

{
for(o,i= cmem[c].start;;i++){
o,q= mem[i];
if(o,value(q))break;
}
o,vmem[q>>1].breakcount--;
}

/*:45*/
#line 794 "sat8.w"

else{
oo,i= cmem[c].fplace,q= cmem[--fcount].fslot;
oo,cmem[i].fslot= q,cmem[q].fplace= i;
h++;
}
}
}

/*:43*/
#line 778 "sat8.w"
;
o,vmem[p].breakcount= h,vmem[p].val= 0;
k= vmem[p].pos_start,kk= vmem[p].neg_start;
/*44:*/
#line 803 "sat8.w"

for(;k<kk;k++){
ooo,c= tmem[k],j= cmem[c].tcount-1,cmem[c].tcount= j;
if(j<=1){
if(j)/*46:*/
#line 829 "sat8.w"

{
for(o,ii= i= cmem[c].start;;i++){
o,q= mem[i];
if(o,value(q))break;
}
o,vmem[q>>1].breakcount++;
if(i!=ii)oo,mem[i]= mem[ii],mem[ii]= q;
}

/*:46*/
#line 807 "sat8.w"

else{
oo,cmem[fcount].fslot= c,cmem[c].fplace= fcount++;
}
}
}

/*:44*/
#line 781 "sat8.w"
;
}else{
o,k= vmem[p].pos_start,kk= vmem[p].neg_start;
/*43:*/
#line 790 "sat8.w"

for(h= 0;k<kk;k++){
ooo,c= tmem[k],j= cmem[c].tcount,cmem[c].tcount= j+1;
if(j<=1){
if(j)/*45:*/
#line 816 "sat8.w"

{
for(o,i= cmem[c].start;;i++){
o,q= mem[i];
if(o,value(q))break;
}
o,vmem[q>>1].breakcount--;
}

/*:45*/
#line 794 "sat8.w"

else{
oo,i= cmem[c].fplace,q= cmem[--fcount].fslot;
oo,cmem[i].fslot= q,cmem[q].fplace= i;
h++;
}
}
}

/*:43*/
#line 784 "sat8.w"
;
o,vmem[p].breakcount= h,vmem[p].val= 1;
o,k= kk,kk= vmem[p+1].pos_start;
/*44:*/
#line 803 "sat8.w"

for(;k<kk;k++){
ooo,c= tmem[k],j= cmem[c].tcount-1,cmem[c].tcount= j;
if(j<=1){
if(j)/*46:*/
#line 829 "sat8.w"

{
for(o,ii= i= cmem[c].start;;i++){
o,q= mem[i];
if(o,value(q))break;
}
o,vmem[q>>1].breakcount++;
if(i!=ii)oo,mem[i]= mem[ii],mem[ii]= q;
}

/*:46*/
#line 807 "sat8.w"

else{
oo,cmem[fcount].fslot= c,cmem[c].fplace= fcount++;
}
}
}

/*:44*/
#line 787 "sat8.w"
;
}

/*:42*/
#line 704 "sat8.w"
;
}
}
printf("~?\n");
if(verbose&show_basics)fprintf(stderr,"DUNNO\n");
trial--;
done:

/*:37*/
#line 85 "sat8.w"
;
if(verbose&show_basics)
fprintf(stderr,
"Altogether %llu+%llu mems, %llu bytes, %d trial%s, %llu steps.\n",
imems,mems,bytes,trial+1,trial?"s":"",step);
}

/*:3*/
