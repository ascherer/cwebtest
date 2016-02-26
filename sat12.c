#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define O "%" \

#define show_basics 1
#define show_rounds 2
#define show_details 4
#define show_resolutions 8
#define show_lit_ids 16
#define show_subtrials 32
#define show_restrials 64
#define show_initial_clauses 128 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define is_lit(k) ((k) <lit_head_top) 
#define is_cls(k) ((k) <cls_head_top) 
#define up(k) mem[k].litinf.u2[0]
#define down(k) mem[k].litinf.u2[1]
#define left(k) mem[k].clsinf.u2[0]
#define right(k) mem[k].clsinf.u2[1]
#define litsig(k) mem[k].clsinf.lng
#define clssig(k) mem[k].litinf.lng
#define occurs(l) mem[l].lit
#define littime(l) mem[l].cls
#define size(c) mem[c].cls
#define clstime(c) mem[c].lit \

#define thevar(l) ((l) >>1) 
#define litname(l) (l) &1?"~":"",vmem[thevar(l) ].name.ch8
#define pos_lit(v) ((v) <<1) 
#define neg_lit(v) (((v) <<1) +1) 
#define bar(l) ((l) ^1) 
#define touch(w) o,vmem[thevar(w) ].stable= 0
#define norm 0
#define elim_quiet 1
#define elim_res 2
#define forced_true 3
#define forced_false 4 \

#define sentinel 1
#define slink(c) cmem[c-lit_head_top].link
#define newsize(c) cmem[c-lit_head_top].size \

#define sanity_checking 0 \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

/*3:*/
#line 112 "sat12.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*6:*/
#line 268 "sat12.w"

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

/*:6*//*7:*/
#line 295 "sat12.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:7*//*25:*/
#line 651 "sat12.w"

typedef struct cell_struct{
uint lit;
uint cls;
octa litinf,clsinf;
}cel;

/*:25*//*27:*/
#line 707 "sat12.w"

typedef struct var_struct{
octa name;
uint link;
char status;
char stable;
short spare;
uint blink;
uint filler;
}variable;

/*:27*//*28:*/
#line 721 "sat12.w"

typedef struct lit_struct{
ullng extra;
}literal;

/*:28*//*29:*/
#line 735 "sat12.w"

typedef struct cls_struct{
uint link;
uint size;
}clause;

/*:29*/
#line 119 "sat12.w"
;
/*4:*/
#line 164 "sat12.w"

int random_seed= 0;
int verbose= show_basics;
int hbits= 8;
int buf_size= 1024;
FILE*erp_file;
char erp_file_name[100]= "/tmp/erp";
ullng imems,mems;
ullng bytes;
uint xcells;
int cutoff= 10;
ullng optimism= 25;
int buckets= 32;
ullng mem_max= 100000;
uint sub_total,str_total;
ullng sub_tries,sub_false,str_tries,str_false;
int maxrounds= 0x7fffffff;
ullng timeout= 0x1fffffffffffffff;
ullng elim_tries,func_total;

/*:4*//*8:*/
#line 301 "sat12.w"

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

/*:8*//*39:*/
#line 932 "sat12.w"

cel*mem;
uint lit_head_top;
uint cls_head_top;
uint avail;
uint to_do;
uint strengthened;
variable*vmem;
literal*lmem;
clause*cmem;
int vars_gone;
int clauses_gone;
uint time;

/*:39*//*81:*/
#line 1728 "sat12.w"

ullng stamp;
uint beta0;
uint alpha0;
uint last_new;
uint alf,bet;
int clauses_saved;
uint*bucket;

/*:81*/
#line 120 "sat12.w"
;
/*26:*/
#line 661 "sat12.w"

void show_cell(uint k){
fprintf(stderr,"mem["O"u]=",k);
if(is_lit(k))
fprintf(stderr,"occ "O"u, time "O"u, sig "O"llx, up "O"u, dn "O"u\n",
occurs(k),littime(k),litsig(k),up(k),down(k));
else if(is_cls(k))
fprintf(stderr,"size "O"u, time "O"u, sig "O"llx, left "O"u, right "O"u\n",
size(k),clstime(k),clssig(k),left(k),right(k));
else fprintf(stderr,
"lit "O"u, cls "O"u, lft "O"u, rt "O"u, up "O"u, dn "O"u\n",
mem[k].lit,mem[k].cls,left(k),right(k),up(k),down(k));
}

/*:26*//*30:*/
#line 750 "sat12.w"

void print_clause(int c){
register uint k,l;
if(is_cls(c)&&!is_lit(c)){
if(!size(c))return;
fprintf(stderr,""O"d:",c);
for(k= right(c);!is_cls(k);k= right(k)){
l= mem[k].lit;
fprintf(stderr," "O"s"O".8s",litname(l));
if(verbose&show_lit_ids)fprintf(stderr,"("O"u)",l);
}
fprintf(stderr,"\n");
}else fprintf(stderr,"there is no clause "O"d!\n",c);
}

/*:30*//*31:*/
#line 767 "sat12.w"

void print_all(void){
register uint c;
for(c= lit_head_top;is_cls(c);c++)if(size(c))
print_clause(c);
}

/*:31*//*32:*/
#line 777 "sat12.w"

void print_clauses_for(int l){
register uint k;
if(is_lit(l)&&l>=2){
if(vmem[thevar(l)].status){
fprintf(stderr," "O"s has been %s!\n",vmem[thevar(l)].name.ch8,
vmem[thevar(l)].status==elim_res?"eliminated":
vmem[thevar(l)].status==elim_quiet?"quietly eliminated":
vmem[thevar(l)].status==forced_true?"forced true":
vmem[thevar(l)].status==forced_false?"forced false":"clobbered");
return;
}
fprintf(stderr," "O"s"O".8s",litname(l));
if(verbose&show_lit_ids)fprintf(stderr,"("O"u)",l);
fprintf(stderr," is in");
for(k= down(l);!is_lit(k);k= down(k))
fprintf(stderr," "O"u",mem[k].cls);
fprintf(stderr,"\n");
}else fprintf(stderr,"There is no literal "O"d!\n",l);
}

/*:32*//*33:*/
#line 803 "sat12.w"

