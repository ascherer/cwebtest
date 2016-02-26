#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define O "%"
#define mod % \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_gory_details 8
#define show_warmlearn 16 \

#define show_recycling 32
#define show_recycling_details 64 \

#define show_restarts 128
#define show_initial_clauses 256 \

#define show_watches 512
#define show_experiments 4096 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define size(c) mem[(c) -1].lit
#define link0(c) mem[(c) -2].lit
#define link1(c) mem[(c) -3].lit
#define clause_extra 3
#define sign_bit 0x80000000
#define range(c) mem[(c) -4].lit
#define activ(c) mem[(c) -5].flt
#define activ_as_lit(c) ((ullng) mem[(c) -5].lit<<32) 
#define learned_supplement 2 \

#define learned_extra (clause_extra+learned_supplement)  \

#define bar(l) ((l) ^1) 
#define thevar(l) ((l) >>1) 
#define litname(l) (l) &1?"~":"",vmem[thevar(l) ].name.ch8
#define poslit(v) ((v) <<1) 
#define neglit(v) (((v) <<1) +1) 
#define unset 0xffffffff
#define isknown(l) (vmem[thevar(l) ].value!=unset) 
#define iscontrary(l) ((vmem[thevar(l) ].value^(l) ) &1)  \

#define sanity_checking 0 \

#define two_to_the_32 4294967296.0 \

#define memk_max_default 26 \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

#define two_to_the_31 ((unsigned long) 0x80000000)  \

#define tiny 2.225073858507201383e-308 \
 \

#define single_tiny 1.1754943508222875080e-38 \
 \

#define buckets 256
#define badlevel 16.0 \

/*2:*/
#line 93 "./sat13.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*9:*/
#line 384 "./sat13.w"

