#define O "%" \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define true 1
#define false -1
#define unknown 0
#define thevar(l) ((l) >>1) 
#define bar(l) ((l) ^1) 
#define litname(l) (l) &1?"~":"",vmem[thevar(l) ].ch8 \

#define hack_out(q) (((ullng) q) &0x7) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-8) )  \

/*1:*/
#line 37 "sat12-erp.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*4:*/
#line 122 "sat12-erp.w"

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

/*:4*//*5:*/
#line 149 "sat12-erp.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:5*/
#line 44 "sat12-erp.w"
;
/*2:*/
#line 62 "sat12-erp.w"

int random_seed= 0;
int hbits= 8;
int buf_size= 1024;
FILE*erp_file;
char erp_file_name[100]= "/tmp/erp";

/*:2*//*6:*/
#line 155 "sat12-erp.w"

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
ullng cells;
int kkk;

/*:6*//*24:*/
#line 434 "sat12-erp.w"

octa*vmem;
char*lmem;

/*:24*/
#line 45 "sat12-erp.w"
;
/*29:*/
#line 497 "sat12-erp.w"

void confusion(char*id){
fprintf(stderr,"This can't happen ("O"s)!\n",id);
exit(-69);
}

void debugstop(int foo){
fprintf(stderr,"You rang("O"d)?\n",foo);
}

/*:29*/
#line 46 "sat12-erp.w"
;
main(int argc,char*argv[]){
register uint c,h,i,j,k,kk,l,p,v,vv;
/*3:*/
#line 84 "sat12-erp.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'h':k|= (sscanf(argv[j]+1,""O"d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,""O"d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1);break;
case'e':sprintf(erp_file_name,""O".99s",argv[j]+1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<11){
fprintf(stderr,
"Usage: "O"s [v<n>] [h<n>] [b<n>] [s<n>] [efoo.erp] [m<n>]",argv[0]);
fprintf(stderr," [c<n>] < foo.dat\n");
exit(-1);
}
if(!(erp_file= fopen(erp_file_name,"r"))){
fprintf(stderr,"I couldn't open file "O"s for reading!\n",erp_file_name);
exit(-16);
}

/*:3*/
#line 49 "sat12-erp.w"
;
/*7:*/
#line 170 "sat12-erp.w"

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

/*:7*//*15:*/
#line 326 "sat12-erp.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:15*/
#line 50 "sat12-erp.w"
;
/*8:*/
#line 199 "sat12-erp.w"

while(1){
k= fscanf(erp_file,""O"10s <-"O"d",buf,&kkk);
if(k!=2)break;
clauses++;
/*20:*/
#line 377 "sat12-erp.w"

if(buf[0]=='~')i= j= 1;
else i= j= 0;
/*12:*/
#line 277 "sat12-erp.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 297 "sat12-erp.w"

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
#line 280 "sat12-erp.w"
;
/*16:*/
#line 330 "sat12-erp.w"

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
if(l==0){
fprintf(stderr,"Illegal appearance of ~ on line "O"lld!\n",clauses);
exit(-668);
}
j+= l;
h&= (1<<hbits)-1;

/*:16*/
#line 282 "sat12-erp.w"
;
/*17:*/
#line 349 "sat12-erp.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 283 "sat12-erp.w"
;
if(p->stamp==clauses||p->stamp==-clauses){
fprintf(stderr,"Duplicate literal encountered on line "O"lld!\n",clauses);
exit(-669);
}else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 310 "sat12-erp.w"

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
#line 289 "sat12-erp.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:12*/
#line 380 "sat12-erp.w"
;

/*:20*/
#line 204 "sat12-erp.w"
;
*(cur_cell-1)= hack_in(*(cur_cell-1),4);
if(!fgets(buf,buf_size,erp_file)||buf[0]!='\n')
confusion("erp group intro line format");
/*9:*/
#line 211 "sat12-erp.w"

for(kk= 0;kk<kkk;kk++){
if(!fgets(buf,buf_size,erp_file))break;
clauses++;
if(buf[strlen(buf)-1]!='\n'){
fprintf(stderr,
"The clause on line "O"lld ("O".20s...) is too long for me;\n",clauses,buf);
fprintf(stderr," my buf_size is only "O"d!\n",buf_size);
fprintf(stderr,"Please use the command-line option b<newsize>.\n");
exit(-4);
}
/*11:*/
#line 254 "sat12-erp.w"

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
#line 277 "sat12-erp.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 297 "sat12-erp.w"

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
#line 280 "sat12-erp.w"
;
/*16:*/
#line 330 "sat12-erp.w"

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
if(l==0){
fprintf(stderr,"Illegal appearance of ~ on line "O"lld!\n",clauses);
exit(-668);
}
j+= l;
h&= (1<<hbits)-1;

/*:16*/
#line 282 "sat12-erp.w"
;
/*17:*/
#line 349 "sat12-erp.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 283 "sat12-erp.w"
;
if(p->stamp==clauses||p->stamp==-clauses){
fprintf(stderr,"Duplicate literal encountered on line "O"lld!\n",clauses);
exit(-669);
}else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 310 "sat12-erp.w"

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
#line 289 "sat12-erp.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:12*/
#line 265 "sat12-erp.w"
;
}
if(k==0){
fprintf(stderr,"Empty line "O"lld in file "O"s!\n",clauses,erp_file_name);
exit(-663);
}
cells+= k;

/*:11*/
#line 222 "sat12-erp.w"
;
}
if(kk<kkk){
fprintf(stderr,"file "O"s ended prematurely: "O"d clauses missing!\n",
erp_file_name,kkk-kk);
exit(-667);
}

/*:9*/
#line 208 "sat12-erp.w"
;
}

