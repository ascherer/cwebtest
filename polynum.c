#define wmax 23
#define nmax (wmax+wmax+126) 
#define bad(k,v) sscanf(argv[k],"%d",&v) !=1 \

#define filelength_threshold (1<<30) 
#define buf_size (1<<16)  \
 \

#define targ_bit 0x40000000 \

#define end_of_buffer &out.buf[buf_size] \

#define mid_or_rt(x) (((x) &2) ==2)  \

#define unity (1<<8) 
#define epsilon 1
#define uunity (unity+epsilon) 
#define int_part(x) ((x) >>8) 
#define eps_part(x) ((x) &0xff)  \

#define hash_width 20
#define hash_mask ((1<<hash_width) -1)  \

#define f(x,y) ((x<<3) +y)  \

#define gig_threshold 5 \
 \

#define hist_size 100 \

/*4:*/
#line 123 "./polynum.w"

#include <stdio.h> 
/*12:*/
#line 294 "./polynum.w"

typedef enum{sync,clear,copy,add,
inc_src,dec_src,inc_trg,dec_trg}opcode;

/*:12*//*26:*/
#line 584 "./polynum.w"

typedef enum{zero,one,rt,mid,lft,eol}code;


/*:26*//*28:*/
#line 602 "./polynum.w"

typedef unsigned short cost;

typedef struct{
cost c[2][2];
}cost_matrix;

/*:28*//*29:*/
#line 615 "./polynum.w"

typedef enum{ytyp,xtyp,otyp}gap_type;

/*:29*//*33:*/
#line 666 "./polynum.w"

typedef struct{
cost gap;
gap_type gap_typ;
cost_matrix closed_cost;
cost_matrix open_cost;
}stack_entry;

/*:33*//*54:*/
#line 992 "./polynum.w"

typedef struct{
unsigned int h,l;
}octa;
typedef union{
octa o;
unsigned char byte[8];
}cstring;

/*:54*//*60:*/
#line 1086 "./polynum.w"

typedef enum{active,raw,deleted}status;

/*:60*//*61:*/
#line 1092 "./polynum.w"

typedef struct conf_struct{
cstring s;
unsigned int addr;
struct conf_struct*link;
char lo;
char hi;
char lim;
status state;
}config;

/*:61*/
#line 125 "./polynum.w"

/*7:*/
#line 180 "./polynum.w"

int n;
int w;

int conf_size;
int slave_size;


/*:7*//*8:*/
#line 205 "./polynum.w"

FILE*out_file;
union{
unsigned char buf[buf_size+4];
int foo;
}out;
unsigned char*buf_ptr;
int bytes_out;
unsigned int checksum;
FILE*ck_file;
int file_extension;
char*base_name,filename[100];

/*:8*//*18:*/
#line 385 "./polynum.w"

char*sym[4]= {"sync","clear","copy","add"};
int cur_src,cur_trg;
int verbose= 0;

/*:18*//*27:*/
#line 588 "./polynum.w"

char decode[5]= {'0','1',')','-','('};
code reflect[5]= {zero,one,lft,mid,rt};

/*:27*//*34:*/
#line 677 "./polynum.w"

code c[64];
stack_entry stk[64];

/*:34*//*39:*/
#line 785 "./polynum.w"

gap_type typ;
cost_matrix cm;
cost_matrix acm;

const cost_matrix zero_cost= {0,0,0,0};
const cost_matrix base_cost0= {0,unity,unity,unity};
const cost_matrix base_cost1= {0,uunity,uunity,uunity};


/*:39*//*57:*/
#line 1062 "./polynum.w"

code sc[64];

/*:57*//*65:*/
#line 1141 "./polynum.w"

unsigned int hash_bits[8][256];
config*hash_table[hash_mask+1];

/*:65*//*68:*/
#line 1165 "./polynum.w"

config*conf;
config*conf_end;
config*src;
config*trg;
int ssrc,strg;

/*:68*//*71:*/
#line 1218 "./polynum.w"

int holes;
int sholes;
int min_space= 1000000000;
int min_holes;
int space_row,space_col;
int min_sspace= 1000000000;
int min_sholes;
int slave_row,slave_col;
int moves;
int configs;
int hconfigs;
int r;
int re;
config*slot[nmax+1];

/*:71*//*85:*/
#line 1555 "./polynum.w"

int pair;
int viable;

/*:85*//*98:*/
#line 1864 "./polynum.w"

cstring target;
int hash;

/*:98*//*108:*/
#line 2025 "./polynum.w"

int dump_data[5];

/*:108*//*110:*/
#line 2084 "./polynum.w"

int hhist[hist_size];
int chist[nmax+1];
int jj;

/*:110*/
#line 126 "./polynum.w"

/*5:*/
#line 140 "./polynum.w"

void panic(char*mess)
{
fprintf(stderr,"%s!\n",mess);
exit(-1);
}

/*:5*//*9:*/
#line 218 "./polynum.w"

void open_it()
{
sprintf(filename,"%.90s.%d",base_name,file_extension);
out_file= fopen(filename,"wb");
if(!out_file){
fprintf(stderr,"I can't open file %s",filename);
panic(" for output");
}
bytes_out= checksum= 0;
}

/*:9*//*10:*/
#line 230 "./polynum.w"

void close_it()
{
if(fwrite(&checksum,sizeof(unsigned int),1,ck_file)!=1)
panic("I couldn't write the check sum");
if(fclose(out_file)!=0)panic("I couldn't close the output file");
printf("[%d bytes written on file %s, checksum %u.]\n",
bytes_out,filename,checksum);
}

/*:10*//*11:*/
#line 240 "./polynum.w"

void write_it(int bytes)
{
register int k;register unsigned int s;
if(bytes_out>=filelength_threshold){
if(bytes_out!=filelength_threshold)panic("Improper buffer size");
close_it();
file_extension++;
open_it();
}
if(fwrite(&out.buf,sizeof(unsigned char),bytes,out_file)!=bytes)
panic("Bad write");
bytes_out+= bytes;
for(k= s= 0;k<bytes;k++)s= (s<<1)+out.buf[k];
checksum+= s;
}

/*:11*//*13:*/
#line 300 "./polynum.w"

void put_inst(unsigned char o,unsigned char p)
{
register unsigned char*b= buf_ptr;
*b++= 0x80+(o<<4)+(p<16?p:0);
if(p>=16)*b++= p;
if(b>=end_of_buffer){
write_it(buf_size);
out.buf[0]= out.buf[buf_size];
b-= buf_size;
}
buf_ptr= b;
}

/*:13*//*14:*/
#line 314 "./polynum.w"

void put_four(register unsigned int x)
{
register unsigned char*b= buf_ptr;
*b= x>>24;
*(b+1)= (x>>16)&0xff;
*(b+2)= (x>>8)&0xff;
*(b+3)= x&0xff;
b+= 4;
if(b>=end_of_buffer){
write_it(buf_size);
out.buf[0]= out.buf[buf_size];
out.buf[1]= out.buf[buf_size+1];
out.buf[2]= out.buf[buf_size+2];
b-= buf_size;
}
buf_ptr= b;
}

/*:14*//*17:*/
#line 364 "./polynum.w"