void sanity(void){
register uint l,k,c,countl,countc,counta,s;
register ullng bits;
for(l= 2,countl= 0;is_lit(l);l++)
if(vmem[thevar(l)].status==norm)/*34:*/
#line 820 "sat12.w"

{
for(k= down(l),s= 0;!is_lit(k);k= down(k)){
if(k>=xcells){
fprintf(stderr,"address in lit list "O"u out of range!\n",l);
goto bad_l;
}
if(mem[k].lit!=l)
fprintf(stderr,"literal wrong at cell "O"u ("O"u not "O"u)!\n",
k,mem[k].lit,l);
if(down(up(k))!=k){
fprintf(stderr,"down/up link wrong at cell "O"u of lit list "O"u!\n",k,l);
goto bad_l;
}
countl++,s++;
}
if(k!=l)
fprintf(stderr,"lit list "O"u ends at "O"u!\n",l,k);
else if(down(up(k))!=k)
fprintf(stderr,"down/up link wrong at lit list head "O"u!\n",l);
if(s!=occurs(l))
fprintf(stderr,"literal "O"u occurs in "O"u clauses, not "O"u!\n",l,s,occurs(l));
bad_l:continue;
}

/*:34*/
#line 808 "sat12.w"
;
for(c= l,countc= 0;is_cls(c);c++)
if(size(c))/*35:*/
#line 847 "sat12.w"

{
bits= 0;
for(k= right(c),l= s= 0;!is_cls(k);k= right(k)){
if(k>=xcells){
fprintf(stderr,"address in cls list "O"u out of range!\n",c);
goto bad_c;
}
if(mem[k].cls!=c)
fprintf(stderr,"clause wrong at cell "O"u ("O"u not "O"u)!\n",
k,mem[k].cls,c);
if(right(left(k))!=k){
fprintf(stderr,"right/left link wrong at cell "O"u of cls list "O"u!\n",k,c);
goto bad_c;
}
if(thevar(mem[k].lit)<=thevar(l))
fprintf(stderr,
"literals "O"u and "O"u out of order in cell "O"u of clause "O"u!\n",
l,mem[k].lit,k,c);
l= mem[k].lit;
bits|= litsig(l);
countc++,s++;
}
if(k!=c)
fprintf(stderr,"cls list "O"u ends at "O"u!\n",c,k);
else if(right(left(k))!=k)
fprintf(stderr,"right/left link wrong of cls list head "O"u!\n",c);
if(bits!=clssig(c))
fprintf(stderr,"signature wrong at clause "O"u!\n",c);
if(s!=size(c))
fprintf(stderr,"clause "O"u has "O"u literals, not "O"u!\n",c,s,size(c));
bad_c:continue;
}

/*:35*/
#line 810 "sat12.w"
;
if(countl!=countc&&to_do==0)
fprintf(stderr,""O"u cells in lit lists but "O"u cells in cls lists!\n",
countl,countc);
/*36:*/
#line 885 "sat12.w"

for(k= avail,counta= 0;k;k= left(k)){
if(k>=xcells||is_cls(k)){
fprintf(stderr,"address out of range in avail stack!\n");
break;
}
counta++;
}

/*:36*/
#line 814 "sat12.w"
;
if(xcells!=cls_head_top+countc+counta+1)
fprintf(stderr,"memory leak of "O"d cells!\n",
(int)(xcells-cls_head_top-countc-counta-1));
}

/*:33*//*37:*/
#line 901 "sat12.w"

uint get_cell(void){
register uint k;
if(avail){
k= avail;
o,avail= left(k);
return k;
}
if(xcells==mem_max){
fprintf(stderr,
"Oops, we're out of memory (mem_max="O"llu)!\nTry option m.\n",mem_max);
exit(-9);
}
return xcells++;
}

/*:37*//*38:*/
#line 919 "sat12.w"

void free_cell(uint k){
o,left(k)= avail;

avail= k;
}

void free_cells(uint k,uint kk){

o,left(k)= avail;
avail= kk;
}

/*:38*//*98:*/
#line 2009 "sat12.w"

void confusion(char*id){
fprintf(stderr,"This can't happen ("O"s)!\n",id);
exit(-69);
}

void debugstop(int foo){
fprintf(stderr,"You rang("O"d)?\n",foo);
}

/*:98*/
#line 121 "sat12.w"
;
main(int argc,char*argv[]){
register uint aa,b,c,cc,h,i,j,k,l,ll,p,pp,q,qq,r,s,t,u,uu,v,vv,w,ww,x;
register uint rbits= 0;
register ullng bits;
register specialcase;
/*5:*/
#line 223 "sat12.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,""O"d",&verbose)-1);break;
case'h':k|= (sscanf(argv[j]+1,""O"d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,""O"d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1);break;
case'e':sprintf(erp_file_name,""O".99s",argv[j]+1);break;
case'm':k|= (sscanf(argv[j]+1,""O"llu",&mem_max)-1);break;
case'c':k|= (sscanf(argv[j]+1,""O"d",&cutoff)-1);break;
case'C':k|= (sscanf(argv[j]+1,""O"llu",&optimism)-1);break;
case'B':k|= (sscanf(argv[j]+1,""O"d",&buckets)-1);break;
case't':k|= (sscanf(argv[j]+1,""O"d",&maxrounds)-1);break;
case'T':k|= (sscanf(argv[j]+1,""O"lld",&timeout)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0){
fprintf(stderr,
"Usage: "O"s [v<n>] [h<n>] [b<n>] [s<n>] [efoo.erp] [m<n>]",argv[0]);
fprintf(stderr," [c<n>] [C<n>] [B<n>] [t<n]] [T<n>] < foo.sat\n");
exit(-1);
}
if(!(erp_file= fopen(erp_file_name,"w"))){
fprintf(stderr,"I couldn't open file "O"s for writing!\n",erp_file_name);
exit(-16);
}

/*:5*/
#line 127 "sat12.w"
;
/*9:*/
#line 316 "sat12.w"

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

/*:9*//*15:*/
#line 454 "sat12.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:15*/
#line 128 "sat12.w"
;
/*10:*/
#line 345 "sat12.w"

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
#line 382 "sat12.w"

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
/*12:*/
#line 407 "sat12.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 425 "sat12.w"

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
#line 410 "sat12.w"
;
/*16:*/
#line 458 "sat12.w"

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

/*:16*/
#line 412 "sat12.w"
;
/*17:*/
#line 474 "sat12.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 413 "sat12.w"
;
if(p->stamp==clauses||p->stamp==-clauses)/*18:*/
#line 488 "sat12.w"

{
if((p->stamp> 0)==(i> 0))goto empty_clause;
}

/*:18*/
#line 414 "sat12.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 438 "sat12.w"

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
#line 417 "sat12.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:12*/
#line 393 "sat12.w"
;
}
if(k==0){
fprintf(stderr,"(Empty line "O"lld is being ignored)\n",clauses);
nullclauses++;
}
goto clause_done;
empty_clause:/*19:*/
#line 498 "sat12.w"

while(k){
/*20:*/
#line 507 "sat12.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 500 "sat12.w"
;
k--;
}
if((buf[0]!='~')||(buf[1]!=' '))
fprintf(stderr,"(The clause on line "O"lld is always satisfied)\n",clauses);
nullclauses++;

/*:19*/
#line 400 "sat12.w"
;
clause_done:cells+= k;

/*:11*/
#line 356 "sat12.w"
;
}
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

/*:10*/
#line 129 "sat12.w"
;
if(verbose&show_basics)
/*22:*/
#line 525 "sat12.w"

fprintf(stderr,
"("O"lld variables, "O"lld clauses, "O"llu literals successfully read)\n",
vars,clauses,cells);

/*:22*/
#line 131 "sat12.w"
;
/*40:*/
#line 951 "sat12.w"

/*41:*/
#line 965 "sat12.w"

free(buf);free(hash);
lit_head_top= vars+vars+2;
cls_head_top= lit_head_top+clauses;
xcells= cls_head_top+cells+1;
if(xcells+cells> mem_max)mem_max= xcells+cells;
if(mem_max>=0x100000000)mem_max= 0xffffffff;
mem= (cel*)malloc(mem_max*sizeof(cel));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the big mem array!\n");
exit(-10);
}
bytes= mem_max*sizeof(cel);
vmem= (variable*)malloc((vars+1)*sizeof(variable));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-11);
}
bytes+= (vars+1)*sizeof(variable);

/*:41*/
#line 952 "sat12.w"
;
/*42:*/
#line 985 "sat12.w"

for(l= 2;is_lit(l);l++)o,down(l)= l;
for(c= clauses,j= cls_head_top;c;c--){
/*43:*/
#line 1002 "sat12.w"

for(i= 0;i<2;){
/*20:*/
#line 507 "sat12.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:20*/
#line 1004 "sat12.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1)+2;
o,mem[j].lit= p,mem[j].cls= cc= c+lit_head_top-1;
ooo,down(j)= down(p),down(p)= j++;
}
o,left(cc)= cc;

/*:43*/
#line 988 "sat12.w"
;
}
if(j!=cls_head_top+cells){
fprintf(stderr,"Oh oh, something happened to "O"d cells!\n",
(int)(cls_head_top+cells-j));
exit(-15);
}

/*:42*/
#line 953 "sat12.w"
;
/*44:*/
#line 1013 "sat12.w"

