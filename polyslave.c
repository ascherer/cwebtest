#define maxm (1<<31)  \

#define filelength_threshold (1<<30)  \

#define buf_size (1<<16)  \

#define end_of_buffer &in.buf[buf_size+4] \

#define targ_bit 0x40000000 \

#define advance_b if(++b==end_of_buffer) {read_it() ;b= &in.buf[4];} \

/*1:*/
#line 16 "polyslave.w"

#include <stdio.h> 
#include <setjmp.h> 
jmp_buf restart_point;
/*2:*/
#line 59 "polyslave.w"

typedef unsigned int counter;

/*:2*//*11:*/
#line 222 "polyslave.w"

typedef enum{sync,clear,copy,add,
inc_src,dec_src,inc_trg,dec_trg}opcode;

/*:11*/
#line 20 "polyslave.w"

/*5:*/
#line 94 "polyslave.w"

unsigned int modulus;
char*base_name,filename[100];
FILE*math_file;

/*:5*//*7:*/
#line 112 "polyslave.w"

FILE*in_file;
union{
unsigned char buf[buf_size+10];
unsigned int foo;
}in;
unsigned char*buf_ptr;
int bytes_in;
unsigned int checksum;
FILE*ck_file;
unsigned int checkbuf;
int file_extension;

/*:7*//*14:*/
#line 279 "polyslave.w"

opcode op;
int verbose= 0;
char*sym[4]= {"sync","clear","copy","add"};
int cur_src,cur_trg;

/*:14*//*16:*/
#line 318 "polyslave.w"

int n;
int last_row;
int prev_row;
int w;

int slave_size;
counter*count;
counter*scount;

/*:16*//*28:*/
#line 439 "polyslave.w"

unsigned int dump_data[5];
FILE*out_file;

/*:28*/
#line 21 "polyslave.w"

/*3:*/
#line 68 "polyslave.w"

void panic(char*mess)
{
fprintf(stderr,"%s!\n",mess);
exit(-1);
}

/*:3*//*8:*/
#line 125 "polyslave.w"

void open_it()
{
sprintf(filename,"%.90s.%d",base_name,file_extension);
in_file= fopen(filename,"rb");
if(!in_file){
fprintf(stderr,"I can't open file %s",filename);
panic(" for input");
}
bytes_in= checksum= 0;
}

/*:8*//*9:*/
#line 141 "polyslave.w"

void close_it()
{
if(fread(&checkbuf,sizeof(unsigned int),1,ck_file)!=1)
panic("I couldn't read the check sum");
if(fclose(in_file)!=0)panic("I couldn't close the input file");
printf("[%d bytes read from file %s, checksum %u.]\n",
bytes_in,filename,checksum);
if(checkbuf!=checksum){
printf("Checksum mismatch! Restarting...\n");
longjmp(restart_point,1);
}
fflush(stdout);
}

/*:9*//*10:*/
#line 166 "polyslave.w"

void read_it()
{
register int t,k;register unsigned int s;
if(bytes_in>=filelength_threshold){
if(bytes_in!=filelength_threshold)panic("Improper buffer size");
close_it();
file_extension++;
open_it();
}
t= fread(in.buf+4,sizeof(unsigned char),buf_size,in_file);
if(t<buf_size)
in.buf[t+4]= in.buf[t+5]= in.buf[t+6]= in.buf[t+7]= in.buf[t+8]= 0x81;

bytes_in+= t;
for(k= s= 0;k<t;k++)s= (s<<1)+in.buf[k+4];
checksum+= s;
}

/*:10*//*12:*/
#line 234 "polyslave.w"

opcode get_inst()
{
register unsigned char*b= buf_ptr;
register opcode o;
register int p;
restart:advance_b;
if(!(*b&0x80))
/*13:*/
#line 265 "polyslave.w"

{
if(b+3>=end_of_buffer){
*(b-buf_size)= *b,*(b+1-buf_size)= *(b+1),*(b+2-buf_size)= *(b+2);
read_it();
b-= buf_size;
}
p= ((*b&0x3f)<<24)+(*(b+1)<<16)+(*(b+2)<<8)+*(b+3);
if(*b&0x40)cur_trg= p;
else cur_src= p;
b+= 3;
goto restart;
}

/*:13*/
#line 242 "polyslave.w"
;
o= (*b>>4)&7;
p= *b&0xf;
if(!p){
advance_b;
p= *b;
if(!p)panic("Parameter is zero");
}
switch(o){
case inc_src:cur_src+= p;goto restart;
case dec_src:cur_src-= p;goto restart;
case inc_trg:cur_trg+= p;goto restart;
case dec_trg:cur_trg-= p;goto restart;
default:op= o;
}
if(verbose){
if(op==clear)printf("{clear %d ->%d}\n",p,cur_trg);
else if(op> clear)printf("{%s %d %d->%d}\n",sym[op],p,cur_src,cur_trg);
}
buf_ptr= b;
return p;
}

/*:12*/
#line 22 "polyslave.w"


