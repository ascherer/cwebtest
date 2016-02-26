#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define O "%" \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_gory_details 8
#define show_doubly_gory_details 16
#define show_unused_vars 32
#define show_scores 64
#define show_strong_comps 128
#define show_looks 256 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define thevar(l) ((l) >>1) 
#define bar(l) ((l) ^1) 
#define poslit(x) ((x) <<1) 
#define neglit(x) (((x) <<1) +1)  \

#define sanity_checking 0 \

#define litname(l) (l) &1?"~":"",vmem[thevar(l) ].name.ch8 \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

#define linkf(b) mem[b]
#define linkb(b) mem[(b) +1]
#define kval(b) mem[(b) +2]
#define avail(k) (((k) -2) <<2) 
#define memfree(b) ((int) mem[b]<0) 
#define memk_max_default 22 \

#define real_truth 0xfffffffe
#define near_truth 0xfffffffc
#define proto_truth 0xfffffffa
#define isfixed(l) (o,stamp[thevar(l) ]>=cs) 
#define isfree(l) (o,stamp[thevar(l) ]<real_truth) 
#define iscontrary(l) ((stamp[thevar(l) ]^l) &1) 
#define stamptrue(l) (o,stamp[thevar(l) ]= cs+(l&1) )  \

#define htable(lev) &hmem[(lev) *(int) lits-2] \

#define infty badlit \

#define height untagged
#define child min
#define root 1 \

/*2:*/
#line 101 "sat11.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*5:*/
#line 293 "sat11.w"

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
#line 320 "sat11.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:6*//*26:*/
#line 737 "sat11.w"

typedef struct bdata_struct{
uint addr;
uint size;
uint alloc;
uint k;
}bdata;
typedef struct idata_struct{
uint lit;
uint size;
}idata;

/*:26*//*27:*/
#line 761 "sat11.w"

typedef struct tdata_struct{
uint addr;
uint size;
}tdata;
typedef struct tpair_struct{
uint u,v;
uint link;
uint spare;
}tpair;

/*:27*//*28:*/
#line 780 "sat11.w"

typedef struct ndata_struct{
uint decision;
int branch;
int rptr,iptr,lptr;
}ndata;

/*:28*//*34:*/
#line 904 "sat11.w"

typedef struct lit_struct{
int rank;
int link;
int untagged;
int min;
int parent;
int vcomp;
int arcs;
uint bstamp;
uint dl_fail;
uint istamp;
float wnb;
uint filler;
}literal;

/*:34*//*35:*/
#line 924 "sat11.w"

typedef struct var_struct{
octa name;
int pfx,len;
}variable;

/*:35*//*88:*/
#line 2016 "sat11.w"

typedef struct cdata_struct{
uint var;
float rating;
}cdata;

/*:88*//*106:*/
#line 2414 "sat11.w"

typedef struct arc_struct{
uint tip;
int next;
}arc;

/*:106*//*118:*/
#line 2696 "sat11.w"

typedef struct ldata_struct{
uint lit;
uint offset;
}ldata;

/*:118*/
#line 108 "sat11.w"
;
/*3:*/
#line 142 "sat11.w"

int random_seed= 0;
int verbose= show_basics+show_unused_vars;
int show_choices_max= 1000000;
int hbits= 8;
int print_state_cutoff= 32*80;
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
uint memk_max= memk_max_default;
float alpha= 3.5;
float max_score= 20.0;
int hlevel_max= 50;
int levelcand= 600;
int mincutoff= 30;
int max_prelook_arcs= 1000;
int dl_max_iter= 32;
float dl_rho= 0.9995;

/*:3*//*7:*/
#line 326 "sat11.w"

char*buf;
tmp_var**hash;
uint hash_bits[93][8];
vchunk*cur_vchunk;
vchunk*last_vchunk;
tmp_var*cur_tmp_var;
tmp_var*bad_tmp_var;
chunk*cur_chunk;
tmp_var**cur_cell;
tmp_var**bad_cell;
ullng vars;
ullng clauses;
ullng nullclauses;
int ternaries;
ullng cells;
int non_clause;

/*:7*//*24:*/
#line 685 "sat11.w"

uint*stamp;
uint*mem;
bdata*bimp;
tpair*tmem;
tdata*timp;
uint*freevar,*freeloc;
int freevars;
uint*rstack;
int rptr;
idata*istack;
int iptr;
int iptr_max;
ndata*nstack;
int level;
literal*lmem;
variable*vmem;

/*:24*//*36:*/
#line 935 "sat11.w"

uint lits;
uint badlit;

/*:36*//*48:*/
#line 1198 "sat11.w"

int memk;

/*:48*//*60:*/
#line 1430 "sat11.w"

uint bestlit;
uint cs;
uint look_cs,dlook_cs;
int fptr,eptr,lfptr;

/*:60*//*67:*/
#line 1546 "sat11.w"

uint istamp;
uint bstamp= 32;

/*:67*//*89:*/
#line 2022 "sat11.w"

cdata*cand;
int cands;
float sum;
int no_newbies;
float*rating;
uint prefix;
int plevel;
int maxcand;

/*:89*//*91:*/
#line 2083 "sat11.w"

float*hmem;
int hmem_alloc_level;
float*heur;

/*:91*//*107:*/
#line 2420 "sat11.w"

arc*cand_arc;
int cand_arc_alloc;
int active;
int settled;

/*:107*//*119:*/
#line 2702 "sat11.w"

ldata*look;
int looks;

/*:119*//*123:*/
#line 2787 "sat11.w"

ullng base,last_base;
uint*forcedlit;
int forcedlits,fl;
int last_change;
int looki;
uint looklit;
uint old_looklit;

/*:123*//*131:*/
#line 2988 "sat11.w"

uint*wstack;
int wptr;
float weighted_new_binaries;

/*:131*//*139:*/
#line 3147 "sat11.w"

float dl_trigger;
uint dl_truth;
int dlooki;
uint dlooklit;
uint dl_last_change;

/*:139*/
#line 109 "sat11.w"
;
/*29:*/
#line 790 "sat11.w"

void print_bimp(int l){
register uint la,ls;
printf(""O"s"O".8s ->",litname(l));
for(la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--)
printf(" "O"s"O".8s",litname(mem[la]));
printf("\n");
}

/*:29*//*30:*/
#line 801 "sat11.w"

void print_timp(int l){
register uint la,ls;
printf(""O"s"O".8s ->",litname(l));
for(la= timp[l].addr,ls= timp[l].size;ls;la++,ls--)
printf(" "O"s"O".8s|"O"s"O".8s",litname(tmem[la].u),litname(tmem[la].v));
printf("\n");
}

void print_full_timp(int l){
register uint la,k;
printf(""O"s"O".8s ->",litname(l));
for(la= timp[l].addr,k= 0;k<timp[l].size;k++)
printf(" "O"s"O".8s|"O"s"O".8s",litname(tmem[la+k].u),litname(tmem[la+k].v));
if(la+k!=timp[l-1].addr){
printf(" #");
for(;la+k<timp[l-1].addr;k++)
printf(" "O"s"O".8s|"O"s"O".8s",litname(tmem[la+k].u),litname(tmem[la+k].v));
}
printf("\n");
}

/*:30*//*31:*/
#line 828 "sat11.w"

void sanity(void){
register int j,k,l,la,ls,los,p,q,u,v;
for(k= 0;k<vars;k++){
if(freevar[freeloc[k+1]]!=k+1)
fprintf(stderr,"freeloc["O"d] is wrong!\n",k+1);
if(freeloc[freevar[k]]!=k)
fprintf(stderr,"freevar["O"d] is wrong!\n",k);
}
for(k= 0;k<rptr;k++){
l= rstack[k];
if(freeloc[thevar(l)]<freevars)
fprintf(stderr,"literal "O"d on rstack is free!\n",l);
}
if(rptr+freevars!=vars)
fprintf(stderr,"rptr="O"d, freevars="O"d, vars="O"lld\n",rptr,freevars,vars);
/*49:*/
#line 1201 "sat11.w"

for(l= 2;l<badlit;l++){
la= bimp[l].addr,k= bimp[l].k;
if(la&((1<<k)-1))
fprintf(stderr,"addr of bimp["O"d] is clobbered (0x"O"x, k="O"d)!\n",
l,la,k);
else if(bimp[l].alloc!=1<<k)
fprintf(stderr,"alloc of bimp["O"d] is clobbered ("O"d, k="O"d)!\n",
l,bimp[l].alloc,k);
else if(bimp[l].size> bimp[l].alloc)
fprintf(stderr,"size of bimp["O"d] is clobbered ("O"d>"O"d)!\n",
l,bimp[l].size,bimp[l].alloc);
else if(la>=1<<memk)
fprintf(stderr,"addr of bimp["O"d] is out of bounds (0x"O"d>0x"O"d)!\n",
l,la,1<<memk);
else if(memfree(la))
fprintf(stderr,"block 0x"O"x of bimp["O"d] isn't reserved!\n",
la,l);
else for(j= bimp[l].size-1;j>=0;j--)if(mem[la+j]<2||mem[la+j]>=badlit)
fprintf(stderr,"literal "O"d in bimp["O"d] is out of bounds!\n",
mem[la+j],l);
}
for(k= 2;k<memk;k++){
for(p= ~mem[avail(k)];;p= ~linkf(p)){
if((p&((1<<k)-1))&&p!=avail(k))
fprintf(stderr,"link in avail("O"d) is clobbered (0x"O"x)!\n",
k,p);
else if(p>=1<<memk)
fprintf(stderr,"link in avail("O"d) is out of bounds (0x"O"d>0x"O"d)!\n",
k,p,1<<memk);
else if(kval(p)!=k)
fprintf(stderr,"kval of 0x"O"x in avail("O"d) is "O"d!\n",
p,k,kval(p));
else if(memfree(p^(1<<k))&&kval(p^(1<<k))==k)
fprintf(stderr,"buddy of 0x"O"x in avail("O"d) is also in that list!\n",
p,k);
else if(~linkf(~linkb(p))!=p)
fprintf(stderr,"linking anomaly at 0x"O"x in avail("O"d)!\n",
p,k);
if(~linkf(p)==avail(k))break;
}
}

/*:49*/
#line 844 "sat11.w"
;
/*32:*/
#line 848 "sat11.w"

for(l= 2;l<badlit;l++){
la= timp[l].addr,ls= timp[l].size,los= timp[l-1].addr-la;
for(k= 0;k<los;k++){
if(tmem[tmem[tmem[la+k].link].link].link!=la+k)
fprintf(stderr,"links clobbered in tmem[0x"O"x]!\n",
la+k);
u= tmem[la+k].u,v= tmem[la+k].v;
if(k<ls){
if(stamp[thevar(l)]<real_truth){
if(stamp[thevar(u)]>=real_truth)
fprintf(stderr,"active timp u for free lit "O"d has assigned lit "O"d!\n",
l,u);
if(stamp[thevar(v)]>=real_truth)
fprintf(stderr,"active timp v for free lit "O"d has assigned lit "O"d!\n",
l,v);
}
}else if(stamp[thevar(u)]<real_truth&&stamp[thevar(v)]<real_truth)
fprintf(stderr,"inactive timp entry for "O"d has unassigned "O"d and "O"d!\n",
l,u,v);
}
}

/*:32*/
#line 845 "sat11.w"
;
}

