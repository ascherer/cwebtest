#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define O "%" \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_unused_vars 8 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define false 0
#define true 1
#define unknown -1 \

#define sanity_checking 0 \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

/*2:*/
#line 75 "./sat10.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*5:*/
#line 207 "./sat10.w"

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

/*:5*//*6:*/
#line 234 "./sat10.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:6*//*24:*/
#line 545 "./sat10.w"

typedef struct{
uint start;
uint wlink;
}clause;

/*:24*//*25:*/
#line 562 "./sat10.w"

typedef struct{
uint wlist0,wlist1;
int next;
uint spare;
octa name;
}variable;

/*:25*//*26:*/
#line 572 "./sat10.w"

typedef struct{
int var;
int move;
}state;

/*:26*/
#line 82 "./sat10.w"
;
/*3:*/
#line 107 "./sat10.w"

int random_seed= 0;
int verbose= show_basics+show_unused_vars;
int show_choices_max= 1000000;
int hbits= 8;
int buf_size= 1024;
FILE*out_file;
char*out_name;
FILE*primary_file;
char*primary_name;
int primary_vars;
ullng imems,mems;
ullng bytes;
ullng nodes;
ullng thresh= 0;
ullng delta= 0;
ullng timeout= 0x1fffffffffffffff;
float eps= 0.1;

/*:3*//*7:*/
#line 240 "./sat10.w"

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
int non_clause;

/*:7*//*27:*/
#line 578 "./sat10.w"

uint*mem;
clause*cmem;
variable*vmem;
char*val;
state*smem;
uint active;

/*:27*/
#line 83 "./sat10.w"
;
/*28:*/
#line 594 "./sat10.w"

void print_clause(int c){
register uint k,l;
printf(""O"d:",c);
for(k= cmem[c].start;k<cmem[c-1].start;k++){
l= mem[k];
printf(" "O"s"O".8s",l&1?"~":"",vmem[l>>1].name.ch8);
}
printf("\n");
}

/*:28*//*29:*/
#line 608 "./sat10.w"

void print_watches_for(int l){
register int c;
if(l&1)c= vmem[l>>1].wlist1;
else c= vmem[l>>1].wlist0;
for(;c;c= cmem[c].wlink)
print_clause(c);
}

/*:29*//*30:*/
#line 617 "./sat10.w"

void print_ring(void){
register int p;
printf("Ring:");
if(active){
for(p= vmem[active].next;;p= vmem[p].next){
printf(" "O".8s",
vmem[p].name.ch8);
if(p==active)break;
}
}
printf("\n");
}

/*:30*//*31:*/
#line 636 "./sat10.w"

void sanity(void){
register int k,l,c,v;
if(active){
for(v= vmem[active].next;;v= vmem[v].next){
vmem[v].spare= 1;
if(v==active)break;
}
}
k= 0;
for(v= 1;v<=vars;v++){
for(c= vmem[v].wlist0;c;c= cmem[c].wlink){
k++;
if(mem[cmem[c].start]!=v+v)
fprintf(stderr,"Clause "O"d watches "O"u, not "O"u!\n",
c,mem[cmem[c].start],v+v);
else if(val[v]==false)
fprintf(stderr,"Clause "O"d watches the false literal "O"u!\n",
c,(v+v));
}
for(c= vmem[v].wlist1;c;c= cmem[c].wlink){
k++;
if(mem[cmem[c].start]!=v+v+1)
fprintf(stderr,"Clause "O"d watches "O"u, not "O"u!\n",
c,mem[cmem[c].start],v+v+1);
else if(val[v]==true)
fprintf(stderr,"Clause "O"d watches the false literal "O"u!\n",
c,(v+v+1));
}
if(vmem[v].spare==0&&val[v]==unknown&&
(vmem[v].wlist0||vmem[v].wlist1))
fprintf(stderr,"Variable "O".8s should be in the active ring!\n",
vmem[v].name.ch8);
if(vmem[v].spare==1&&(val[v]!=unknown||
((vmem[v].wlist0|vmem[v].wlist1)==0)))
fprintf(stderr,"Variable "O".8s should not be in the active ring!\n",
vmem[v].name.ch8);
vmem[v].spare= 0;
}
if(k!=clauses)
fprintf(stderr,"Oops: "O"d of "O"lld clauses are being watched!\n",
k,clauses);
}