void basic_inst(int op,int src_addr,int trg_addr,unsigned char count)
{
register int del;
if(verbose> 1){
if(op==clear)printf("{clear %d ->%d}\n",count,trg_addr);
else printf("{%s %d %d->%d}\n",sym[op],count,src_addr,trg_addr);
}
del= src_addr-cur_src;
if(del> 0&&del<256)put_inst(inc_src,del);
else if(del<0&&del> -256)put_inst(dec_src,-del);
else if(del)put_four(src_addr);
cur_src= src_addr;
del= trg_addr-cur_trg;
if(del> 0&&del<256)put_inst(inc_trg,del);
else if(del<0&&del> -256)put_inst(dec_trg,-del);
else if(del)put_four(trg_addr+targ_bit);
cur_trg= trg_addr;
put_inst(op,count);
}

/*:17*//*30:*/
#line 621 "./polynum.w"

cost_matrix a_n_b(cost_matrix a,cost_matrix b)
{
cost_matrix c;
c.c[0][0]= a.c[0][0]+b.c[0][0];
c.c[0][1]= a.c[0][0]+b.c[0][1];
c.c[1][0]= a.c[1][0]+b.c[0][0];
c.c[1][1]= a.c[1][0]+b.c[0][1];
return c;
}

/*:30*//*31:*/
#line 632 "./polynum.w"

cost_matrix a_x_b(cost_matrix a,cost_matrix b,gap_type typ,cost g)

{
cost_matrix c;
if(typ==otyp)return b;
c.c[0][0]= a.c[0][typ]+g+b.c[1][0];
c.c[0][1]= a.c[0][typ]+g+b.c[1][1];
c.c[1][0]= a.c[1][typ]+g+b.c[1][0];
c.c[1][1]= a.c[1][typ]+g+b.c[1][1];
return c;
}

/*:31*//*32:*/
#line 645 "./polynum.w"

void min_mat(cost_matrix*a,cost_matrix b)
{
if((*a).c[0][0]> b.c[0][0])(*a).c[0][0]= b.c[0][0];
if((*a).c[0][1]> b.c[0][1])(*a).c[0][1]= b.c[0][1];
if((*a).c[1][0]> b.c[1][0])(*a).c[1][0]= b.c[1][0];
if((*a).c[1][1]> b.c[1][1])(*a).c[1][1]= b.c[1][1];
}

/*:32*//*35:*/
#line 708 "./polynum.w"

cost connectivity(register int row_end)
{
register int k;
register int s;
int g;
gap_type typ;
int open;
/*36:*/
#line 728 "./polynum.w"

s= open= 0;
stk[0].closed_cost= zero_cost;
k= 1;
if(mid_or_rt(c[1])){
g= 0,typ= xtyp;
if(row_end==1)goto scan_tokens1;
else goto scan_tokens0;
}

/*:36*/
#line 716 "./polynum.w"
;
scan_zeros0:/*37:*/
#line 738 "./polynum.w"

if(k==row_end){
g= 0,typ= ytyp;goto scan_zeros1x;
}
if(c[k])panic("Syntax error, 0 expected");
typ= xtyp,g= unity,k++;
while(c[k]==0){
if(k==row_end){
g+= unity+uunity,k++;
goto scan_zeros1x;
}
g+= unity,k++;
}
if(k==row_end){
g+= unity;goto scan_tokens1;
}

/*:37*/
#line 717 "./polynum.w"
;
scan_tokens0:/*38:*/
#line 755 "./polynum.w"

cm= base_cost0;
k++;
switch(c[k-1]){
case lft:if(!mid_or_rt(c[k]))goto scan_open0;
/*40:*/
#line 795 "./polynum.w"

{
do{
if(k==row_end)goto scan_tokens1a;
k++;
if(c[k-1]==rt)break;
}while(mid_or_rt(c[k]));
cm.c[1][1]= unity+unity;
}

/*:40*/
#line 760 "./polynum.w"

if(c[k-1]!=rt)goto scan_open0;
case one:/*42:*/
#line 815 "./polynum.w"

if(s)/*43:*/
#line 819 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,cm);
stk[s].open_cost= a_x_b(stk[s].open_cost,cm,typ,g);
min_mat(&stk[s].open_cost,acm);
}

/*:43*/
#line 816 "./polynum.w"
;
stk[s].closed_cost= a_x_b(stk[s].closed_cost,cm,typ,g);

/*:42*/
#line 762 "./polynum.w"
;
open= 0;goto scan_zeros0;
case mid:if(!(mid_or_rt(c[k])))goto scan_mid0;
/*41:*/
#line 805 "./polynum.w"

{
do{
if(k==row_end)goto scan_tokens1b;
k++;
if(c[k-1]==rt)break;
}while(mid_or_rt(c[k]));
cm.c[1][1]= unity+unity;
}

/*:41*/
#line 765 "./polynum.w"

if(c[k-1]!=rt)goto scan_mid0;
case rt:if(!s){
if(stk[0].closed_cost.c[1][1])panic("Unmatched )");
stk[0].closed_cost= cm;
}else{
/*43:*/
#line 819 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,cm);
stk[s].open_cost= a_x_b(stk[s].open_cost,cm,typ,g);
min_mat(&stk[s].open_cost,acm);
}

/*:43*/
#line 771 "./polynum.w"
;
/*44:*/
#line 826 "./polynum.w"

s--;
if(s)/*45:*/
#line 832 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,stk[s+1].open_cost);
stk[s].open_cost= a_x_b(stk[s].open_cost,stk[s+1].open_cost,
stk[s+1].gap_typ,stk[s+1].gap);
min_mat(&stk[s].open_cost,acm);
}

/*:45*/
#line 828 "./polynum.w"
;
stk[s].closed_cost= a_x_b(stk[s].closed_cost,stk[s+1].open_cost,
stk[s+1].gap_typ,stk[s+1].gap);

/*:44*/
#line 772 "./polynum.w"
;
}
open= 0;goto scan_zeros0;
case eol:goto scan_eol0;
default:panic("Illegal code");
}
scan_open0:/*46:*/
#line 840 "./polynum.w"

stk[++s].gap_typ= typ;
stk[s].gap= g;
stk[s].closed_cost= stk[s].open_cost= cm;

/*:46*/
#line 778 "./polynum.w"
;goto check_eol0;
scan_mid0:/*47:*/
#line 845 "./polynum.w"

if(!s){
if(stk[0].closed_cost.c[1][1])panic("Unmatched -");
s= 1,stk[1].gap_typ= otyp,stk[1].closed_cost= stk[1].open_cost= cm;
}else{
/*43:*/
#line 819 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,cm);
stk[s].open_cost= a_x_b(stk[s].open_cost,cm,typ,g);
min_mat(&stk[s].open_cost,acm);
}

/*:43*/
#line 850 "./polynum.w"
;
stk[s].closed_cost= stk[s].open_cost;
}

/*:47*/
#line 779 "./polynum.w"
;
check_eol0:open= 1;
if(c[k]!=eol)goto scan_zeros0;
if(k==row_end)goto scan_eol1;
scan_eol0:panic("Row end missed");

/*:38*/
#line 718 "./polynum.w"
;
scan_zeros1:/*48:*/
#line 854 "./polynum.w"

if(c[k])panic("Syntax error, 0 expected");
typ= xtyp,g= uunity,k++;
scan_zeros1x:while(c[k]==0){
g+= uunity,k++;
}

/*:48*/
#line 719 "./polynum.w"
;
scan_tokens1:/*49:*/
#line 861 "./polynum.w"