/*:31*//*33:*/
#line 882 "sat11.w"

void print_state(int lev){
register int k,r;
fprintf(stderr," after "O"lld mems:",mems);
for(k= r= 0;k<lev;k++){
for(;r<nstack[k].rptr;r++)
fprintf(stderr,""O"c",'6'+(rstack[r]&1));
if(nstack[k].branch<0)fprintf(stderr,"|");
else fprintf(stderr,""O"c",'0'+(rstack[r++]&1)+(nstack[k].branch<<1));
for(;r<nstack[k+1].lptr;r++)
fprintf(stderr,""O"c",'4'+(rstack[r]&1));
if(k>=print_state_cutoff){
fprintf(stderr,"...");break;
}
}
fprintf(stderr,"\n");
fflush(stderr);
}

/*:33*//*50:*/
#line 1252 "sat11.w"

void resize(register int l){
register uint a,j,k,kk,n,p,q,r,s;
mems+= 4;
oo,a= bimp[l].addr,n= bimp[l].size,k= bimp[l].k,s= 1<<k,p= a^s;
if((o,memfree(p))&&(o,kval(p)==k))/*51:*/
#line 1266 "sat11.w"

{
/*52:*/
#line 1275 "sat11.w"

q= ~linkb(p),r= ~linkf(p);
oo,linkf(q)= ~r,linkb(r)= ~q;

/*:52*/
#line 1268 "sat11.w"
;
if((a&s)==0)goto finish;
oo,mem[p]= mem[a];
for(j= 1;j<n;j++)oo,mem[p+j]= mem[a+j];
o,bimp[l].addr= p;
}

/*:51*/
#line 1257 "sat11.w"

else/*53:*/
#line 1282 "sat11.w"

{
/*54:*/
#line 1291 "sat11.w"

for(kk= k+1;kk<memk;kk++)
if(o,linkf(avail(kk))!=~avail(kk)){
p= ~linkf(avail(kk));
o;/*52:*/
#line 1275 "sat11.w"

q= ~linkb(p),r= ~linkf(p);
oo,linkf(q)= ~r,linkb(r)= ~q;

/*:52*/
#line 1295 "sat11.w"
;
goto found;
}
if(memk==memk_max){
fprintf(stderr,"Sorry... more memory is needed! (Try option m"O"d.)\n",
memk_max+1);
fprintf(stderr,"Job aborted after "O"llu mems, "O"llu nodes.\n",mems,nodes);
exit(-666);
}
p= 1<<memk;
o,linkf(avail(memk))= linkb(avail(memk))= ~avail(memk);
o,kval(avail(memk))= memk;
bytes+= p*sizeof(uint),memk++;
found:
while(--kk> k)/*55:*/
#line 1311 "sat11.w"

{
o,q= ~linkf(avail(kk)),r= p+(1<<kk);
oo,linkf(avail(kk))= linkb(q)= ~r;
oo,linkb(r)= ~avail(kk),linkf(r)= ~q,kval(r)= kk;
}

/*:55*/
#line 1309 "sat11.w"
;

/*:54*/
#line 1284 "sat11.w"
;
oo,mem[p]= mem[a];
for(j= 1;j<n;j++)oo,mem[p+j]= mem[a+j];
/*56:*/
#line 1321 "sat11.w"

o,q= ~linkf(avail(k));
oo,linkf(avail(k))= linkb(q)= ~a;
oo,linkb(a)= ~avail(k),linkf(a)= ~q,kval(a)= k;

/*:56*/
#line 1287 "sat11.w"
;
o,bimp[l].addr= p;
}

/*:53*/
#line 1258 "sat11.w"
;
finish:o,bimp[l].alloc= s+s,bimp[l].k= k+1;
}

/*:50*//*61:*/
#line 1439 "sat11.w"

void print_truths(uint cs){
register int x;
if(cs>=proto_truth){
switch((cs-proto_truth)>>1){
case 0:fprintf(stderr,"proto_truths or better:");break;
case 1:fprintf(stderr,"near_truths or better:");break;
case 2:fprintf(stderr,"real_truths:");break;
}
}else fprintf(stderr,"truths at least "O"d:",
cs);
for(x= 1;x<=vars;x++)if(stamp[x]>=cs)
fprintf(stderr," "O"s"O".8s",
stamp[x]&1?"~":"",vmem[x].name.ch8);
fprintf(stderr,"\n");
}

void print_proto_truths(void){
print_truths(proto_truth);
}

void print_near_truths(void){
print_truths(near_truth);
}

void print_real_truths(void){
print_truths(real_truth);
}

/*:61*//*93:*/
#line 2106 "sat11.w"

void hscores(float*h,float*hp){
register int j,l,la,ls,u,v;
register float sum,tsum,factor,sqfactor,afactor,pos,neg;
for(sum= 0.0,j= 0;j<freevars;j++){
o,l= poslit(freevar[j]);
o,sum+= h[l]+h[bar(l)];
}
factor= 2.0*freevars/sum;
sqfactor= factor*factor;
afactor= alpha*factor;
for(j= 0;j<freevars;j++){
o,l= poslit(freevar[j]);
/*94:*/
#line 2134 "sat11.w"

for(o,la= bimp[l].addr,ls= bimp[l].size,sum= 0.0;ls;la++,ls--){
o,u= mem[la];
if(isfree(u))o,sum+= h[u];
}
for(o,la= timp[l].addr,ls= timp[l].size,tsum= 0.0;ls;la++,ls--){
o,u= tmem[la].u,v= tmem[la].v;
oo,tsum+= h[u]*h[v];
}
sum= 0.1+sum*afactor+tsum*sqfactor;

/*:94*/
#line 2119 "sat11.w"
;
pos= sum,l++;
/*94:*/
#line 2134 "sat11.w"

for(o,la= bimp[l].addr,ls= bimp[l].size,sum= 0.0;ls;la++,ls--){
o,u= mem[la];
if(isfree(u))o,sum+= h[u];
}
for(o,la= timp[l].addr,ls= timp[l].size,tsum= 0.0;ls;la++,ls--){
o,u= tmem[la].u,v= tmem[la].v;
oo,tsum+= h[u]*h[v];
}
sum= 0.1+sum*afactor+tsum*sqfactor;

/*:94*/
#line 2121 "sat11.w"
;
neg= sum;
if(verbose&show_scores)
fprintf(stderr,"("O".8s: pos "O".2f neg "O".2f r="O".4g)\n",
vmem[l>>1].name.ch8,pos,neg,
(pos<max_score?pos:max_score)*(neg<max_score?neg:max_score));
if(pos> max_score)pos= max_score;
if(neg> max_score)neg= max_score;
o,hp[l-1]= pos,hp[l]= neg;
o,rating[thevar(l)]= pos*neg;
}
}

/*:93*//*152:*/
#line 3371 "sat11.w"

void confusion(char*id){
fprintf(stderr,"This can't happen ("O"s)!\n",id);
exit(-666);
}

void debugstop(int foo){
fprintf(stderr,"You rang("O"d)?\n",foo);
}

/*:152*/
#line 110 "sat11.w"
;
main(int argc,char*argv[]){
register int au,av,aw,h,i,j,jj,k,kk,l,ll,p,pp,q,qq,r,s;
register int c,cc,hh,la,lp,ls,ola,ols,tla,tls,tll,sl,su,sv,sw;
register int t,tt,u,uu,v0,v,vv,w,ww,x,y,xl,pu,aa,ss,pv,ua,va;
/*4:*/
#line 235 "sat11.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,""O"d",&verbose)-1);break;
case'c':k|= (sscanf(argv[j]+1,""O"d",&show_choices_max)-1);break;
case'H':k|= (sscanf(argv[j]+1,""O"d",&print_state_cutoff)-1);break;
case'h':k|= (sscanf(argv[j]+1,""O"d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,""O"d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1);break;
case'd':k|= (sscanf(argv[j]+1,""O"lld",&delta)-1);thresh= delta;break;
case'm':k|= (sscanf(argv[j]+1,""O"d",&memk_max)-1);break;
case'a':k|= (sscanf(argv[j]+1,""O"f",&alpha)-1);break;
case't':k|= (sscanf(argv[j]+1,""O"f",&max_score)-1);break;
case'l':k|= (sscanf(argv[j]+1,""O"d",&hlevel_max)-1);break;
case'p':k|= (sscanf(argv[j]+1,""O"d",&levelcand)-1);break;
case'q':k|= (sscanf(argv[j]+1,""O"d",&mincutoff)-1);break;
case'z':k|= (sscanf(argv[j]+1,""O"d",&max_prelook_arcs)-1);break;
case'i':k|= (sscanf(argv[j]+1,""O"d",&dl_max_iter)-1);break;
case'r':k|= (sscanf(argv[j]+1,""O"f",&dl_rho)-1);break;
case'x':out_name= argv[j]+1,out_file= fopen(out_name,"w");
if(!out_file)
fprintf(stderr,"I can't open file `"O"s' for output!\n",out_name);
break;
case'V':primary_name= argv[j]+1,primary_file= fopen(primary_name,"r");
if(!primary_file)
fprintf(stderr,"I can't open file `"O"s' for input!\n",primary_name);
break;
case'T':k|= (sscanf(argv[j]+1,""O"lld",&timeout)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0||memk_max<2||memk_max> 31||
alpha<=0.0||max_score<=0.0||hlevel_max<3||levelcand<=0||
mincutoff<=0||max_prelook_arcs<=0||dl_max_iter<=0){
fprintf(stderr,
"Usage: "O"s [v<n>] [c<n>] [h<n>] [b<n>] [s<n>] [d<n>] [m<n>]",argv[0]);
fprintf(stderr," [H<n>] [a<f>] [t<f>] [l<n>] [p<n>] [q<n] [z<n>]");
fprintf(stderr," [i<n>] [r<f>] [x<foo>] [V<foo>] [T<n>] < foo.sat\n");
exit(-1);
}

/*:4*/
#line 115 "sat11.w"
;
/*8:*/
#line 344 "sat11.w"

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
#line 522 "sat11.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:15*/
#line 116 "sat11.w"
;
/*9:*/
#line 373 "sat11.w"

if(primary_file)/*10:*/
#line 418 "sat11.w"

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
#line 439 "sat11.w"

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
#line 472 "sat11.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 493 "sat11.w"

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
#line 475 "sat11.w"
;
/*16:*/
#line 526 "sat11.w"

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
#line 477 "sat11.w"
;
if(!non_clause){
/*17:*/
#line 541 "sat11.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 479 "sat11.w"
;
if(clauses&&(p->stamp==clauses||p->stamp==-clauses))
/*18:*/
#line 555 "sat11.w"

{
if((p->stamp> 0)==(i> 0))non_clause= 1;
}

/*:18*/
#line 481 "sat11.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 506 "sat11.w"

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
#line 484 "sat11.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}
}

/*:12*/
#line 450 "sat11.w"
;
}
if(k==0&&!non_clause){
fprintf(stderr,"(Empty line "O"lld is being ignored)\n",clauses);
nullclauses++;
}
if(non_clause)/*19:*/
#line 565 "sat11.w"

{
while(k){
/*20:*/
#line 576 "sat11.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 568 "sat11.w"
;
k--;
}
if(non_clause&&((buf[0]!='~')||(buf[1]!=' ')))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;
}

/*:19*/
#line 456 "sat11.w"

else{
if(k> 3){
fprintf(stderr,
"Sorry: This program accepts unary, binary, and ternary clauses only!");
fprintf(stderr," (line "O"lld)\n",clauses);
exit(-1);
}
if(k==3)ternaries++;
}
cells+= k;

/*:11*/
#line 429 "sat11.w"
;
/*19:*/
#line 565 "sat11.w"

{
while(k){
/*20:*/
#line 576 "sat11.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 568 "sat11.w"
;
k--;
}
if(non_clause&&((buf[0]!='~')||(buf[1]!=' ')))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;
}

/*:19*/
#line 430 "sat11.w"
;
}
cells= nullclauses= 0;
primary_vars= vars;
if(verbose&show_basics)
fprintf(stderr,"("O"d primary variables read from "O"s)\n",
primary_vars,primary_name);
}