typedef union{
char ch8[8];
uint u2[2];
ullng lng;
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

/*:9*//*10:*/
#line 411 "./sat13.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:10*//*28:*/
#line 848 "./sat13.w"

typedef union{
uint lit;
float flt;
}cel;

/*:28*//*29:*/
#line 876 "./sat13.w"

typedef struct{
octa name;
double activity;
uint value;
int tloc;
int hloc;
uint oldval;
uint stamp;
uint filler;
}variable;

/*:29*//*30:*/
#line 892 "./sat13.w"

typedef struct{
int reason;
uint watch;
uint bimp_start;
uint bimp_end;
}literal;

/*:30*/
#line 100 "./sat13.w"
;
/*4:*/
#line 218 "./sat13.w"

int random_seed= 0;
int verbose= show_basics;
uint show_choices_max= 1000000;
int hbits= 8;
int print_state_cutoff= 0;
int buf_size= 1024;
FILE*out_file;
char*out_name;
FILE*restart_file;
char*restart_name;
FILE*learned_file;
char*learned_name;
int learn_save= 10000;
ullng learned_out;
FILE*polarity_infile;
char*polarity_in_name;
FILE*polarity_outfile;
char*polarity_out_name;
ullng imems,mems;
ullng bytes;
ullng nodes;
ullng thresh= 0;
ullng delta= 0;
ullng timeout= 0x1fffffffffffffff;
uint memk_max= memk_max_default;
uint max_cells_used;
int trivial_limit= 10;
float var_rho= 0.9;
float clause_rho= 0.9995;
float rand_prob= 0.02;
float true_prob= 0.50;
uint rand_prob_thresh;
uint true_prob_thresh;
float alpha= 0.4;
int warmups= 0;
ullng total_learned;
double cells_learned;
double cells_prelearned;
ullng discards;
ullng trivials;
ullng subsumptions;
ullng doomsday= 0x8000000000000000;

ullng next_recycle;
ullng recycle_bump= 1000;
ullng recycle_inc= 500;
ullng next_restart;
ullng restart_psi;
float restart_psi_fraction= .05;
ullng actual_restarts;

/*:4*//*11:*/
#line 417 "./sat13.w"

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
int unaries;
int binaries;
ullng cells;

/*:11*//*27:*/
#line 770 "./sat13.w"

cel*mem;
uint memsize;
uint min_learned;
uint first_learned;
uint max_learned;
int max_lit;
uint*bmem;
literal*lmem;
variable*vmem;
uint*heap;
int hn;
uint*trail;
int eptr;
int ebptr;
int lptr;
int lbptr;
char*history;
int llevel;
int*leveldat;

/*:27*//*41:*/
#line 1162 "./sat13.w"

ullng depth_per_decision;
ullng trail_per_decision;
ullng mems_per_confl,lits_per_confl,lits_per_nontriv;
ullng res_per_confl,glucose_per_confl;
ullng props_per_confl= two_to_the_32;
uint short_per_confl;
uint agility;
ullng mems_at_prev_confl;
uint props;

/*:41*//*59:*/
#line 1544 "./sat13.w"

uint lt;
uint lat;
uint wa,next_wa;

/*:59*//*80:*/
#line 1951 "./sat13.w"

double var_bump= 1.0;
float clause_bump= 1.0;
double var_bump_factor;
float clause_bump_factor;

/*:80*//*90:*/
#line 2117 "./sat13.w"

uint curstamp;
uint*learn;
int oldptr;
int jumplev;
int tl;
int xnew;
int clevels;
uint resols;
uint learned_size;
int prelearned_size;
int trivial_learning;

/*:90*//*97:*/
#line 2228 "./sat13.w"

int*stack;
int stackptr;
int*conflictdat;
int conflict_level;
uint*levstamp;

/*:97*//*111:*/
#line 2662 "./sat13.w"

int*rangedist;
int asserts;
int minrange;
int maxrange;
int recycle_point;
int budget;
ullng*clause_heap;
int clause_heap_size;

/*:111*//*140:*/
#line 3229 "./sat13.w"

int full_run;
int conflict_seen;
int decisionvar;
int prev_learned;
int warmup_cycles;
int next_learned;
int restart_u,restart_v;
ullng restart_thresh;
int trail_marker;
int minjumplev;

/*:140*/
#line 101 "./sat13.w"
;
/*139:*/
#line 3219 "./sat13.w"

void confusion(char*id){
fprintf(stderr,"This can't happen ("O"s)!\n",id);
exit(-666);
}

void debugstop(int foo){
fprintf(stderr,"You rang("O"d)?\n",foo);
}

/*:139*/
#line 102 "./sat13.w"
;
/*31:*/
#line 903 "./sat13.w"

void print_bimp(int l){
register uint la;
printf(""O"s"O".8s("O"d) ->",litname(l),l);
if(lmem[l].bimp_end){
for(la= lmem[l].bimp_start;la<lmem[l].bimp_end;la++)
printf(" "O"s"O".8s("O"d)",litname(bmem[la]),bmem[la]);
}
printf("\n");
}

/*:31*//*32:*/
#line 917 "./sat13.w"

void print_watches_for(int l){
register uint c;
printf(""O"s"O".8s("O"d) watched in",litname(l),l);
for(c= lmem[l].watch;c;){
printf(" "O"u",c);
if(mem[c].lit==l)c= link0(c);
else c= link1(c);
}
printf("\n");
}

/*:32*//*33:*/
#line 931 "./sat13.w"

void print_clause(uint c){
register int k,l;
printf(""O"u:",c);
if(c<clause_extra||c>=max_learned){
printf(" clause "O"d doesn't exist!\n",c);
return;
}
for(k= 0;k<size(c);k++){
l= mem[c+k].lit;
if(l<2||l> max_lit){
printf(" BAD!\n");
return;
}
printf(" "O"s"O".8s("O"u)",litname(l),l);
}
while(mem[c+k].lit&sign_bit){
l= mem[c+k].lit^sign_bit;
if(l<2||l> max_lit){
printf(" !BAD!\n");
return;
}
printf(" !"O"s"O".8s("O"u)",litname(l),l);
k++;
}
printf("\n");
}

/*:33*//*34:*/
#line 964 "./sat13.w"

void sanity(int eptr){
register uint k,l,c,endc,u,v,clauses,watches,vals,llevel;
/*35:*/
#line 974 "./sat13.w"

for(clauses= k= 0,c= clause_extra;c<min_learned;k= c,c= endc+clause_extra){
endc= c+size(c);
clauses++;
if(link0(c)>=max_learned){
fprintf(stderr,"bad link0("O"u)!\n",c);
return;
}
if(link1(c)>=max_learned){
fprintf(stderr,"bad link1("O"u)!\n",c);
return;
}
if(size(c)<2)
fprintf(stderr,"size("O"u)="O"d!\n",c,size(c));
for(k= 0;k<size(c);k++)
if(mem[c+k].lit<2||mem[c+k].lit> max_lit)
fprintf(stderr,"bad lit "O"d of "O"d!\n",k,c);
while(mem[c+k].lit&sign_bit){
if(mem[c+k].lit<2+sign_bit||mem[c+k].lit> max_lit+sign_bit)
fprintf(stderr,"bad deleted lit "O"d of "O"d!\n",k,c);
k++,endc++;
}
}
if(c!=min_learned)
fprintf(stderr,"bad last unlearned clause ("O"d)!\n",k);
else{
for(k= 0,c= first_learned;c<max_learned;k= c,c= endc+learned_extra){
endc= c+size(c);
clauses++;
if(link0(c)>=max_learned){
fprintf(stderr,"bad link0("O"u)!\n",c);
return;
}
if(link1(c)>=max_learned){
fprintf(stderr,"bad link1("O"u)!\n",c);
return;
}
if(size(c)<2)
fprintf(stderr,"size("O"u)="O"d!\n",c,size(c));
for(k= 0;k<size(c);k++)
if(mem[c+k].lit<2||mem[c+k].lit> max_lit)
fprintf(stderr,"bad lit "O"d of "O"d!\n",k,c);
while(mem[c+k].lit&sign_bit){
if(mem[c+k].lit<2+sign_bit||mem[c+k].lit> max_lit+sign_bit)
fprintf(stderr,"bad deleted lit "O"d of "O"d!\n",k,c);
k++,endc++;
}
}
if(c!=max_learned)
fprintf(stderr,"bad last learned clause ("O"d)!\n",k);
if(mem[c-learned_extra].lit)
fprintf(stderr,"missing zero at end of mem!\n");
}

/*:35*/
#line 967 "./sat13.w"
;
/*36:*/
#line 1031 "./sat13.w"

for(watches= 0,l= 2;l<=max_lit;l++){
for(c= lmem[l].watch;c;){
watches++;
if(c<clause_extra||c>=max_learned){
fprintf(stderr,"clause "O"u in watch list "O"u out of range!\n",c,l);
return;
}
if(mem[c].lit==l)c= link0(c);
else if(mem[c+1].lit==l)c= link1(c);
else{
fprintf(stderr,"clause "O"u improperly on watch list "O"u!\n",c,l);
return;
}
}
}
if(watches!=clauses+clauses)
fprintf(stderr,""O"u clauses but "O"u watches!\n",clauses,watches);

/*:36*/
#line 968 "./sat13.w"
;
/*72:*/
#line 1775 "./sat13.w"

for(k= 1;k<=vars;k++){
if(vmem[k].hloc>=hn)
fprintf(stderr,"hloc of "O".8s exceeds "O"d!\n",vmem[k].name.ch8,hn-1);
else if(vmem[k].hloc>=0&&heap[vmem[k].hloc]!=k)
fprintf(stderr,"hloc of "O".8s errs!\n",vmem[k].name.ch8);
}
for(k= 0;k<hn;k++){
v= heap[k];
if(v<=0||v> vars)
fprintf(stderr,"heap["O"d]="O"d!\n",k,v);
else if(k){
u= heap[(k-1)>>1];
if(u> 0&&u<=vars&&vmem[u].activity<vmem[v].activity)
fprintf(stderr,"heap["O"d]act<heap["O"d]act!\n",(k-1)>>1,k);
}
}

/*:72*/
#line 969 "./sat13.w"
;
/*37:*/
#line 1050 "./sat13.w"

for(k= llevel= 0;k<eptr;k++){
l= trail[k];
if(l<2||l> max_lit){
fprintf(stderr,"bad lit "O"u in trail["O"u]!\n",l,k);
return;
}
if(vmem[thevar(l)].tloc!=k)
fprintf(stderr,""O"s"O".8s has bad tloc ("O"d not "O"d)!\n",
litname(l),vmem[thevar(l)].tloc,k);
if(k==leveldat[llevel+2]){
llevel+= 2;
if(lmem[l].reason)
fprintf(stderr,""O"s"O".8s("O"u), level "O"u, shouldn't have reason!\n",
litname(l),l,llevel>>1);
}else{
if(llevel&&!lmem[l].reason)
fprintf(stderr,""O"s"O".8s("O"u), level "O"u, should have reason!\n",
litname(l),l,llevel>>1);
}
if(lmem[bar(l)].reason)
fprintf(stderr,""O"s"O".8s("O"u), level "O"u, comp has reason!\n",
litname(l),l,llevel>>1);
if(vmem[thevar(l)].value!=llevel+(l&1))
fprintf(stderr,""O"s"O".8s("O"u), level "O"u, has bad value!\n",
litname(l),l,llevel>>1);
if(llevel){
if(lmem[l].reason<=0){
if(lmem[l].reason==-1||lmem[l].reason<-max_lit)
fprintf(stderr,
""O"s"O".8s("O"u), level "O"u, has wrong binary reason ("O"u)!\n",
litname(l),l,llevel>>1,c);
}else{
c= lmem[l].reason;
if(mem[c].lit!=l)
fprintf(stderr,""O"s"O".8s("O"u), level "O"u, has wrong reason ("O"u)!\n",
litname(l),l,llevel>>1,c);
u= bar(mem[c+1].lit);
if(vmem[thevar(u)].value!=llevel+(u&1))
fprintf(stderr,""O"s"O".8s("O"u), level "O"u, has bad reason ("O"u)!\n",
litname(l),l,llevel>>1,c);
}
}
}

/*:37*/
#line 970 "./sat13.w"
;
/*38:*/
#line 1095 "./sat13.w"

for(vals= 0,v= 1;v<=vars;v++){
if(vmem[v].value> llevel+1){
if(vmem[v].value!=unset)
fprintf(stderr,"strange val "O".8s (level "O"u)!\n",
vmem[v].name.ch8,vmem[v].value>>1);
else if(vmem[v].hloc<0)
fprintf(stderr,""O".8s should be in the heap!\n",vmem[v].name.ch8);
}else vals++;
}
if(vals!=eptr)
fprintf(stderr,"I found "O"u values, but eptr="O"u!\n",vals,eptr);

/*:38*/
#line 971 "./sat13.w"
;
}

/*:34*//*39:*/
#line 1123 "./sat13.w"

void print_stats(void){
register double mpc= mems_per_confl,ppc= props_per_confl;
fprintf(stderr,"z="O"d d="O".1f t="O".1f m="O".1f p="O".1f m/p="O".1f",
leveldat[2],
(double)depth_per_decision/two_to_the_32,
(double)trail_per_decision/two_to_the_32,
mpc/two_to_the_32,ppc/two_to_the_32,mpc/ppc);
fprintf(stderr," r="O".1f L="O".1f l="O".1f g="O".1f s="O".2f a="O".2f\n",
(double)res_per_confl/two_to_the_32,
(double)lits_per_confl/two_to_the_32,
(double)lits_per_nontriv/two_to_the_32,
(double)glucose_per_confl/two_to_the_32,
(double)short_per_confl/two_to_the_32,
(double)agility/two_to_the_32);
}

/*:39*//*42:*/
#line 1202 "./sat13.w"

void print_state(int eptr){
register uint j,k;
fprintf(stderr," after "O"lld mems:",mems);
if(print_state_cutoff){
for(k= 0;k<eptr;k++){
if(k==trail_marker)fprintf(stderr,"|");
fprintf(stderr,""O"d",history[k]+(trail[k]&1));
if(k>=print_state_cutoff){
fprintf(stderr,"...");break;
}
}
fprintf(stderr,"\n");
}
fprintf(stderr," ");
print_stats();
fflush(stderr);
}

/*:42*//*43:*/
#line 1223 "./sat13.w"

void print_trail(int eptr){
register int k,l;
for(k= 0;k<eptr;k++){
l= trail[k];
if(k>=vars||l<2||l> max_lit)return;
fprintf(stderr,""O"d: "O"d "O"d "O"s"O".8s("O"d)",
k,history[k]+(l&1),vmem[thevar(l)].value>>1,litname(l),l);
if(lmem[l].reason> 0){
if((vmem[thevar(l)].value>>1)||lmem[l].reason<min_learned)
fprintf(stderr," #"O"u\n",lmem[l].reason);
else fprintf(stderr," learned\n");
}else if(lmem[l].reason<0)
fprintf(stderr," <- "O"s"O".8s\n",litname(-lmem[l].reason));
else fprintf(stderr,"\n");
}
}

/*:43*//*44:*/
#line 1245 "./sat13.w"

void print_unsat(void){
register int c,endc,k,l;
for(c= clause_extra;c<min_learned;c= endc+clause_extra){
endc= c+size(c);
for(k= endc-1;k>=c;k--){
l= mem[k].lit;
if(isknown(l)&&!iscontrary(l))break;
}
if(k<c){
fprintf(stderr,""O"d:",c);
for(k= 0;k<size(c);k++){
l= mem[c+k].lit;
if(!isknown(l))fprintf(stderr," "O"s"O".8s",litname(l));
}
fprintf(stderr," |");
for(k= 0;k<size(c);k++){
l= mem[c+k].lit;
if(isknown(l))fprintf(stderr," "O"s"O".8s",litname(l));
}
fprintf(stderr,"\n");
}
while(mem[endc].lit&sign_bit)endc++;
}
}

/*:44*//*71:*/
#line 1766 "./sat13.w"

void print_heap(void){
register int k;
for(k= 0;k<hn;k++){
fprintf(stderr,""O"d: "O".8s "O"e\n",
k,vmem[heap[k]].name.ch8,vmem[heap[k]].activity);
}
}

/*:71*/
#line 103 "./sat13.w"
;
main(int argc,char*argv[]){
register int h,hp,i,j,jj,k,kk,l,ll,lll,p,q,r,s;
register int c,cc,endc,la,t,u,v,w,x,y;
register double au,av;
/*3:*/
#line 211 "./sat13.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
/*5:*/
#line 270 "./sat13.w"

case'v':k|= (sscanf(argv[j]+1,""O"d",&verbose)-1);break;
case'c':k|= (sscanf(argv[j]+1,""O"d",&show_choices_max)-1);break;
case'H':k|= (sscanf(argv[j]+1,""O"d",&print_state_cutoff)-1);break;
case'h':k|= (sscanf(argv[j]+1,""O"d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,""O"d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1);break;
case'd':k|= (sscanf(argv[j]+1,""O"lld",&delta)-1);thresh= delta;break;
case'D':k|= (sscanf(argv[j]+1,""O"lld",&doomsday)-1);break;
case'm':k|= (sscanf(argv[j]+1,""O"d",&memk_max)-1);break;
case't':k|= (sscanf(argv[j]+1,""O"d",&trivial_limit)-1);break;
case'w':k|= (sscanf(argv[j]+1,""O"d",&warmups)-1);break;
case'j':k|= (sscanf(argv[j]+1,""O"lld",&recycle_bump)-1);break;
case'J':k|= (sscanf(argv[j]+1,""O"lld",&recycle_inc)-1);break;
case'K':k|= (sscanf(argv[j]+1,""O"d",&learn_save)-1);break;
case'f':k|= (sscanf(argv[j]+1,""O"f",&restart_psi_fraction)-1);break;
case'a':k|= (sscanf(argv[j]+1,""O"f",&alpha)-1);break;
case'r':k|= (sscanf(argv[j]+1,""O"f",&var_rho)-1);break;
case'R':k|= (sscanf(argv[j]+1,""O"f",&clause_rho)-1);break;
case'p':k|= (sscanf(argv[j]+1,""O"f",&rand_prob)-1);break;
case'P':k|= (sscanf(argv[j]+1,""O"f",&true_prob)-1);break;
case'x':out_name= argv[j]+1,out_file= fopen(out_name,"w");
if(!out_file)
fprintf(stderr,"Sorry, I can't open file `"O"s' for writing!\n",out_name);
break;
case'l':learned_name= argv[j]+1,learned_file= fopen(learned_name,"w");
if(!learned_file)
fprintf(stderr,"Sorry, I can't open file `"O"s' for writing!\n",
learned_name);
break;
case'L':restart_name= argv[j]+1,restart_file= fopen(restart_name,"w");
if(!restart_file)
fprintf(stderr,"Sorry, I can't open file `"O"s' for writing!\n",
restart_name);
break;
case'z':polarity_in_name= argv[j]+1,
polarity_infile= fopen(polarity_in_name,"r");
if(!polarity_infile)
fprintf(stderr,"Sorry, I can't open file `"O"s' for reading!\n",
polarity_in_name);
break;
case'Z':polarity_out_name= argv[j]+1,
polarity_outfile= fopen(polarity_out_name,"w");
if(!polarity_outfile)
fprintf(stderr,"Sorry, I can't open file `"O"s' for writing!\n",
polarity_out_name);
break;
case'T':k|= (sscanf(argv[j]+1,""O"lld",&timeout)-1);break;

/*:5*/
#line 213 "./sat13.w"
;
default:k= 1;
}
/*6:*/
#line 319 "./sat13.w"

if(k||hbits<0||hbits> 30||buf_size<=0||memk_max<2||memk_max> 31||
trivial_limit<=0||(int)recycle_inc<0||alpha<0.0||alpha> 1.0||
rand_prob<0.0||true_prob<0.0||var_rho<=0.0||clause_rho<=0.0){
fprintf(stderr,
"Usage: "O"s [v<n>] [c<n>] [H<n>] [h<n>] [b<n>] [s<n>] [d<n>]",argv[0]);
fprintf(stderr," [D<n>] [m<n>] [t<n>] [w<n>] [j<n>] [J<n>] [K<n>]");
fprintf(stderr," [f<f>] [a<f>] [r<f>] [R<f>] [p<f>] [P<f>]");
fprintf(stderr," [x<foo>] [l<bar>] [L<baz>] [z<poi>] [Z<poo>] [T<n>] < foo.sat\n");
exit(-1);
}

/*:6*/
#line 216 "./sat13.w"
;

/*:3*/
#line 108 "./sat13.w"
;
/*12:*/
#line 434 "./sat13.w"

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

/*:12*//*18:*/
#line 574 "./sat13.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:18*/
#line 109 "./sat13.w"
;
/*13:*/
#line 463 "./sat13.w"

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
/*14:*/
#line 500 "./sat13.w"

for(j= k= 0;;){
while(buf[j]==' ')j++;
if(buf[j]=='\n')break;
if(buf[j]<' '||buf[j]> '~'){
fprintf(stderr,"Illegal character (code #"O"x) in the clause on line "O"lld!\n",
buf[j],clauses);
exit(-5);
}
if(buf[j]=='~')i= 1,j++;
else i= 0;
/*15:*/
#line 527 "./sat13.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*16:*/
#line 545 "./sat13.w"

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

/*:16*/
#line 530 "./sat13.w"
;
/*19:*/
#line 578 "./sat13.w"

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
if(l==0)goto empty_clause;
j+= l;
h&= (1<<hbits)-1;

/*:19*/
#line 532 "./sat13.w"
;
/*20:*/
#line 594 "./sat13.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:20*/
#line 533 "./sat13.w"
;
if(p->stamp==clauses||p->stamp==-clauses)/*21:*/
#line 608 "./sat13.w"

{
if((p->stamp> 0)==(i> 0))goto empty_clause;
}

/*:21*/
#line 534 "./sat13.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*17:*/
#line 558 "./sat13.w"

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

/*:17*/
#line 537 "./sat13.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:15*/
#line 511 "./sat13.w"
;
}
if(k==0){
fprintf(stderr,"(Empty line "O"lld is being ignored)\n",clauses);
nullclauses++;
}
goto clause_done;
empty_clause:/*22:*/
#line 618 "./sat13.w"

while(k){
/*23:*/
#line 627 "./sat13.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:23*/
#line 620 "./sat13.w"
;
k--;
}
if((buf[0]!='~')||(buf[1]!=' '))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;

/*:22*/
#line 518 "./sat13.w"
;
clause_done:cells+= k;
if(k==1)unaries++;
else if(k==2)binaries++;

/*:14*/
#line 474 "./sat13.w"
;
}
if((vars>>hbits)>=10){
fprintf(stderr,"There are "O"lld variables but only "O"d hash tables;\n",
vars,1<<hbits);
for(h= hbits+1;(vars>>h)>=10;h++);
fprintf(stderr," maybe you should use command-line option h"O"d?\n",h);
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

/*:13*/
#line 110 "./sat13.w"
;
if(verbose&show_basics)
/*25:*/
#line 645 "./sat13.w"

fprintf(stderr,
"("O"lld variables, "O"lld clauses, "O"llu literals successfully read)\n",
vars,clauses,cells);

/*:25*/
#line 112 "./sat13.w"
;
/*45:*/
#line 1276 "./sat13.w"

/*46:*/
#line 1287 "./sat13.w"

vmem= (variable*)malloc((vars+1)*sizeof(variable));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-12);
}
bytes+= (vars+1)*sizeof(variable);
for(k= 1;k<=vars;k++)o,vmem[k].value= unset,vmem[k].tloc= -1;
heap= (uint*)malloc(vars*sizeof(uint));
if(!heap){
fprintf(stderr,"Oops, I can't allocate the heap array!\n");
exit(-11);
}
bytes+= vars*sizeof(uint);