for(c= vars;c;c--){
/*21:*/
#line 516 "sat12.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:21*/
#line 1015 "sat12.w"
;
o,vmem[c].name.lng= cur_tmp_var->name.lng;
o,vmem[c].stable= vmem[c].status= 0;
}

/*:44*/
#line 954 "sat12.w"
;
/*45:*/
#line 1023 "sat12.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL)
confusion("consistency");
free(cur_chunk);free(cur_vchunk);

/*:45*/
#line 955 "sat12.w"
;
/*46:*/
#line 1031 "sat12.w"

for(l= 2;is_lit(l);l++)
/*47:*/
#line 1045 "sat12.w"

{
for(j= l,k= down(j),s= 0;!is_lit(k);o,j= k,k= down(j)){
o,up(k)= j;
o,c= mem[k].cls;
ooo,left(k)= left(c),left(c)= k;
s++;
}
if(k!=l)confusion("lit init");
o,occurs(l)= s,littime(l)= 0;
o,up(l)= j;
if(s==0){
w= l;
if(verbose&show_details)
fprintf(stderr,"no input clause contains the literal "O"s"O".8s\n",
litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1061 "sat12.w"
;
}else/*48:*/
#line 1071 "sat12.w"

{
if(rbits<0x40)
mems+= 4,rbits= gb_next_rand()|(1U<<30);
o,litsig(l)= 1LLU<<(rbits&0x3f);
rbits>>= 6;
if(rbits<0x40)
mems+= 4,rbits= gb_next_rand()|(1U<<30);
o,litsig(l)|= 1LLU<<(rbits&0x3f);
rbits>>= 6;
}

/*:48*/
#line 1062 "sat12.w"
;
}

/*:47*/
#line 1033 "sat12.w"
;
for(c= l;is_cls(c);c++)
/*49:*/
#line 1083 "sat12.w"

{
bits= 0;
for(j= c,k= left(j),s= 0;!is_cls(k);o,j= k,k= left(k)){
o,right(k)= j;
o,w= mem[k].lit;
o,bits|= litsig(w);
s++;
}
if(k!=c)confusion("cls init");
o,size(c)= s,clstime(c)= 0;
oo,clssig(c)= bits,right(c)= j;
if(s<=1){
if(s==0)confusion("empty clause");
if(verbose&show_details)
fprintf(stderr,"clause "O"u is the single literal "O"s"O".8s\n",
c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1100 "sat12.w"
;
}
}

/*:49*/
#line 1035 "sat12.w"
;

/*:46*/
#line 956 "sat12.w"
;
/*52:*/
#line 1148 "sat12.w"

lmem= (literal*)malloc(lit_head_top*sizeof(literal));
if(!lmem){
fprintf(stderr,"Oops, I can't allocate the lmem array!\n");
exit(-12);
}
bytes+= lit_head_top*sizeof(literal);
for(l= 0;l<lit_head_top;l++)o,lmem[l].extra= 0;
cmem= (clause*)malloc(clauses*sizeof(clause));
if(!cmem){
fprintf(stderr,"Oops, I can't allocate the cmem array!\n");
exit(-13);
}
bytes+= clauses*sizeof(clause);

/*:52*//*82:*/
#line 1737 "sat12.w"

if(buckets<2)buckets= 2;
bucket= (uint*)malloc((buckets+1)*sizeof(uint));
if(!bucket){
fprintf(stderr,"Oops, I can't allocate the bucket array!\n");
exit(-14);
}
bytes+= (buckets+1)*sizeof(uint);

/*:82*/
#line 957 "sat12.w"
;

/*:40*/
#line 132 "sat12.w"
;
imems= mems,mems= 0;
/*91:*/
#line 1888 "sat12.w"

if(verbose&show_initial_clauses)print_all();
if(sanity_checking)sanity();
/*92:*/
#line 1909 "sat12.w"

o,slink(lit_head_top)= sentinel,newsize(lit_head_top)= 0;
for(c= lit_head_top+1;is_cls(c);c++)
o,slink(c)= c-1,newsize(c)= 0;
strengthened= c-1;

/*:92*/
#line 1891 "sat12.w"
;
/*65:*/
#line 1437 "sat12.w"

{
register uint c;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1440 "sat12.w"
;
while(strengthened!=sentinel){
c= strengthened;
o,strengthened= slink(c);
if(o,size(c)){
o,slink(c)= 0;
/*57:*/
#line 1260 "sat12.w"

if(verbose&show_subtrials)
fprintf(stderr," trying subsumption by "O"u\n",c);
/*58:*/
#line 1278 "sat12.w"

ooo,p= right(c),l= mem[p].lit,k= occurs(l);
for(o,p= right(p);!is_cls(p);o,p= right(p)){
o,ll= mem[p].lit;
if(o,occurs(ll)<k)k= occurs(ll),l= ll;
}

/*:58*/
#line 1263 "sat12.w"
;
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,pp= down(l);!is_lit(pp);o,pp= down(pp)){
o,cc= mem[pp].cls;
if(cc==c)continue;
sub_tries++;
if(o,bits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*59:*/
#line 1288 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:59*/
#line 1271 "sat12.w"
;
if(l> ll)sub_false++;
else/*60:*/
#line 1308 "sat12.w"

{
if(verbose&show_details)
fprintf(stderr,"clause "O"u subsumes clause "O"u\n",c,cc);
sub_total++;
for(o,p= right(cc);!is_cls(p);o,p= right(p)){
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
o,w= mem[p].lit;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(q));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1322 "sat12.w"
;
}
}
free_cells(right(cc),left(cc));
o,size(cc)= 0,clauses_gone++;
}

/*:60*/
#line 1273 "sat12.w"
;
}

/*:57*/
#line 1446 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1447 "sat12.w"
;
if(o,size(c)){
specialcase= 0;
/*61:*/
#line 1340 "sat12.w"

{
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,vv= v;!is_cls(vv);o,vv= left(vv)){
register ullng ubits;
o,u= mem[vv].lit;
if(specialcase)/*95:*/
#line 1968 "sat12.w"

{
if(o,littime(bar(u))!=time)continue;
if(o,newsize(c)>>2!=size(c)-(littime(u)!=time))continue;

}

/*:95*/
#line 1346 "sat12.w"
;
if(verbose&show_subtrials)
fprintf(stderr," trying to strengthen by "O"u and "O"s"O".8s\n",
c,litname(u));
o,ubits= bits&~litsig(u);
for(o,pp= down(bar(u));!is_lit(pp);o,pp= down(pp)){
str_tries++;
o,cc= mem[pp].cls;
if(o,ubits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*62:*/
#line 1363 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
if(l==u)l= bar(l);
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:62*/
#line 1356 "sat12.w"
;
if(l> ll)str_false++;
else/*63:*/
#line 1384 "sat12.w"

{
register ullng ccbits= 0;
if(verbose&show_details)
fprintf(stderr,"clause "O"u loses literal "O"s"O".8s via clause "O"u\n",
cc,litname(bar(u)),c);
str_total++;
for(o,p= right(cc);;o,p= right(p)){
o,w= mem[p].lit;
touch(w);
if(w==bar(u))break;
o,ccbits|= litsig(w);
}
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1401 "sat12.w"
;
}
o,q= up(p),w= down(p);
oo,down(q)= w,up(w)= q;
o,q= right(p),w= left(p);
oo,left(q)= w,right(w)= q;
free_cell(p);
for(p= q;!is_cls(p);o,p= right(p)){
o,q= mem[p].lit;
touch(q);
o,ccbits|= litsig(q);
}
o,clssig(cc)= ccbits;
/*64:*/
#line 1423 "sat12.w"

oo,size(cc)--;
if(size(cc)<=1){
if(size(cc)==0)confusion("strengthening");
oo,w= mem[right(cc)].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",cc,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1430 "sat12.w"
;
}

/*:64*/
#line 1414 "sat12.w"
;
if(o,slink(cc)==0)o,slink(cc)= strengthened,strengthened= cc;
}

/*:63*/
#line 1358 "sat12.w"
;
}
}
}

/*:61*/
#line 1450 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1451 "sat12.w"
;
o,clstime(c)= time;
o,newsize(c)= 0;
}
}
}
}