/*:31*//*32:*/
#line 682 "./sat10.w"

void print_state(int l){
register int k;
fprintf(stderr," after "O"lld mems:",mems);
for(k= 1;k<=l;k++)fprintf(stderr,""O"c",smem[k].move+'0');
fprintf(stderr,"\n");
fflush(stderr);
}

/*:32*/
#line 84 "./sat10.w"
;
main(int argc,char*argv[]){
register uint h,i,j,l,p,q,r,level,kk,pp,qq,ll,force,nextmove;
register int c,cc,k,v0,v,vv,vvv;
/*4:*/
#line 161 "./sat10.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,""O"d",&verbose)-1);break;
case'c':k|= (sscanf(argv[j]+1,""O"d",&show_choices_max)-1);break;
case'h':k|= (sscanf(argv[j]+1,""O"d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,""O"d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1);break;
case'd':k|= (sscanf(argv[j]+1,""O"lld",&delta)-1);thresh= delta;break;
case'e':k|= (sscanf(argv[j]+1,""O"f",&eps)-1);break;
case'x':out_name= argv[j]+1,out_file= fopen(out_name,"w");
if(!out_file)
fprintf(stderr,"I can't open file `"O"s' for output!\n",out_name);
break;
case'V':primary_name= argv[j]+1,primary_file= fopen(primary_name,"r");
if(!primary_file)
fprintf(stderr,"I can't open file `"O"s' for input!\n",primary_name);
break;
case'T':k|= (sscanf(argv[j]+1,"%lld",&timeout)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0){
fprintf(stderr,
"Usage: "O"s [v<n>] [c<n>] [h<n>] [b<n>] [s<n>] [d<n>] [e<f>]",argv[0]);
fprintf(stderr," [x<foo>] [V<foo>] [T<n>] < foo.sat\n");
exit(-1);
}

/*:4*/
#line 88 "./sat10.w"
;
/*8:*/
#line 256 "./sat10.w"

gb_init_rand(random_seed);
buf= (char*)malloc(buf_size*sizeof(char));
if(!buf){
fprintf(stderr,"Couldn't allocate the input buffer (buf_size="O"d)!\n",
buf_size);
exit(-2);
}
hash= (tmp_var**)malloc(sizeof(tmp_var)<<hbits);
if(!hash){
fprintf(stderr,"Couldn't allocate "O"d hash list heads (hbits="O"d)!\n",
1<<hbits,hbits);
exit(-3);
}
for(h= 0;h<1<<hbits;h++)hash[h]= NULL;

/*:8*//*15:*/
#line 425 "./sat10.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:15*/
#line 89 "./sat10.w"
;
/*9:*/
#line 285 "./sat10.w"

if(primary_file)/*10:*/
#line 330 "./sat10.w"

{
while(1){
if(!fgets(buf,buf_size,primary_file))break;
if(buf[strlen(buf)-1]!='\n'){
fprintf(stderr,
"The clause on line "O"lld ("O".20s...) is too long for me;\n",clauses,buf);
fprintf(stderr," my buf_size is only "O"d!\n",buf_size);
fprintf(stderr,"Please use the command-line option b<newsize>.\n");
exit(-4);
}
/*11:*/
#line 351 "./sat10.w"

for(j= k= non_clause= 0;!non_clause;){
while(buf[j]==' ')j++;
if(buf[j]=='\n')break;
if(buf[j]<' '||buf[j]> '~'){
fprintf(stderr,"Illegal character (code #"O"x) in the clause on line "O"lld!\n",
buf[j],clauses);
exit(-5);
}
if(buf[j]=='~')i= 1,j++;
else i= 0;
/*12:*/
#line 375 "./sat10.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 396 "./sat10.w"

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
#line 378 "./sat10.w"
;
/*16:*/
#line 429 "./sat10.w"

cur_tmp_var->name.lng= 0;
for(h= l= 0;buf[j+l]> ' '&&buf[j+l]<='~';l++){
if(l> 7){
fprintf(stderr,
"Variable name "O".9s... in the clause on line "O"lld is too long!\n",
buf+j,clauses);
exit(-8);
}
h^= hash_bits[buf[j+l]-'!'][l];
cur_tmp_var->name.ch8[l]= buf[j+l];
}
if(l==0)non_clause= 1;
else j+= l,h&= (1<<hbits)-1;

/*:16*/
#line 380 "./sat10.w"
;
if(!non_clause){
/*17:*/
#line 444 "./sat10.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 382 "./sat10.w"
;
if(clauses&&(p->stamp==clauses||p->stamp==-clauses))
/*18:*/
#line 458 "./sat10.w"

{
if((p->stamp> 0)==(i> 0))non_clause= 1;
}

/*:18*/
#line 384 "./sat10.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 409 "./sat10.w"

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
#line 387 "./sat10.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}
}

/*:12*/
#line 362 "./sat10.w"
;
}
if(k==0&&!non_clause){
fprintf(stderr,"(Empty line "O"lld is being ignored)\n",clauses);
nullclauses++;
}
if(non_clause)/*19:*/
#line 468 "./sat10.w"

{
while(k){
/*20:*/
#line 479 "./sat10.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 471 "./sat10.w"
;
k--;
}
if(non_clause&&((buf[0]!='~')||(buf[1]!=' ')))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;
}

/*:19*/
#line 368 "./sat10.w"
;
cells+= k;

/*:11*/
#line 341 "./sat10.w"
;
/*19:*/
#line 468 "./sat10.w"

{
while(k){
/*20:*/
#line 479 "./sat10.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 471 "./sat10.w"
;
k--;
}
if(non_clause&&((buf[0]!='~')||(buf[1]!=' ')))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;
}

/*:19*/
#line 342 "./sat10.w"
;
}
cells= nullclauses= 0;
primary_vars= vars;
if(verbose&show_basics)
fprintf(stderr,"("O"d primary variables read from "O"s)\n",
primary_vars,primary_name);
}

/*:10*/
#line 286 "./sat10.w"
;
while(1){
if(!fgets(buf,buf_size,stdin))break;
clauses++;
if(buf[strlen(buf)-1]!='\n'){
fprintf(stderr,
"The clause on line "O"lld ("O".20s...) is too long for me;\n",clauses,buf);
fprintf(stderr," my buf_size is only "O"d!\n",buf_size);
fprintf(stderr,"Please use the command-line option b<newsize>.\n");
exit(-4);
}
/*11:*/
#line 351 "./sat10.w"

for(j= k= non_clause= 0;!non_clause;){
while(buf[j]==' ')j++;
if(buf[j]=='\n')break;
if(buf[j]<' '||buf[j]> '~'){
fprintf(stderr,"Illegal character (code #"O"x) in the clause on line "O"lld!\n",
buf[j],clauses);
exit(-5);
}
if(buf[j]=='~')i= 1,j++;
else i= 0;
/*12:*/
#line 375 "./sat10.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 396 "./sat10.w"

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
#line 378 "./sat10.w"
;
/*16:*/
#line 429 "./sat10.w"

cur_tmp_var->name.lng= 0;
for(h= l= 0;buf[j+l]> ' '&&buf[j+l]<='~';l++){
if(l> 7){
fprintf(stderr,
"Variable name "O".9s... in the clause on line "O"lld is too long!\n",
buf+j,clauses);
exit(-8);
}
h^= hash_bits[buf[j+l]-'!'][l];
cur_tmp_var->name.ch8[l]= buf[j+l];
}
if(l==0)non_clause= 1;
else j+= l,h&= (1<<hbits)-1;

/*:16*/
#line 380 "./sat10.w"
;
if(!non_clause){
/*17:*/
#line 444 "./sat10.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 382 "./sat10.w"
;
if(clauses&&(p->stamp==clauses||p->stamp==-clauses))
/*18:*/
#line 458 "./sat10.w"

{
if((p->stamp> 0)==(i> 0))non_clause= 1;
}

/*:18*/
#line 384 "./sat10.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 409 "./sat10.w"

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
#line 387 "./sat10.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}
}

/*:12*/
#line 362 "./sat10.w"
;
}
if(k==0&&!non_clause){
fprintf(stderr,"(Empty line "O"lld is being ignored)\n",clauses);
nullclauses++;
}
if(non_clause)/*19:*/
#line 468 "./sat10.w"

{
while(k){
/*20:*/
#line 479 "./sat10.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 471 "./sat10.w"
;
k--;
}
if(non_clause&&((buf[0]!='~')||(buf[1]!=' ')))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;
}

/*:19*/
#line 368 "./sat10.w"
;
cells+= k;

/*:11*/
#line 297 "./sat10.w"
;
}
if(!primary_file)primary_vars= vars;
if((vars>>hbits)>=10){
fprintf(stderr,"There are "O"lld variables but only "O"d hash tables;\n",
vars,1<<hbits);
while((vars>>hbits)>=10)hbits++;
fprintf(stderr," maybe you should use command-line option h"O"d?\n",hbits);
}
clauses-= nullclauses;
if(clauses==0){
fprintf(stderr,"No clauses were input!\n");
exit(-77);
}
if(vars>=0x80000000){
fprintf(stderr,"Whoa, the input had "O"llu variables!\n",cells);
exit(-664);
}
if(clauses>=0x80000000){
fprintf(stderr,"Whoa, the input had "O"llu clauses!\n",cells);
exit(-665);
}
if(cells>=0x100000000){
fprintf(stderr,"Whoa, the input had "O"llu occurrences of literals!\n",cells);
exit(-666);
}

/*:9*/
#line 90 "./sat10.w"
;
if(verbose&show_basics)
/*22:*/
#line 497 "./sat10.w"

fprintf(stderr,
"("O"lld variables, "O"lld clauses, "O"llu literals successfully read)\n",
vars,clauses,cells);

/*:22*/
#line 92 "./sat10.w"
;
/*33:*/
#line 696 "./sat10.w"

/*34:*/
#line 704 "./sat10.w"

free(buf);free(hash);
mem= (uint*)malloc(cells*sizeof(uint));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the big mem array!\n");
exit(-10);
}
bytes= cells*sizeof(uint);
cmem= (clause*)malloc((clauses+1)*sizeof(clause));
if(!cmem){
fprintf(stderr,"Oops, I can't allocate the cmem array!\n");
exit(-11);
}
bytes+= (clauses+1)*sizeof(clause);
vmem= (variable*)malloc((vars+1)*sizeof(variable));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-12);
}
bytes+= (vars+1)*sizeof(variable);
smem= (state*)malloc((vars+1)*sizeof(state));
if(!smem){
fprintf(stderr,"Oops, I can't allocate the smem array!\n");
exit(-13);
}
bytes+= (vars+1)*sizeof(state);
val= (char*)malloc((vars+1)*sizeof(char));
if(!val){
fprintf(stderr,"Oops, I can't allocate the val array!\n");
exit(-14);
}
bytes+= (vars+1)*sizeof(char);