/*:10*/
#line 374 "sat11.w"
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
#line 439 "sat11.w"

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
#line 472 "sat11.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 493 "sat11.w"

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
#line 475 "sat11.w"
;
/*16:*/
#line 526 "sat11.w"

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
#line 477 "sat11.w"
;
if(!non_clause){
/*17:*/
#line 541 "sat11.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 479 "sat11.w"
;
if(clauses&&(p->stamp==clauses||p->stamp==-clauses))
/*18:*/
#line 555 "sat11.w"

{
if((p->stamp> 0)==(i> 0))non_clause= 1;
}

/*:18*/
#line 481 "sat11.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 506 "sat11.w"

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
#line 484 "sat11.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}
}

/*:12*/
#line 450 "sat11.w"
;
}
if(k==0&&!non_clause){
fprintf(stderr,"(Empty line "O"lld is being ignored)\n",clauses);
nullclauses++;
}
if(non_clause)/*19:*/
#line 565 "sat11.w"

{
while(k){
/*20:*/
#line 576 "sat11.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 568 "sat11.w"
;
k--;
}
if(non_clause&&((buf[0]!='~')||(buf[1]!=' ')))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;
}

/*:19*/
#line 456 "sat11.w"

else{
if(k> 3){
fprintf(stderr,
"Sorry: This program accepts unary, binary, and ternary clauses only!");
fprintf(stderr," (line "O"lld)\n",clauses);
exit(-1);
}
if(k==3)ternaries++;
}
cells+= k;

/*:11*/
#line 385 "sat11.w"
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
#line 117 "sat11.w"
;
if(verbose&show_basics)
/*22:*/
#line 597 "sat11.w"

fprintf(stderr,
"("O"lld variables, "O"lld clauses, "O"llu literals successfully read)\n",
vars,clauses,cells);

/*:22*/
#line 119 "sat11.w"
;
/*37:*/
#line 939 "sat11.w"

lits= vars<<1,badlit= lits+2;
last_vchunk= cur_vchunk;
/*38:*/
#line 951 "sat11.w"

stamp= (uint*)malloc((vars+1)*sizeof(uint));
if(!stamp){
fprintf(stderr,"Oops, I can't allocate the stamp array!\n");
exit(-10);
}
bytes+= (vars+1)*sizeof(uint);

bimp= (bdata*)malloc(badlit*sizeof(bdata));
if(!bimp){
fprintf(stderr,"Oops, I can't allocate the bimp array!\n");
exit(-10);
}
bytes+= badlit*sizeof(bdata);
/*57:*/
#line 1330 "sat11.w"

for(memk= 4;1<<memk<4*(memk_max-2+lits);memk++);
if(memk> memk_max){
fprintf(stderr,"The value of memk_max is way too small for "O"d literals!\n",
lits);
exit(-667);
}
mem= (uint*)malloc((1<<memk_max)*sizeof(uint));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the mem array!\n");
exit(-10);
}
bytes+= (1<<memk)*sizeof(uint);
j= avail(memk_max);
for(l= 2;l<badlit;l++){
oo,mem[j]= 0,bimp[l].addr= j,bimp[l].size= 0,j+= 4;
o,bimp[l].alloc= 4,bimp[l].k= 2;
}
for(k= 2;k<memk;k++){
if(j&(1<<k)){
o,linkf(avail(k))= linkb(avail(k))= ~j;
o,linkf(j)= linkb(j)= ~avail(k);
oo,kval(avail(k))= kval(j)= k;
j+= 1<<k;
}else{
o,linkf(avail(k))= linkb(avail(k))= ~avail(k);
o,kval(avail(k))= k;
}
}

/*:57*/
#line 965 "sat11.w"
;

timp= (tdata*)malloc(badlit*sizeof(tdata));
if(!timp){
fprintf(stderr,"Oops, I can't allocate the timp array!\n");
exit(-10);
}
bytes+= badlit*sizeof(tdata);
tmem= (tpair*)malloc(3*ternaries*sizeof(tpair));
if(!tmem){
fprintf(stderr,"Oops, I can't allocate the tmem array!\n");
exit(-10);
}
bytes+= 3*ternaries*sizeof(tpair);

freevar= (uint*)malloc(vars*sizeof(uint));
if(!freevar){
fprintf(stderr,"Oops, I can't allocate the freevar array!\n");
exit(-10);
}
bytes+= vars*sizeof(uint);
freeloc= (uint*)malloc((vars+1)*sizeof(uint));
if(!freeloc){
fprintf(stderr,"Oops, I can't allocate the freeloc array!\n");
exit(-10);
}
bytes+= (vars+1)*sizeof(uint);
for(k= 0;k<vars;k++){
mems+= 4,j= gb_unif_rand(k+1);
if(j!=k){
o,i= freevar[j];
oo,freevar[k]= i,freeloc[i]= k;
oo,freevar[j]= k+1,freeloc[k+1]= j;
}else oo,freevar[k]= k+1,freeloc[k+1]= k;
}
freevars= vars;

/*:38*//*39:*/
#line 1006 "sat11.w"

rstack= (uint*)malloc((vars+1)*sizeof(uint));
if(!rstack){
fprintf(stderr,"Oops, I can't allocate the rstack array!\n");
exit(-10);
}
bytes+= (vars+1)*sizeof(uint);

nstack= (ndata*)malloc((vars+1)*sizeof(ndata));
if(!nstack){
fprintf(stderr,"Oops, I can't allocate the nstack array!\n");
exit(-10);
}
bytes+= (vars+1)*sizeof(ndata);

lmem= (literal*)malloc(badlit*sizeof(literal));
if(!lmem){
fprintf(stderr,"Oops, I can't allocate the lmem array!\n");
exit(-10);
}
bytes+= badlit*sizeof(literal);
for(l= 2;l<badlit;l++)oo,lmem[l].dl_fail= lmem[l].bstamp= lmem[l].istamp= 0;

vmem= (variable*)malloc((vars+1)*sizeof(variable));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-10);
}
bytes+= (vars+1)*sizeof(variable);

forcedlit= (uint*)malloc(vars*sizeof(uint));
if(!forcedlit){
fprintf(stderr,"Oops, I can't allocate the forcedlit array!\n");
exit(-10);
}
bytes+= vars*sizeof(uint);

/*:39*/
#line 942 "sat11.w"
;
/*46:*/
#line 1143 "sat11.w"

for(c= vars;c;c--){
/*21:*/
#line 588 "sat11.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:21*/
#line 1145 "sat11.w"
;
o,vmem[c].name.lng= cur_tmp_var->name.lng;
o,vmem[c].len= vars+1;
}

/*:46*/
#line 943 "sat11.w"
;
/*40:*/
#line 1044 "sat11.w"

forcedlits= 0;
for(l= 2;l<badlit;l++)o,timp[l].addr= timp[l].size= 0;
for(c= clauses,k= 0;c;c--){
/*41:*/
#line 1059 "sat11.w"

for(i= j= 0;i<2;){
/*20:*/
#line 576 "sat11.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 1061 "sat11.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1);
rstack[j++]= p+2;

}
u= rstack[0],v= rstack[1],w= rstack[2];
if(out_file){
for(jj= 0;jj<j;jj++)fprintf(out_file," "O"s"O".8s",litname(rstack[jj]));
fprintf(out_file,"\n");
}
if(j==1)/*42:*/
#line 1082 "sat11.w"

{
if(o,timp[u].addr==0){
if(o,timp[bar(u)].addr){
if(verbose&show_choices)
fprintf(stderr,"Unary clause "O"d contradicts unary clause "O"d\n",
c,timp[bar(u)].addr);
goto unsat;
}
o,timp[u].addr= c;
o,forcedlit[forcedlits++]= u;
}
}

/*:42*/
#line 1073 "sat11.w"

else if(j==2)/*43:*/
#line 1096 "sat11.w"

{
o,la= bimp[bar(u)].addr,ls= bimp[bar(u)].size;
if(o,ls==bimp[bar(u)].alloc)
resize(bar(u)),o,la= bimp[bar(u)].addr;
oo,mem[la+ls]= v,bimp[bar(u)].size= ls+1;
o,la= bimp[bar(v)].addr,ls= bimp[bar(v)].size;
if(o,ls==bimp[bar(v)].alloc)
resize(bar(v)),o,la= bimp[bar(v)].addr;
oo,mem[la+ls]= u,bimp[bar(v)].size= ls+1;
}

/*:43*/
#line 1074 "sat11.w"

else/*44:*/
#line 1111 "sat11.w"

{
oo,timp[bar(u)].size++;
oo,timp[bar(v)].size++;
oo,timp[bar(w)].size++;
ooo,tmem[k].spare= u,tmem[k+1].spare= v,tmem[k+2].spare= w;
k+= 3;
}

/*:44*/
#line 1075 "sat11.w"
;

/*:41*/
#line 1048 "sat11.w"
;
}
/*45:*/
#line 1120 "sat11.w"