/*:65*/
#line 1892 "sat12.w"
;
for(time= 1;time<=maxrounds;time++){
int progress= vars_gone;
if(verbose&show_rounds)
fprintf(stderr,
"beginning round "O"u ("O"d vars, "O"d clauses gone, "O"llu mems)\n",
time,vars_gone,clauses_gone,mems);
/*83:*/
#line 1746 "sat12.w"

/*84:*/
#line 1759 "sat12.w"

for(b= 2;b<=buckets;b++)o,bucket[b]= 0;
for(x= vars;x;x--){
if(o,vmem[x].stable)continue;
if(vmem[x].status)confusion("touched and eliminated");
l= pos_lit(x);
oo,p= occurs(l),q= occurs(l+1);
if(p> cutoff&&q> cutoff)goto reject;
b= p+q;
if((ullng)p*q> b+optimism)goto reject;
if(b> buckets)b= buckets;
oo,vmem[x].blink= bucket[b];
o,bucket[b]= x;continue;
reject:o,vmem[x].stable= 1;
}

/*:84*/
#line 1747 "sat12.w"
;
for(b= 2;b<=buckets;b++)if(o,bucket[b]){
for(x= bucket[b];x;o,x= vmem[x].blink)if(o,vmem[x].stable==0){
if(sanity_checking)sanity();
/*78:*/
#line 1663 "sat12.w"

l= pos_lit(x);
oo,clauses_saved= occurs(l)+occurs(l+1);
if((occurs(l)> cutoff)&&(occurs(l+1)> cutoff))goto elim_done;
if((ullng)occurs(l)*occurs(l+1)> occurs(l)+occurs(l+1)+optimism)goto elim_done;
elim_tries++;
/*74:*/
#line 1602 "sat12.w"

{
register ullng stbits= 0;
beta0= 0,stamp++;
ll= bar(l);
/*75:*/
#line 1623 "sat12.w"

for(o,p= down(l);!is_lit(p);o,p= down(p)){
if(oo,size(mem[p].cls)==2){
o,q= right(p);
if(is_cls(q))o,q= left(p);
oo,lmem[mem[q].lit].extra= stamp;
o,stbits|= litsig(bar(mem[q].lit));
}
}

/*:75*/
#line 1607 "sat12.w"
;
if(stbits){
o,stbits|= litsig(ll);
for(o,p= down(ll);!is_lit(p);o,p= down(p)){
o,c= mem[p].cls;
if(o,(clssig(c)&~stbits)==0)
/*76:*/
#line 1633 "sat12.w"

{
for(o,q= left(p);q!=p;o,q= left(q)){
if(is_cls(q))continue;
if(oo,lmem[bar(mem[q].lit)].extra!=stamp)break;
}
if(q==p){
beta0= c;
break;
}
}

/*:76*/
#line 1614 "sat12.w"
;
}
}
if(beta0){
stamp++;
/*77:*/
#line 1645 "sat12.w"

if(mem[p].cls!=beta0||mem[p].lit!=ll)confusion("partitioning");
for(o,q= left(p);q!=p;o,q= left(q)){
if(is_cls(q))continue;
oo,lmem[bar(mem[q].lit)].extra= stamp;
}

/*:77*/
#line 1619 "sat12.w"
;
}
}

/*:74*/
#line 1669 "sat12.w"
;
if(beta0==0){
l++;
/*74:*/
#line 1602 "sat12.w"

{
register ullng stbits= 0;
beta0= 0,stamp++;
ll= bar(l);
/*75:*/
#line 1623 "sat12.w"

for(o,p= down(l);!is_lit(p);o,p= down(p)){
if(oo,size(mem[p].cls)==2){
o,q= right(p);
if(is_cls(q))o,q= left(p);
oo,lmem[mem[q].lit].extra= stamp;
o,stbits|= litsig(bar(mem[q].lit));
}
}

/*:75*/
#line 1607 "sat12.w"
;
if(stbits){
o,stbits|= litsig(ll);
for(o,p= down(ll);!is_lit(p);o,p= down(p)){
o,c= mem[p].cls;
if(o,(clssig(c)&~stbits)==0)
/*76:*/
#line 1633 "sat12.w"

{
for(o,q= left(p);q!=p;o,q= left(q)){
if(is_cls(q))continue;
if(oo,lmem[bar(mem[q].lit)].extra!=stamp)break;
}
if(q==p){
beta0= c;
break;
}
}

/*:76*/
#line 1614 "sat12.w"
;
}
}
if(beta0){
stamp++;
/*77:*/
#line 1645 "sat12.w"

if(mem[p].cls!=beta0||mem[p].lit!=ll)confusion("partitioning");
for(o,q= left(p);q!=p;o,q= left(q)){
if(is_cls(q))continue;
oo,lmem[bar(mem[q].lit)].extra= stamp;
}

/*:77*/
#line 1619 "sat12.w"
;
}
}

/*:74*/
#line 1672 "sat12.w"
;
}
if(beta0)func_total++;
if(verbose&show_restrials){
if(beta0)fprintf(stderr," maybe elim "O"s ("O"u,"O"d)\n",
vmem[x].name.ch8,beta0,size(beta0)-1);
else fprintf(stderr," maybe elim "O"s\n",vmem[x].name.ch8);
}
last_new= 0;
for(o,alf= down(l);!is_lit(alf);o,alf= down(alf)){
o,c= mem[alf].cls;
/*79:*/
#line 1701 "sat12.w"

if(beta0==0)alpha0= 1;
else{
alpha0= 0;
if(o,size(c)==2){
o,q= right(c);
if(q==alf)q= left(c);
if(oo,lmem[mem[q].lit].extra==stamp)
alpha0= 1;
}
}

/*:79*/
#line 1683 "sat12.w"
;
for(o,bet= down(ll);!is_lit(bet);o,bet= down(bet)){
o,cc= mem[bet].cls;
if(cc==beta0&&alpha0)continue;
if(cc!=beta0&&!alpha0)continue;
/*66:*/
#line 1492 "sat12.w"

p= 1;
oo,v= left(c),u= mem[v].lit;
oo,vv= left(cc),uu= mem[vv].lit;
while(u+uu){
if(u==uu)/*72:*/
#line 1539 "sat12.w"

{
q= p,p= get_cell();
oo,left(q)= p,mem[p].lit= u;
/*69:*/
#line 1519 "sat12.w"

{
/*67:*/
#line 1505 "sat12.w"

{
o,v= left(v);
if(is_cls(v))u= 0;
else o,u= mem[v].lit;
}

/*:67*/
#line 1521 "sat12.w"
;
/*68:*/
#line 1512 "sat12.w"

{
o,vv= left(vv);
if(is_cls(vv))uu= 0;
else o,uu= mem[vv].lit;
}

/*:68*/
#line 1522 "sat12.w"
;
}

/*:69*/
#line 1543 "sat12.w"
;
}

/*:72*/
#line 1497 "sat12.w"

else if(u==bar(uu)){
if(u==l)/*69:*/
#line 1519 "sat12.w"

{
/*67:*/
#line 1505 "sat12.w"

{
o,v= left(v);
if(is_cls(v))u= 0;
else o,u= mem[v].lit;
}

/*:67*/
#line 1521 "sat12.w"
;
/*68:*/
#line 1512 "sat12.w"

{
o,vv= left(vv);
if(is_cls(vv))uu= 0;
else o,uu= mem[vv].lit;
}

/*:68*/
#line 1522 "sat12.w"
;
}

/*:69*/
#line 1499 "sat12.w"

else/*73:*/
#line 1546 "sat12.w"

{
if(p!=1)o,free_cells(p,left(1));
p= 0;
break;
}

/*:73*/
#line 1500 "sat12.w"
;
}else if(u> uu)/*70:*/
#line 1525 "sat12.w"

{
q= p,p= get_cell();
oo,left(q)= p,mem[p].lit= u;
/*67:*/
#line 1505 "sat12.w"

{
o,v= left(v);
if(is_cls(v))u= 0;
else o,u= mem[v].lit;
}

/*:67*/
#line 1529 "sat12.w"
;
}

/*:70*/
#line 1501 "sat12.w"

else/*71:*/
#line 1532 "sat12.w"

{
q= p,p= get_cell();
oo,left(q)= p,mem[p].lit= uu;
/*68:*/
#line 1512 "sat12.w"

{
o,vv= left(vv);
if(is_cls(vv))uu= 0;
else o,uu= mem[vv].lit;
}

/*:68*/
#line 1536 "sat12.w"
;
}

/*:71*/
#line 1502 "sat12.w"
;
}

/*:66*/
#line 1688 "sat12.w"
;
if(p){
o,left(p)= 0;
oo,down(last_new)= left(1);
o,last_new= left(1),right(last_new)= p;
if(--clauses_saved<0)
/*80:*/
#line 1715 "sat12.w"

{
for(o,p= down(0);;o,p= down(p)){
o,free_cells(right(p),p);
if(p==last_new)break;
}
goto elim_done;
}

/*:80*/
#line 1694 "sat12.w"
;
up(last_new)= c,mem[last_new].cls= cc;
}
}
}
o,down(last_new)= 0;

/*:78*/
#line 1751 "sat12.w"
;
/*85:*/
#line 1775 "sat12.w"

if(verbose&show_details){
fprintf(stderr,"elimination of "O"s",vmem[x].name.ch8);
if(beta0)fprintf(stderr," ("O"u,"O"d)",beta0,size(beta0)-1);
fprintf(stderr," saves "O"d clause"O"s\n",
clauses_saved,clauses_saved==1?"":"s");
}
if(verbose&show_resolutions)
print_clauses_for(pos_lit(x)),print_clauses_for(neg_lit(x));
/*86:*/
#line 1796 "sat12.w"

if(beta0){
fprintf(erp_file,""O"s"O".8s <-1\n",litname(l));
for(o,q= right(beta0);!is_cls(q);o,q= right(q))
if(o,mem[q].lit!=ll)
fprintf(erp_file," "O"s"O".8s",litname(mem[q].lit));
fprintf(erp_file,"\n");
}else{
o,k= occurs(l),v= l;
if(o,k> occurs(ll))k= occurs(ll),v= ll;
fprintf(erp_file,""O"s"O".8s <-"O"d\n",litname(bar(v)),k);
for(o,p= down(v);!is_lit(p);o,p= down(p)){
for(o,q= right(p);q!=p;o,q= right(q))
if(!is_cls(q))o,fprintf(erp_file," "O"s"O".8s",litname(mem[q].lit));
fprintf(erp_file,"\n");
}
}

/*:86*/
#line 1784 "sat12.w"
;
oo,down(last_new)= 0,last_new= down(0);
v= pos_lit(x);
/*87:*/
#line 1819 "sat12.w"

for(o,p= down(v);!is_lit(p);o,p= down(p)){
o,c= mem[p].cls;
o,q= right(c),r= left(c);
oo,left(q)= r,right(r)= q;
/*89:*/
#line 1850 "sat12.w"

if(last_new){
if(verbose&show_details)
fprintf(stderr,"clause "O"u now "O"u res "O"u\n",
c,up(last_new),mem[last_new].cls);
o,pp= down(last_new);
/*90:*/
#line 1863 "sat12.w"

for(q= last_new,r= c,s= 0,bits= 0;q;o,r= q,q= left(q)){
o,u= mem[q].lit;
oo,occurs(u)++;
o,w= up(u);
oo,up(u)= down(w)= q;
o,up(q)= w,down(q)= u;
o,bits|= litsig(u);
o,right(q)= r;
o,mem[q].cls= c;
s++;
}
oo,size(c)= s,clssig(c)= bits;
oo,left(c)= last_new,right(c)= r,left(r)= c;
if(s==1){
o,w= mem[r].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is just "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1881 "sat12.w"
;
}

/*:90*/
#line 1856 "sat12.w"
;
if(verbose&show_resolutions)
print_clause(c);
o,newsize(c)= 1;
o,last_new= pp;
}else o,size(c)= 0;

/*:89*/
#line 1824 "sat12.w"
;
}

/*:87*/
#line 1787 "sat12.w"
;
v++;
/*87:*/
#line 1819 "sat12.w"

for(o,p= down(v);!is_lit(p);o,p= down(p)){
o,c= mem[p].cls;
o,q= right(c),r= left(c);
oo,left(q)= r,right(r)= q;
/*89:*/
#line 1850 "sat12.w"

if(last_new){
if(verbose&show_details)
fprintf(stderr,"clause "O"u now "O"u res "O"u\n",
c,up(last_new),mem[last_new].cls);
o,pp= down(last_new);
/*90:*/
#line 1863 "sat12.w"

for(q= last_new,r= c,s= 0,bits= 0;q;o,r= q,q= left(q)){
o,u= mem[q].lit;
oo,occurs(u)++;
o,w= up(u);
oo,up(u)= down(w)= q;
o,up(q)= w,down(q)= u;
o,bits|= litsig(u);
o,right(q)= r;
o,mem[q].cls= c;
s++;
}
oo,size(c)= s,clssig(c)= bits;
oo,left(c)= last_new,right(c)= r,left(r)= c;
if(s==1){
o,w= mem[r].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is just "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1881 "sat12.w"
;
}

/*:90*/
#line 1856 "sat12.w"
;
if(verbose&show_resolutions)
print_clause(c);
o,newsize(c)= 1;
o,last_new= pp;
}else o,size(c)= 0;

/*:89*/
#line 1824 "sat12.w"
;
}

/*:87*/
#line 1789 "sat12.w"
;
/*88:*/
#line 1830 "sat12.w"

for(o,p= down(v);!is_lit(p);o,p= down(p)){
for(o,q= right(p);q!=p;o,q= right(q)){
o,r= up(q),w= down(q);
oo,down(r)= w,up(w)= r;
o,w= mem[q].lit;
touch(w);
oo,occurs(w)--,littime(w)= time;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1841 "sat12.w"
;
}
}
free_cells(right(p),p);
}

/*:88*/
#line 1790 "sat12.w"
;
v--;
/*88:*/
#line 1830 "sat12.w"

for(o,p= down(v);!is_lit(p);o,p= down(p)){
for(o,q= right(p);q!=p;o,q= right(q)){
o,r= up(q),w= down(q);
oo,down(r)= w,up(w)= r;
o,w= mem[q].lit;
touch(w);
oo,occurs(w)--,littime(w)= time;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1841 "sat12.w"
;
}
}
free_cells(right(p),p);
}

/*:88*/
#line 1792 "sat12.w"
;
o,vmem[x].status= elim_res,vars_gone++;
clauses_gone+= clauses_saved;

/*:85*/
#line 1752 "sat12.w"
;
if(sanity_checking)sanity();
/*65:*/
#line 1437 "sat12.w"

{
register uint c;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1440 "sat12.w"
;
while(strengthened!=sentinel){
c= strengthened;
o,strengthened= slink(c);
if(o,size(c)){
o,slink(c)= 0;
/*57:*/
#line 1260 "sat12.w"

if(verbose&show_subtrials)
fprintf(stderr," trying subsumption by "O"u\n",c);
/*58:*/
#line 1278 "sat12.w"

ooo,p= right(c),l= mem[p].lit,k= occurs(l);
for(o,p= right(p);!is_cls(p);o,p= right(p)){
o,ll= mem[p].lit;
if(o,occurs(ll)<k)k= occurs(ll),l= ll;
}

/*:58*/
#line 1263 "sat12.w"
;
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,pp= down(l);!is_lit(pp);o,pp= down(pp)){
o,cc= mem[pp].cls;
if(cc==c)continue;
sub_tries++;
if(o,bits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*59:*/
#line 1288 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:59*/
#line 1271 "sat12.w"
;
if(l> ll)sub_false++;
else/*60:*/
#line 1308 "sat12.w"

{
if(verbose&show_details)
fprintf(stderr,"clause "O"u subsumes clause "O"u\n",c,cc);
sub_total++;
for(o,p= right(cc);!is_cls(p);o,p= right(p)){
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
o,w= mem[p].lit;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(q));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1322 "sat12.w"
;
}
}
free_cells(right(cc),left(cc));
o,size(cc)= 0,clauses_gone++;
}

/*:60*/
#line 1273 "sat12.w"
;
}

/*:57*/
#line 1446 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1447 "sat12.w"
;
if(o,size(c)){
specialcase= 0;
/*61:*/
#line 1340 "sat12.w"

{
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,vv= v;!is_cls(vv);o,vv= left(vv)){
register ullng ubits;
o,u= mem[vv].lit;
if(specialcase)/*95:*/
#line 1968 "sat12.w"

{
if(o,littime(bar(u))!=time)continue;
if(o,newsize(c)>>2!=size(c)-(littime(u)!=time))continue;

}

/*:95*/
#line 1346 "sat12.w"
;
if(verbose&show_subtrials)
fprintf(stderr," trying to strengthen by "O"u and "O"s"O".8s\n",
c,litname(u));
o,ubits= bits&~litsig(u);
for(o,pp= down(bar(u));!is_lit(pp);o,pp= down(pp)){
str_tries++;
o,cc= mem[pp].cls;
if(o,ubits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*62:*/
#line 1363 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
if(l==u)l= bar(l);
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:62*/
#line 1356 "sat12.w"
;
if(l> ll)str_false++;
else/*63:*/
#line 1384 "sat12.w"

{
register ullng ccbits= 0;
if(verbose&show_details)
fprintf(stderr,"clause "O"u loses literal "O"s"O".8s via clause "O"u\n",
cc,litname(bar(u)),c);
str_total++;
for(o,p= right(cc);;o,p= right(p)){
o,w= mem[p].lit;
touch(w);
if(w==bar(u))break;
o,ccbits|= litsig(w);
}
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1401 "sat12.w"
;
}
o,q= up(p),w= down(p);
oo,down(q)= w,up(w)= q;
o,q= right(p),w= left(p);
oo,left(q)= w,right(w)= q;
free_cell(p);
for(p= q;!is_cls(p);o,p= right(p)){
o,q= mem[p].lit;
touch(q);
o,ccbits|= litsig(q);
}
o,clssig(cc)= ccbits;
/*64:*/
#line 1423 "sat12.w"

oo,size(cc)--;
if(size(cc)<=1){
if(size(cc)==0)confusion("strengthening");
oo,w= mem[right(cc)].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",cc,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1430 "sat12.w"
;
}

/*:64*/
#line 1414 "sat12.w"
;
if(o,slink(cc)==0)o,slink(cc)= strengthened,strengthened= cc;
}

/*:63*/
#line 1358 "sat12.w"
;
}
}
}

/*:61*/
#line 1450 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1451 "sat12.w"
;
o,clstime(c)= time;
o,newsize(c)= 0;
}
}
}
}