/*:46*/
#line 1277 "./sat13.w"
;
if(polarity_infile)/*79:*/
#line 1919 "./sat13.w"

{
if(true_prob>=1.0)true_prob_thresh= 0x80000000;
else true_prob_thresh= (int)(true_prob*2147483648.0);
for(q= 0;;){
register tmp_var*p;
if(fscanf(polarity_infile,""O"s",buf)!=1)break;
if(buf[0]=='~')i= j= 1;
else i= j= 0;
/*19:*/
#line 578 "./sat13.w"

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
if(l==0)goto empty_clause;
j+= l;
h&= (1<<hbits)-1;

/*:19*/
#line 1928 "./sat13.w"
;
for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(p){
v= p->serial+1;
o,vmem[v].oldval= i,vmem[v].hloc= q;
o,heap[q]= v;
o,vmem[v].activity= (vars-q)/(double)vars;
o,vmem[v].tloc= 0;
q++;
}
}
for(v= 0;q<vars;q++){
while(o,vmem[++v].tloc==0);
vmem[v].hloc= q;
if(true_prob_thresh&&(mems+= 4,gb_next_rand()<true_prob_thresh))
vmem[v].oldval= 0;
else vmem[v].oldval= 1;
o,heap[q]= v;
}
hn= vars;
}

/*:79*/
#line 1278 "./sat13.w"

else/*78:*/
#line 1890 "./sat13.w"

{
if(true_prob>=1.0)true_prob_thresh= 0x80000000;
else true_prob_thresh= (int)(true_prob*2147483648.0);
for(k= 1;k<=vars;k++)o,heap[k-1]= k;
for(hn= vars;hn> 1;){
/*76:*/
#line 1858 "./sat13.w"

{
register unsigned long t= two_to_the_31-(two_to_the_31 mod hn);
register long r;
do{
mems+= 4,r= gb_next_rand();
}while(t<=(unsigned long)r);
h= r mod hn;
}

/*:76*/
#line 1896 "./sat13.w"
;
hn--;
if(h!=hn){
o,k= heap[h];
ooo,heap[h]= heap[hn],heap[hn]= k;
}
}
for(h= 0;h<vars;h++){
o,v= heap[h];
o,vmem[v].hloc= h;
if(true_prob_thresh&&(mems+= 4,gb_next_rand()<true_prob_thresh))
vmem[v].oldval= 0;
else vmem[v].oldval= 1;
o,vmem[v].activity= 0.0;
}
hn= vars;
}

/*:78*/
#line 1279 "./sat13.w"
;
/*47:*/
#line 1302 "./sat13.w"

free(buf);free(hash);
/*48:*/
#line 1341 "./sat13.w"

{
ullng proto_memsize= (clauses-unaries-binaries)*clause_extra
+(cells-unaries-2*binaries)+clause_extra;
min_learned= proto_memsize;
proto_memsize+= 2*binaries+learned_supplement;
if(proto_memsize>=0x80000000){
fprintf(stderr,"Sorry, I can't handle "O"llu cells (2^31 is my limit)!\n",
proto_memsize);
exit(-665);
}
max_cells_used= proto_memsize-learned_supplement+2;
first_learned= max_learned= min_learned+learned_supplement;
memsize= 1<<memk_max;
if(max_cells_used> memsize){
fprintf(stderr,
"Immediate memory overflow (memsize="O"u<"O"u), please increase m!\n",
memsize,max_cells_used);
exit(-666);
}
if(verbose&show_details)
fprintf(stderr,"(learned clauses begin at "O"u)\n",first_learned);
}

/*:48*/
#line 1304 "./sat13.w"
;
mem= (cel*)malloc(memsize*sizeof(cel));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the big mem array!\n");
exit(-10);
}
bytes+= max_cells_used*sizeof(cel);
max_lit= vars+vars+1;
lmem= (literal*)malloc((max_lit+1)*sizeof(literal));
if(!lmem){
fprintf(stderr,"Oops, I can't allocate the lmem array!\n");
exit(-13);
}
bytes+= (max_lit+1)*sizeof(literal);
trail= (uint*)malloc(vars*sizeof(uint));
if(!trail){
fprintf(stderr,"Oops, I can't allocate the trail array!\n");
exit(-14);
}
bytes+= vars*sizeof(uint);

/*:47*//*56:*/
#line 1473 "./sat13.w"

bmem= (uint*)malloc(binaries*2*sizeof(uint));
if(!bmem){
fprintf(stderr,"Oops, I can't allocate the bmem array!\n");
exit(-16);
}
bytes+= binaries*2*sizeof(uint);
history= (char*)malloc(vars*sizeof(char));
if(!history){
fprintf(stderr,"Oops, I can't allocate the history array!\n");
exit(-15);
}
bytes+= vars*sizeof(char);

/*:56*/
#line 1280 "./sat13.w"
;
/*49:*/
#line 1369 "./sat13.w"

eptr= 0;
for(l= 2;l<=max_lit;l++)
oo,lmem[l].reason= lmem[l].watch= lmem[l].bimp_end= 0;
for(c= clause_extra,j= clauses,jj= min_learned+2;j;j--){
k= 0;
/*50:*/
#line 1406 "./sat13.w"

for(i= 0;i<2;){
/*23:*/
#line 627 "./sat13.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:23*/
#line 1408 "./sat13.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1)+2;
o,mem[c+k++].lit= p;
}

/*:50*/
#line 1375 "./sat13.w"
;
if(k<=2)/*51:*/
#line 1415 "./sat13.w"

{
if(k<2)/*52:*/
#line 1429 "./sat13.w"

{
l= mem[c].lit,v= thevar(l);
if(o,vmem[v].value==unset){
o,vmem[v].value= l&1,vmem[v].tloc= eptr;
o,history[eptr]= 6,trail[eptr++]= l;
}else if(vmem[v].value!=(l&1))goto unsat;
}

/*:52*/
#line 1417 "./sat13.w"

else{
l= mem[c].lit,ll= mem[c+1].lit;
oo,lmem[bar(l)].bimp_end++;
oo,lmem[bar(ll)].bimp_end++;
o,mem[jj].lit= l,mem[jj+1].lit= ll,jj+= 2;
}
}

/*:51*/
#line 1376 "./sat13.w"

else{
o,size(c)= k;
l= mem[c].lit;
ooo,link0(c)= lmem[l].watch,lmem[l].watch= c;
l= mem[c+1].lit;
ooo,link1(c)= lmem[l].watch,lmem[l].watch= c;
c+= k+clause_extra;
}
}
o,mem[c-clause_extra].lit= 0;
if(c!=min_learned){
fprintf(stderr,
"Oh oh, I didn't load the correct number of cells ("O"u:"O"u)!\n",
c,min_learned);
exit(-17);
}
if(jj!=max_cells_used){
fprintf(stderr,
"Oh oh, I miscounted binaries somehow ("O"u:"O"u)!\n",jj,max_cells_used);
exit(-18);
}
/*53:*/
#line 1438 "./sat13.w"

for(l= 2,jj= 0;l<=max_lit;l++){
o,k= lmem[l].bimp_end;
if(k)
o,lmem[l].bimp_start= lmem[l].bimp_end= jj,jj+= k;
}
for(jj= min_learned+2,j= binaries;j;j--){
o,l= mem[jj].lit,ll= mem[jj+1].lit,jj+= 2;
ooo,k= lmem[bar(l)].bimp_end,bmem[k]= ll,lmem[bar(l)].bimp_end= k+1;
ooo,k= lmem[bar(ll)].bimp_end,bmem[k]= l,lmem[bar(ll)].bimp_end= k+1;
}

/*:53*/
#line 1398 "./sat13.w"
;

/*:49*/
#line 1282 "./sat13.w"
;
/*54:*/
#line 1450 "./sat13.w"

for(c= vars;c;c--){
/*24:*/
#line 636 "./sat13.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:24*/
#line 1452 "./sat13.w"
;
o,vmem[c].name.lng= cur_tmp_var->name.lng;
o,vmem[c].stamp= 0;
}

/*:54*/
#line 1283 "./sat13.w"
;
/*55:*/
#line 1460 "./sat13.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL){
fprintf(stderr,"This can't happen (consistency check failure)!\n");
exit(-14);
}
free(cur_chunk);free(cur_vchunk);

/*:55*/
#line 1284 "./sat13.w"
;
/*57:*/
#line 1490 "./sat13.w"

leveldat= (int*)malloc(vars*2*sizeof(int));
if(!leveldat){
fprintf(stderr,"Oops, I can't allocate the leveldat array!\n");
exit(-16);
}
bytes+= vars*2*sizeof(int);

/*:57*//*89:*/
#line 2109 "./sat13.w"

learn= (uint*)malloc(vars*sizeof(uint));
if(!learn){
fprintf(stderr,"Oops, I can't allocate the learn array!\n");
exit(-16);
}
bytes+= vars*sizeof(uint);

/*:89*//*96:*/
#line 2207 "./sat13.w"