for(j= 0,l= badlit-1;l>=2;l--){
oo,timp[l].addr= j,j+= timp[l].size,timp[l].size= 0;
}
o,timp[l].addr= j;
if(k!=j||k!=3*ternaries)confusion("ternaries");
while(k){
k-= 3;
ooo,u= tmem[k].spare,v= tmem[k+1].spare,w= tmem[k+2].spare;
o,la= timp[bar(u)].addr,ls= timp[bar(u)].size,uu= la+ls;
o,timp[bar(u)].size= ls+1;
o,tmem[uu].u= v,tmem[uu].v= w;
o,la= timp[bar(v)].addr,ls= timp[bar(v)].size,vv= la+ls;
o,tmem[uu].link= vv;
o,timp[bar(v)].size= ls+1;
o,tmem[vv].u= w,tmem[vv].v= u;
o,la= timp[bar(w)].addr,ls= timp[bar(w)].size,ww= la+ls;
o,tmem[vv].link= ww;
o,timp[bar(w)].size= ls+1;
o,tmem[ww].u= u,tmem[ww].v= v;
o,tmem[ww].link= uu;
}

/*:45*/
#line 1050 "sat11.w"
;
if(out_file)fflush(out_file);

/*:40*/
#line 944 "sat11.w"
;
/*47:*/
#line 1153 "sat11.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL)confusion("consistency");
free(cur_chunk);
for(cur_vchunk= last_vchunk;cur_vchunk;cur_vchunk= last_vchunk){
last_vchunk= cur_vchunk->prev;
free(cur_vchunk);
}

/*:47*/
#line 945 "sat11.w"
;
/*58:*/
#line 1365 "sat11.w"

istack= (idata*)malloc((1<<memk_max)*sizeof(idata));
if(!istack){
fprintf(stderr,"Oops, I can't allocate the istack array!\n");
exit(-10);
}
bytes+= (1<<memk)*sizeof(idata);
iptr_max= 1<<memk;

/*:58*//*90:*/
#line 2032 "sat11.w"

cand= (cdata*)malloc(vars*sizeof(cdata));
if(!cand){
fprintf(stderr,"Oops, I can't allocate the cand array!\n");
exit(-10);
}
bytes+= vars*sizeof(cdata);
rating= (float*)malloc((vars+1)*sizeof(float));
if(!rating){
fprintf(stderr,"Oops, I can't allocate the rating array!\n");
exit(-10);
}
bytes+= (vars+1)*sizeof(float);

/*:90*//*92:*/
#line 2088 "sat11.w"

hmem= (float*)malloc(lits*(hlevel_max+1)*sizeof(float));
if(!hmem){
fprintf(stderr,"Oops, I can't allocate the hmem array!\n");
exit(-10);
}
hmem_alloc_level= 2;
bytes+= lits*3*sizeof(float);
for(k= 0;k<lits;k++)o,hmem[k]= 1.0;

/*:92*//*108:*/
#line 2428 "sat11.w"

max_prelook_arcs&= -2;
cand_arc= (arc*)malloc(max_prelook_arcs*sizeof(arc));
if(!cand_arc){
fprintf(stderr,"Oops, I can't allocate the cand_arc array!\n");
exit(-10);
}

/*:108*//*120:*/
#line 2706 "sat11.w"

look= (ldata*)malloc(lits*sizeof(ldata));
if(!look){
fprintf(stderr,"Oops, I can't allocate the look array!\n");
exit(-10);
}
bytes+= lits*sizeof(ldata);

/*:120*//*132:*/
#line 2993 "sat11.w"

wstack= (uint*)malloc(lits*sizeof(uint));
if(!wstack){
fprintf(stderr,"Oops, I can't allocate the wstack array!\n");
exit(-10);
}
bytes+= lits*sizeof(uint);

/*:132*/
#line 946 "sat11.w"
;

/*:37*/
#line 120 "sat11.w"
;
imems= mems,mems= 0;
/*150:*/
#line 3342 "sat11.w"

level= 0;
if(forcedlits){
o,nstack[0].branch= -1;
goto special_start;
}
enter_level:if(sanity_checking)sanity();
/*59:*/
#line 1385 "sat11.w"

nstack[level].lptr= rptr,nodes++;
if(delta&&(mems>=thresh))thresh+= delta,print_state(level);
if(mems> timeout){
fprintf(stderr,"TIMEOUT!\n");
goto done;
}
o,nstack[level].branch= -1,plevel= level;
/*122:*/
#line 2758 "sat11.w"

/*87:*/
#line 2007 "sat11.w"

if(freevars==0)goto satisfied;
/*96:*/
#line 2190 "sat11.w"

/*95:*/
#line 2149 "sat11.w"

if(level<=1){
hscores(htable(0),htable(1));
hscores(htable(1),htable(2));
hscores(htable(2),htable(1));
hscores(htable(1),htable(2));
hscores(htable(2),htable(1));
heur= htable(1);
}else if(level<hlevel_max){
if(level> hmem_alloc_level)hmem_alloc_level++,bytes+= lits*sizeof(float);
hscores(htable(level-1),htable(level));
heur= htable(level);
}else{
if(hlevel_max> hmem_alloc_level)hmem_alloc_level++,bytes+= lits*sizeof(float);
hscores(htable(hlevel_max-1),htable(hlevel_max));

heur= htable(hlevel_max);
}

/*:95*/
#line 2191 "sat11.w"
;
maxcand= (level==0?freevars:levelcand/level);
if(maxcand<mincutoff)maxcand= mincutoff;
/*97:*/
#line 2211 "sat11.w"

no_newbies= (plevel> 0);
init_cand:for(cands= k= 0,sum= 0.0;k<freevars;k++){
o,x= freevar[k];
o,stamp[x]= 0;
if(no_newbies){
if(x> primary_vars)continue;
o,t= vmem[x].pfx,l= vmem[x].len;
if(l==plevel){
if(t!=prefix)continue;
}else if(l> plevel)continue;
else if(t!=(l<32?prefix&-(uint)(1LL<<(32-l)):prefix))continue;
}
oo,cand[cands].var= x,cand[cands].rating= rating[x];
cands++,sum+= rating[x];
}
if(cands==0){
/*98:*/
#line 2233 "sat11.w"

for(j= 0;j<freevars;j++){
o,x= freevar[j];
l= poslit(x);
/*99:*/
#line 2244 "sat11.w"

if(o,timp[l].size)goto nogood;
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,u= mem[la];
if(o,stamp[thevar(u)]!=real_truth+(u&1))goto nogood;
}

/*:99*/
#line 2237 "sat11.w"
;
l++;
/*99:*/
#line 2244 "sat11.w"

if(o,timp[l].size)goto nogood;
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,u= mem[la];
if(o,stamp[thevar(u)]!=real_truth+(u&1))goto nogood;
}

/*:99*/
#line 2239 "sat11.w"
;
}
goto satisfied;
nogood:

/*:98*/
#line 2228 "sat11.w"
;
no_newbies= 0;
goto init_cand;
}

/*:97*/
#line 2194 "sat11.w"
;
/*100:*/
#line 2255 "sat11.w"

for(k= 1;cands>=2*maxcand&&k;){
register float mean= 0.9999*sum/(double)cands;
for(j= k= 0,sum= 0.0;j<cands;){
if(o,cand[j].rating>=mean)sum+= cand[j].rating,j++;
else oo,k= 1,cand[j]= cand[--cands];
}
}
if(cands> maxcand)/*101:*/
#line 2273 "sat11.w"

{
j= cands>>1;
while(j> 0){
j--;
/*102:*/
#line 2287 "sat11.w"

{
register float r;
cdata c;
o,c= cand[j],r= c.rating;
for(i= j,jj= (j<<1)+1;jj<cands;i= jj,jj= (jj<<1)+1){
if(jj+1<cands&&(o,cand[jj+1].rating<cand[jj].rating))jj++;
if(o,r<=cand[jj].rating)break;
o,cand[i]= cand[jj];
}
if(i> j)o,cand[i]= c;
}

/*:102*/
#line 2278 "sat11.w"
;
}
while(1){
oo,cand[0]= cand[--cands];
if(cands==maxcand)break;
/*102:*/
#line 2287 "sat11.w"

{
register float r;
cdata c;
o,c= cand[j],r= c.rating;
for(i= j,jj= (j<<1)+1;jj<cands;i= jj,jj= (jj<<1)+1){
if(jj+1<cands&&(o,cand[jj+1].rating<cand[jj].rating))jj++;
if(o,r<=cand[jj].rating)break;
o,cand[i]= cand[jj];
}
if(i> j)o,cand[i]= c;
}

/*:102*/
#line 2283 "sat11.w"
;
}
}

/*:101*/
#line 2263 "sat11.w"
;
if(cands==0)confusion("cands");

/*:100*/
#line 2195 "sat11.w"
;

/*:96*/
#line 2009 "sat11.w"
;
/*103:*/
#line 2357 "sat11.w"

/*105:*/
#line 2398 "sat11.w"

/*66:*/
#line 1540 "sat11.w"

if(++bstamp==0){
bstamp= 1;
for(l= 2;l<badlit;l++)o,lmem[l].bstamp= 0;
}

/*:66*/
#line 2399 "sat11.w"
;
for(i= 0;i<cands;i++){
o,l= poslit(cand[i].var);
oo,lmem[l].rank= 0,lmem[l].arcs= -1,lmem[l].bstamp= bstamp;
oo,lmem[l+1].rank= 0,lmem[l+1].arcs= -1,lmem[l+1].bstamp= bstamp;
}
/*109:*/
#line 2436 "sat11.w"

for(j= i= 0;i<cands;i++){
o,l= poslit(cand[i].var);
/*110:*/
#line 2451 "sat11.w"

for(oo,la= bimp[l].addr,ls= bimp[l].size,p= lmem[bar(l)].arcs;ls;la++,ls--){
o,u= mem[la];
if(u<l)continue;
if(o,lmem[u].bstamp!=bstamp)continue;

o,cand_arc[j].tip= bar(u),cand_arc[j].next= p,p= j;

oo,cand_arc[j+1].tip= l,cand_arc[j+1].next= lmem[u].arcs;
o,lmem[u].arcs= j+1,j+= 2;
if(j==max_prelook_arcs){
if(verbose&show_details)
fprintf(stderr,"prelook arcs cut off at "O"d; see option z\n",
max_prelook_arcs);
o,lmem[bar(l)].arcs= lmem[bar(l)].untagged= p;
goto arcs_done;
}
}
o,lmem[bar(l)].arcs= lmem[bar(l)].untagged= p;

/*:110*/
#line 2439 "sat11.w"
;
l++;
/*110:*/
#line 2451 "sat11.w"

for(oo,la= bimp[l].addr,ls= bimp[l].size,p= lmem[bar(l)].arcs;ls;la++,ls--){
o,u= mem[la];
if(u<l)continue;
if(o,lmem[u].bstamp!=bstamp)continue;

o,cand_arc[j].tip= bar(u),cand_arc[j].next= p,p= j;

oo,cand_arc[j+1].tip= l,cand_arc[j+1].next= lmem[u].arcs;
o,lmem[u].arcs= j+1,j+= 2;
if(j==max_prelook_arcs){
if(verbose&show_details)
fprintf(stderr,"prelook arcs cut off at "O"d; see option z\n",
max_prelook_arcs);
o,lmem[bar(l)].arcs= lmem[bar(l)].untagged= p;
goto arcs_done;
}
}
o,lmem[bar(l)].arcs= lmem[bar(l)].untagged= p;

/*:110*/
#line 2441 "sat11.w"
;
}
arcs_done:if(j> cand_arc_alloc)
bytes+= (j-cand_arc_alloc)*sizeof(arc),cand_arc_alloc= j;