cm= base_cost1;
k++;
switch(c[k-1]){
case lft:if(!mid_or_rt(c[k]))goto scan_open1;
/*50:*/
#line 890 "./polynum.w"

{
scan_tokens1a:do{
k++;
if(c[k-1]==rt)break;
}while(mid_or_rt(c[k]));
cm.c[0][1]= uunity;
cm.c[1][1]+= uunity;


}

/*:50*/
#line 866 "./polynum.w"

if(c[k-1]!=rt)goto scan_open1;
case one:/*42:*/
#line 815 "./polynum.w"

if(s)/*43:*/
#line 819 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,cm);
stk[s].open_cost= a_x_b(stk[s].open_cost,cm,typ,g);
min_mat(&stk[s].open_cost,acm);
}

/*:43*/
#line 816 "./polynum.w"
;
stk[s].closed_cost= a_x_b(stk[s].closed_cost,cm,typ,g);

/*:42*/
#line 868 "./polynum.w"
;
open= 0;goto scan_zeros1;
case mid:if(!(mid_or_rt(c[k])))goto scan_mid1;
/*51:*/
#line 902 "./polynum.w"

{
scan_tokens1b:do{
k++;
if(c[k-1]==rt)break;
}while(mid_or_rt(c[k]));
cm.c[0][1]= uunity;
cm.c[1][1]+= uunity;


}

/*:51*/
#line 871 "./polynum.w"

if(c[k-1]!=rt)goto scan_mid1;
case rt:if(!s){
if(stk[0].closed_cost.c[1][1])panic("Unmatched )");
stk[0].closed_cost= cm;
}else{
/*43:*/
#line 819 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,cm);
stk[s].open_cost= a_x_b(stk[s].open_cost,cm,typ,g);
min_mat(&stk[s].open_cost,acm);
}

/*:43*/
#line 877 "./polynum.w"
;
/*44:*/
#line 826 "./polynum.w"

s--;
if(s)/*45:*/
#line 832 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,stk[s+1].open_cost);
stk[s].open_cost= a_x_b(stk[s].open_cost,stk[s+1].open_cost,
stk[s+1].gap_typ,stk[s+1].gap);
min_mat(&stk[s].open_cost,acm);
}

/*:45*/
#line 828 "./polynum.w"
;
stk[s].closed_cost= a_x_b(stk[s].closed_cost,stk[s+1].open_cost,
stk[s+1].gap_typ,stk[s+1].gap);

/*:44*/
#line 878 "./polynum.w"
;
}
open= 0;goto scan_zeros1;
case eol:goto scan_eol1;
default:panic("Illegal code");
}
scan_open1:/*46:*/
#line 840 "./polynum.w"

stk[++s].gap_typ= typ;
stk[s].gap= g;
stk[s].closed_cost= stk[s].open_cost= cm;

/*:46*/
#line 884 "./polynum.w"
;goto check_eol1;
scan_mid1:/*47:*/
#line 845 "./polynum.w"

if(!s){
if(stk[0].closed_cost.c[1][1])panic("Unmatched -");
s= 1,stk[1].gap_typ= otyp,stk[1].closed_cost= stk[1].open_cost= cm;
}else{
/*43:*/
#line 819 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,cm);
stk[s].open_cost= a_x_b(stk[s].open_cost,cm,typ,g);
min_mat(&stk[s].open_cost,acm);
}

/*:43*/
#line 850 "./polynum.w"
;
stk[s].closed_cost= stk[s].open_cost;
}

/*:47*/
#line 885 "./polynum.w"
;
check_eol1:open= 1;
if(c[k]!=eol)goto scan_zeros1;
scan_eol1:

/*:49*/
#line 720 "./polynum.w"
;
/*52:*/
#line 918 "./polynum.w"

if(open){
/*44:*/
#line 826 "./polynum.w"

s--;
if(s)/*45:*/
#line 832 "./polynum.w"

{
acm= a_n_b(stk[s].closed_cost,stk[s+1].open_cost);
stk[s].open_cost= a_x_b(stk[s].open_cost,stk[s+1].open_cost,
stk[s+1].gap_typ,stk[s+1].gap);
min_mat(&stk[s].open_cost,acm);
}

/*:45*/
#line 828 "./polynum.w"
;
stk[s].closed_cost= a_x_b(stk[s].closed_cost,stk[s+1].open_cost,
stk[s+1].gap_typ,stk[s+1].gap);

/*:44*/
#line 920 "./polynum.w"
;
if(s)panic("Missing )");
return stk[0].closed_cost.c[0][0];
}else{
if(s)panic("Missing )");
if(int_part(g)==eps_part(g))return stk[0].closed_cost.c[0][typ]+g;
return stk[0].closed_cost.c[0][1]+((int_part(g)-1)<<8);
}

/*:52*/
#line 721 "./polynum.w"
;
}

/*:35*//*55:*/
#line 1009 "./polynum.w"

cstring packit()
{
register int j,k;
cstring packed;
k= w-1,j= c[w];
if(w<=10)packed.o.h= 0;
else{
for(;k> 10;k--)j= (j<<2)+j+c[k];
packed.o.h= j;
k= 9,j= c[10];
}
for(;k> 0;k--)j= (j<<3)+c[k];
packed.o.l= j;
return packed;
}

/*:55*//*56:*/
#line 1040 "./polynum.w"

void unpackit(cstring s)
{
register int j,k,q;
if(w> 10){
for(k= 1,j= s.o.l;k<10;k++){
sc[k]= c[k]= j&7;
j>>= 3;
}
sc[10]= c[10]= j;
for(k= 11,j= s.o.h;k<w;k++){
q= j/5;
sc[k]= c[k]= j-5*q;
j= q;
}
}else for(k= 1,j= s.o.l;k<w;k++){
sc[k]= c[k]= j&7;
j>>= 3;
}
sc[k]= c[k]= j;
}

/*:56*//*59:*/
#line 1068 "./polynum.w"

void print_config(int row_end)
{
register int k;
for(k= 1;k<=w;k++){
if(row_end==k)printf("^");
if(c[k]<eol)printf("%c",decode[c[k]]);
else printf("?");
}
}

/*:59*//*63:*/
#line 1114 "./polynum.w"

void update(config*p,config*q,char hi)
{
if(!q)basic_inst(add,p->addr,p->lo,hi+1-p->lo);
else if(q->state==raw){
q->state= active;
if(q->lo!=p->lo||q->hi!=hi)
basic_inst(clear,cur_src,q->addr,q->hi+1-q->lo);
basic_inst(copy,p->addr,q->addr+p->lo-q->lo,hi+1-p->lo);
}else basic_inst(add,p->addr,q->addr+p->lo-q->lo,hi+1-p->lo);
}

/*:63*//*64:*/
#line 1132 "./polynum.w"

int mangle(cstring s)
{
register unsigned int h,l;
for(l= 1,h= hash_bits[0][s.byte[0]];l<8;l++)
h+= hash_bits[l][s.byte[l]];
return h&hash_mask;
}

/*:64*//*69:*/
#line 1187 "./polynum.w"

config*get_slot_up(register int s)
{
register config*p= slot[s];
if(p){
slot[s]= p->link;
holes--,sholes-= s+1;
}else{
p= trg++;
/*70:*/
#line 1202 "./polynum.w"

{
if(src-trg<min_space){
min_space= src-trg;
if(min_space<0)panic("Memory overflow");
min_holes= holes,space_row= r,space_col= re;
}
p->addr= strg;
strg+= s+1;
if(ssrc-strg<min_sspace){
min_sspace= ssrc-strg;
if(min_sspace<0)panic("Slave memory overflow");
min_sholes= sholes,slave_row= r,slave_col= re;
}
}

/*:70*/
#line 1196 "./polynum.w"
;
}
p->state= raw;
return p;
}