main(int argc,char*argv[])
{
/*6:*/
#line 99 "polyslave.w"

register int k;
register unsigned int m;

/*:6*//*18:*/
#line 346 "polyslave.w"

register int p;
register unsigned int a;

/*:18*/
#line 26 "polyslave.w"
;
/*4:*/
#line 82 "polyslave.w"

if(argc!=3||sscanf(argv[2],"%u",&modulus)!=1){
fprintf(stderr,"Usage: %s infilename modulus\n",argv[0]);
exit(-2);
}
base_name= argv[1];
if(modulus<2||modulus> maxm)panic("Improper modulus");
m= modulus;
sprintf(filename,"%.90s-%u.m",base_name,modulus);
math_file= fopen(filename,"w");
if(!math_file)panic("I can't open the output file");

/*:4*/
#line 27 "polyslave.w"
;
setjmp(restart_point);
/*15:*/
#line 298 "polyslave.w"

sprintf(filename,"%.90s.ck",base_name);
ck_file= fopen(filename,"rb");
if(!ck_file)panic("I can't open the checksum file");
open_it();
read_it();
n= in.buf[4];
last_row= in.buf[5];
prev_row= 0;
slave_size= (in.buf[6]<<24)+(in.buf[7]<<16)+(in.buf[8]<<8)+in.buf[9];
buf_ptr= &in.buf[9];
w= n+2-last_row;
if(w<2||n<w+w-1||n> w+w+126)panic("Bad bytes at the beginning");
count= (counter*)calloc(slave_size,sizeof(counter));
if(!count)panic("I couldn't allocate the counter array");
count[0]= 1;
cur_src= cur_trg= 0;
scount= (counter*)calloc(n+1,sizeof(counter));
if(!scount)panic("I couldn't allocate the array of totals");

/*:15*/
#line 29 "polyslave.w"
;
/*17:*/
#line 332 "polyslave.w"

while(1){
p= get_inst();
if(cur_trg+p> slave_size&&op>=clear)panic("Target address out of range");
if(cur_src+p> slave_size&&op>=copy)panic("Source address out of range");
switch(op){
case sync:/*22:*/
#line 369 "polyslave.w"

/*23:*/
#line 375 "polyslave.w"

if(p==255)/*26:*/
#line 413 "polyslave.w"

{
close_it();
printf("Checkpoint stop: After processing with all desired moduli,\n");
printf(" please resume with polynum-restart and polyslave-restart.\n");
sprintf(filename,"%.90s-%u.dump",base_name,m);
out_file= fopen(filename,"wb");
if(!out_file)panic("I can't open the dump file");
/*27:*/
#line 425 "polyslave.w"

dump_data[0]= n;
dump_data[1]= w;
dump_data[2]= m;
dump_data[3]= slave_size;
dump_data[4]= prev_row;
if(fwrite(dump_data,sizeof(unsigned int),5,out_file)!=5)
panic("Bad write at beginning of dump");
if(fwrite(scount,sizeof(counter),n+1,out_file)!=n+1)
panic("Couldn't dump the subtotals");
if(fwrite(count,sizeof(counter),slave_size,out_file)!=slave_size)
panic("Couldn't dump the counters");
printf("[%u bytes written on file %s.]\n",ftell(out_file),filename);

/*:27*/
#line 421 "polyslave.w"
;
exit(1);
}

/*:26*/
#line 376 "polyslave.w"
;
if(p==1)panic("File read error");
if(!prev_row){
if(p!=w+1)panic("Bad first sync");
}else if(p!=prev_row+1)panic("Out of sync");
prev_row= p;

/*:23*/
#line 370 "polyslave.w"
;
/*24:*/
#line 383 "polyslave.w"

printf("Polyominoes that span %dx%d rectangles (mod %u):\n",p-1,w,m);
fprintf(math_file,"p[%d,%d,%u]={0",p-1,w,m);
for(k= 2;k<w+p-2;k++)fprintf(math_file,",0");
for(;k<=n;k++){
if(count[k]>=scount[k])a= count[k]-scount[k];
else a= count[k]+m-scount[k];
printf(" %d:%d",a,k);
fprintf(math_file,",%d",a);
}
printf("\n");
fflush(stdout);
fprintf(math_file,"}\n");

/*:24*/
#line 371 "polyslave.w"
;
for(k= 1;k<=n;k++)scount[k]= count[k];
if(p==last_row)goto done;

/*:22*/
#line 338 "polyslave.w"
;break;
case clear:/*19:*/
#line 350 "polyslave.w"

for(k= 0;k<p;k++)count[cur_trg+k]= 0;

/*:19*/
#line 339 "polyslave.w"
;break;
case copy:/*20:*/
#line 353 "polyslave.w"

for(k= 0;k<p;k++)count[cur_trg+k]= count[cur_src+k];

/*:20*/
#line 340 "polyslave.w"
;break;
case add:/*21:*/
#line 359 "polyslave.w"

for(k= 0;k<p;k++){
a= count[cur_trg+k]+count[cur_src+k];
if(a>=m)a-= m;
count[cur_trg+k]= a;
}

/*:21*/
#line 341 "polyslave.w"
;break;
}
}
done:

/*:17*/
#line 30 "polyslave.w"
;
/*25:*/
#line 397 "polyslave.w"

printf("All done! Final totals (mod %u):\n",m);
for(k= w+w-1;k<=n;k++){
printf(" %d:%d",count[k],k);
}
printf("\n");
close_it();

/*:25*/
#line 31 "polyslave.w"
;
exit(0);
}

/*:1*/