/*:65*/
#line 1754 "sat12.w"
;
elim_done:o,vmem[x].stable= 1;
}
}

/*:83*/
#line 1899 "sat12.w"
;
if(progress==vars_gone||vars_gone==vars)break;
/*93:*/
#line 1937 "sat12.w"

for(l= 2;is_lit(l);l++){
if((l&1)==0&&(o,vmem[thevar(l)].status)){
l++;continue;
}
if(o,littime(l)==time)/*96:*/
#line 1975 "sat12.w"

{
for(o,p= down(l);!is_lit(p);o,p= down(p)){
o,c= mem[p].cls;
oo,newsize(c)+= 4;
}
for(o,p= down(bar(l));!is_lit(p);o,p= down(p)){
o,c= mem[p].cls;
oo,newsize(c)|= 2;
}
}

/*:96*/
#line 1942 "sat12.w"
;
}
for(c= lit_head_top;is_cls(c);c++)if(o,size(c)){
if(clstime(c)<time){
if(o,size(c)==newsize(c)>>2){
/*57:*/
#line 1260 "sat12.w"

if(verbose&show_subtrials)
fprintf(stderr," trying subsumption by "O"u\n",c);
/*58:*/
#line 1278 "sat12.w"

ooo,p= right(c),l= mem[p].lit,k= occurs(l);
for(o,p= right(p);!is_cls(p);o,p= right(p)){
o,ll= mem[p].lit;
if(o,occurs(ll)<k)k= occurs(ll),l= ll;
}

/*:58*/
#line 1263 "sat12.w"
;
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,pp= down(l);!is_lit(pp);o,pp= down(pp)){
o,cc= mem[pp].cls;
if(cc==c)continue;
sub_tries++;
if(o,bits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*59:*/
#line 1288 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:59*/
#line 1271 "sat12.w"
;
if(l> ll)sub_false++;
else/*60:*/
#line 1308 "sat12.w"

{
if(verbose&show_details)
fprintf(stderr,"clause "O"u subsumes clause "O"u\n",c,cc);
sub_total++;
for(o,p= right(cc);!is_cls(p);o,p= right(p)){
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
o,w= mem[p].lit;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(q));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1322 "sat12.w"
;
}
}
free_cells(right(cc),left(cc));
o,size(cc)= 0,clauses_gone++;
}

/*:60*/
#line 1273 "sat12.w"
;
}

/*:57*/
#line 1947 "sat12.w"
;
/*65:*/
#line 1437 "sat12.w"

{
register uint c;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1440 "sat12.w"
;
while(strengthened!=sentinel){
c= strengthened;
o,strengthened= slink(c);
if(o,size(c)){
o,slink(c)= 0;
/*57:*/
#line 1260 "sat12.w"

if(verbose&show_subtrials)
fprintf(stderr," trying subsumption by "O"u\n",c);
/*58:*/
#line 1278 "sat12.w"

ooo,p= right(c),l= mem[p].lit,k= occurs(l);
for(o,p= right(p);!is_cls(p);o,p= right(p)){
o,ll= mem[p].lit;
if(o,occurs(ll)<k)k= occurs(ll),l= ll;
}

/*:58*/
#line 1263 "sat12.w"
;
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,pp= down(l);!is_lit(pp);o,pp= down(pp)){
o,cc= mem[pp].cls;
if(cc==c)continue;
sub_tries++;
if(o,bits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*59:*/
#line 1288 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:59*/
#line 1271 "sat12.w"
;
if(l> ll)sub_false++;
else/*60:*/
#line 1308 "sat12.w"

{
if(verbose&show_details)
fprintf(stderr,"clause "O"u subsumes clause "O"u\n",c,cc);
sub_total++;
for(o,p= right(cc);!is_cls(p);o,p= right(p)){
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
o,w= mem[p].lit;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(q));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1322 "sat12.w"
;
}
}
free_cells(right(cc),left(cc));
o,size(cc)= 0,clauses_gone++;
}

/*:60*/
#line 1273 "sat12.w"
;
}

/*:57*/
#line 1446 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1447 "sat12.w"
;
if(o,size(c)){
specialcase= 0;
/*61:*/
#line 1340 "sat12.w"

{
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,vv= v;!is_cls(vv);o,vv= left(vv)){
register ullng ubits;
o,u= mem[vv].lit;
if(specialcase)/*95:*/
#line 1968 "sat12.w"

{
if(o,littime(bar(u))!=time)continue;
if(o,newsize(c)>>2!=size(c)-(littime(u)!=time))continue;

}

/*:95*/
#line 1346 "sat12.w"
;
if(verbose&show_subtrials)
fprintf(stderr," trying to strengthen by "O"u and "O"s"O".8s\n",
c,litname(u));
o,ubits= bits&~litsig(u);
for(o,pp= down(bar(u));!is_lit(pp);o,pp= down(pp)){
str_tries++;
o,cc= mem[pp].cls;
if(o,ubits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*62:*/
#line 1363 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
if(l==u)l= bar(l);
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:62*/
#line 1356 "sat12.w"
;
if(l> ll)str_false++;
else/*63:*/
#line 1384 "sat12.w"

{
register ullng ccbits= 0;
if(verbose&show_details)
fprintf(stderr,"clause "O"u loses literal "O"s"O".8s via clause "O"u\n",
cc,litname(bar(u)),c);
str_total++;
for(o,p= right(cc);;o,p= right(p)){
o,w= mem[p].lit;
touch(w);
if(w==bar(u))break;
o,ccbits|= litsig(w);
}
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1401 "sat12.w"
;
}
o,q= up(p),w= down(p);
oo,down(q)= w,up(w)= q;
o,q= right(p),w= left(p);
oo,left(q)= w,right(w)= q;
free_cell(p);
for(p= q;!is_cls(p);o,p= right(p)){
o,q= mem[p].lit;
touch(q);
o,ccbits|= litsig(q);
}
o,clssig(cc)= ccbits;
/*64:*/
#line 1423 "sat12.w"

oo,size(cc)--;
if(size(cc)<=1){
if(size(cc)==0)confusion("strengthening");
oo,w= mem[right(cc)].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",cc,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1430 "sat12.w"
;
}

/*:64*/
#line 1414 "sat12.w"
;
if(o,slink(cc)==0)o,slink(cc)= strengthened,strengthened= cc;
}

/*:63*/
#line 1358 "sat12.w"
;
}
}
}

/*:61*/
#line 1450 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1451 "sat12.w"
;
o,clstime(c)= time;
o,newsize(c)= 0;
}
}
}
}