/*:69*//*73:*/
#line 1245 "./polynum.w"

config*get_slot_down(register int s)
{
register config*p= slot[s];
if(p){
slot[s]= p->link;
holes--,sholes-= s+1;
}else{
p= trg--;
/*74:*/
#line 1260 "./polynum.w"

{
if(trg-src<min_space){
min_space= trg-src;
if(min_space<0)panic("Memory overflow");
min_holes= holes,space_row= r,space_col= re;
}
strg-= s+1;
if(strg-ssrc<min_sspace){
min_sspace= strg-ssrc;
if(min_sspace<0)panic("Slave memory overflow");
min_sholes= sholes,slave_row= r,slave_col= re;
}
p->addr= strg+1;
}

/*:74*/
#line 1254 "./polynum.w"
;
}
p->state= raw;
return p;
}

/*:73*//*75:*/
#line 1285 "./polynum.w"

config*move_down(config*p,int lo,int hi)
{
register config*q,*r;
register int s= p->lo,t= p->hi;
r= p->link;
p->link= slot[t-s],slot[t-s]= p;
p->state= deleted;
holes++,sholes+= t-s+1;
if(s> lo)s= lo;
if(t<hi)t= hi;
q= get_slot_down(t-s);
q->lo= s,q->hi= t;
q->s= p->s,q->lim= p->lim;
hash_table[hash]= q,q->link= r;
update(p,q,p->hi);
moves++;
return q;
}

/*:75*//*76:*/
#line 1305 "./polynum.w"

config*move_up(config*p,int lo,int hi)
{
register config*q,*r;
register int s= p->lo,t= p->hi;
r= p->link;
p->link= slot[t-s],slot[t-s]= p;
p->state= deleted;
holes++,sholes+= t-s+1;
if(s> lo)s= lo;
if(t<hi)t= hi;
q= get_slot_up(t-s);
q->lo= s,q->hi= t;
q->s= p->s,q->lim= p->lim;
hash_table[hash]= q,q->link= r;
update(p,q,p->hi);
moves++;
return q;
}

/*:76*/
#line 127 "./polynum.w"