stack= (int*)malloc(vars*2*sizeof(int));
if(!stack){
fprintf(stderr,"Oops, I can't allocate the stack array!\n");
exit(-16);
}
bytes+= vars*2*sizeof(int);
conflictdat= (int*)malloc(vars*2*sizeof(int));
if(!conflictdat){
fprintf(stderr,"Oops, I can't allocate the conflictdat array!\n");
exit(-16);
}
bytes+= vars*2*sizeof(int);
levstamp= (uint*)malloc(2*vars*sizeof(uint));
if(!levstamp){
fprintf(stderr,"Oops, I can't allocate the levstamp array!\n");
exit(-16);
}
bytes+= 2*vars*sizeof(uint);
for(k= 0;k<vars;k++)o,levstamp[k+k]= 0;

/*:96*//*109:*/
#line 2618 "./sat13.w"

rangedist= (int*)malloc(buckets*sizeof(int));
if(!rangedist){
fprintf(stderr,"Oops, I can't allocate the rangedist array!\n");
exit(-16);
}
bytes+= buckets*sizeof(int);
for(k= 0;k+k<buckets;k++)o,rangedist[k+k]= rangedist[k+k+1]= 0;

/*:109*//*116:*/
#line 2758 "./sat13.w"

clause_heap_size= recycle_bump>>1;
clause_heap= (ullng*)malloc(clause_heap_size*sizeof(ullng));
if(!clause_heap){
fprintf(stderr,"Oops, I can't allocate the clause_heap array!\n");
exit(-16);
}
bytes+= clause_heap_size*sizeof(ullng);

/*:116*/
#line 1285 "./sat13.w"
;

/*:45*/
#line 113 "./sat13.w"
;
imems= mems,mems= 0;
/*124:*/
#line 2873 "./sat13.w"

/*130:*/
#line 3009 "./sat13.w"

if(rand_prob>=1.0)rand_prob_thresh= 0x80000000;
else rand_prob_thresh= (int)(rand_prob*2147483648.0);
var_bump_factor= 1.0/(double)var_rho;
clause_bump_factor= 1.0/clause_rho;
show_choices_max<<= 1;
next_recycle= recycle_bump;
if(next_recycle> doomsday)next_recycle= doomsday;
restart_psi= two_to_the_32*(double)restart_psi_fraction;
restart_u= restart_v= next_restart= 1;
if(verbose&show_details){
for(k= 0;k<eptr;k++)
fprintf(stderr,""O"s"O".8s is given\n",litname(trail[k]));
}
for(k= 0;k<vars;k++)o,leveldat[k+k]= -1,leveldat[k+k+1]= 0;

/*:130*/
#line 2874 "./sat13.w"
;
square_one:llevel= warmup_cycles= 0;
if(sanity_checking)sanity(eptr);
if(verbose&show_initial_clauses)print_unsat();
lptr= 0;
startup:conflict_level= 0;
full_run= (warmup_cycles<warmups?1:0);
proceed:conflict_seen= 0;
/*127:*/
#line 2966 "./sat13.w"

ebptr= eptr;
while(lptr<eptr){
o,lt= trail[lptr++];
if(lptr<=ebptr){
o,lat= lmem[lt].bimp_end;
if(lat){
l= lt;
/*58:*/
#line 1512 "./sat13.w"

for(lbptr= eptr;;){
for(la= lmem[l].bimp_start;la<lat;la++){
o,ll= bmem[la];
if(o,isknown(ll)){
if(iscontrary(ll)){
props++;
/*66:*/
#line 1671 "./sat13.w"

{
if(verbose&show_details)
fprintf(stderr," "O"s"O".8s -> "O"s"O".8s #\n",
litname(l),litname(ll));
if(full_run&&llevel)/*68:*/
#line 1707 "./sat13.w"

{
if(!conflict_seen){
conflict_seen= 1;
o,leveldat[llevel+1]= -l;
o,conflictdat[llevel+1]= ll;
conflictdat[llevel]= conflict_level,conflict_level= llevel;
}
}

/*:68*/
#line 1676 "./sat13.w"

else{
c= -l;
goto confl;
}
}

/*:66*/
#line 1519 "./sat13.w"
;
}
}else{
props++;
if(verbose&show_details)
fprintf(stderr," "O"s"O".8s -> "O"s"O".8s\n",
litname(l),litname(ll));
o,history[eptr]= 4,trail[eptr]= ll;
o,lmem[ll].reason= -l;
o,vmem[thevar(ll)].value= llevel+(ll&1),vmem[thevar(ll)].tloc= eptr++;
agility-= agility>>13;
if(o,(vmem[thevar(ll)].oldval+ll)&1)agility+= 1<<19;
}
}
while(1){
if(lbptr==eptr){
l= 0;break;
}
o,l= trail[lbptr++];
o,lat= lmem[l].bimp_end;
if(lat)break;
}
if(l==0)break;
}

/*:58*/
#line 2974 "./sat13.w"
;
}
}
/*60:*/
#line 1566 "./sat13.w"

o,wa= lmem[bar(lt)].watch;
if(wa){
for(q= 0;wa;wa= next_wa){
o,ll= mem[wa].lit;
if(ll==bar(lt)){
o,ll= mem[wa+1].lit;
oo,mem[wa].lit= ll,mem[wa+1].lit= bar(lt);
o,next_wa= link0(wa);
o,link0(wa)= link1(wa),link1(wa)= next_wa;
}else o,next_wa= link1(wa);
/*63:*/
#line 1624 "./sat13.w"

if((o,isknown(ll))&&!iscontrary(ll)){
/*64:*/
#line 1635 "./sat13.w"

if(q==0)o,lmem[bar(lt)].watch= wa;
else o,link1(q)= wa;
q= wa;

/*:64*/
#line 1626 "./sat13.w"
;
continue;
}

/*:63*/
#line 1578 "./sat13.w"
;
for(o,s= size(wa),j= wa+s-1;j> wa+1;j--){
o,l= mem[j].lit;
if(o,!isknown(l)||!iscontrary(l))break;
if(vmem[thevar(l)].value<2&&llevel)
/*61:*/
#line 1598 "./sat13.w"

{
o,size(wa)= --s;
if(j!=wa+s)oo,mem[j].lit= mem[wa+s].lit;
o,mem[wa+s].lit= l+sign_bit;
}

/*:61*/
#line 1583 "./sat13.w"
;
}
if(j> wa+1)/*62:*/
#line 1605 "./sat13.w"

{
if(verbose&show_watches)
fprintf(stderr," "O"s"O".8s watched in "O"d\n",litname(l),wa);
oo,mem[wa+1].lit= l,mem[j].lit= bar(lt);
o,link1(wa)= lmem[l].watch;
o,lmem[l].watch= wa;
continue;
}

/*:62*/
#line 1585 "./sat13.w"
;
/*64:*/
#line 1635 "./sat13.w"

if(q==0)o,lmem[bar(lt)].watch= wa;
else o,link1(q)= wa;
q= wa;

/*:64*/
#line 1586 "./sat13.w"
;
/*65:*/
#line 1647 "./sat13.w"

props++;
if(isknown(ll))/*67:*/
#line 1683 "./sat13.w"

{
if(verbose&show_details)
fprintf(stderr," "O"s"O".8s from "O"d #\n",
litname(ll),wa);
if(full_run&&llevel)/*69:*/
#line 1717 "./sat13.w"

{
if(!conflict_seen){
conflict_seen= 1;
o,leveldat[llevel+1]= wa;
o,conflictdat[llevel]= conflict_level,conflict_level= llevel;
}
}

/*:69*/
#line 1688 "./sat13.w"

else{
c= wa;
goto confl;
}
}

/*:67*/
#line 1649 "./sat13.w"

else{
if(verbose&show_details)
fprintf(stderr," "O"s"O".8s from "O"d\n",litname(ll),wa);
o,history[eptr]= 4,trail[eptr]= ll;
o,vmem[thevar(ll)].tloc= eptr++;
vmem[thevar(ll)].value= llevel+(ll&1);
agility-= agility>>13;
if(o,(vmem[thevar(ll)].oldval+ll)&1)agility+= 1<<19;
o,lmem[ll].reason= wa;
o,lat= lmem[ll].bimp_end;
if(lat){
l= ll;
/*58:*/
#line 1512 "./sat13.w"

for(lbptr= eptr;;){
for(la= lmem[l].bimp_start;la<lat;la++){
o,ll= bmem[la];
if(o,isknown(ll)){
if(iscontrary(ll)){
props++;
/*66:*/
#line 1671 "./sat13.w"

{
if(verbose&show_details)
fprintf(stderr," "O"s"O".8s -> "O"s"O".8s #\n",
litname(l),litname(ll));
if(full_run&&llevel)/*68:*/
#line 1707 "./sat13.w"

{
if(!conflict_seen){
conflict_seen= 1;
o,leveldat[llevel+1]= -l;
o,conflictdat[llevel+1]= ll;
conflictdat[llevel]= conflict_level,conflict_level= llevel;
}
}

/*:68*/
#line 1676 "./sat13.w"

else{
c= -l;
goto confl;
}
}

/*:66*/
#line 1519 "./sat13.w"
;
}
}else{
props++;
if(verbose&show_details)
fprintf(stderr," "O"s"O".8s -> "O"s"O".8s\n",
litname(l),litname(ll));
o,history[eptr]= 4,trail[eptr]= ll;
o,lmem[ll].reason= -l;
o,vmem[thevar(ll)].value= llevel+(ll&1),vmem[thevar(ll)].tloc= eptr++;
agility-= agility>>13;
if(o,(vmem[thevar(ll)].oldval+ll)&1)agility+= 1<<19;
}
}
while(1){
if(lbptr==eptr){
l= 0;break;
}
o,l= trail[lbptr++];
o,lat= lmem[l].bimp_end;
if(lat)break;
}
if(l==0)break;
}

/*:58*/
#line 1662 "./sat13.w"
;
}
}

/*:65*/
#line 1587 "./sat13.w"
;
}
/*64:*/
#line 1635 "./sat13.w"

if(q==0)o,lmem[bar(lt)].watch= wa;
else o,link1(q)= wa;
q= wa;

/*:64*/
#line 1589 "./sat13.w"
;
}

/*:60*/
#line 2977 "./sat13.w"
;
}