/*:8*/
#line 51 "sat12-erp.w"
;
if(!clauses)fprintf(stderr,"(The erp file is empty!)\n");
/*21:*/
#line 382 "sat12-erp.w"

clauses++;
k= 0;
while(1){
if(scanf(""O"10s",buf)!=1)break;
if(buf[0]=='~'&&buf[1]==0){
printf("~\n");
exit(0);
}
/*20:*/
#line 377 "sat12-erp.w"

if(buf[0]=='~')i= j= 1;
else i= j= 0;
/*12:*/
#line 277 "sat12-erp.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*13:*/
#line 297 "sat12-erp.w"

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
#line 280 "sat12-erp.w"
;
/*16:*/
#line 330 "sat12-erp.w"

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
if(l==0){
fprintf(stderr,"Illegal appearance of ~ on line "O"lld!\n",clauses);
exit(-668);
}
j+= l;
h&= (1<<hbits)-1;

/*:16*/
#line 282 "sat12-erp.w"
;
/*17:*/
#line 349 "sat12-erp.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:17*/
#line 283 "sat12-erp.w"
;
if(p->stamp==clauses||p->stamp==-clauses){
fprintf(stderr,"Duplicate literal encountered on line "O"lld!\n",clauses);
exit(-669);
}else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*14:*/
#line 310 "sat12-erp.w"

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
#line 289 "sat12-erp.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:12*/
#line 380 "sat12-erp.w"
;

/*:20*/
#line 391 "sat12-erp.w"
;
}

/*:21*/
#line 53 "sat12-erp.w"
;
/*10:*/
#line 230 "sat12-erp.w"

if((vars>>hbits)>=10){
fprintf(stderr,"There are "O"lld variables but only "O"d hash tables;\n",
vars,1<<hbits);
while((vars>>hbits)>=10)hbits++;
fprintf(stderr," maybe you should use command-line option h"O"d?\n",hbits);
}
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
#line 54 "sat12-erp.w"
;
/*22:*/
#line 404 "sat12-erp.w"

/*23:*/
#line 423 "sat12-erp.w"

vmem= (octa*)malloc((vars+1)*sizeof(octa));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-10);
}
lmem= (char*)malloc((vars+vars+2)*sizeof(char));
if(!lmem){
fprintf(stderr,"Oops, I can't allocate the lmem array!\n");
}

/*:23*/
#line 405 "sat12-erp.w"
;
for(l= 2;l<vars+vars+2;l++)lmem[l]= unknown;
/*25:*/
#line 438 "sat12-erp.w"

for(c= vars;c;c--){
/*19:*/
#line 368 "sat12-erp.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:19*/
#line 440 "sat12-erp.w"
;
vmem[c].lng= cur_tmp_var->name.lng;
}

/*:25*/
#line 407 "sat12-erp.w"
;
if(k)/*26:*/
#line 447 "sat12-erp.w"

{
for(i= 0;i<2;){
/*18:*/
#line 359 "sat12-erp.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:18*/
#line 450 "sat12-erp.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1)+2;
printf(" "O"s"O"s",litname(p));
lmem[p]= true,lmem[bar(p)]= false;
}
}

/*:26*/
#line 408 "sat12-erp.w"
;
/*27:*/
#line 463 "sat12-erp.w"

v= true;
for(c= clauses-1;c;c--){
vv= false;
for(i= 0;i<2;){
/*18:*/
#line 359 "sat12-erp.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:18*/
#line 468 "sat12-erp.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1)+2;
if(i>=4)break;
if(lmem[p]==unknown){
printf(" "O"s"O"s",litname(p));
lmem[p]= true,lmem[bar(p)]= false;
}
if(lmem[p]==true)vv= true;
}
if(i<4){
if(vv==false)v= false;
}else{
lmem[p]= v,lmem[bar(p)]= -v;
if(v==true)printf(" "O"s"O"s",litname(p));
else printf(" "O"s"O"s",litname(bar(p)));
v= true;
}
}

/*:27*/
#line 409 "sat12-erp.w"
;
/*28:*/
#line 489 "sat12-erp.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL)
confusion("consistency");
free(cur_chunk);free(cur_vchunk);

/*:28*/
#line 410 "sat12-erp.w"
;
printf("\n");

/*:22*/
#line 55 "sat12-erp.w"
;
}

/*:1*/