/*:109*/
#line 2405 "sat11.w"
;
for(i= 0;i<cands;i++){
o,l= poslit(cand[i].var);
oo,lmem[l].untagged= lmem[l].arcs;
oo,lmem[l+1].untagged= lmem[l+1].arcs;
}
k= 0;
active= settled= 0;

/*:105*/
#line 2358 "sat11.w"
;
for(i= 0;i<cands;i++){
o,l= poslit(cand[i].var);
check_rank:if(o,lmem[l].rank==0)
/*111:*/
#line 2471 "sat11.w"

{
v= l;
o,lmem[l].parent= 0;
/*112:*/
#line 2481 "sat11.w"

o,lmem[v].rank= ++k;
lmem[v].link= active,active= v;
o,lmem[v].min= v;

/*:112*/
#line 2475 "sat11.w"
;
do/*113:*/
#line 2489 "sat11.w"

{
o,vv= lmem[v].untagged,ll= lmem[v].min;
if(vv>=0){
o,u= cand_arc[vv].tip,vv= cand_arc[vv].next;
o,lmem[v].untagged= vv;
o,j= lmem[u].rank;
if(j){
if(o,j<lmem[ll].rank)lmem[v].min= u;

}else{
lmem[u].parent= v;
v= u;
/*112:*/
#line 2481 "sat11.w"

o,lmem[v].rank= ++k;
lmem[v].link= active,active= v;
o,lmem[v].min= v;

/*:112*/
#line 2502 "sat11.w"
;
}
}else{
o,u= lmem[v].parent;
if(v==ll)
/*114:*/
#line 2526 "sat11.w"

{
float r,rr;
t= active;
o,r= rating[thevar(v)],w= v;
o,active= lmem[v].link;
o,lmem[v].rank= infty;
lmem[v].link= settled,settled= t;

while(t!=v){
if(t==bar(v)){
if(verbose&show_gory_details)
fprintf(stderr,"the binary clauses are inconsistent\n");
goto look_bad;
}
o,lmem[t].rank= infty;
o,lmem[t].parent= v;
o,rr= rating[thevar(t)];
if(rr> r)r= rr,w= t;
o,t= lmem[t].link;
}
o,lmem[v].parent= v,lmem[v].vcomp= w;
if(o,lmem[bar(v)].rank==infty)
oo,lmem[v].vcomp= bar(lmem[lmem[bar(v)].parent].vcomp);
}

/*:114*/
#line 2508 "sat11.w"

else{
if(ooo,lmem[ll].rank<lmem[lmem[u].min].rank)
o,lmem[u].min= ll;
}
v= u;
}
}

/*:113*/
#line 2477 "sat11.w"

while(v> 0);
}

/*:111*/
#line 2363 "sat11.w"
;
if((l&1)==0){
l++;goto check_rank;
}
}
if(verbose&show_strong_comps)/*104:*/
#line 2370 "sat11.w"

{
fprintf(stderr,"Strong components:\n");
for(l= settled;l;l= lmem[l].link){
fprintf(stderr," "O"s"O".8s ",
litname(l));
if(lmem[l].parent!=l)fprintf(stderr,"with "O"s"O".8s\n",
litname(lmem[l].parent));
else{
if(lmem[l].vcomp!=l)fprintf(stderr,"-> "O"s"O".8s ",
litname(lmem[l].vcomp));
fprintf(stderr,""O".4g\n",
rating[thevar(lmem[l].vcomp)]);
}
}
}

/*:104*/
#line 2368 "sat11.w"
;

/*:103*/
#line 2010 "sat11.w"
;
/*116:*/
#line 2644 "sat11.w"

/*117:*/
#line 2663 "sat11.w"

o,lmem[root].child= 0,lmem[root].height= -1,pp= root;
for(u= settled;u;u= uu){
oo,uu= lmem[u].link,p= lmem[u].parent;
if(p!=pp)h= 0,w= root,pp= p;

for(o,j= lmem[bar(u)].arcs;j>=0;j= cand_arc[j].next){
o,v= bar(cand_arc[j].tip);
o,vv= lmem[v].parent;
if(vv==p)continue;
o,hh= lmem[vv].height;
if(hh>=h)h= hh+1,w= vv;
}
if(p==u){
o,v= lmem[w].child;
oo,lmem[u].height= h,lmem[u].child= 0,lmem[u].link= v;
o,lmem[w].child= u;
}
}

/*:117*/
#line 2645 "sat11.w"
;
/*121:*/
#line 2722 "sat11.w"

o,u= lmem[root].child,j= k= v= 0;
while(1){
oo,look[k].lit= lmem[u].vcomp;
o,lmem[u].rank= k++;
if(o,lmem[u].child){
o,lmem[u].parent= v;
v= u,u= lmem[u].child;
}else{
post:o,i= lmem[u].rank;
o,look[i].offset= j,j+= 2;
if(v)oo,lmem[u].parent= lmem[v].vcomp;
else o,lmem[u].parent= 0;
if(o,lmem[u].link)u= lmem[u].link;
else if(v){
o,u= v,v= lmem[u].parent;
goto post;
}else break;
}
}
looks= k;
if(j!=k+k)confusion("looks");

/*:121*/
#line 2646 "sat11.w"
;

/*:116*/
#line 2011 "sat11.w"
;

/*:87*/
#line 2759 "sat11.w"
;
if(verbose&show_looks){
fprintf(stderr,"Looks at level "O"d:\n",
level);
for(i= 0;i<looks;i++)
fprintf(stderr," "O"s"O".8s "O"d\n",
litname(look[i].lit),look[i].offset);
}
fl= forcedlits,last_change= -1;
base= 2;
while(1){
for(looki= 0;looki<looks;looki++){
if(looki==last_change)goto look_done;
o,l= look[looki].lit,cs= base+look[looki].offset;
/*125:*/
#line 2852 "sat11.w"

looklit= l;
o,ll= lmem[looklit].parent;
if(ll)oo,lmem[looklit].wnb= lmem[ll].wnb;
else o,lmem[l].wnb= 0.0;
if(verbose&show_gory_details)
fprintf(stderr,"looking at "O"s"O".8s ("O"d)\n",
litname(looklit),cs);
if(isfixed(l)){
if(iscontrary(l)&&stamp[thevar(l)]<proto_truth)
/*128:*/
#line 2941 "sat11.w"

{
looklit= bar(looklit);
forcedlit[forcedlits++]= looklit;
look_cs= cs,cs= proto_truth;
/*130:*/
#line 2976 "sat11.w"

wptr= 0;fptr= eptr= rptr;
weighted_new_binaries= 0;
l= looklit;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 2980 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*133:*/
#line 3002 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_gory_details)
fprintf(stderr,"  looking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*134:*/
#line 3013 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto contra;
}else{
l= v;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3021 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3029 "sat11.w"
;
}
}else weighted_new_binaries+= heur[u]*heur[v];
}

/*:134*/
#line 3008 "sat11.w"
;
}

/*:133*/
#line 2984 "sat11.w"
;
}
/*135:*/
#line 3036 "sat11.w"

if(wptr){
oo,sl= bimp[looklit].size,ls= bimp[looklit].alloc;
/*136:*/
#line 3056 "sat11.w"

if(o,lmem[looklit].istamp!=istamp){
o,lmem[looklit].istamp= istamp;
o,istack[iptr].lit= looklit,istack[iptr].size= sl;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 3060 "sat11.w"
;
}

/*:136*/
#line 3039 "sat11.w"
;
while(sl+wptr> ls)resize(looklit),ls<<= 1;
o,bimp[looklit].size= sl+wptr;
for(o,la= bimp[looklit].addr+sl;wptr;wptr--){
o,u= wstack[wptr-1];
o,mem[la++]= u;
if(verbose&show_gory_details)
fprintf(stderr," windfall "O"s"O".8s->"O"s"O".8s\n",
litname(looklit),litname(u));
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 3049 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
o,mem[au+su]= bar(looklit);
o,bimp[bar(u)].size= su+1;
}
}

/*:135*/
#line 2986 "sat11.w"
;

/*:130*/
#line 2946 "sat11.w"
;
cs= look_cs;
}

/*:128*/
#line 2862 "sat11.w"
;
}else{
/*130:*/
#line 2976 "sat11.w"

wptr= 0;fptr= eptr= rptr;
weighted_new_binaries= 0;
l= looklit;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 2980 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*133:*/
#line 3002 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_gory_details)
fprintf(stderr,"  looking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*134:*/
#line 3013 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto contra;
}else{
l= v;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3021 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3029 "sat11.w"
;
}
}else weighted_new_binaries+= heur[u]*heur[v];
}

/*:134*/
#line 3008 "sat11.w"
;
}

/*:133*/
#line 2984 "sat11.w"
;
}
/*135:*/
#line 3036 "sat11.w"

if(wptr){
oo,sl= bimp[looklit].size,ls= bimp[looklit].alloc;
/*136:*/
#line 3056 "sat11.w"

if(o,lmem[looklit].istamp!=istamp){
o,lmem[looklit].istamp= istamp;
o,istack[iptr].lit= looklit,istack[iptr].size= sl;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 3060 "sat11.w"
;
}

/*:136*/
#line 3039 "sat11.w"
;
while(sl+wptr> ls)resize(looklit),ls<<= 1;
o,bimp[looklit].size= sl+wptr;
for(o,la= bimp[looklit].addr+sl;wptr;wptr--){
o,u= wstack[wptr-1];
o,mem[la++]= u;
if(verbose&show_gory_details)
fprintf(stderr," windfall "O"s"O".8s->"O"s"O".8s\n",
litname(looklit),litname(u));
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 3049 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
o,mem[au+su]= bar(looklit);
o,bimp[bar(u)].size= su+1;
}
}

/*:135*/
#line 2986 "sat11.w"
;

/*:130*/
#line 2865 "sat11.w"
;
if(weighted_new_binaries==0)/*126:*/
#line 2891 "sat11.w"

{
if(lmem[looklit].wnb==0){
if(verbose&show_gory_details)
fprintf(stderr," autarky at "O"s"O".8s\n",
litname(looklit));
looklit= bar(looklit);
/*128:*/
#line 2941 "sat11.w"

{
looklit= bar(looklit);
forcedlit[forcedlits++]= looklit;
look_cs= cs,cs= proto_truth;
/*130:*/
#line 2976 "sat11.w"

wptr= 0;fptr= eptr= rptr;
weighted_new_binaries= 0;
l= looklit;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 2980 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*133:*/
#line 3002 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_gory_details)
fprintf(stderr,"  looking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*134:*/
#line 3013 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto contra;
}else{
l= v;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3021 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3029 "sat11.w"
;
}
}else weighted_new_binaries+= heur[u]*heur[v];
}

/*:134*/
#line 3008 "sat11.w"
;
}