/*:34*/
#line 697 "./sat10.w"
;
/*35:*/
#line 737 "./sat10.w"

for(j= 1;j<=vars;j++){
o,vmem[j].wlist0= vmem[j].wlist1= 0;
o,val[j]= unknown;
}
for(c= clauses,j= 0;c;c--){
o,cmem[c].start= k= j;
/*36:*/
#line 762 "./sat10.w"

for(i= 0;i<2;){
/*20:*/
#line 479 "./sat10.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 764 "./sat10.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1)+2;
o,mem[j++]= p;
}

/*:36*/
#line 744 "./sat10.w"
;
l= mem[k];
if(l&1)ooo,p= vmem[l>>1].wlist1,cmem[c].wlink= p,vmem[l>>1].wlist1= c;
else ooo,p= vmem[l>>1].wlist0,cmem[c].wlink= p,vmem[l>>1].wlist0= c;
}
if(j!=cells){
fprintf(stderr,"Oh oh, something happened to "O"d cells!\n",
(int)cells-j);
exit(-15);
}
o,cmem[c].start= j;

/*:35*/
#line 699 "./sat10.w"
;
/*37:*/
#line 771 "./sat10.w"

for(c= vars;c;c--){
/*21:*/
#line 488 "./sat10.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:21*/
#line 773 "./sat10.w"
;
o,vmem[c].name.lng= cur_tmp_var->name.lng;
}