/*:127*/
#line 2882 "./sat13.w"
;
newlevel:if(sanity_checking)sanity(eptr);
if(delta&&(mems>=thresh))thresh+= delta,print_state(eptr);
if(mems>=timeout){
fprintf(stderr,"TIMEOUT!\n");goto all_done;
}
if(eptr==vars){
if(!conflict_level)goto satisfied;
/*133:*/
#line 3045 "./sat13.w"

if(total_learned>=next_recycle){
if(verbose&(show_details+show_gory_details+
show_recycling+show_recycling_details))
fprintf(stderr,"Preparing to recycle ("O"llu conflicts, "O"llu mems)\n",
total_learned,mems);
/*112:*/
#line 2696 "./sat13.w"

recycle_point= max_learned;
minrange= buckets,maxrange= 0;
asserts= 0;
for(k= 0;k<vars;k++)o,levstamp[k+k+1]= 0;
for(h= 0,c= first_learned;c<max_learned;h++,c= endc+learned_extra){
o,endc= c+size(c);
/*110:*/
#line 2632 "./sat13.w"

{
o,l= mem[c].lit;
if(o,lmem[l].reason==c){
if(o,vmem[thevar(l)].value&-2)o,range(c)= 0,asserts++;
else goto its_true;
}else{
for(p= q= 0,k= c+size(c)-1;k>=c;k--){
oo,l= mem[k].lit,v= vmem[thevar(l)].value;
if(v<2){
if((v^l)&1)continue;
its_true:v= buckets+1;o,range(c)= buckets+1;
goto range_set;
}else{
if(o,levstamp[(v&-2)+1]<c)
o,levstamp[(v&-2)+1]= c,q++;
if(levstamp[(v&-2)+1]==c&&(((l^v)&1)==0))
o,levstamp[(v&-2)+1]= c+1,p++;
}
}
v= (int)((buckets/badlevel)*((float)p+alpha*(float)(q-p)));
if(v>=buckets)v= buckets-1;
o,range(c)= v;
if(v<minrange)minrange= v;
if(v> maxrange)maxrange= v;
oo,rangedist[v]++;
}
range_set:;
}

/*:110*/
#line 2703 "./sat13.w"
;
while(o,mem[endc].lit&sign_bit)endc++;
}
budget= h/2;
prev_learned= 0;

/*:112*/
#line 3051 "./sat13.w"
;
}else{
warmup_cycles++;
if(verbose&(show_choices+show_details+show_gory_details+show_warmlearn))
fprintf(stderr,"Finishing warmup round "O"d:\n",warmup_cycles);
}
o,leveldat[llevel+2]= eptr;
minjumplev= max_lit;
for(;conflict_level;)/*134:*/
#line 3081 "./sat13.w"

{
o,jumplev= conflict_level,conflict_level= conflictdat[conflict_level];
/*128:*/
#line 2980 "./sat13.w"

{
o,k= leveldat[jumplev+2];
while(eptr> k){
o,l= trail[--eptr],v= thevar(l);
oo,vmem[v].oldval= vmem[v].value;
o,vmem[v].value= unset;
o,lmem[l].reason= 0;
if(eptr<lptr&&(o,vmem[v].hloc<0))/*74:*/
#line 1815 "./sat13.w"

{
o,av= vmem[v].activity;
h= hn++,j= 0;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1819 "./sat13.w"
;
if(j==0)oo,heap[h]= v,vmem[v].hloc= h;
}

/*:74*/
#line 2988 "./sat13.w"
;
}
lptr= eptr;
if(sanity_checking){
while(llevel> jumplev)leveldat[llevel]= -1,llevel-= 2;
}else llevel= jumplev;
}

/*:128*/
#line 3084 "./sat13.w"
;
o,c= leveldat[llevel+1];
if(c<0)o,l= -c,ll= conflictdat[llevel+1];
goto prep_clause;
store_clause:


if(trivial_learning&&conflict_level){
cells_prelearned-= prelearned_size;
cells_learned-= learned_size,total_learned--,trivials--;
}else{
if(jumplev<=minjumplev){
if(jumplev<minjumplev)minjumplev= jumplev,next_learned= 0;
o,conflictdat[llevel]= next_learned,conflictdat[llevel+1]= lll;
next_learned= llevel;
}
if(learned_size==1){
o,leveldat[llevel+1]= 0;
if(learned_file){
fprintf(learned_file," "O"s"O".8s\n",litname(lll));
fflush(learned_file);
learned_out++;
}
if(verbose&show_warmlearn)
fprintf(stderr,"(learned unit clause "O"s"O".8s)\n",litname(lll));
}else{
/*103:*/
#line 2448 "./sat13.w"

{
/*104:*/
#line 2466 "./sat13.w"

if(prev_learned){
o,l= mem[prev_learned].lit;
if(!trivial_learning&&
(o,lmem[l].reason==0)&&(o,vmem[thevar(l)].value==unset))
/*105:*/
#line 2493 "./sat13.w"

{
for(o,k= size(prev_learned)-1,q= learned_size;q&&k>=q;k--){
oo,l= mem[prev_learned+k].lit,r= vmem[thevar(l)].value&-2;
if((l==lll||(uint)r<=jumplev)&&
(o,vmem[thevar(l)].stamp==curstamp))q--;

}
if(q==0){
max_learned= prev_learned;
if(verbose&show_gory_details)
fprintf(stderr,"(clause "O"d discarded)\n",prev_learned);
discards++;
o,c= prev_learned,activ(c)= 0;
o,l= mem[c].lit,r= link0(c);
/*106:*/
#line 2516 "./sat13.w"

for(o,wa= lmem[l].watch,q= 0;wa!=c;q= wa,wa= next_wa){
o,p= mem[wa].lit;
o,next_wa= (p==l?link0(wa):link1(wa));
}
if(!q)o,lmem[l].watch= r;
else if(p==l)o,link0(q)= r;
else o,link1(q)= r;

/*:106*/
#line 2508 "./sat13.w"
;
oo,l= mem[c+1].lit,r= link1(c);
/*106:*/
#line 2516 "./sat13.w"

for(o,wa= lmem[l].watch,q= 0;wa!=c;q= wa,wa= next_wa){
o,p= mem[wa].lit;
o,next_wa= (p==l?link0(wa):link1(wa));
}
if(!q)o,lmem[l].watch= r;
else if(p==l)o,link0(q)= r;
else o,link1(q)= r;

/*:106*/
#line 2510 "./sat13.w"
;
}
}

/*:105*/
#line 2472 "./sat13.w"
;
}
c= max_learned;
o,mem[c+learned_size].lit= 0;
max_learned+= learned_size+learned_extra;
if(max_learned> max_cells_used){
if(max_learned>=memsize){
fprintf(stderr,
"Memory overflow (memsize="O"u<"O"u), please increase m!\n",
memsize,max_cells_used+1);
exit(-666);
}
bytes+= (max_learned-max_cells_used)*sizeof(cel);
max_cells_used= max_learned;
}

/*:104*/
#line 2450 "./sat13.w"
;
/*107:*/
#line 2525 "./sat13.w"

if(activ(c))confusion("bumps");
size(c)= learned_size;

o,mem[c].lit= lll;
oo,link0(c)= lmem[lll].watch;
o,lmem[lll].watch= c;
if(trivial_learning){
for(j= 1,k= jumplev;k;j++,k-= 2){
oo,l= bar(trail[leveldat[k]]);
if(j==1)ooo,link1(c)= lmem[l].watch,lmem[l].watch= c;
o,mem[c+j].lit= l;
}
if(verbose&show_gory_details)
fprintf(stderr,"(trivial clause is substituted)\n");
}else for(k= 1,j= 0,jj= 1;k<learned_size;j++){
o,l= learn[j];
if(o,vmem[thevar(l)].stamp==curstamp){
o,r= vmem[thevar(l)].value;
if(jj&&r>=jumplev){
o,mem[c+1].lit= l;
oo,link1(c)= lmem[l].watch;
o,lmem[l].watch= c;
jj= 0;
}else o,mem[c+k+jj].lit= l;
k++;
}
}

/*:107*/
#line 2451 "./sat13.w"
;
prev_learned= c;
if(learned_file&&learned_size<=learn_save)
/*108:*/
#line 2554 "./sat13.w"

{
for(k= c;k<c+learned_size;k++)
fprintf(learned_file," "O"s"O".8s",litname(mem[k].lit));
fprintf(learned_file,"\n");
fflush(learned_file);
learned_out++;
}

/*:108*/
#line 2454 "./sat13.w"
;
}

/*:103*/
#line 3110 "./sat13.w"
;
o,leveldat[llevel+1]= c;
if(verbose&show_warmlearn)
fprintf(stderr,"(learned clause "O"d of size "O"d)\n",c,learned_size);
}
}
}

/*:134*/
#line 3059 "./sat13.w"
;
if(recycle_point)jumplev= 0;
else jumplev= minjumplev;
/*128:*/
#line 2980 "./sat13.w"

{
o,k= leveldat[jumplev+2];
while(eptr> k){
o,l= trail[--eptr],v= thevar(l);
oo,vmem[v].oldval= vmem[v].value;
o,vmem[v].value= unset;
o,lmem[l].reason= 0;
if(eptr<lptr&&(o,vmem[v].hloc<0))/*74:*/
#line 1815 "./sat13.w"

{
o,av= vmem[v].activity;
h= hn++,j= 0;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1819 "./sat13.w"
;
if(j==0)oo,heap[h]= v,vmem[v].hloc= h;
}

/*:74*/
#line 2988 "./sat13.w"
;
}
lptr= eptr;
if(sanity_checking){
while(llevel> jumplev)leveldat[llevel]= -1,llevel-= 2;
}else llevel= jumplev;
}

/*:128*/
#line 3062 "./sat13.w"
;
trail_marker= eptr;
if(jumplev==minjumplev)
/*135:*/
#line 3118 "./sat13.w"

while(next_learned){
o,lll= conflictdat[next_learned+1];
o,c= leveldat[next_learned+1];
next_learned= conflictdat[next_learned];
if(verbose&(show_details+show_choices)){
if((verbose&show_details)||llevel<=show_choices_max){
if(c)fprintf(stderr,"level "O"d, "O"s"O".8s from "O"d\n",
llevel>>1,litname(lll),c);
else fprintf(stderr,"level 0, "O"s"O".8s\n",litname(lll));
}
}
o,vmem[thevar(lll)].value= llevel+(lll&1),vmem[thevar(lll)].tloc= eptr;
o,lmem[lll].reason= c;
o,history[eptr]= 4,trail[eptr++]= lll;
}

/*:135*/
#line 3065 "./sat13.w"
;
/*82:*/
#line 1970 "./sat13.w"

var_bump*= var_bump_factor;
clause_bump*= clause_bump_factor;

/*:82*/
#line 3066 "./sat13.w"
;
if(recycle_point){
/*113:*/
#line 2709 "./sat13.w"

/*114:*/
#line 2714 "./sat13.w"

for(o,j= minrange,s= asserts+rangedist[j];s<budget&&j<maxrange;)
o,s+= rangedist[++j];
if(s> budget)/*115:*/
#line 2747 "./sat13.w"

{
register ullng accum;
t= s-budget;
jj= rangedist[j]-t;
if(jj> clause_heap_size)jj= clause_heap_size;
/*117:*/
#line 2776 "./sat13.w"

for(h= 0,c= first_learned;h<jj;c= endc+learned_extra){
if(c>=recycle_point)confusion("rangedist1");
o,endc= c+size(c);
while(o,mem[endc].lit&sign_bit)endc++;
if(o,range(c)==j)clause_heap[h++]= activ_as_lit(c)+c;
}

/*:117*/
#line 2753 "./sat13.w"
;
/*118:*/
#line 2784 "./sat13.w"

for(h= jj>>1;h;){
q= h+h,p= --h,o,accum= clause_heap[p];
/*119:*/
#line 2792 "./sat13.w"

while(q<=jj){
if(q==jj||(oo,clause_heap[q-1]<clause_heap[q]))q--;
if(accum<=clause_heap[q])break;
o,clause_heap[p]= clause_heap[q];
p= q,q= p+p+2;
}
o,clause_heap[p]= accum;

/*:119*/
#line 2787 "./sat13.w"
;
}

/*:118*/
#line 2754 "./sat13.w"
;
/*120:*/
#line 2804 "./sat13.w"

for(;;c= endc+learned_extra){
if(c>=recycle_point)confusion("rangedist2");
if(o,range(c)==j){
o,accum= activ_as_lit(c)+c;
if(o,accum<clause_heap[0]){
o,range(c)= j+1;
if(--t==0)break;
}else{
o,range((int)(clause_heap[0]&0xffffffff))= j+1;
if(--t==0)break;
p= 0,q= 2;
/*119:*/
#line 2792 "./sat13.w"

while(q<=jj){
if(q==jj||(oo,clause_heap[q-1]<clause_heap[q]))q--;
if(accum<=clause_heap[q])break;
o,clause_heap[p]= clause_heap[q];
p= q,q= p+p+2;
}
o,clause_heap[p]= accum;

/*:119*/
#line 2816 "./sat13.w"
;
}
}
o,endc= c+size(c);
while(o,mem[endc].lit&sign_bit)endc++;
}

/*:120*/
#line 2755 "./sat13.w"
;
}

/*:115*/
#line 2717 "./sat13.w"
;
for(k= minrange>>1;k+k<=maxrange;k++)o,rangedist[k+k]= rangedist[k+k+1]= 0;
for(h= 0,cc= c= first_learned;c<max_learned;c= endc+learned_extra){
o,jj= endc= c+size(c);
while(o,mem[endc].lit&sign_bit)o,mem[endc++].lit= 0;
if(c<recycle_point&&(o,range(c)> j))continue;

for(kk= cc,k= c;k<jj;k++){
o,l= mem[k].lit;
o,v= vmem[thevar(l)].value;
if((uint)v!=unset){
if((v^l)&1)continue;
break;
}else o,mem[kk++].lit= l;
}
if(k<jj)continue;
h++;/*121:*/
#line 2830 "./sat13.w"

if(kk>=cc+2){
if(verbose&show_recycling_details)
fprintf(stderr," clause "O"d = recycled "O"d (size "O"d)\n",
cc,c,kk-cc);
ooo,size(cc)= kk-cc,activ(cc)= activ(c),cc= kk+learned_extra;
}else if(kk==cc)goto unsat;
else{
o,l= mem[cc].lit;
o,vmem[thevar(l)].value= l&1,vmem[thevar(l)].tloc= eptr;
o,history[eptr]= 4,trail[eptr++]= l;
if(verbose&(show_choices+show_details+show_recycling_details))
fprintf(stderr," level 0, "O"s"O".8s from recycled "O"d\n",
litname(l),c);
}

/*:121*/
#line 2733 "./sat13.w"
;
}
max_learned= cc,prev_learned= 0;
o,mem[max_learned-learned_extra].lit= 0;
if(verbose&(show_recycling+show_recycling_details))
fprintf(stderr," (recycling reduced "O"d learned clauses to "O"d)\n",
budget*2+1,h);

/*:114*/
#line 2710 "./sat13.w"
;
/*122:*/
#line 2846 "./sat13.w"

for(l= 2;l<=max_lit;l++)o,lmem[l].watch= 0;
for(c= clause_extra;c<min_learned;c= endc+clause_extra){
o,endc= c+size(c);
/*123:*/
#line 2861 "./sat13.w"

{
o,l= mem[c].lit;
ooo,link0(c)= lmem[l].watch,lmem[l].watch= c;
l= mem[c+1].lit;
ooo,link1(c)= lmem[l].watch,lmem[l].watch= c;
}

/*:123*/
#line 2850 "./sat13.w"
;
while(o,mem[endc].lit&sign_bit)endc++;
}
for(c= first_learned;c<max_learned;c= endc+learned_extra){
o,endc= c+size(c);
/*123:*/
#line 2861 "./sat13.w"

{
o,l= mem[c].lit;
ooo,link0(c)= lmem[l].watch,lmem[l].watch= c;
l= mem[c+1].lit;
ooo,link1(c)= lmem[l].watch,lmem[l].watch= c;
}

/*:123*/
#line 2855 "./sat13.w"
;
}

/*:122*/
#line 2711 "./sat13.w"
;
recycle_point= 0;

/*:113*/
#line 3068 "./sat13.w"
;
if(sanity_checking)sanity(eptr);
/*132:*/
#line 3032 "./sat13.w"

recycle_bump+= recycle_inc;
next_recycle= total_learned+recycle_bump;
if(next_recycle> doomsday)next_recycle= doomsday;

/*:132*/
#line 3070 "./sat13.w"
;
}

/*:133*/
#line 2890 "./sat13.w"
;
goto startup;
}
if(!conflict_level){
if(total_learned>=doomsday)/*138:*/
#line 3187 "./sat13.w"

{
if(verbose&show_basics)
fprintf(stderr,"Timeout: Terminating an incomplete run (level "O"d).\n",
llevel>>1);
print_state(eptr);
if(polarity_outfile){
for(k= 0;k<eptr;k++){
o,l= trail[k];
fprintf(polarity_outfile," "O"s"O".8s",litname(l));
o,vmem[thevar(l)].oldval= unset;
}
fprintf(polarity_outfile,"\n");
for(v= 1;v<=vars;v++)if(o,vmem[v].oldval!=unset)
fprintf(polarity_outfile,""O"s"O".8s\n",
vmem[v].oldval&1?"~":"",vmem[v].name.ch8);
fprintf(stderr,"Polarity data written to file `"O"s'.\n",polarity_out_name);
}
if(restart_file){
for(o,k= 0;k<leveldat[2];k++)
o,fprintf(restart_file," "O"s"O".8s\n",litname(trail[k]));
for(c= first_learned;c<max_learned;c= kk+learned_extra){
for(o,k= c,kk= c+size(c);k<kk;k++)
o,fprintf(restart_file," "O"s"O".8s",litname(mem[k].lit));
fprintf(restart_file,"\n");
}
fprintf(stderr,"Current learned clauses written to file `"O"s'.\n",
restart_name);
}
goto all_done;
}

/*:138*/
#line 2894 "./sat13.w"
;
if(total_learned>=next_recycle)full_run= 1;
else if(total_learned>=next_restart)/*136:*/
#line 3140 "./sat13.w"

{
/*131:*/
#line 3025 "./sat13.w"

if((restart_u&-restart_u)==restart_v)
restart_u++,restart_v= 1,restart_thresh= restart_psi;
else restart_v<<= 1,restart_thresh+= restart_thresh>>4;
next_restart= total_learned+restart_v;
if(next_restart> doomsday)next_restart= doomsday;

/*:131*/
#line 3142 "./sat13.w"
;
if(agility<=restart_thresh)/*137:*/
#line 3159 "./sat13.w"

{
actual_restarts++;
if(verbose&(show_details+show_choices+show_restarts))
fprintf(stderr,
"Restarting ("O"llu conflicts, "O"llu mems, agility "O".2f)\n",
total_learned,mems,(double)agility/two_to_the_32);
if(llevel){
while(1){
o,v= heap[0];
if(o,vmem[v].value==unset)break;
/*77:*/
#line 1870 "./sat13.w"

o,vmem[v].hloc= -1;
if(--hn){
o,u= heap[hn];
o,au= vmem[u].activity;
for(h= 0,hp= 1;hp<hn;h= hp,hp= h+h+1){
oo,av= vmem[heap[hp]].activity;
if(hp+1<hn&&(oo,vmem[heap[hp+1]].activity> av))
hp++,av= vmem[heap[hp]].activity;
if(au>=av)break;
o,heap[h]= heap[hp];
o,vmem[heap[hp]].hloc= h;
}
o,heap[h]= u;
o,vmem[u].hloc= h;
}

/*:77*/
#line 3170 "./sat13.w"
;
}
o,av= vmem[v].activity;
for(jumplev= 0;jumplev<llevel;jumplev+= 2){
oo,v= thevar(trail[leveldat[jumplev+2]]);
if(o,vmem[v].activity<av)break;
}
if(jumplev<llevel)/*128:*/
#line 2980 "./sat13.w"

{
o,k= leveldat[jumplev+2];
while(eptr> k){
o,l= trail[--eptr],v= thevar(l);
oo,vmem[v].oldval= vmem[v].value;
o,vmem[v].value= unset;
o,lmem[l].reason= 0;
if(eptr<lptr&&(o,vmem[v].hloc<0))/*74:*/
#line 1815 "./sat13.w"

{
o,av= vmem[v].activity;
h= hn++,j= 0;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1819 "./sat13.w"
;
if(j==0)oo,heap[h]= v,vmem[v].hloc= h;
}

/*:74*/
#line 2988 "./sat13.w"
;
}
lptr= eptr;
if(sanity_checking){
while(llevel> jumplev)leveldat[llevel]= -1,llevel-= 2;
}else llevel= jumplev;
}

/*:128*/
#line 3177 "./sat13.w"
;
}
trail_marker= eptr;
warmup_cycles= 0;
goto startup;
}

/*:137*/
#line 3143 "./sat13.w"

else if(verbose&show_restarts)
fprintf(stderr,
"No restart ("O"llu conflicts, "O"llu mems, agility "O".2f)\n",
total_learned,mems,(double)agility/two_to_the_32);
}

/*:136*/
#line 2896 "./sat13.w"
;
}
llevel+= 2;
/*75:*/
#line 1840 "./sat13.w"