/*:133*/
#line 2984 "sat11.w"
;
}
/*135:*/
#line 3036 "sat11.w"

if(wptr){
oo,sl= bimp[looklit].size,ls= bimp[looklit].alloc;
/*136:*/
#line 3056 "sat11.w"

if(o,lmem[looklit].istamp!=istamp){
o,lmem[looklit].istamp= istamp;
o,istack[iptr].lit= looklit,istack[iptr].size= sl;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 3060 "sat11.w"
;
}

/*:136*/
#line 3039 "sat11.w"
;
while(sl+wptr> ls)resize(looklit),ls<<= 1;
o,bimp[looklit].size= sl+wptr;
for(o,la= bimp[looklit].addr+sl;wptr;wptr--){
o,u= wstack[wptr-1];
o,mem[la++]= u;
if(verbose&show_gory_details)
fprintf(stderr," windfall "O"s"O".8s->"O"s"O".8s\n",
litname(looklit),litname(u));
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 3049 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
o,mem[au+su]= bar(looklit);
o,bimp[bar(u)].size= su+1;
}
}

/*:135*/
#line 2986 "sat11.w"
;

/*:130*/
#line 2946 "sat11.w"
;
cs= look_cs;
}

/*:128*/
#line 2898 "sat11.w"
;
}else{
ll= lmem[looklit].parent;
if(verbose&show_gory_details)
fprintf(stderr," autarky "O"s"O".8s -> "O"s"O".8s\n",
litname(ll),litname(looklit));
/*127:*/
#line 2926 "sat11.w"

{
u= bar(ll);
o,au= bimp[ll].addr,su= bimp[ll].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 2930 "sat11.w"
;
if(o,su==bimp[ll].alloc)resize(ll),o,au= bimp[ll].addr;
oo,mem[au+su]= looklit,bimp[ll].size= su+1;
u= looklit;
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 2935 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
oo,mem[au+su]= bar(ll),bimp[bar(u)].size= su+1;
oo,stamp[thevar(looklit)]= stamp[thevar(ll)]^((looklit^ll)&1);
}

/*:127*/
#line 2904 "sat11.w"
;
}
}

/*:126*/
#line 2866 "sat11.w"

else o,lmem[looklit].wnb+= weighted_new_binaries;
/*140:*/
#line 3154 "sat11.w"

if(level&&(o,lmem[looklit].dl_fail!=istamp)){
if(lmem[looklit].wnb> dl_trigger){
if(cs+2*looks*((ullng)dl_max_iter+1)<proto_truth){
/*141:*/
#line 3189 "sat11.w"

last_base= cs+2*looks*dl_max_iter;
dl_truth= last_base+cs-base;
base= cs;
cs= dl_truth,l= looklit;
wptr= 0;eptr= rptr;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3196 "sat11.w"
;
/*142:*/
#line 3206 "sat11.w"

dl_last_change= 0;
while(1){
for(dlooki= 0;dlooki<looks;dlooki++){
o,l= look[dlooki].lit,cs= base+look[dlooki].offset;
if(l==dl_last_change)goto dlook_done;
/*144:*/
#line 3256 "sat11.w"

dlooklit= l;
if(verbose&show_doubly_gory_details)
fprintf(stderr,"dlooking at "O"s"O".8s ("O"d)\n",
litname(dlooklit),cs);
if(isfixed(l)){
if(stamp[thevar(l)]<dl_truth&&iscontrary(l))
/*145:*/
#line 3272 "sat11.w"

{
dl_last_change= dlooklit;
dlooklit= bar(dlooklit);
dlook_cs= cs,cs= dl_truth;
/*147:*/
#line 3300 "sat11.w"

fptr= eptr= rptr;
l= dlooklit;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3303 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*148:*/
#line 3311 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_doubly_gory_details)
fprintf(stderr,"  dlooking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*149:*/
#line 3321 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto dl_contra;
}else{
l= v;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3328 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3335 "sat11.w"
;
}
}
}

/*:149*/
#line 3318 "sat11.w"
;
}

/*:148*/
#line 3307 "sat11.w"
;
}

/*:147*/
#line 3277 "sat11.w"
;
cs= dlook_cs;
wstack[wptr++]= dlooklit;
}

/*:145*/
#line 3263 "sat11.w"
;
}else{
/*147:*/
#line 3300 "sat11.w"

fptr= eptr= rptr;
l= dlooklit;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3303 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*148:*/
#line 3311 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_doubly_gory_details)
fprintf(stderr,"  dlooking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*149:*/
#line 3321 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto dl_contra;
}else{
l= v;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3328 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3335 "sat11.w"
;
}
}
}

/*:149*/
#line 3318 "sat11.w"
;
}

/*:148*/
#line 3307 "sat11.w"
;
}

/*:147*/
#line 3266 "sat11.w"
;
}

/*:144*/
#line 3213 "sat11.w"
;
dlook_on:continue;
}
if(dl_last_change==0)break;
base+= 2*looks;
if(base==last_base)break;
}
dlook_done:base= last_base,cs= dl_truth;

/*:142*/
#line 3198 "sat11.w"
;
/*135:*/
#line 3036 "sat11.w"

if(wptr){
oo,sl= bimp[looklit].size,ls= bimp[looklit].alloc;
/*136:*/
#line 3056 "sat11.w"

if(o,lmem[looklit].istamp!=istamp){
o,lmem[looklit].istamp= istamp;
o,istack[iptr].lit= looklit,istack[iptr].size= sl;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 3060 "sat11.w"
;
}

/*:136*/
#line 3039 "sat11.w"
;
while(sl+wptr> ls)resize(looklit),ls<<= 1;
o,bimp[looklit].size= sl+wptr;
for(o,la= bimp[looklit].addr+sl;wptr;wptr--){
o,u= wstack[wptr-1];
o,mem[la++]= u;
if(verbose&show_gory_details)
fprintf(stderr," windfall "O"s"O".8s->"O"s"O".8s\n",
litname(looklit),litname(u));
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 3049 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
o,mem[au+su]= bar(looklit);
o,bimp[bar(u)].size= su+1;
}
}

/*:135*/
#line 3199 "sat11.w"
;

/*:141*/
#line 3159 "sat11.w"
;
o,dl_trigger= lmem[looklit].wnb;

o,lmem[looklit].dl_fail= istamp;

}
}else dl_trigger*= dl_rho;

}

/*:140*/
#line 2868 "sat11.w"
;
/*137:*/
#line 3070 "sat11.w"

old_looklit= looklit;
for(o,ola= bimp[bar(looklit)].addr,ols= bimp[bar(looklit)].size;ols;ols--){
o,looklit= bar(mem[ola+ols-1]);
if((isfixed(looklit))&&(stamp[thevar(looklit)]<proto_truth)&&
iscontrary(looklit)){
if(verbose&show_gory_details)
fprintf(stderr," necessary "O"s"O".8s\n",
litname(bar(looklit)));
/*128:*/
#line 2941 "sat11.w"

{
looklit= bar(looklit);
forcedlit[forcedlits++]= looklit;
look_cs= cs,cs= proto_truth;
/*130:*/
#line 2976 "sat11.w"

wptr= 0;fptr= eptr= rptr;
weighted_new_binaries= 0;
l= looklit;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 2980 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*133:*/
#line 3002 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_gory_details)
fprintf(stderr,"  looking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*134:*/
#line 3013 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto contra;
}else{
l= v;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3021 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3029 "sat11.w"
;
}
}else weighted_new_binaries+= heur[u]*heur[v];
}

/*:134*/
#line 3008 "sat11.w"
;
}

/*:133*/
#line 2984 "sat11.w"
;
}
/*135:*/
#line 3036 "sat11.w"

if(wptr){
oo,sl= bimp[looklit].size,ls= bimp[looklit].alloc;
/*136:*/
#line 3056 "sat11.w"

if(o,lmem[looklit].istamp!=istamp){
o,lmem[looklit].istamp= istamp;
o,istack[iptr].lit= looklit,istack[iptr].size= sl;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 3060 "sat11.w"
;
}

/*:136*/
#line 3039 "sat11.w"
;
while(sl+wptr> ls)resize(looklit),ls<<= 1;
o,bimp[looklit].size= sl+wptr;
for(o,la= bimp[looklit].addr+sl;wptr;wptr--){
o,u= wstack[wptr-1];
o,mem[la++]= u;
if(verbose&show_gory_details)
fprintf(stderr," windfall "O"s"O".8s->"O"s"O".8s\n",
litname(looklit),litname(u));
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 3049 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
o,mem[au+su]= bar(looklit);
o,bimp[bar(u)].size= su+1;
}
}

/*:135*/
#line 2986 "sat11.w"
;

/*:130*/
#line 2946 "sat11.w"
;
cs= look_cs;
}

/*:128*/
#line 3079 "sat11.w"
;
o,ola= bimp[bar(old_looklit)].addr;
}
}

/*:137*/
#line 2869 "sat11.w"
;
}

/*:125*/
#line 2774 "sat11.w"
;
look_on:if(forcedlits> fl)fl= forcedlits,last_change= looki;
}
if(last_change==-1)break;
base+= 2*looks;
if(base+2*looks>=proto_truth)break;
}
look_done:

/*:122*/
#line 1394 "sat11.w"
;
if(forcedlits)/*64:*/
#line 1505 "sat11.w"

{
special_start:if(verbose&show_details)
fprintf(stderr,"(lookahead for level "O"d forces "O"d)\n",
level,forcedlits);
cs= near_truth;
fptr= eptr= rptr;
/*65:*/
#line 1530 "sat11.w"

if(++istamp==0){
istamp= 1;
for(l= 2;l<badlit;l++)o,lmem[l].istamp= 0;
}

/*:65*/
#line 1512 "sat11.w"
;
for(i= 0;i<forcedlits;i++){
o,l= forcedlit[i];
/*68:*/
#line 1561 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr,"nearfixing "O"s"O".8s\n",
litname(l));
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr," nearfixing "O"s"O".8s\n",
litname(lp));
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:68*/
#line 1515 "sat11.w"
;
}
goto promote;
}

/*:64*/
#line 1397 "sat11.w"
;
chooseit:/*138:*/
#line 3102 "sat11.w"