/*:37*/
#line 700 "./sat10.w"
;
/*38:*/
#line 780 "./sat10.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL){
fprintf(stderr,"This can't happen (consistency check failure)!\n");
exit(-14);
}
free(cur_chunk);free(cur_vchunk);

/*:38*/
#line 701 "./sat10.w"
;
if(out_file)/*39:*/
#line 790 "./sat10.w"

{
for(k= 0,c= clauses;c;c--){
for(;k<cmem[c-1].start;k++){
l= mem[k];
fprintf(out_file," "O"s"O".8s",
l&1?"~":"",vmem[l>>1].name.ch8);
}
fprintf(out_file,"\n");
}
fflush(out_file);
}

/*:39*/
#line 702 "./sat10.w"
;

/*:33*/
#line 93 "./sat10.w"
;
imems= mems,mems= 0;
/*40:*/
#line 814 "./sat10.w"

o,level= 0,smem[0].move= 0;
/*41:*/
#line 836 "./sat10.w"

for(active= j= 0,k= vars;k;k--)
if((o,vmem[k].wlist0)||(vmem[k].wlist1)){
if(active==0)active= k;
o,vmem[k].next= j,j= k;
}
if(active)o,vmem[active].next= j;

/*:41*/
#line 816 "./sat10.w"
;
choose:if(sanity_checking)sanity();
if(delta&&(mems>=thresh))thresh+= delta,print_state(level);
if(mems> timeout){
fprintf(stderr,"TIMEOUT!\n");
goto done;
}
/*42:*/
#line 860 "./sat10.w"