/*:65*/
#line 1948 "sat12.w"
;
}else if(newsize(c)&1)confusion("new clause not all new");
if(newsize(c)&0x3)/*94:*/
#line 1955 "sat12.w"

{
if(newsize(c)&1)specialcase= 0;
else{
if(newsize(c)>>2<size(c)-1)specialcase= -1;
else specialcase= 1;
}
if(specialcase>=0){
/*61:*/
#line 1340 "sat12.w"

{
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,vv= v;!is_cls(vv);o,vv= left(vv)){
register ullng ubits;
o,u= mem[vv].lit;
if(specialcase)/*95:*/
#line 1968 "sat12.w"

{
if(o,littime(bar(u))!=time)continue;
if(o,newsize(c)>>2!=size(c)-(littime(u)!=time))continue;

}

/*:95*/
#line 1346 "sat12.w"
;
if(verbose&show_subtrials)
fprintf(stderr," trying to strengthen by "O"u and "O"s"O".8s\n",
c,litname(u));
o,ubits= bits&~litsig(u);
for(o,pp= down(bar(u));!is_lit(pp);o,pp= down(pp)){
str_tries++;
o,cc= mem[pp].cls;
if(o,ubits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*62:*/
#line 1363 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
if(l==u)l= bar(l);
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:62*/
#line 1356 "sat12.w"
;
if(l> ll)str_false++;
else/*63:*/
#line 1384 "sat12.w"

{
register ullng ccbits= 0;
if(verbose&show_details)
fprintf(stderr,"clause "O"u loses literal "O"s"O".8s via clause "O"u\n",
cc,litname(bar(u)),c);
str_total++;
for(o,p= right(cc);;o,p= right(p)){
o,w= mem[p].lit;
touch(w);
if(w==bar(u))break;
o,ccbits|= litsig(w);
}
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1401 "sat12.w"
;
}
o,q= up(p),w= down(p);
oo,down(q)= w,up(w)= q;
o,q= right(p),w= left(p);
oo,left(q)= w,right(w)= q;
free_cell(p);
for(p= q;!is_cls(p);o,p= right(p)){
o,q= mem[p].lit;
touch(q);
o,ccbits|= litsig(q);
}
o,clssig(cc)= ccbits;
/*64:*/
#line 1423 "sat12.w"

oo,size(cc)--;
if(size(cc)<=1){
if(size(cc)==0)confusion("strengthening");
oo,w= mem[right(cc)].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",cc,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1430 "sat12.w"
;
}

/*:64*/
#line 1414 "sat12.w"
;
if(o,slink(cc)==0)o,slink(cc)= strengthened,strengthened= cc;
}

/*:63*/
#line 1358 "sat12.w"
;
}
}
}

/*:61*/
#line 1963 "sat12.w"
;
/*65:*/
#line 1437 "sat12.w"

{
register uint c;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1440 "sat12.w"
;
while(strengthened!=sentinel){
c= strengthened;
o,strengthened= slink(c);
if(o,size(c)){
o,slink(c)= 0;
/*57:*/
#line 1260 "sat12.w"

if(verbose&show_subtrials)
fprintf(stderr," trying subsumption by "O"u\n",c);
/*58:*/
#line 1278 "sat12.w"

ooo,p= right(c),l= mem[p].lit,k= occurs(l);
for(o,p= right(p);!is_cls(p);o,p= right(p)){
o,ll= mem[p].lit;
if(o,occurs(ll)<k)k= occurs(ll),l= ll;
}

/*:58*/
#line 1263 "sat12.w"
;
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,pp= down(l);!is_lit(pp);o,pp= down(pp)){
o,cc= mem[pp].cls;
if(cc==c)continue;
sub_tries++;
if(o,bits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*59:*/
#line 1288 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:59*/
#line 1271 "sat12.w"
;
if(l> ll)sub_false++;
else/*60:*/
#line 1308 "sat12.w"

{
if(verbose&show_details)
fprintf(stderr,"clause "O"u subsumes clause "O"u\n",c,cc);
sub_total++;
for(o,p= right(cc);!is_cls(p);o,p= right(p)){
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
o,w= mem[p].lit;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(q));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1322 "sat12.w"
;
}
}
free_cells(right(cc),left(cc));
o,size(cc)= 0,clauses_gone++;
}

/*:60*/
#line 1273 "sat12.w"
;
}

/*:57*/
#line 1446 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1447 "sat12.w"
;
if(o,size(c)){
specialcase= 0;
/*61:*/
#line 1340 "sat12.w"

{
ooo,s= size(c),bits= clssig(c),v= left(c);
for(o,vv= v;!is_cls(vv);o,vv= left(vv)){
register ullng ubits;
o,u= mem[vv].lit;
if(specialcase)/*95:*/
#line 1968 "sat12.w"

{
if(o,littime(bar(u))!=time)continue;
if(o,newsize(c)>>2!=size(c)-(littime(u)!=time))continue;

}

/*:95*/
#line 1346 "sat12.w"
;
if(verbose&show_subtrials)
fprintf(stderr," trying to strengthen by "O"u and "O"s"O".8s\n",
c,litname(u));
o,ubits= bits&~litsig(u);
for(o,pp= down(bar(u));!is_lit(pp);o,pp= down(pp)){
str_tries++;
o,cc= mem[pp].cls;
if(o,ubits&~clssig(cc))continue;
if(o,size(cc)<s)continue;
/*62:*/
#line 1363 "sat12.w"

o,q= v,qq= left(cc);
while(1){
oo,l= mem[q].lit,ll= mem[qq].lit;
if(l==u)l= bar(l);
while(l<ll){
o,qq= left(qq);
if(is_cls(qq))ll= 0;
else o,ll= mem[qq].lit;
}
if(l> ll)break;
o,q= left(q);
if(is_cls(q)){
l= 0;break;
}
o,qq= left(qq);
if(is_cls(qq)){
ll= 0;break;
}
}

/*:62*/
#line 1356 "sat12.w"
;
if(l> ll)str_false++;
else/*63:*/
#line 1384 "sat12.w"

{
register ullng ccbits= 0;
if(verbose&show_details)
fprintf(stderr,"clause "O"u loses literal "O"s"O".8s via clause "O"u\n",
cc,litname(bar(u)),c);
str_total++;
for(o,p= right(cc);;o,p= right(p)){
o,w= mem[p].lit;
touch(w);
if(w==bar(u))break;
o,ccbits|= litsig(w);
}
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1401 "sat12.w"
;
}
o,q= up(p),w= down(p);
oo,down(q)= w,up(w)= q;
o,q= right(p),w= left(p);
oo,left(q)= w,right(w)= q;
free_cell(p);
for(p= q;!is_cls(p);o,p= right(p)){
o,q= mem[p].lit;
touch(q);
o,ccbits|= litsig(q);
}
o,clssig(cc)= ccbits;
/*64:*/
#line 1423 "sat12.w"

oo,size(cc)--;
if(size(cc)<=1){
if(size(cc)==0)confusion("strengthening");
oo,w= mem[right(cc)].lit;
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",cc,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1430 "sat12.w"
;
}

/*:64*/
#line 1414 "sat12.w"
;
if(o,slink(cc)==0)o,slink(cc)= strengthened,strengthened= cc;
}

/*:63*/
#line 1358 "sat12.w"
;
}
}
}

/*:61*/
#line 1450 "sat12.w"
;
/*53:*/
#line 1168 "sat12.w"

while(to_do){
register uint c;
k= to_do;
o,to_do= vmem[k].link;
if(vmem[k].status!=elim_quiet){
l= vmem[k].status==forced_true?pos_lit(k):neg_lit(k);
fprintf(erp_file,""O"s"O".8s <-0\n",litname(l));
o,vmem[k].stable= 1;
/*56:*/
#line 1215 "sat12.w"

for(o,ll= down(l);!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
if(verbose&show_details)
fprintf(stderr,"clause "O"u is satisfied by "O"s"O".8s\n",c,litname(l));
for(o,p= right(c);!is_cls(p);o,p= right(p))if(p!=ll){
o,w= mem[p].lit;
o,q= up(p),r= down(p);
oo,down(q)= r,up(r)= q;
touch(w);
oo,occurs(w)--;
if(occurs(w)==0){
if(verbose&show_details)
fprintf(stderr,"literal "O"s"O".8s no longer appears\n",litname(w));
/*51:*/
#line 1138 "sat12.w"

{
register int k= thevar(w);
if(o,vmem[k].status==norm){
o,vmem[k].status= (w&1?forced_true:forced_false);
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==(w&1?forced_false:forced_true))
o,vmem[k].status= elim_quiet,vmem[k].stable= 1;
}

/*:51*/
#line 1229 "sat12.w"
;
}
}
free_cells(right(c),left(c));
o,size(c)= 0,clauses_gone++;
}

/*:56*/
#line 1177 "sat12.w"
;
/*54:*/
#line 1188 "sat12.w"

for(o,ll= down(bar(l));!is_lit(ll);o,ll= down(ll)){
o,c= mem[ll].cls;
o,p= left(ll),q= right(ll);
oo,right(p)= q,left(q)= p;
free_cell(ll);
o,j= size(c)-1;
o,size(c)= j;
if(j==1){
o,w= (p==c?mem[q].lit:mem[p].lit);
if(verbose&show_details)
fprintf(stderr,"clause "O"u reduces to "O"s"O".8s\n",c,litname(w));
/*50:*/
#line 1110 "sat12.w"

{
register int k= thevar(w);
if(w&1){
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_false;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_true)goto unsat;
}else{
if(o,vmem[k].status==norm){
o,vmem[k].status= forced_true;
vmem[k].link= to_do,to_do= k;
}else if(vmem[k].status==forced_false)goto unsat;
}
}

/*:50*/
#line 1200 "sat12.w"
;
}
/*55:*/
#line 1206 "sat12.w"

{
register ullng bits= 0;
register uint t;
for(o,t= right(c);!is_cls(t);o,t= right(t))
oo,bits|= litsig(mem[t].lit);
o,clssig(c)= bits;
}

/*:55*/
#line 1202 "sat12.w"
;
if(o,slink(c)==0)o,slink(c)= strengthened,strengthened= c;
}

/*:54*/
#line 1178 "sat12.w"
;
}
vars_gone++;
if(sanity_checking)sanity();
}
if(mems> timeout){
if(verbose&show_basics)fprintf(stderr,"Timeout!\n");
goto finish_up;
}

/*:53*/
#line 1451 "sat12.w"
;
o,clstime(c)= time;
o,newsize(c)= 0;
}
}
}
}