{
float best_score;
if(freevars==0)goto satisfied;
for(i= 0,best_score= -1.0,bestlit= 0;i<looks;i++){
o,l= look[i].lit;
if((l&1)==0){
float pos,neg,score;
oo,pos= lmem[l].wnb,neg= lmem[l+1].wnb;
score= (pos+.1)*(neg+.1);
if(verbose&show_gory_details)
fprintf(stderr," "O".8s, "O".4g:"O".4g ("O".4g)\n",
vmem[thevar(l)].name.ch8,pos,neg,score);
if(score> best_score){
best_score= score;
bestlit= (pos> neg?l+1:l);
}
}
}
if(!isfree(bestlit))bestlit= 0;
if(bestlit+forcedlits==0)confusion("choice");
}

/*:138*/
#line 1398 "sat11.w"
;
o,nstack[level].rptr= rptr,nstack[level].iptr= iptr;
if(bestlit){
o,nstack[level].decision= bestlit,nstack[level].branch= 0;
tryit:l= bestlit,plevel= level+1;
if((verbose&show_choices)&&level<=show_choices_max)
fprintf(stderr,"Level "O"d"O"s: "O"s"O".8s ("O"lld mems)\n",
level,nstack[level].branch?"'":"",litname(l),mems);
/*62:*/
#line 1478 "sat11.w"

cs= near_truth;
fptr= eptr= rptr;
/*65:*/
#line 1530 "sat11.w"

if(++istamp==0){
istamp= 1;
for(l= 2;l<badlit;l++)o,lmem[l].istamp= 0;
}

/*:65*/
#line 1481 "sat11.w"
;
/*68:*/
#line 1561 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr,"nearfixing "O"s"O".8s\n",
litname(l));
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr," nearfixing "O"s"O".8s\n",
litname(lp));
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:68*/
#line 1483 "sat11.w"
;
promote:/*63:*/
#line 1492 "sat11.w"

while(fptr<eptr){
o,ll= rstack[fptr++];
/*69:*/
#line 1591 "sat11.w"

o,stamp[thevar(ll)]= real_truth+(ll&1);
if(verbose&show_details)
fprintf(stderr,"fixing "O"s"O".8s\n",
litname(ll));
/*70:*/
#line 1608 "sat11.w"

x= thevar(ll);
o,y= freevar[--freevars];
if(x!=y){
o,xl= freeloc[x];
o,freevar[xl]= y;
o,freeloc[y]= xl;
o,freeloc[x]= freevars;
o,freevar[freevars]= x;
}

/*:70*/
#line 1596 "sat11.w"
;
tll= ll&-2;/*71:*/
#line 1637 "sat11.w"

for(o,la= timp[tll].addr,ls= timp[tll].size;ls;la++,ls--){
o,u= tmem[la].u,v= tmem[la].v;
o,pu= tmem[la].link;
o,pv= tmem[pu].link;
o,aa= timp[bar(u)].addr,ss= timp[bar(u)].size-1;
o,timp[bar(u)].size= ss;
if(pu!=aa+ss){
o,uu= tmem[aa+ss].u,vv= tmem[aa+ss].v;
oo,q= tmem[aa+ss].link,qq= tmem[q].link;
oo,tmem[qq].link= pu,tmem[la].link= aa+ss;
oo,tmem[pu].u= uu,tmem[pu].v= vv,tmem[pu].link= q;
pu= aa+ss;
oo,tmem[pu].u= v,tmem[pu].v= bar(tll),tmem[pu].link= pv;
}
o,aa= timp[bar(v)].addr,ss= timp[bar(v)].size-1;
o,timp[bar(v)].size= ss;
if(pv!=aa+ss){
o,uu= tmem[aa+ss].u,vv= tmem[aa+ss].v;
oo,q= tmem[aa+ss].link,qq= tmem[q].link;
oo,tmem[qq].link= pv,tmem[pu].link= aa+ss;
oo,tmem[pv].u= uu,tmem[pv].v= vv,tmem[pv].link= q;
pv= aa+ss;
oo,tmem[pv].u= bar(tll),tmem[pv].v= u,tmem[pv].link= la;
}
}

/*:71*/
#line 1597 "sat11.w"
;
tll++;/*71:*/
#line 1637 "sat11.w"

for(o,la= timp[tll].addr,ls= timp[tll].size;ls;la++,ls--){
o,u= tmem[la].u,v= tmem[la].v;
o,pu= tmem[la].link;
o,pv= tmem[pu].link;
o,aa= timp[bar(u)].addr,ss= timp[bar(u)].size-1;
o,timp[bar(u)].size= ss;
if(pu!=aa+ss){
o,uu= tmem[aa+ss].u,vv= tmem[aa+ss].v;
oo,q= tmem[aa+ss].link,qq= tmem[q].link;
oo,tmem[qq].link= pu,tmem[la].link= aa+ss;
oo,tmem[pu].u= uu,tmem[pu].v= vv,tmem[pu].link= q;
pu= aa+ss;
oo,tmem[pu].u= v,tmem[pu].v= bar(tll),tmem[pu].link= pv;
}
o,aa= timp[bar(v)].addr,ss= timp[bar(v)].size-1;
o,timp[bar(v)].size= ss;
if(pv!=aa+ss){
o,uu= tmem[aa+ss].u,vv= tmem[aa+ss].v;
oo,q= tmem[aa+ss].link,qq= tmem[q].link;
oo,tmem[qq].link= pv,tmem[pu].link= aa+ss;
oo,tmem[pv].u= uu,tmem[pv].v= vv,tmem[pv].link= q;
pv= aa+ss;
oo,tmem[pv].u= bar(tll),tmem[pv].v= u,tmem[pv].link= la;
}
}

/*:71*/
#line 1598 "sat11.w"
;
for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_details)
fprintf(stderr,"  "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*86:*/
#line 1981 "sat11.w"

x= thevar(u);
o,p= vmem[x].pfx,q= vmem[x].len;
if(q<plevel){
t= prefix;
if(q<32)t&= -(1LL<<(32-q));
if(p!=t)o,vmem[x].pfx= prefix,vmem[x].len= plevel;
}else o,vmem[x].pfx= prefix,vmem[x].len= plevel;
x= thevar(v);
o,p= vmem[x].pfx,q= vmem[x].len;
if(q<plevel){
t= prefix;
if(q<32)t&= -(1LL<<(32-q));
if(p!=t)o,vmem[x].pfx= prefix,vmem[x].len= plevel;
}else o,vmem[x].pfx= prefix,vmem[x].len= plevel;

/*:86*/
#line 1604 "sat11.w"
;
/*72:*/
#line 1688 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto conflict;
}else{
l= v;
/*68:*/
#line 1561 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr,"nearfixing "O"s"O".8s\n",
litname(l));
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr," nearfixing "O"s"O".8s\n",
litname(lp));
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:68*/
#line 1695 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
/*68:*/
#line 1561 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr,"nearfixing "O"s"O".8s\n",
litname(l));
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr," nearfixing "O"s"O".8s\n",
litname(lp));
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:68*/
#line 1702 "sat11.w"
;
}
}else/*73:*/
#line 1733 "sat11.w"

{
/*66:*/
#line 1540 "sat11.w"

if(++bstamp==0){
bstamp= 1;
for(l= 2;l<badlit;l++)o,lmem[l].bstamp= 0;
}

/*:66*/
#line 1735 "sat11.w"
;
o,lmem[bar(u)].bstamp= bstamp;
for(o,au= bimp[bar(u)].addr,k= su= bimp[bar(u)].size;k;au++,k--)
oo,lmem[mem[au]].bstamp= bstamp;
if(o,lmem[bar(v)].bstamp==bstamp){
fix_u:l= u;/*68:*/
#line 1561 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr,"nearfixing "O"s"O".8s\n",
litname(l));
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr," nearfixing "O"s"O".8s\n",
litname(lp));
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:68*/
#line 1740 "sat11.w"
;
}else if(o,lmem[v].bstamp!=bstamp){
o,ua= bimp[bar(u)].alloc;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 1743 "sat11.w"
;
/*76:*/
#line 1792 "sat11.w"

for(o,la= bimp[v].addr,ls= bimp[v].size;ls;la++,ls--){
o,w= mem[la];
if(!isfixed(w)){
if(o,lmem[bar(w)].bstamp==bstamp)
goto fix_u;
if(o,lmem[w].bstamp!=bstamp){
if(verbose&show_details)
fprintf(stderr,"   ->"O"s"O".8s|"O"s"O".8s\n",
litname(u),litname(w));
if(su==ua)resize(bar(u)),ua+= ua,o,au= bimp[bar(u)].addr+su;
oo,mem[au++]= w,bimp[bar(u)].size= ++su;
o,aw= bimp[bar(w)].addr,sw= bimp[bar(w)].size;
/*78:*/
#line 1824 "sat11.w"

if(o,lmem[bar(w)].istamp!=istamp){
o,lmem[bar(w)].istamp= istamp;
o,istack[iptr].lit= bar(w),istack[iptr].size= sw;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1828 "sat11.w"
;
}

/*:78*/
#line 1805 "sat11.w"
;
if(o,sw==bimp[bar(w)].alloc)resize(bar(w)),o,aw= bimp[bar(w)].addr;
o,bimp[bar(w)].size= sw+1;
o,mem[aw+sw]= u;
}
}
}

/*:76*/
#line 1745 "sat11.w"
;
/*66:*/
#line 1540 "sat11.w"

if(++bstamp==0){
bstamp= 1;
for(l= 2;l<badlit;l++)o,lmem[l].bstamp= 0;
}

/*:66*/
#line 1746 "sat11.w"
;
o,lmem[bar(v)].bstamp= bstamp;
for(o,av= bimp[bar(v)].addr,k= sv= bimp[bar(v)].size;k;av++,k--)
oo,lmem[mem[av]].bstamp= bstamp;
if(o,lmem[bar(u)].bstamp==bstamp){
fix_v:l= v;/*68:*/
#line 1561 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr,"nearfixing "O"s"O".8s\n",
litname(l));
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto conflict;
}else{
if(verbose&show_details)
fprintf(stderr," nearfixing "O"s"O".8s\n",
litname(lp));
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:68*/
#line 1751 "sat11.w"
;
}else{
o,va= bimp[bar(v)].alloc;
/*77:*/
#line 1815 "sat11.w"

if(o,lmem[bar(v)].istamp!=istamp){
o,lmem[bar(v)].istamp= istamp;
o,istack[iptr].lit= bar(v),istack[iptr].size= sv;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1819 "sat11.w"
;
}

/*:77*/
#line 1754 "sat11.w"
;
/*79:*/
#line 1834 "sat11.w"

for(o,la= bimp[u].addr,ls= bimp[u].size;ls;la++,ls--){
o,w= mem[la];
if(!isfixed(w)){
if(o,lmem[bar(w)].bstamp==bstamp)
goto fix_v;
if(o,lmem[w].bstamp!=bstamp){
if(verbose&show_details)
fprintf(stderr,"   ->"O"s"O".8s|"O"s"O".8s\n",
litname(v),litname(w));
if(sv==va)resize(bar(v)),va+= va,o,av= bimp[bar(v)].addr+sv;
oo,mem[av++]= w,bimp[bar(v)].size= ++sv;
o,aw= bimp[bar(w)].addr,sw= bimp[bar(w)].size;
/*78:*/
#line 1824 "sat11.w"

if(o,lmem[bar(w)].istamp!=istamp){
o,lmem[bar(w)].istamp= istamp;
o,istack[iptr].lit= bar(w),istack[iptr].size= sw;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1828 "sat11.w"
;
}

/*:78*/
#line 1847 "sat11.w"
;
if(o,sw==bimp[bar(w)].alloc)resize(bar(w)),o,aw= bimp[bar(w)].addr;
o,bimp[bar(w)].size= sw+1;
o,mem[aw+sw]= v;
}
}
}

/*:79*/
#line 1756 "sat11.w"
;
if(su==ua)resize(bar(u)),ua+= ua,o,au= bimp[bar(u)].addr+su;
oo,mem[au]= v,bimp[bar(u)].size= su+1;
if(sv==va)resize(bar(v)),va+= va,o,av= bimp[bar(v)].addr+sv;
oo,mem[av]= u,bimp[bar(v)].size= sv+1;
}
}
}

