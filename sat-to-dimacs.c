#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

/*1:*/
#line 16 "./sat-to-dimacs.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
#include <time.h> 
time_t myclock;
typedef unsigned int uint;
typedef unsigned long long ullng;
/*4:*/
#line 91 "./sat-to-dimacs.w"

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

/*:4*//*5:*/
#line 118 "./sat-to-dimacs.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:5*/
#line 25 "./sat-to-dimacs.w"
;
/*2:*/
#line 39 "./sat-to-dimacs.w"

int random_seed= 0;
int verbose= 1;
int hbits= 8;
int buf_size= 1024;

/*:2*//*6:*/
#line 124 "./sat-to-dimacs.w"

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

/*:6*/
#line 26 "./sat-to-dimacs.w"
;
main(int argc,char*argv[]){
register uint c,h,i,j,k,l,p,q,r,level,kk,pp,qq,ll;
/*3:*/
#line 59 "./sat-to-dimacs.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,"%d",&verbose)-1);break;
case'h':k|= (sscanf(argv[j]+1,"%d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,"%d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,"%d",&random_seed)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0){
fprintf(stderr,"Usage: %s [v<n>] [h<n>] [b<n>] [s<n>] < foo.dat\n",argv[0]);
exit(-1);
}

/*:3*/
#line 29 "./sat-to-dimacs.w"
;
/*7:*/
#line 139 "./sat-to-dimacs.w"

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

/*:7*//*13:*/
#line 277 "./sat-to-dimacs.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:13*/
#line 30 "./sat-to-dimacs.w"
;
/*8:*/
#line 168 "./sat-to-dimacs.w"

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
/*9:*/
#line 205 "./sat-to-dimacs.w"

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
/*10:*/
#line 230 "./sat-to-dimacs.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*11:*/
#line 248 "./sat-to-dimacs.w"

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

/*:11*/
#line 233 "./sat-to-dimacs.w"
;
/*14:*/
#line 281 "./sat-to-dimacs.w"

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

/*:14*/
#line 235 "./sat-to-dimacs.w"
;
/*15:*/
#line 297 "./sat-to-dimacs.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:15*/
#line 236 "./sat-to-dimacs.w"
;
if(p->stamp==clauses||p->stamp==-clauses)/*16:*/
#line 311 "./sat-to-dimacs.w"

{
if((p->stamp> 0)==(i> 0))goto empty_clause;
}

/*:16*/
#line 237 "./sat-to-dimacs.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*12:*/
#line 261 "./sat-to-dimacs.w"

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

/*:12*/
#line 240 "./sat-to-dimacs.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:10*/
#line 216 "./sat-to-dimacs.w"
;
}
if(k==0){
fprintf(stderr,"(Empty line %lld is being ignored)\n",clauses);
nullclauses++;
}
goto clause_done;
empty_clause:/*17:*/
#line 321 "./sat-to-dimacs.w"

while(k){
/*18:*/
#line 331 "./sat-to-dimacs.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:18*/
#line 323 "./sat-to-dimacs.w"
;
k--;
}
if((buf[0]!='~')||(buf[1]!=' '))
fprintf(stderr,"(The clause on line %lld is always satisfied)\n",clauses);
else if(vars==0)printf("c %s",buf+2);
nullclauses++;

/*:17*/
#line 223 "./sat-to-dimacs.w"
;
clause_done:cells+= k;

/*:9*/
#line 179 "./sat-to-dimacs.w"
;
}
if((vars>>hbits)>=10){
fprintf(stderr,"There are %lld variables but only %d hash tables;\n",
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

/*:8*/
#line 31 "./sat-to-dimacs.w"
;
if(verbose)
/*20:*/
#line 352 "./sat-to-dimacs.w"

fprintf(stderr,"(%lld variables, %lld clauses, %llu literals successfully read)\n",
vars,clauses,cells);

/*:20*/
#line 33 "./sat-to-dimacs.w"
;
myclock= time(0);
printf("c file created by SAT-TO-DIMACS %s",ctime(&myclock));
/*21:*/
#line 359 "./sat-to-dimacs.w"

/*22:*/
#line 368 "./sat-to-dimacs.w"

for(c= vars;c;c--){
/*19:*/
#line 343 "./sat-to-dimacs.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:19*/
#line 370 "./sat-to-dimacs.w"
;
printf("c %.8s -> %d\n",cur_tmp_var->name.ch8,c);
}

/*:22*/
#line 360 "./sat-to-dimacs.w"
;
printf("p cnf %lld %lld\n",vars,clauses);
/*23:*/
#line 374 "./sat-to-dimacs.w"

for(c= clauses;c;c--){
/*24:*/
#line 383 "./sat-to-dimacs.w"

for(i= 0;i<2;j++){
/*18:*/
#line 331 "./sat-to-dimacs.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:18*/
#line 385 "./sat-to-dimacs.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
printf(" %s%d",i&1?"-":"",p+1);
}

/*:24*/
#line 376 "./sat-to-dimacs.w"
;
printf(" 0\n");
}

/*:23*/
#line 362 "./sat-to-dimacs.w"
;
/*25:*/
#line 391 "./sat-to-dimacs.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL){
fprintf(stderr,"This can't happen (consistency check failure)!\n");
exit(-14);
}

/*:25*/
#line 363 "./sat-to-dimacs.w"
;

/*:21*/
#line 36 "./sat-to-dimacs.w"
;
}

/*:1*/