if(active==0)goto satisfied;
if(verbose&show_details){
fprintf(stderr," active ring:");
for(v= vmem[active].next;;v= vmem[v].next){
fprintf(stderr," "O".8s",vmem[v].name.ch8);
if(v==active)break;
}
fprintf(stderr,"\n");
}
vv= active,vvv= 0;
newv:o,v= vmem[vv].next;
force= 0;
/*43:*/
#line 901 "./sat10.w"

for(o,c= vmem[v].wlist0;c;o,c= cmem[c].wlink){
for(oo,k= cmem[c].start+1;k<cmem[c-1].start;k++)
if(oo,val[mem[k]>>1]!=(mem[k]&1))goto unforced0;
if(verbose&show_details)
fprintf(stderr,"(Clause "O"d reduced to "O".8s)\n",
c,vmem[v].name.ch8);
force= 1;
goto forced0;
unforced0:continue;
}
forced0:

/*:43*/
#line 873 "./sat10.w"
;
/*44:*/
#line 914 "./sat10.w"

for(o,c= vmem[v].wlist1;c;o,c= cmem[c].wlink){
for(oo,k= cmem[c].start+1;k<cmem[c-1].start;k++)
if(oo,val[mem[k]>>1]!=(mem[k]&1))goto unforced1;
if(verbose&show_details)
fprintf(stderr,"(Clause "O"d reduced to ~"O".8s)\n",
c,vmem[v].name.ch8);
force+= 2;
goto forced1;
unforced1:continue;
}
forced1:

/*:44*/
#line 874 "./sat10.w"
;
if(force==3)goto backup;
if(force){
nextmove= force+3;
active= vv;
goto forcedmove;
}
if(vvv==0&&v<=primary_vars)vvv= vv;

if(v==active){
if(vvv)vv= active= vvv;
v= vmem[active].next;
goto branch;
}
vv= v;goto newv;

/*:42*/
#line 824 "./sat10.w"
;
branch:o,nextmove= (vmem[v].wlist0==0||vmem[v].wlist1!=0);
nodes++;
forcedmove:level++;

o,smem[level].var= v,smem[level].move= nextmove;
if(active==v)active= 0;
else oo,h= vmem[v].next,vmem[active].next= h;
makemove:/*45:*/
#line 927 "./sat10.w"