/*:73*/
#line 1704 "sat11.w"
;
}

/*:72*/
#line 1605 "sat11.w"
;
}

/*:69*/
#line 1496 "sat11.w"
;
}
rptr= eptr;

/*:63*/
#line 1485 "sat11.w"
;
if(o,nstack[level].branch<0){
if(level)goto chooseit;
forcedlits= 0;
goto enter_level;
}

/*:62*/
#line 1407 "sat11.w"
;
}else if((verbose&show_choices)&&level<=show_choices_max)
fprintf(stderr,"Level "O"d: no branch\n",
level);

/*:59*/
#line 3349 "sat11.w"
;
forcedlits= 0;
level++;
goto enter_level;
/*84:*/
#line 1929 "sat11.w"

dl_contra:/*146:*/
#line 3293 "sat11.w"

if(cs<dl_truth){
/*145:*/
#line 3272 "sat11.w"

{
dl_last_change= dlooklit;
dlooklit= bar(dlooklit);
dlook_cs= cs,cs= dl_truth;
/*147:*/
#line 3300 "sat11.w"

fptr= eptr= rptr;
l= dlooklit;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3303 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*148:*/
#line 3311 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_doubly_gory_details)
fprintf(stderr,"  dlooking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*149:*/
#line 3321 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto dl_contra;
}else{
l= v;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3328 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
/*143:*/
#line 3222 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr,"dlfixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto dl_contra;
}else{
if(verbose&show_doubly_gory_details){
if(cs>=dl_truth)fprintf(stderr," dlfixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:143*/
#line 3335 "sat11.w"
;
}
}
}

/*:149*/
#line 3318 "sat11.w"
;
}

/*:148*/
#line 3307 "sat11.w"
;
}

/*:147*/
#line 3277 "sat11.w"
;
cs= dlook_cs;
wstack[wptr++]= dlooklit;
}

/*:145*/
#line 3295 "sat11.w"
;
goto dlook_on;
}
base= last_base;

/*:146*/
#line 1930 "sat11.w"
;
contra:/*129:*/
#line 2959 "sat11.w"

if(cs<proto_truth){
/*128:*/
#line 2941 "sat11.w"

{
looklit= bar(looklit);
forcedlit[forcedlits++]= looklit;
look_cs= cs,cs= proto_truth;
/*130:*/
#line 2976 "sat11.w"

wptr= 0;fptr= eptr= rptr;
weighted_new_binaries= 0;
l= looklit;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 2980 "sat11.w"
;
while(fptr<eptr){
o,ll= rstack[fptr++];
/*133:*/
#line 3002 "sat11.w"

for(o,tla= timp[ll].addr,tls= timp[ll].size;tls;tla++,tls--){
o,u= tmem[tla].u,v= tmem[tla].v;
if(verbose&show_gory_details)
fprintf(stderr,"  looking "O"s"O".8s->"O"s"O".8s|"O"s"O".8s\n",
litname(ll),litname(u),litname(v));
/*134:*/
#line 3013 "sat11.w"

if(isfixed(u)){
if(iscontrary(u)){
if(isfixed(v)){
if(iscontrary(v))goto contra;
}else{
l= v;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3021 "sat11.w"
;
}
}
}else{
if(isfixed(v)){
if(iscontrary(v)){
l= u;
wstack[wptr++]= l;
/*124:*/
#line 2809 "sat11.w"

if(isfixed(l)){
if(iscontrary(l))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr,"protofixing "O"s"O".8s\n",
litname(l));
else fprintf(stderr,""O"dfixing "O"s"O".8s\n",
cs,litname(l));
}
stamptrue(l);
lfptr= eptr;
o,rstack[eptr++]= l;
while(lfptr<eptr){
o,l= rstack[lfptr++];
for(o,la= bimp[l].addr,ls= bimp[l].size;ls;la++,ls--){
o,lp= mem[la];
if(isfixed(lp)){
if(iscontrary(lp))goto contra;
}else{
if(verbose&show_gory_details){
if(cs>=proto_truth)fprintf(stderr," protofixing "O"s"O".8s\n",
litname(lp));
else fprintf(stderr," "O"dfixing "O"s"O".8s\n",
cs,litname(lp));
}
stamptrue(lp);
o,rstack[eptr++]= lp;
}
}
}
}

/*:124*/
#line 3029 "sat11.w"
;
}
}else weighted_new_binaries+= heur[u]*heur[v];
}

/*:134*/
#line 3008 "sat11.w"
;
}

/*:133*/
#line 2984 "sat11.w"
;
}
/*135:*/
#line 3036 "sat11.w"

if(wptr){
oo,sl= bimp[looklit].size,ls= bimp[looklit].alloc;
/*136:*/
#line 3056 "sat11.w"

if(o,lmem[looklit].istamp!=istamp){
o,lmem[looklit].istamp= istamp;
o,istack[iptr].lit= looklit,istack[iptr].size= sl;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 3060 "sat11.w"
;
}

/*:136*/
#line 3039 "sat11.w"
;
while(sl+wptr> ls)resize(looklit),ls<<= 1;
o,bimp[looklit].size= sl+wptr;
for(o,la= bimp[looklit].addr+sl;wptr;wptr--){
o,u= wstack[wptr-1];
o,mem[la++]= u;
if(verbose&show_gory_details)
fprintf(stderr," windfall "O"s"O".8s->"O"s"O".8s\n",
litname(looklit),litname(u));
o,au= bimp[bar(u)].addr,su= bimp[bar(u)].size;
/*74:*/
#line 1767 "sat11.w"

if(o,lmem[bar(u)].istamp!=istamp){
o,lmem[bar(u)].istamp= istamp;
o,istack[iptr].lit= bar(u),istack[iptr].size= su;
/*75:*/
#line 1774 "sat11.w"

iptr++;
if(iptr==iptr_max){
bytes+= iptr*sizeof(idata);
iptr_max<<= 1;
}

/*:75*/
#line 1771 "sat11.w"
;
}

/*:74*/
#line 3049 "sat11.w"
;
if(o,su==bimp[bar(u)].alloc)resize(bar(u)),o,au= bimp[bar(u)].addr;
o,mem[au+su]= bar(looklit);
o,bimp[bar(u)].size= su+1;
}
}

/*:135*/
#line 2986 "sat11.w"
;

/*:130*/
#line 2946 "sat11.w"
;
cs= look_cs;
}

/*:128*/
#line 2961 "sat11.w"
;
goto look_on;
}

/*:129*/
#line 1931 "sat11.w"
;
goto look_bad;
conflict:/*81:*/
#line 1880 "sat11.w"

for(j= fptr;j<eptr;j++)oo,stamp[thevar(rstack[j])]= 0;

/*:81*/
#line 1933 "sat11.w"
;
backtrack:/*82:*/
#line 1913 "sat11.w"

for(j= fptr-1;j>=rptr;j--){
o,ll= rstack[j];
tll= ll|1;/*83:*/
#line 1922 "sat11.w"

for(o,ls= timp[tll].size,la= timp[tll].addr+ls-1;ls;ls--,la--){
o,u= tmem[la].u,v= tmem[la].v;
oo,timp[bar(u)].size++;
oo,timp[bar(v)].size++;
}

/*:83*/
#line 1916 "sat11.w"
;
tll--;/*83:*/
#line 1922 "sat11.w"

for(o,ls= timp[tll].size,la= timp[tll].addr+ls-1;ls;ls--,la--){
o,u= tmem[la].u,v= tmem[la].v;
oo,timp[bar(u)].size++;
oo,timp[bar(v)].size++;
}

/*:83*/
#line 1917 "sat11.w"
;
freevars++;
o,stamp[thevar(ll)]= 0;
}

/*:82*/
#line 1934 "sat11.w"
;
/*80:*/
#line 1866 "sat11.w"

if(o,nstack[level].branch>=0){
for(o,j= nstack[level].iptr;iptr> j;iptr--){
o,l= istack[iptr-1].lit,sl= istack[iptr-1].size;
o,bimp[l].size= sl;
}
}

/*:80*/
#line 1935 "sat11.w"
;
if(o,nstack[level].branch==0)/*85:*/
#line 1961 "sat11.w"

{
bestlit= bar(nstack[level].decision);
o,nstack[level].decision= bestlit,nstack[level].branch= 1;
if(level<32)prefix+= 1<<(31-level);
goto tryit;
}

/*:85*/
#line 1936 "sat11.w"
;
look_bad:if(level){
level--;
if(level<31)prefix&= -(1<<(31-level));
fptr= rptr;
o,rptr= nstack[level].rptr;
goto backtrack;
}
unsat:if(1){
printf("~\n");
if(verbose&show_basics)fprintf(stderr,"UNSAT\n");
}else{
satisfied:if(verbose&show_basics)fprintf(stderr,"!SAT!\n");
/*151:*/
#line 3355 "sat11.w"

for(k= 0;k<rptr;k++){
printf(" "O"s"O".8s",litname(rstack[k]));
if(out_file)fprintf(out_file," "O"s"O".8s",litname(bar(rstack[k])));
}
printf("\n");
if(freevars){
if(verbose&show_unused_vars)printf("(Unused:");
for(k= 0;k<freevars;k++){
if(verbose&show_unused_vars)printf(" "O".8s",vmem[freevar[k]].name.ch8);
if(out_file)fprintf(out_file," "O".8s",vmem[freevar[k]].name.ch8);
}
if(verbose&show_unused_vars)printf(")\n");
}
if(out_file)fprintf(out_file,"\n");

/*:151*/
#line 1949 "sat11.w"
;
}

/*:84*/
#line 3353 "sat11.w"
;

/*:150*/
#line 122 "sat11.w"
;
done:if(verbose&show_basics)
fprintf(stderr,
"Altogether "O"llu+"O"llu mems, "O"llu bytes, "O"llu nodes.\n",
imems,mems,bytes,nodes);
}

/*:2*/