/*:65*/
#line 1964 "sat12.w"
;
}
}

/*:94*/
#line 1950 "sat12.w"
;
}
o,newsize(c)= 0;
}

/*:93*/
#line 1901 "sat12.w"
;
}
if(time> maxrounds)time= maxrounds;

/*:91*/
#line 134 "sat12.w"
;
finish_up:/*97:*/
#line 1987 "sat12.w"

for(c= lit_head_top;is_cls(c);c++)if(o,size(c)){
for(o,p= right(c);!is_cls(p);o,p= right(p)){
o,l= mem[p].lit;
printf(" "O"s"O".8s",litname(l));
}
printf("\n");
}
if(vars_gone==vars){
if(clauses_gone!=clauses)confusion("vars gone but not clauses");
if(verbose&show_basics)
fprintf(stderr,"No clauses remain.\n");
}else if(clauses_gone==clauses)confusion("clauses gone but not vars");
else if(verbose&show_basics)
fprintf(stderr,
""O"d variable"O"s and "O"d clause"O"s removed ("O"d round"O"s).\n",
vars_gone,vars_gone==1?"":"s",clauses_gone,clauses_gone==1?"":"s",
time,time==1?"":"s");
if(0){
unsat:fprintf(stderr,"The clauses are unsatisfiable.\n");
}

/*:97*/
#line 135 "sat12.w"
;
if(verbose&show_basics){
fprintf(stderr,
"Altogether "O"llu+"O"llu mems, "O"llu bytes, "O"u cells;\n",
imems,mems,bytes,xcells);
if(sub_total+str_total)
fprintf(stderr,
" "O"u subsumption"O"s, "O"u strengthening"O"s.\n",
sub_total,sub_total!=1?"s":"",
str_total,str_total!=1?"s":"");
fprintf(stderr," false hit rates "O".3f of "O"llu, "O".3f of "O"llu.\n",
sub_tries?(double)sub_false/(double)sub_tries:0.0,sub_tries,
str_tries?(double)str_false/(double)str_tries:0.0,str_tries);
if(elim_tries)
fprintf(stderr," "O".3f functional dependencies among "O"llu trials.\n",
(double)func_total/(double)elim_tries,elim_tries);
fprintf(stderr,"erp data written to file "O"s.\n",erp_file_name);
}
}

/*:3*/