if(rand_prob_thresh){
mems+= 4,h= gb_next_rand();
if(h<rand_prob_thresh){
/*76:*/
#line 1858 "./sat13.w"

{
register unsigned long t= two_to_the_31-(two_to_the_31 mod hn);
register long r;
do{
mems+= 4,r= gb_next_rand();
}while(t<=(unsigned long)r);
h= r mod hn;
}

/*:76*/
#line 1844 "./sat13.w"

o,v= heap[h];
if(o,vmem[v].value!=unset)h= 0;
}else h= 0;
}else h= 0;
if(h==0){
while(1){
o,v= heap[0];
/*77:*/
#line 1870 "./sat13.w"

o,vmem[v].hloc= -1;
if(--hn){
o,u= heap[hn];
o,au= vmem[u].activity;
for(h= 0,hp= 1;hp<hn;h= hp,hp= h+h+1){
oo,av= vmem[heap[hp]].activity;
if(hp+1<hn&&(oo,vmem[heap[hp+1]].activity> av))
hp++,av= vmem[heap[hp]].activity;
if(au>=av)break;
o,heap[h]= heap[hp];
o,vmem[heap[hp]].hloc= h;
}
o,heap[h]= u;
o,vmem[u].hloc= h;
}

/*:77*/
#line 1852 "./sat13.w"
;
if(o,vmem[v].value==unset)break;
}
}
o,l= poslit(v)+(vmem[v].oldval&1);

/*:75*/
#line 2899 "./sat13.w"
;
if(verbose&show_choices&&llevel<=show_choices_max)
fprintf(stderr,"Level "O"d, trying "O"s"O".8s ("O"lld mems)\n",
llevel>>1,litname(l),mems);
depth_per_decision+= -(depth_per_decision>>7)+((ullng)llevel<<24);
trail_per_decision+= -(trail_per_decision>>7)+((ullng)eptr<<25);
o,lmem[l].reason= 0;
history[eptr]= 0;
launch:nodes++;
o,leveldat[llevel]= eptr;
o,trail[eptr++]= l;
o,vmem[thevar(l)].tloc= lptr;
vmem[thevar(l)].value= llevel+(l&1);
agility-= agility>>13;
goto proceed;
/*125:*/
#line 2921 "./sat13.w"