if((verbose&show_choices)&&level<=show_choices_max){
fprintf(stderr,"Level "O"d, ",
level);
switch(nextmove){
case 0:fprintf(stderr,"trying "O".8s",
vmem[v].name.ch8);break;
case 1:fprintf(stderr,"trying ~"O".8s",
vmem[v].name.ch8);break;
case 2:fprintf(stderr,"retrying "O".8s",
vmem[v].name.ch8);break;
case 3:fprintf(stderr,"retrying ~"O".8s",
vmem[v].name.ch8);break;
case 4:fprintf(stderr,"forcing "O".8s",
vmem[v].name.ch8);break;
case 5:fprintf(stderr,"forcing ~"O".8s",
vmem[v].name.ch8);break;
}
fprintf(stderr,", "O"lld mems\n",
mems);
}
if(nextmove&1){
o,val[v]= false;
oo,c= vmem[v].wlist0,vmem[v].wlist0= 0,ll= v+v;
}else{
o,val[v]= true;
oo,c= vmem[v].wlist1,vmem[v].wlist1= 0,ll= v+v+1;
}
/*46:*/
#line 957 "./sat10.w"

for(;c;c= cc){
o,cc= cmem[c].wlink;
for(oo,j= cmem[c].start,k= j+1;k<cmem[c-1].start;k++){
o,l= mem[k];
if(o,val[l>>1]!=(l&1))break;
}
if(k==cmem[c-1].start){
fprintf(stderr,"Clause "O"d can't be watched!\n",
c);
exit(-18);
}
oo,mem[k]= ll,mem[j]= l;
/*47:*/
#line 979 "./sat10.w"

if(verbose&show_details)
fprintf(stderr,"(Clause "O"d now watches "O"s"O".8s)\n",
c,l&1?"~":"",vmem[l>>1].name.ch8);
o,p= vmem[l>>1].wlist0,q= vmem[l>>1].wlist1;
if(val[l>>1]==unknown&&p==0&&q==0){
if(active==0)o,active= h= l>>1,vmem[active].next= h;
else oo,vmem[l>>1].next= h,h= l>>1,vmem[active].next= h;
}
if(l&1)oo,cmem[c].wlink= q,vmem[l>>1].wlist1= c;
else oo,cmem[c].wlink= p,vmem[l>>1].wlist0= c;

/*:47*/
#line 970 "./sat10.w"
;
}

/*:46*/
#line 955 "./sat10.w"
;

/*:45*/
#line 832 "./sat10.w"
;
goto choose;
backup:/*48:*/
#line 994 "./sat10.w"

active= vv,h= v;
while(o,smem[level].move>=2){
v= smem[level].var;
o,val[v]= unknown;
if((o,vmem[v].wlist0!=0)||(vmem[v].wlist1!=0))
oo,vmem[v].next= h,h= v,vmem[active].next= h;
level--;
}
if(level){
nextmove= 3-smem[level].move;
oo,v= smem[level].var,smem[level].move= nextmove;
goto makemove;
}
if(1){
printf("~\n");
if(verbose&show_basics)fprintf(stderr,"UNSAT\n");
}else{
satisfied:if(verbose&show_basics)fprintf(stderr,"!SAT!\n");
/*49:*/
#line 1016 "./sat10.w"

for(k= 1;k<=level;k++){
l= (smem[k].var<<1)+(smem[k].move&1);
printf(" "O"s"O".8s",l&1?"~":"",vmem[l>>1].name.ch8);
if(out_file)fprintf(out_file," "O"s"O".8s",l&1?"":"~",vmem[l>>1].name.ch8);
}
printf("\n");
if(level<vars){
if(verbose&show_unused_vars)printf("(Unused:");
for(v= 0;v<vars;v++)if(val[v]==unknown){
if(verbose&show_unused_vars)printf(" "O".8s",vmem[v].name.ch8);
if(out_file)fprintf(out_file," "O".8s",vmem[v].name.ch8);
}
if(verbose&show_unused_vars)printf(")\n");
}
if(out_file)fprintf(out_file,"\n");

/*:49*/
#line 1013 "./sat10.w"
;
}

/*:48*/
#line 834 "./sat10.w"
;

/*:40*/
#line 95 "./sat10.w"
;
done:if(verbose&show_basics)
fprintf(stderr,
"Altogether "O"llu+"O"llu mems, "O"llu bytes, "O"llu nodes.\n",
imems,mems,bytes,nodes);
}

/*:2*/