main(int argc,char*argv[])
{
/*67:*/
#line 1155 "./polynum.w"

register int j,k;
register int row_end;

/*:67*//*99:*/
#line 1868 "./polynum.w"

register config*p;

/*:99*/
#line 131 "./polynum.w"
;
/*6:*/
#line 169 "./polynum.w"

if(argc!=6||bad(1,n)||bad(2,w)||bad(3,conf_size)||bad(4,slave_size)){
fprintf(stderr,"Usage: %s n w confsize slavesize outfilename\n",argv[0]);
exit(-2);
}
if(w> wmax)panic("Sorry, that w is too big for this implementation");
if(w<2)panic("No, w must be at least 2");
if(n<w+w-1)panic("There are no solutions for such a small n");
if(n> w+w+126)panic("Eh? That n is incredible");
base_name= argv[5];

/*:6*/
#line 132 "./polynum.w"
;
/*15:*/
#line 346 "./polynum.w"

sprintf(filename,"%.90s.ck",base_name);
ck_file= fopen(filename,"wb");
if(!ck_file)panic("I can't open the checksum file");
open_it();
out.buf[0]= n;
out.buf[1]= n+2-w;
buf_ptr= &out.buf[2];
put_four(slave_size);

/*:15*//*58:*/
#line 1065 "./polynum.w"

c[0]= sc[0]= c[w+1]= sc[w+1]= eol;

/*:58*//*62:*/
#line 1103 "./polynum.w"

conf= (config*)calloc(conf_size,sizeof(config));
if(!conf)panic("I can't allocate the config table");
conf_end= conf+conf_size;

/*:62*//*66:*/
#line 1148 "./polynum.w"

row_end= 314159265;
for(j= 0;j<8;j++)for(k= 1;k<256;k++){
row_end= 69069*row_end+1;
hash_bits[j][k]= row_end>>(32-hash_width);
}

/*:66*//*77:*/
#line 1342 "./polynum.w"

r= 0,row_end= w;
trg= conf+1;
strg= 1;

/*:77*/
#line 133 "./polynum.w"
;
/*78:*/
#line 1352 "./polynum.w"

while(1){
/*79:*/
#line 1360 "./polynum.w"

if(row_end<w){
row_end++;
printf("Beginning column %d",row_end);
/*81:*/
#line 1402 "./polynum.w"

if(src==conf-1)
printf(" (%d,%d,",conf_end-1-trg,slave_size-1-strg);
else printf(" (%d,%d,",trg-conf,strg-n-1);
printf("%d,%d,%d,%d,%d)\n",
conf_size-min_space,min_holes,slave_size-min_sspace,min_sholes,bytes_out);
/*109:*/
#line 2054 "./polynum.w"

for(k= 0;k<hist_size;k++)hhist[k]= 0;
for(k= 0;k<=nmax;k++)chist[k]= 0;
jj= 0;
for(k= 0;k<=hash_mask;k++){
for(p= hash_table[k],j= 0;p;p= p->link,j++)chist[p->hi-p->lo]++;
if(j> jj){
if(j>=hist_size)j= hist_size-1;
jj= j;
}
hhist[j]++;
hash_table[k]= NULL;
}
printf("Hash histogram:");
for(j= 1;j<=jj;j++)printf(" %d",hhist[j]);
printf("\nCounters:");
for(k= nmax;k>=0;k--)if(chist[k])break;
for(j= 0;j<=k;j++)printf(" %d",chist[j]);
for(k= nmax;k>=0;k--)if(slot[k])break;
if(k>=0){
printf("\nHoles:");
for(j= 0;j<=k;j++){
for(p= slot[j],jj= 0;p;p= p->link,jj++);
printf(" %d",jj);
slot[j]= NULL;
}
}
printf("\n");
holes= sholes= 0;

/*:109*/
#line 1408 "./polynum.w"
;
fflush(stdout);

/*:81*/
#line 1364 "./polynum.w"
;
}else{
if(r){
printf("Finished row %d",r);
/*81:*/
#line 1402 "./polynum.w"

if(src==conf-1)
printf(" (%d,%d,",conf_end-1-trg,slave_size-1-strg);
else printf(" (%d,%d,",trg-conf,strg-n-1);
printf("%d,%d,%d,%d,%d)\n",
conf_size-min_space,min_holes,slave_size-min_sspace,min_sholes,bytes_out);
/*109:*/
#line 2054 "./polynum.w"

for(k= 0;k<hist_size;k++)hhist[k]= 0;
for(k= 0;k<=nmax;k++)chist[k]= 0;
jj= 0;
for(k= 0;k<=hash_mask;k++){
for(p= hash_table[k],j= 0;p;p= p->link,j++)chist[p->hi-p->lo]++;
if(j> jj){
if(j>=hist_size)j= hist_size-1;
jj= j;
}
hhist[j]++;
hash_table[k]= NULL;
}
printf("Hash histogram:");
for(j= 1;j<=jj;j++)printf(" %d",hhist[j]);
printf("\nCounters:");
for(k= nmax;k>=0;k--)if(chist[k])break;
for(j= 0;j<=k;j++)printf(" %d",chist[j]);
for(k= nmax;k>=0;k--)if(slot[k])break;
if(k>=0){
printf("\nHoles:");
for(j= 0;j<=k;j++){
for(p= slot[j],jj= 0;p;p= p->link,jj++);
printf(" %d",jj);
slot[j]= NULL;
}
}
printf("\n");
holes= sholes= 0;

/*:109*/
#line 1408 "./polynum.w"
;
fflush(stdout);

/*:81*/
#line 1368 "./polynum.w"
;
if(r> w)put_inst(sync,r);
}
/*105:*/
#line 1989 "./polynum.w"

if(file_extension>=gig_threshold&&trg!=conf){
/*106:*/
#line 2004 "./polynum.w"

put_inst(sync,255);
/*16:*/
#line 358 "./polynum.w"

if(buf_ptr!=&out.buf[0])write_it(buf_ptr-&out.buf[0]);
close_it();

/*:16*/
#line 2006 "./polynum.w"
;
printf("Checkpoint stop: Please process that data with polyslave,\n");
printf("then resume the computation with polynum-restart.\n");

/*:106*/
#line 1991 "./polynum.w"
;
sprintf(filename,"%.90s.dump",base_name);
out_file= fopen(filename,"wb");
if(!out_file)panic("I can't open the dump file");
/*107:*/
#line 2014 "./polynum.w"

dump_data[0]= n;
dump_data[1]= w;
dump_data[2]= r;
dump_data[3]= trg-conf;
dump_data[4]= strg;
if(fwrite(dump_data,sizeof(int),5,out_file)!=5)
panic("Bad write at beginning of dump");
if(fwrite(conf,sizeof(config),trg-conf,out_file)!=trg-conf)
panic("Couldn't dump the configuration table");

/*:107*/
#line 1995 "./polynum.w"
;
/*72:*/
#line 1234 "./polynum.w"

printf("Altogether ");
if(hconfigs)printf("%d%09d",hconfigs,configs);
else printf("%d",configs);
printf(" viable configurations examined;\n");
printf(" %d slots needed (with %d holes) in position (%d,%d);\n",
conf_size-min_space,min_holes,space_row,space_col);
printf(" %d counters needed (with %d wasted) in position (%d,%d);\n",
slave_size-min_sspace,min_sholes,slave_row,slave_col);
printf(" %d moves.\n",moves);

/*:72*/
#line 1996 "./polynum.w"
;
printf("[%d bytes written on file %s.]\n",ftell(out_file),filename);
exit(1);
}

/*:105*/
#line 1371 "./polynum.w"
;
r++,row_end= 1;
}
if(trg==conf)break;
src= trg-1;
ssrc= strg-1;
trg= conf_end-1;
strg= slave_size-1;
re= row_end;

/*:79*/
#line 1354 "./polynum.w"
;
/*80:*/
#line 1381 "./polynum.w"

while(src>=conf){
if(src->state==active){
unpackit(src->s);

if(verbose){
print_config(row_end);printf("\n");
}
/*84:*/
#line 1503 "./polynum.w"

pair= f(sc[row_end-1],sc[row_end]);
c[row_end]= zero;
viable= 1;
switch(pair){
case f(zero,one):
case f(one,one):
case f(mid,one):
case f(rt,one):
case f(eol,rt):
viable= 0;

case f(zero,zero):
case f(one,zero):
case f(lft,zero):
case f(mid,zero):
case f(rt,zero):
case f(eol,zero):
case f(lft,mid):
case f(mid,mid):
break;

case f(zero,lft):
case f(one,lft):
case f(mid,lft):
case f(rt,lft):
/*86:*/
#line 1571 "./polynum.w"

for(k= row_end+1,j= 0;;k++){
switch(c[k]){
case lft:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= lft;break;
case rt:if(j){j--;continue;}
c[k]= one;break;
case eol:if(j)panic("Unexpected eol");
viable= 0;
}
break;
}

/*:86*/
#line 1529 "./polynum.w"
;break;

case f(zero,rt):
case f(one,rt):
case f(lft,rt):
case f(mid,rt):
case f(rt,rt):
/*87:*/
#line 1588 "./polynum.w"

for(k= row_end-1,j= 0;;k--){
switch(c[k]){
case rt:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= rt;break;
case lft:if(j){j--;continue;}
c[k]= one;break;
case eol:if(j)panic("Unexpected eol");
viable= 0;
}
break;
}

/*:87*/
#line 1536 "./polynum.w"
;break;

case f(zero,mid):
case f(eol,mid):
/*88:*/
#line 1603 "./polynum.w"

for(k= row_end-1;c[k]==zero;k--);
if(c[k]==eol){
for(k= row_end+1;c[k]==zero;k++);
switch(c[k]){
case mid:case rt:case eol:break;
default:if(c[k]==one)c[k]= rt,j= 0;
else c[k]= mid,j= 1;
for(k++;;k++){
switch(c[k]){
case lft:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= lft;break;
case rt:if(j){j--;continue;}
c[k]= one;break;
case eol:panic("This can't happen");
}
break;
}
}
}

/*:88*/
#line 1540 "./polynum.w"
;
case f(one,mid):
case f(rt,mid):
/*89:*/
#line 1626 "./polynum.w"

for(k= row_end+1;c[k]==zero;k++);
if(c[k]==eol){
for(k= row_end-1;c[k]==zero;k--);
switch(c[k]){
case mid:case lft:case eol:break;
default:if(c[k]==one)c[k]= lft,j= 0;
else c[k]= mid,j= 1;
for(k--;;k--){
switch(c[k]){
case rt:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= rt;break;
case lft:if(j){j--;continue;}
c[k]= one;break;
case eol:panic("This can't happen");
}
break;
}
}
}

/*:89*/
#line 1543 "./polynum.w"
;
break;

case f(lft,one):
case f(lft,lft):
case f(eol,one):
case f(eol,lft):
panic("Impossible configuration");

default:panic("Impossible pair");
}

/*:84*/
#line 1389 "./polynum.w"
;
if(viable)/*97:*/
#line 1843 "./polynum.w"

{
if(row_end==w)/*100:*/
#line 1880 "./polynum.w"

{
for(j= 1,k= w;j<=k;j++,k--)if(c[j]!=reflect[c[k]])break;
if(c[j]> reflect[c[k]])
for(;j<=k;j++,k--){
register int i= c[k];
c[k]= reflect[c[j]];
c[j]= reflect[i];
}
}

/*:100*/
#line 1845 "./polynum.w"
;
target= packit();
if(target.o.l||target.o.h||(r==1&&row_end<w)){
/*101:*/
#line 1894 "./polynum.w"

hash= mangle(target);
p= hash_table[hash];
if(p&&!(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)){
register config*q;
for(q= p,p= p->link;p;q= p,p= p->link)
if(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)break;
if(p){
q->link= p->link;
p->link= hash_table[hash];
hash_table[hash]= p;
}
}

/*:101*/
#line 1848 "./polynum.w"
;
if(!p)/*102:*/
#line 1912 "./polynum.w"

{
j= connectivity(row_end+1);
if(r>=w)j= int_part(j);
else if(int_part(j)==eps_part(j))j= int_part(j)+(w-r);
else j= int_part(j)+(w-1-r);

if(src->lo+j<=n){
if(++configs==1000000000)configs= 0,hconfigs++;
p= get_slot_down((src->hi> n-j?n-j:src->hi)-src->lo);
p->link= hash_table[hash],hash_table[hash]= p;
p->s= target;
p->lo= src->lo,p->hi= src->hi,p->lim= n-j;
if(p->hi> p->lim)p->hi= p->lim;
}
}

/*:102*/
#line 1849 "./polynum.w"
;
if(p&&(src->lo<=(j= p->lim))){
if(src->hi<j)j= src->hi;
if(j> p->hi||src->lo<p->lo)p= move_down(p,src->lo,j);
if(verbose){
printf(" -> ");print_config(row_end+1);printf("\n");
}
update(src,p,j);
}
}else if(r> w){
if(verbose)printf(" -> 0\n");
update(src,NULL,src->hi);
}
}

/*:97*/
#line 1390 "./polynum.w"
;
for(k= 1;k<=w;k++)c[k]= sc[k];
/*90:*/
#line 1693 "./polynum.w"

viable= 1;
src->lo++,src->hi++;

switch(pair){
case f(one,zero):
case f(one,one):
c[row_end-1]= lft,c[row_end]= rt;

case f(zero,one):
case f(zero,lft):
case f(zero,mid):
case f(zero,rt):
case f(lft,mid):
case f(lft,rt):
case f(mid,mid):
case f(mid,rt):
case f(eol,mid):
case f(eol,rt):
break;

case f(one,lft):
c[row_end-1]= lft,c[row_end]= mid;break;

case f(one,mid):
case f(one,rt):
c[row_end-1]= mid;break;

case f(lft,zero):
case f(mid,zero):
case f(mid,one):
c[row_end]= mid;break;

case f(mid,lft):
c[row_end]= mid;
/*91:*/
#line 1759 "./polynum.w"

for(k= row_end+1,j= 0;;k++){
switch(c[k]){
case lft:j++;
case zero:case one:case mid:continue;
case rt:if(!j)break;
j--;continue;
case eol:panic("Unexpected eol");
}
c[k]= mid;break;
}

/*:91*/
#line 1728 "./polynum.w"
;break;

case f(rt,zero):
case f(rt,one):
c[row_end-1]= mid,c[row_end]= rt;break;

case f(rt,lft):
c[row_end-1]= c[row_end]= mid;break;

case f(rt,mid):
case f(rt,rt):
c[row_end-1]= mid;
/*92:*/
#line 1771 "./polynum.w"

for(k= row_end-2,j= 0;;k--){
switch(c[k]){
case rt:j++;
case zero:case one:case mid:continue;
case lft:if(!j)break;
j--;continue;
case eol:panic("Unexpected eol");
}
c[k]= mid;break;
}

/*:92*/
#line 1740 "./polynum.w"
;break;

case f(eol,zero):
c[row_end]= rt;
/*93:*/
#line 1783 "./polynum.w"

for(k= 2;;k++){
switch(c[k]){
case zero:continue;
case mid:c[k]= lft;
case one:case lft:case eol:break;
case rt:c[k]= one;
}
break;
}

/*:93*/
#line 1744 "./polynum.w"
;break;

case f(zero,zero):
/*94:*/
#line 1794 "./polynum.w"

c[row_end]= one;
for(k= row_end-2;c[k]==zero;k--);
if(!k){
for(k= row_end+1;;k++){
switch(c[k]){
case zero:continue;
case mid:c[k]= lft,c[row_end]= rt;
case one:case lft:case eol:break;
case rt:c[k]= one,c[row_end]= rt;
}
break;
}
}else{
for(j= row_end+1;c[j]==zero;j++);
if(c[j]==eol){
if(c[k]==mid)c[k]= rt,c[row_end]= lft;
else if(c[k]==lft)c[k]= one,c[row_end]= lft;
}
}

/*:94*/
#line 1747 "./polynum.w"
;break;

case f(lft,one):
case f(lft,lft):
case f(eol,one):
case f(eol,lft):
panic("Impossible configuration");

default:panic("Impossible pair");
}
if(row_end==w)/*95:*/
#line 1815 "./polynum.w"

switch(c[row_end]){
case rt:c[row_end]= mid;
case zero:case mid:case lft:break;
case one:c[row_end]= lft;
/*96:*/
#line 1823 "./polynum.w"

for(k= row_end-1;;k--){
switch(c[k]){
case zero:continue;
case mid:c[k]= rt;
case one:case rt:case eol:break;
case lft:c[k]= one;
}
break;
}

/*:96*/
#line 1820 "./polynum.w"
;
}

/*:95*/
#line 1757 "./polynum.w"
;

/*:90*/
#line 1392 "./polynum.w"
;
if(viable)/*97:*/
#line 1843 "./polynum.w"

{
if(row_end==w)/*100:*/
#line 1880 "./polynum.w"

{
for(j= 1,k= w;j<=k;j++,k--)if(c[j]!=reflect[c[k]])break;
if(c[j]> reflect[c[k]])
for(;j<=k;j++,k--){
register int i= c[k];
c[k]= reflect[c[j]];
c[j]= reflect[i];
}
}

/*:100*/
#line 1845 "./polynum.w"
;
target= packit();
if(target.o.l||target.o.h||(r==1&&row_end<w)){
/*101:*/
#line 1894 "./polynum.w"

hash= mangle(target);
p= hash_table[hash];
if(p&&!(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)){
register config*q;
for(q= p,p= p->link;p;q= p,p= p->link)
if(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)break;
if(p){
q->link= p->link;
p->link= hash_table[hash];
hash_table[hash]= p;
}
}

/*:101*/
#line 1848 "./polynum.w"
;
if(!p)/*102:*/
#line 1912 "./polynum.w"

{
j= connectivity(row_end+1);
if(r>=w)j= int_part(j);
else if(int_part(j)==eps_part(j))j= int_part(j)+(w-r);
else j= int_part(j)+(w-1-r);

if(src->lo+j<=n){
if(++configs==1000000000)configs= 0,hconfigs++;
p= get_slot_down((src->hi> n-j?n-j:src->hi)-src->lo);
p->link= hash_table[hash],hash_table[hash]= p;
p->s= target;
p->lo= src->lo,p->hi= src->hi,p->lim= n-j;
if(p->hi> p->lim)p->hi= p->lim;
}
}

/*:102*/
#line 1849 "./polynum.w"
;
if(p&&(src->lo<=(j= p->lim))){
if(src->hi<j)j= src->hi;
if(j> p->hi||src->lo<p->lo)p= move_down(p,src->lo,j);
if(verbose){
printf(" -> ");print_config(row_end+1);printf("\n");
}
update(src,p,j);
}
}else if(r> w){
if(verbose)printf(" -> 0\n");
update(src,NULL,src->hi);
}
}

/*:97*/
#line 1393 "./polynum.w"
;
}
ssrc= src->addr-1;
src--;
}

/*:80*/
#line 1355 "./polynum.w"
;
/*82:*/
#line 1414 "./polynum.w"

if(row_end<w){
row_end++;
printf("Beginning column %d",row_end);
/*81:*/
#line 1402 "./polynum.w"

if(src==conf-1)
printf(" (%d,%d,",conf_end-1-trg,slave_size-1-strg);
else printf(" (%d,%d,",trg-conf,strg-n-1);
printf("%d,%d,%d,%d,%d)\n",
conf_size-min_space,min_holes,slave_size-min_sspace,min_sholes,bytes_out);
/*109:*/
#line 2054 "./polynum.w"

for(k= 0;k<hist_size;k++)hhist[k]= 0;
for(k= 0;k<=nmax;k++)chist[k]= 0;
jj= 0;
for(k= 0;k<=hash_mask;k++){
for(p= hash_table[k],j= 0;p;p= p->link,j++)chist[p->hi-p->lo]++;
if(j> jj){
if(j>=hist_size)j= hist_size-1;
jj= j;
}
hhist[j]++;
hash_table[k]= NULL;
}
printf("Hash histogram:");
for(j= 1;j<=jj;j++)printf(" %d",hhist[j]);
printf("\nCounters:");
for(k= nmax;k>=0;k--)if(chist[k])break;
for(j= 0;j<=k;j++)printf(" %d",chist[j]);
for(k= nmax;k>=0;k--)if(slot[k])break;
if(k>=0){
printf("\nHoles:");
for(j= 0;j<=k;j++){
for(p= slot[j],jj= 0;p;p= p->link,jj++);
printf(" %d",jj);
slot[j]= NULL;
}
}
printf("\n");
holes= sholes= 0;

/*:109*/
#line 1408 "./polynum.w"
;
fflush(stdout);

/*:81*/
#line 1418 "./polynum.w"
;
}else{
if(r){
printf("Finished row %d",r);
/*81:*/
#line 1402 "./polynum.w"

if(src==conf-1)
printf(" (%d,%d,",conf_end-1-trg,slave_size-1-strg);
else printf(" (%d,%d,",trg-conf,strg-n-1);
printf("%d,%d,%d,%d,%d)\n",
conf_size-min_space,min_holes,slave_size-min_sspace,min_sholes,bytes_out);
/*109:*/
#line 2054 "./polynum.w"

for(k= 0;k<hist_size;k++)hhist[k]= 0;
for(k= 0;k<=nmax;k++)chist[k]= 0;
jj= 0;
for(k= 0;k<=hash_mask;k++){
for(p= hash_table[k],j= 0;p;p= p->link,j++)chist[p->hi-p->lo]++;
if(j> jj){
if(j>=hist_size)j= hist_size-1;
jj= j;
}
hhist[j]++;
hash_table[k]= NULL;
}
printf("Hash histogram:");
for(j= 1;j<=jj;j++)printf(" %d",hhist[j]);
printf("\nCounters:");
for(k= nmax;k>=0;k--)if(chist[k])break;
for(j= 0;j<=k;j++)printf(" %d",chist[j]);
for(k= nmax;k>=0;k--)if(slot[k])break;
if(k>=0){
printf("\nHoles:");
for(j= 0;j<=k;j++){
for(p= slot[j],jj= 0;p;p= p->link,jj++);
printf(" %d",jj);
slot[j]= NULL;
}
}
printf("\n");
holes= sholes= 0;

/*:109*/
#line 1408 "./polynum.w"
;
fflush(stdout);

/*:81*/
#line 1422 "./polynum.w"
;
if(r> w)put_inst(sync,r);
}
r++,row_end= 1;
}
if(trg==conf_end-1)break;
src= trg+1;


trg= conf;
strg= n+1;
re= row_end;

/*:82*/
#line 1356 "./polynum.w"
;
/*83:*/
#line 1435 "./polynum.w"

while(src<conf_end){
if(src->state==active){
ssrc= src->addr;
unpackit(src->s);

if(verbose){
print_config(row_end);printf("\n");
}
/*84:*/
#line 1503 "./polynum.w"

pair= f(sc[row_end-1],sc[row_end]);
c[row_end]= zero;
viable= 1;
switch(pair){
case f(zero,one):
case f(one,one):
case f(mid,one):
case f(rt,one):
case f(eol,rt):
viable= 0;

case f(zero,zero):
case f(one,zero):
case f(lft,zero):
case f(mid,zero):
case f(rt,zero):
case f(eol,zero):
case f(lft,mid):
case f(mid,mid):
break;

case f(zero,lft):
case f(one,lft):
case f(mid,lft):
case f(rt,lft):
/*86:*/
#line 1571 "./polynum.w"

for(k= row_end+1,j= 0;;k++){
switch(c[k]){
case lft:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= lft;break;
case rt:if(j){j--;continue;}
c[k]= one;break;
case eol:if(j)panic("Unexpected eol");
viable= 0;
}
break;
}

/*:86*/
#line 1529 "./polynum.w"
;break;

case f(zero,rt):
case f(one,rt):
case f(lft,rt):
case f(mid,rt):
case f(rt,rt):
/*87:*/
#line 1588 "./polynum.w"

for(k= row_end-1,j= 0;;k--){
switch(c[k]){
case rt:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= rt;break;
case lft:if(j){j--;continue;}
c[k]= one;break;
case eol:if(j)panic("Unexpected eol");
viable= 0;
}
break;
}

/*:87*/
#line 1536 "./polynum.w"
;break;

case f(zero,mid):
case f(eol,mid):
/*88:*/
#line 1603 "./polynum.w"

for(k= row_end-1;c[k]==zero;k--);
if(c[k]==eol){
for(k= row_end+1;c[k]==zero;k++);
switch(c[k]){
case mid:case rt:case eol:break;
default:if(c[k]==one)c[k]= rt,j= 0;
else c[k]= mid,j= 1;
for(k++;;k++){
switch(c[k]){
case lft:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= lft;break;
case rt:if(j){j--;continue;}
c[k]= one;break;
case eol:panic("This can't happen");
}
break;
}
}
}

/*:88*/
#line 1540 "./polynum.w"
;
case f(one,mid):
case f(rt,mid):
/*89:*/
#line 1626 "./polynum.w"

for(k= row_end+1;c[k]==zero;k++);
if(c[k]==eol){
for(k= row_end-1;c[k]==zero;k--);
switch(c[k]){
case mid:case lft:case eol:break;
default:if(c[k]==one)c[k]= lft,j= 0;
else c[k]= mid,j= 1;
for(k--;;k--){
switch(c[k]){
case rt:j++;
case zero:case one:continue;
case mid:if(j)continue;
c[k]= rt;break;
case lft:if(j){j--;continue;}
c[k]= one;break;
case eol:panic("This can't happen");
}
break;
}
}
}

/*:89*/
#line 1543 "./polynum.w"
;
break;

case f(lft,one):
case f(lft,lft):
case f(eol,one):
case f(eol,lft):
panic("Impossible configuration");

default:panic("Impossible pair");
}

/*:84*/
#line 1444 "./polynum.w"
;
if(viable)/*103:*/
#line 1929 "./polynum.w"

{
if(row_end==w)/*100:*/
#line 1880 "./polynum.w"

{
for(j= 1,k= w;j<=k;j++,k--)if(c[j]!=reflect[c[k]])break;
if(c[j]> reflect[c[k]])
for(;j<=k;j++,k--){
register int i= c[k];
c[k]= reflect[c[j]];
c[j]= reflect[i];
}
}

/*:100*/
#line 1931 "./polynum.w"
;
target= packit();
if(target.o.l||target.o.h||(r==1&&row_end<w)){
/*101:*/
#line 1894 "./polynum.w"

hash= mangle(target);
p= hash_table[hash];
if(p&&!(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)){
register config*q;
for(q= p,p= p->link;p;q= p,p= p->link)
if(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)break;
if(p){
q->link= p->link;
p->link= hash_table[hash];
hash_table[hash]= p;
}
}

/*:101*/
#line 1934 "./polynum.w"
;
if(!p)/*104:*/
#line 1950 "./polynum.w"

{
j= connectivity(row_end+1);
if(r>=w)j= int_part(j);
else if(int_part(j)==eps_part(j))j= int_part(j)+(w-r);
else j= int_part(j)+(w-1-r);

if(src->lo+j<=n){
if(++configs==1000000000)configs= 0,hconfigs++;
p= get_slot_up((src->hi> n-j?n-j:src->hi)-src->lo);
p->link= hash_table[hash],hash_table[hash]= p;
p->s= target;
p->lo= src->lo,p->hi= src->hi,p->lim= n-j;
if(p->hi> p->lim)p->hi= p->lim;
}
}

/*:104*/
#line 1935 "./polynum.w"
;
if(p&&(src->lo<=(j= p->lim))){
if(src->hi<j)j= src->hi;
if(j> p->hi||src->lo<p->lo)p= move_up(p,src->lo,j);
if(verbose){
printf(" -> ");print_config(row_end+1);printf("\n");
}
update(src,p,j);
}
}else if(r> w){
if(verbose)printf(" -> 0\n");
update(src,NULL,src->hi);
}
}

/*:103*/
#line 1445 "./polynum.w"
;
for(k= 1;k<=w;k++)c[k]= sc[k];
/*90:*/
#line 1693 "./polynum.w"

viable= 1;
src->lo++,src->hi++;

switch(pair){
case f(one,zero):
case f(one,one):
c[row_end-1]= lft,c[row_end]= rt;

case f(zero,one):
case f(zero,lft):
case f(zero,mid):
case f(zero,rt):
case f(lft,mid):
case f(lft,rt):
case f(mid,mid):
case f(mid,rt):
case f(eol,mid):
case f(eol,rt):
break;

case f(one,lft):
c[row_end-1]= lft,c[row_end]= mid;break;

case f(one,mid):
case f(one,rt):
c[row_end-1]= mid;break;

case f(lft,zero):
case f(mid,zero):
case f(mid,one):
c[row_end]= mid;break;

case f(mid,lft):
c[row_end]= mid;
/*91:*/
#line 1759 "./polynum.w"

for(k= row_end+1,j= 0;;k++){
switch(c[k]){
case lft:j++;
case zero:case one:case mid:continue;
case rt:if(!j)break;
j--;continue;
case eol:panic("Unexpected eol");
}
c[k]= mid;break;
}

/*:91*/
#line 1728 "./polynum.w"
;break;

case f(rt,zero):
case f(rt,one):
c[row_end-1]= mid,c[row_end]= rt;break;

case f(rt,lft):
c[row_end-1]= c[row_end]= mid;break;

case f(rt,mid):
case f(rt,rt):
c[row_end-1]= mid;
/*92:*/
#line 1771 "./polynum.w"

for(k= row_end-2,j= 0;;k--){
switch(c[k]){
case rt:j++;
case zero:case one:case mid:continue;
case lft:if(!j)break;
j--;continue;
case eol:panic("Unexpected eol");
}
c[k]= mid;break;
}

/*:92*/
#line 1740 "./polynum.w"
;break;

case f(eol,zero):
c[row_end]= rt;
/*93:*/
#line 1783 "./polynum.w"

for(k= 2;;k++){
switch(c[k]){
case zero:continue;
case mid:c[k]= lft;
case one:case lft:case eol:break;
case rt:c[k]= one;
}
break;
}

/*:93*/
#line 1744 "./polynum.w"
;break;

case f(zero,zero):
/*94:*/
#line 1794 "./polynum.w"

c[row_end]= one;
for(k= row_end-2;c[k]==zero;k--);
if(!k){
for(k= row_end+1;;k++){
switch(c[k]){
case zero:continue;
case mid:c[k]= lft,c[row_end]= rt;
case one:case lft:case eol:break;
case rt:c[k]= one,c[row_end]= rt;
}
break;
}
}else{
for(j= row_end+1;c[j]==zero;j++);
if(c[j]==eol){
if(c[k]==mid)c[k]= rt,c[row_end]= lft;
else if(c[k]==lft)c[k]= one,c[row_end]= lft;
}
}

/*:94*/
#line 1747 "./polynum.w"
;break;

case f(lft,one):
case f(lft,lft):
case f(eol,one):
case f(eol,lft):
panic("Impossible configuration");

default:panic("Impossible pair");
}
if(row_end==w)/*95:*/
#line 1815 "./polynum.w"

switch(c[row_end]){
case rt:c[row_end]= mid;
case zero:case mid:case lft:break;
case one:c[row_end]= lft;
/*96:*/
#line 1823 "./polynum.w"

for(k= row_end-1;;k--){
switch(c[k]){
case zero:continue;
case mid:c[k]= rt;
case one:case rt:case eol:break;
case lft:c[k]= one;
}
break;
}

/*:96*/
#line 1820 "./polynum.w"
;
}

/*:95*/
#line 1757 "./polynum.w"
;

/*:90*/
#line 1447 "./polynum.w"
;
if(viable)/*103:*/
#line 1929 "./polynum.w"

{
if(row_end==w)/*100:*/
#line 1880 "./polynum.w"

{
for(j= 1,k= w;j<=k;j++,k--)if(c[j]!=reflect[c[k]])break;
if(c[j]> reflect[c[k]])
for(;j<=k;j++,k--){
register int i= c[k];
c[k]= reflect[c[j]];
c[j]= reflect[i];
}
}

/*:100*/
#line 1931 "./polynum.w"
;
target= packit();
if(target.o.l||target.o.h||(r==1&&row_end<w)){
/*101:*/
#line 1894 "./polynum.w"

hash= mangle(target);
p= hash_table[hash];
if(p&&!(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)){
register config*q;
for(q= p,p= p->link;p;q= p,p= p->link)
if(p->s.o.l==target.o.l&&p->s.o.h==target.o.h)break;
if(p){
q->link= p->link;
p->link= hash_table[hash];
hash_table[hash]= p;
}
}

/*:101*/
#line 1934 "./polynum.w"
;
if(!p)/*104:*/
#line 1950 "./polynum.w"

{
j= connectivity(row_end+1);
if(r>=w)j= int_part(j);
else if(int_part(j)==eps_part(j))j= int_part(j)+(w-r);
else j= int_part(j)+(w-1-r);

if(src->lo+j<=n){
if(++configs==1000000000)configs= 0,hconfigs++;
p= get_slot_up((src->hi> n-j?n-j:src->hi)-src->lo);
p->link= hash_table[hash],hash_table[hash]= p;
p->s= target;
p->lo= src->lo,p->hi= src->hi,p->lim= n-j;
if(p->hi> p->lim)p->hi= p->lim;
}
}

/*:104*/
#line 1935 "./polynum.w"
;
if(p&&(src->lo<=(j= p->lim))){
if(src->hi<j)j= src->hi;
if(j> p->hi||src->lo<p->lo)p= move_up(p,src->lo,j);
if(verbose){
printf(" -> ");print_config(row_end+1);printf("\n");
}
update(src,p,j);
}
}else if(r> w){
if(verbose)printf(" -> 0\n");
update(src,NULL,src->hi);
}
}

/*:103*/
#line 1448 "./polynum.w"
;
}
src++;
}

/*:83*/
#line 1357 "./polynum.w"
;
}

/*:78*/
#line 134 "./polynum.w"
;
/*72:*/
#line 1234 "./polynum.w"

printf("Altogether ");
if(hconfigs)printf("%d%09d",hconfigs,configs);
else printf("%d",configs);
printf(" viable configurations examined;\n");
printf(" %d slots needed (with %d holes) in position (%d,%d);\n",
conf_size-min_space,min_holes,space_row,space_col);
printf(" %d counters needed (with %d wasted) in position (%d,%d);\n",
slave_size-min_sspace,min_sholes,slave_row,slave_col);
printf(" %d moves.\n",moves);

/*:72*/
#line 135 "./polynum.w"
;
/*16:*/
#line 358 "./polynum.w"

if(buf_ptr!=&out.buf[0])write_it(buf_ptr-&out.buf[0]);
close_it();

/*:16*/
#line 136 "./polynum.w"
;
exit(0);
}

/*:4*/