confl:if(llevel){
prep_clause:/*86:*/
#line 2062 "./sat13.w"

oldptr= jumplev= xnew= clevels= resols= 0;
/*91:*/
#line 2133 "./sat13.w"

if(curstamp>=0xfffffffe){
for(k= 1;k<=vars;k++)oo,vmem[k].stamp= levstamp[k+k-2]= 0;
curstamp= 1;
}else curstamp+= 3;

/*:91*/
#line 2064 "./sat13.w"
;
if(verbose&show_gory_details)
fprintf(stderr,"Preparing to learn");
if(c<0)/*88:*/
#line 2096 "./sat13.w"

{
o,tl= vmem[thevar(ll)].tloc;
o,vmem[thevar(ll)].stamp= curstamp;
l= ll;
/*70:*/
#line 1753 "./sat13.w"

v= thevar(l);
o,av= vmem[v].activity+var_bump;
o,vmem[v].activity= av;
if(av>=1e100)/*83:*/
#line 1982 "./sat13.w"

{
register int v;
register double av;
for(v= 1;v<=vars;v++){
o,av= vmem[v].activity;
if(av)
o,vmem[v].activity= (av*1e-100<tiny?tiny:av*1e-100);
}
var_bump*= 1e-100;
}

/*:83*/
#line 1757 "./sat13.w"
;
o,h= vmem[v].hloc;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1759 "./sat13.w"
;

/*:70*/
#line 2101 "./sat13.w"
;
l= -c;
if(o,vmem[thevar(l)].tloc> tl)tl= vmem[thevar(l)].tloc;
o,vmem[thevar(l)].stamp= curstamp;
/*70:*/
#line 1753 "./sat13.w"

v= thevar(l);
o,av= vmem[v].activity+var_bump;
o,vmem[v].activity= av;
if(av>=1e100)/*83:*/
#line 1982 "./sat13.w"

{
register int v;
register double av;
for(v= 1;v<=vars;v++){
o,av= vmem[v].activity;
if(av)
o,vmem[v].activity= (av*1e-100<tiny?tiny:av*1e-100);
}
var_bump*= 1e-100;
}

/*:83*/
#line 1757 "./sat13.w"
;
o,h= vmem[v].hloc;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1759 "./sat13.w"
;

/*:70*/
#line 2105 "./sat13.w"
;
xnew= 1;
}

/*:88*/
#line 2067 "./sat13.w"

else/*87:*/
#line 2078 "./sat13.w"

{
o,l= bar(mem[c].lit);
o,tl= vmem[thevar(l)].tloc;
o,vmem[thevar(l)].stamp= curstamp;
/*70:*/
#line 1753 "./sat13.w"

v= thevar(l);
o,av= vmem[v].activity+var_bump;
o,vmem[v].activity= av;
if(av>=1e100)/*83:*/
#line 1982 "./sat13.w"

{
register int v;
register double av;
for(v= 1;v<=vars;v++){
o,av= vmem[v].activity;
if(av)
o,vmem[v].activity= (av*1e-100<tiny?tiny:av*1e-100);
}
var_bump*= 1e-100;
}

/*:83*/
#line 1757 "./sat13.w"
;
o,h= vmem[v].hloc;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1759 "./sat13.w"
;

/*:70*/
#line 2083 "./sat13.w"
;
if(c>=first_learned)/*81:*/
#line 1962 "./sat13.w"

{
float ac;
o,ac= activ(c)+clause_bump;
o,activ(c)= ac;
if(ac>=1e20)/*84:*/
#line 1997 "./sat13.w"

{
register int cc,endc;
for(cc= first_learned;cc<max_learned;cc= endc+learned_extra){
o,endc= cc+size(cc);
o,ac= activ(cc);
if(ac)
o,activ(cc)= (ac*1e-20<single_tiny?single_tiny:ac*1e-20);
while(o,mem[endc].lit&sign_bit)endc++;
}
clause_bump*= 1e-20;
}

/*:84*/
#line 1967 "./sat13.w"
;
}

/*:81*/
#line 2084 "./sat13.w"
;
for(o,s= size(c),k= c+s-1;k> c;k--){
o,l= bar(mem[k].lit);
j= vmem[thevar(l)].tloc;
if(j> tl)tl= j;
/*95:*/
#line 2181 "./sat13.w"

{
o,jj= vmem[thevar(l)].value&-2;
if(!jj)confusion("permanently false lit");
else{
o,vmem[thevar(l)].stamp= curstamp;
/*70:*/
#line 1753 "./sat13.w"

v= thevar(l);
o,av= vmem[v].activity+var_bump;
o,vmem[v].activity= av;
if(av>=1e100)/*83:*/
#line 1982 "./sat13.w"

{
register int v;
register double av;
for(v= 1;v<=vars;v++){
o,av= vmem[v].activity;
if(av)
o,vmem[v].activity= (av*1e-100<tiny?tiny:av*1e-100);
}
var_bump*= 1e-100;
}

/*:83*/
#line 1757 "./sat13.w"
;
o,h= vmem[v].hloc;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1759 "./sat13.w"
;

/*:70*/
#line 2187 "./sat13.w"
;
if(jj>=llevel)xnew++;
else{
if(jj> jumplev)jumplev= jj;
o,learn[oldptr++]= bar(l);
if(verbose&show_gory_details)
fprintf(stderr," "O"s"O".8s{"O"d}",
litname(bar(l)),vmem[thevar(l)].value>>1);
if(o,levstamp[jj]<curstamp)o,levstamp[jj]= curstamp,clevels++;
else if(levstamp[jj]==curstamp)o,levstamp[jj]= curstamp+1;
}
}
}

/*:95*/
#line 2089 "./sat13.w"
;
}
}

/*:87*/
#line 2068 "./sat13.w"
;
/*92:*/
#line 2139 "./sat13.w"

while(xnew){
while(1){
o,l= trail[tl--];
if(o,vmem[thevar(l)].stamp==curstamp)break;
}
xnew--;
/*93:*/
#line 2157 "./sat13.w"

resols++;
if(verbose&show_gory_details)
fprintf(stderr," ["O"s"O".8s]",litname(l));
o,c= lmem[l].reason;
if(c<0)/*94:*/
#line 2174 "./sat13.w"

{
l= -c;
if(o,vmem[thevar(l)].stamp!=curstamp)
/*95:*/
#line 2181 "./sat13.w"

{
o,jj= vmem[thevar(l)].value&-2;
if(!jj)confusion("permanently false lit");
else{
o,vmem[thevar(l)].stamp= curstamp;
/*70:*/
#line 1753 "./sat13.w"

v= thevar(l);
o,av= vmem[v].activity+var_bump;
o,vmem[v].activity= av;
if(av>=1e100)/*83:*/
#line 1982 "./sat13.w"

{
register int v;
register double av;
for(v= 1;v<=vars;v++){
o,av= vmem[v].activity;
if(av)
o,vmem[v].activity= (av*1e-100<tiny?tiny:av*1e-100);
}
var_bump*= 1e-100;
}

/*:83*/
#line 1757 "./sat13.w"
;
o,h= vmem[v].hloc;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1759 "./sat13.w"
;

/*:70*/
#line 2187 "./sat13.w"
;
if(jj>=llevel)xnew++;
else{
if(jj> jumplev)jumplev= jj;
o,learn[oldptr++]= bar(l);
if(verbose&show_gory_details)
fprintf(stderr," "O"s"O".8s{"O"d}",
litname(bar(l)),vmem[thevar(l)].value>>1);
if(o,levstamp[jj]<curstamp)o,levstamp[jj]= curstamp,clevels++;
else if(levstamp[jj]==curstamp)o,levstamp[jj]= curstamp+1;
}
}
}

/*:95*/
#line 2178 "./sat13.w"
;
}

/*:94*/
#line 2162 "./sat13.w"

else if(c){
if(c>=first_learned)/*81:*/
#line 1962 "./sat13.w"

{
float ac;
o,ac= activ(c)+clause_bump;
o,activ(c)= ac;
if(ac>=1e20)/*84:*/
#line 1997 "./sat13.w"

{
register int cc,endc;
for(cc= first_learned;cc<max_learned;cc= endc+learned_extra){
o,endc= cc+size(cc);
o,ac= activ(cc);
if(ac)
o,activ(cc)= (ac*1e-20<single_tiny?single_tiny:ac*1e-20);
while(o,mem[endc].lit&sign_bit)endc++;
}
clause_bump*= 1e-20;
}

/*:84*/
#line 1967 "./sat13.w"
;
}

/*:81*/
#line 2164 "./sat13.w"
;
for(o,s= size(c),k= c+s-1;k> c;k--){
o,l= bar(mem[k].lit);
if(o,vmem[thevar(l)].stamp!=curstamp)
/*95:*/
#line 2181 "./sat13.w"

{
o,jj= vmem[thevar(l)].value&-2;
if(!jj)confusion("permanently false lit");
else{
o,vmem[thevar(l)].stamp= curstamp;
/*70:*/
#line 1753 "./sat13.w"

v= thevar(l);
o,av= vmem[v].activity+var_bump;
o,vmem[v].activity= av;
if(av>=1e100)/*83:*/
#line 1982 "./sat13.w"

{
register int v;
register double av;
for(v= 1;v<=vars;v++){
o,av= vmem[v].activity;
if(av)
o,vmem[v].activity= (av*1e-100<tiny?tiny:av*1e-100);
}
var_bump*= 1e-100;
}

/*:83*/
#line 1757 "./sat13.w"
;
o,h= vmem[v].hloc;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1759 "./sat13.w"
;

/*:70*/
#line 2187 "./sat13.w"
;
if(jj>=llevel)xnew++;
else{
if(jj> jumplev)jumplev= jj;
o,learn[oldptr++]= bar(l);
if(verbose&show_gory_details)
fprintf(stderr," "O"s"O".8s{"O"d}",
litname(bar(l)),vmem[thevar(l)].value>>1);
if(o,levstamp[jj]<curstamp)o,levstamp[jj]= curstamp,clevels++;
else if(levstamp[jj]==curstamp)o,levstamp[jj]= curstamp+1;
}
}
}

/*:95*/
#line 2168 "./sat13.w"
;
}
if(xnew+oldptr+1<s&&xnew)
/*98:*/
#line 2258 "./sat13.w"

{
l= mem[c].lit;
o,size(c)= --s,subsumptions++;
if(learned_file&&s<=learn_save){
fprintf(learned_file," ");
for(k= c+1;k<=c+s;k++)
fprintf(learned_file," "O"s"O".8s",litname(mem[k].lit));
fprintf(learned_file,"\n");
fflush(learned_file);
learned_out++;
}
o,r= link0(c);
/*106:*/
#line 2516 "./sat13.w"

for(o,wa= lmem[l].watch,q= 0;wa!=c;q= wa,wa= next_wa){
o,p= mem[wa].lit;
o,next_wa= (p==l?link0(wa):link1(wa));
}
if(!q)o,lmem[l].watch= r;
else if(p==l)o,link0(q)= r;
else o,link1(q)= r;

/*:106*/
#line 2271 "./sat13.w"
;
o,ll= mem[c+s].lit;
for(lll= ll,k= c+s;;k--){
o,r= vmem[thevar(lll)].value&-2;
if(r==llevel)break;
o,lll= mem[k-1].lit;
}
if(lll!=ll)o,mem[k].lit= ll;
oo,mem[c+s].lit= l+sign_bit,mem[c].lit= lll;
ooo,link0(c)= lmem[lll].watch,lmem[lll].watch= c;
if(verbose&show_watches)
fprintf(stderr," ["O"s"O".8s watches "O"d]",litname(lll),c);
}

/*:98*/
#line 2171 "./sat13.w"
;
}

/*:93*/
#line 2146 "./sat13.w"
;
}

/*:92*/
#line 2069 "./sat13.w"
;
while(1){
o,l= trail[tl--];
if(o,vmem[thevar(l)].stamp==curstamp)break;
}
lll= bar(l);
if(verbose&show_gory_details)
fprintf(stderr," "O"s"O".8s\n",litname(lll));

/*:86*/
#line 2923 "./sat13.w"
;
/*102:*/
#line 2412 "./sat13.w"

learned_size= oldptr+1;
cells_prelearned+= learned_size,prelearned_size= learned_size;
for(kk= 0;kk<oldptr;kk++){
o,l= bar(learn[kk]);
oo,s= levstamp[vmem[thevar(l)].value&-2];
if(s<curstamp+1)continue;
/*100:*/
#line 2340 "./sat13.w"

if(stackptr)confusion("stack");
test:ll= l;
o,c= lmem[l].reason;
if(c==0)goto clear_stack;
if(c<0){
l= bar(-c);
o,s= vmem[thevar(l)].stamp;
if(s>=curstamp){
if(s==curstamp+2)goto clear_stack;
}else{
o,stack[stackptr++]= ll;
goto test;
}
}else{
for(o,k= c+size(c)-1;k> c;k--){
oo,l= bar(mem[k].lit),s= vmem[thevar(l)].stamp;
if(s>=curstamp){
if(s==curstamp+2)goto clear_stack;
continue;
}
o,s= vmem[thevar(l)].value&-2;
if(s==0)continue;
o,s= levstamp[s];
if(s<curstamp){
o,vmem[thevar(l)].stamp= curstamp+2;
goto clear_stack;
}
o,stack[stackptr]= k,stack[stackptr+1]= ll,stackptr+= 2;
goto test;
test1:continue;
}
}
is_red:o,vmem[thevar(ll)].stamp= curstamp+1;

if(stackptr){
oo,ll= stack[--stackptr],c= lmem[ll].reason;
if(c<0)goto is_red;
o,k= stack[--stackptr];
goto test1;
}
goto redundant;
/*101:*/
#line 2392 "./sat13.w"

clear_stack:if(stackptr){
o,vmem[thevar(ll)].stamp= curstamp+2;
o,ll= stack[--stackptr];
o,c= lmem[ll].reason;
if(c> 0)stackptr--;
goto clear_stack;
}

/*:101*/
#line 2382 "./sat13.w"
;

/*:100*/
#line 2419 "./sat13.w"
;
continue;
redundant:learned_size--;
if(verbose&show_gory_details)
fprintf(stderr,"("O"s"O".8s is redundant)\n",litname(bar(ll)));
}
if(learned_size<=(jumplev>>1)+trivial_limit)trivial_learning= 0;
else trivial_learning= 1,clevels= jumplev>>1,learned_size= clevels+1,trivials++;
cells_learned+= learned_size,total_learned++;
/*40:*/
#line 1150 "./sat13.w"

mems_per_confl+= -(mems_per_confl>>7)+((mems-mems_at_prev_confl)<<25);
mems_at_prev_confl= mems;
props_per_confl+= -(props_per_confl>>7)+((ullng)props<<25);
props= 0;
res_per_confl+= -(res_per_confl>>7)+((ullng)resols<<25);
lits_per_confl+= -(lits_per_confl>>7)+((ullng)learned_size<<25);
if(!trivial_learning)
lits_per_nontriv+= -(lits_per_nontriv>>7)+((ullng)learned_size<<25);
short_per_confl+= -(short_per_confl>>7)+(learned_size> 6?0:1<<25);
glucose_per_confl+= -(glucose_per_confl>>7)+((ullng)clevels<<25);

/*:40*/
#line 2428 "./sat13.w"
;

/*:102*/
#line 2924 "./sat13.w"
;

if(full_run)goto store_clause;
decisionvar= (lmem[bar(lll)].reason?0:1);
/*128:*/
#line 2980 "./sat13.w"

{
o,k= leveldat[jumplev+2];
while(eptr> k){
o,l= trail[--eptr],v= thevar(l);
oo,vmem[v].oldval= vmem[v].value;
o,vmem[v].value= unset;
o,lmem[l].reason= 0;
if(eptr<lptr&&(o,vmem[v].hloc<0))/*74:*/
#line 1815 "./sat13.w"

{
o,av= vmem[v].activity;
h= hn++,j= 0;
if(h> 0)/*73:*/
#line 1795 "./sat13.w"

{
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity<av){
while(1){
o,heap[h]= u;
o,vmem[u].hloc= h;
h= hp;
if(h==0)break;
hp= (h-1)>>1;
o,u= heap[hp];
if(o,vmem[u].activity>=av)break;
}
o,heap[h]= v;
o,vmem[v].hloc= h;
j= 1;
}
}

/*:73*/
#line 1819 "./sat13.w"
;
if(j==0)oo,heap[h]= v,vmem[v].hloc= h;
}

/*:74*/
#line 2988 "./sat13.w"
;
}
lptr= eptr;
if(sanity_checking){
while(llevel> jumplev)leveldat[llevel]= -1,llevel-= 2;
}else llevel= jumplev;
}

/*:128*/
#line 2928 "./sat13.w"
;
if(learned_size> 1){
/*103:*/
#line 2448 "./sat13.w"

{
/*104:*/
#line 2466 "./sat13.w"

if(prev_learned){
o,l= mem[prev_learned].lit;
if(!trivial_learning&&
(o,lmem[l].reason==0)&&(o,vmem[thevar(l)].value==unset))
/*105:*/
#line 2493 "./sat13.w"

{
for(o,k= size(prev_learned)-1,q= learned_size;q&&k>=q;k--){
oo,l= mem[prev_learned+k].lit,r= vmem[thevar(l)].value&-2;
if((l==lll||(uint)r<=jumplev)&&
(o,vmem[thevar(l)].stamp==curstamp))q--;

}
if(q==0){
max_learned= prev_learned;
if(verbose&show_gory_details)
fprintf(stderr,"(clause "O"d discarded)\n",prev_learned);
discards++;
o,c= prev_learned,activ(c)= 0;
o,l= mem[c].lit,r= link0(c);
/*106:*/
#line 2516 "./sat13.w"

for(o,wa= lmem[l].watch,q= 0;wa!=c;q= wa,wa= next_wa){
o,p= mem[wa].lit;
o,next_wa= (p==l?link0(wa):link1(wa));
}
if(!q)o,lmem[l].watch= r;
else if(p==l)o,link0(q)= r;
else o,link1(q)= r;

/*:106*/
#line 2508 "./sat13.w"
;
oo,l= mem[c+1].lit,r= link1(c);
/*106:*/
#line 2516 "./sat13.w"

for(o,wa= lmem[l].watch,q= 0;wa!=c;q= wa,wa= next_wa){
o,p= mem[wa].lit;
o,next_wa= (p==l?link0(wa):link1(wa));
}
if(!q)o,lmem[l].watch= r;
else if(p==l)o,link0(q)= r;
else o,link1(q)= r;

/*:106*/
#line 2510 "./sat13.w"
;
}
}

/*:105*/
#line 2472 "./sat13.w"
;
}
c= max_learned;
o,mem[c+learned_size].lit= 0;
max_learned+= learned_size+learned_extra;
if(max_learned> max_cells_used){
if(max_learned>=memsize){
fprintf(stderr,
"Memory overflow (memsize="O"u<"O"u), please increase m!\n",
memsize,max_cells_used+1);
exit(-666);
}
bytes+= (max_learned-max_cells_used)*sizeof(cel);
max_cells_used= max_learned;
}

/*:104*/
#line 2450 "./sat13.w"
;
/*107:*/
#line 2525 "./sat13.w"

if(activ(c))confusion("bumps");
size(c)= learned_size;

o,mem[c].lit= lll;
oo,link0(c)= lmem[lll].watch;
o,lmem[lll].watch= c;
if(trivial_learning){
for(j= 1,k= jumplev;k;j++,k-= 2){
oo,l= bar(trail[leveldat[k]]);
if(j==1)ooo,link1(c)= lmem[l].watch,lmem[l].watch= c;
o,mem[c+j].lit= l;
}
if(verbose&show_gory_details)
fprintf(stderr,"(trivial clause is substituted)\n");
}else for(k= 1,j= 0,jj= 1;k<learned_size;j++){
o,l= learn[j];
if(o,vmem[thevar(l)].stamp==curstamp){
o,r= vmem[thevar(l)].value;
if(jj&&r>=jumplev){
o,mem[c+1].lit= l;
oo,link1(c)= lmem[l].watch;
o,lmem[l].watch= c;
jj= 0;
}else o,mem[c+k+jj].lit= l;
k++;
}
}

/*:107*/
#line 2451 "./sat13.w"
;
prev_learned= c;
if(learned_file&&learned_size<=learn_save)
/*108:*/
#line 2554 "./sat13.w"

{
for(k= c;k<c+learned_size;k++)
fprintf(learned_file," "O"s"O".8s",litname(mem[k].lit));
fprintf(learned_file,"\n");
fflush(learned_file);
learned_out++;
}

/*:108*/
#line 2454 "./sat13.w"
;
}

/*:103*/
#line 2930 "./sat13.w"

if(verbose&(show_details+show_choices)){
if((verbose&show_details)||llevel<=show_choices_max)
fprintf(stderr,"level "O"d, "O"s"O".8s from "O"d\n",
llevel>>1,litname(lll),c);
}
o,lmem[lll].reason= c;
}else/*126:*/
#line 2955 "./sat13.w"

{
if(verbose&(show_details+show_choices))
fprintf(stderr,"level 0, learned "O"s"O".8s\n",litname(lll));
if(learned_file){
fprintf(learned_file," "O"s"O".8s\n",litname(lll));
fflush(learned_file);
learned_out++;
}
}

/*:126*/
#line 2937 "./sat13.w"
;
o,vmem[thevar(lll)].value= llevel+(lll&1),vmem[thevar(lll)].tloc= eptr;
history[eptr]= (decisionvar?2:6);
o,trail[eptr++]= lll;
agility-= agility>>13;
agility+= 1<<19;
/*82:*/
#line 1970 "./sat13.w"

var_bump*= var_bump_factor;
clause_bump*= clause_bump_factor;

/*:82*/
#line 2943 "./sat13.w"
;
if(sanity_checking)sanity(eptr);
goto proceed;
}
unsat:if(1){
printf("~\n");
if(verbose&show_basics)fprintf(stderr,"UNSAT\n");
}else{
satisfied:if(verbose&show_basics)fprintf(stderr,"!SAT!\n");
/*129:*/
#line 2996 "./sat13.w"

for(k= 0;k<vars;k++){
o,printf(" "O"s"O".8s",litname(trail[k]));
}
printf("\n");
if(out_file){
for(k= 0;k<vars;k++){
o,fprintf(out_file," "O"s"O".8s",litname(bar(trail[k])));
}
fprintf(out_file,"\n");
fprintf(stderr,"Solution-avoiding clause written to file `"O"s'.\n",out_name);
}

/*:129*/
#line 2952 "./sat13.w"
;
}

/*:125*/
#line 2914 "./sat13.w"
;

/*:124*/
#line 115 "./sat13.w"
;
all_done:/*8:*/
#line 358 "./sat13.w"

if(out_file)fclose(out_file);
if(learned_file)fclose(learned_file);
if(restart_file)fclose(restart_file);
if(polarity_infile)fclose(polarity_infile);
if(polarity_outfile)fclose(polarity_outfile);

/*:8*/
#line 116 "./sat13.w"
;
/*7:*/
#line 331 "./sat13.w"

if(verbose&show_basics){
fprintf(stderr,
"Altogether "O"llu+"O"llu mems, "O"llu bytes, "O"llu node"O"s,",
imems,mems,bytes,nodes,nodes==1?"":"s");
fprintf(stderr," "O"llu clauses learned",total_learned);
if(total_learned)
fprintf(stderr," (ave "O".1f->"O".1f)",
cells_prelearned/(double)total_learned,
cells_learned/(double)total_learned);
fprintf(stderr,", "O"u memcells.\n",max_cells_used);
if(learned_file)
fprintf(stderr,""O"lld learned clauses written to file `"O"s'.\n",
learned_out,learned_name);
if(trivials)
fprintf(stderr,"("O"lld learned clause"O"s trivial.)\n",
trivials,trivials==1?" was":"s were");
if(discards)
fprintf(stderr,"("O"lld learned clause"O"s discarded.)\n",
discards,discards==1?" was":"s were");
if(subsumptions)
fprintf(stderr,"("O"lld clause"O"s subsumed on-the-fly.)\n",
subsumptions,subsumptions==1?" was":"s were");
fprintf(stderr,"("O"lld restart"O"s.)\n",
actual_restarts,actual_restarts==1?"":"s");
}

/*:7*/
#line 117 "./sat13.w"
;
}

/*:2*/
