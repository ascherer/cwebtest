#define nmax 30 \

#define maxlen ((nmax+1) >>1)  \

#define length(pk) pk.bytes[0] \

#define succ_size (maxlen*(1<<maxlen) )  \

#define patt_size 3002000 \

#define hash_width 20
#define hash_mask ((1<<hash_width) -1)  \

#define filelength_threshold 0x10000000 \

#define new_pred_code 0
#define new_level_code 31 \

#define patt_code(q) (((q) -patt_table) +1)  \

/*1:*/
#line 63 "./polyenum.w"

#include <stdio.h> 
/*10:*/
#line 246 "./polyenum.w"

typedef struct{
unsigned char bytes[8];
}patkey;

/*:10*//*11:*/
#line 258 "./polyenum.w"

typedef struct patt_struct{
patkey key;
struct succ_struct*aux;
struct patt_struct*link;
struct patt_struct*next;
}patt;

/*:11*//*12:*/
#line 270 "./polyenum.w"

typedef struct succ_struct{
patt*pat;
char weight;
char degree;
char death;
}succ;

/*:12*/
#line 65 "./polyenum.w"

/*6:*/
#line 138 "./polyenum.w"

char stk[16],dst[16];

/*:6*//*13:*/
#line 283 "./polyenum.w"

succ succ_table[succ_size];
succ*succ_ptr;

/*:13*//*14:*/
#line 292 "./polyenum.w"

patt patt_table[patt_size];
patt*patt_list[nmax+1];
int patt_count[nmax+1];
patt*patt_ptr= patt_table;
patt*bad_patt= patt_table+patt_size-1;

/*:14*//*18:*/
#line 353 "./polyenum.w"

patt*hash_table[hash_mask+1];
unsigned int hash[8][256];

/*:18*//*21:*/
#line 403 "./polyenum.w"

char dig[50];
char adjcell[17];
char intcell[13];
char first[50];
char occ[50];
char touched[16];
char appeared[16];
char rightend;
char leftbound,rightbound;

/*:21*//*31:*/
#line 572 "./polyenum.w"

char save_leftbound,save_rightbound;


/*:31*//*34:*/
#line 622 "./polyenum.w"

char circ[50];
char leader[50];

/*:34*//*37:*/
#line 650 "./polyenum.w"

char class[50];

/*:37*//*40:*/
#line 686 "./polyenum.w"

char newpat[16];
char backpat[16];
char*bestpat;
int weight;

/*:40*//*48:*/
#line 778 "./polyenum.w"

int len;
int mult;

/*:48*//*51:*/
#line 823 "./polyenum.w"

int verbose= 0;
FILE*out_file;
unsigned int buf;
int words_out;
int file_extension;
char*base_name,filename[100];

/*:51*//*58:*/
#line 916 "./polyenum.w"

int prev_pat;

/*:58*//*63:*/
#line 979 "./polyenum.w"

int slices;
int arcs;

/*:63*/
#line 66 "./polyenum.w"

/*2:*/
#line 78 "./polyenum.w"

void panic(char*mess)
{
fprintf(stderr,"%s!\n",mess);
exit(-1);
}

/*:2*//*5:*/
#line 115 "./polyenum.w"

int conn_distance(int len,char*a)
{
register int j,k,m,acc= 0;
stk[0]= a[0],m= 0;
for(j= 1;j<len;j++){
for(k= 1;a[j]==0;j++)k+= 2;
acc+= k;
if(a[j]> stk[m]){
dst[m]= k;
stk[++m]= a[j];
}else{
while(a[j]<stk[m]){
m--;
if(dst[m]> k)k= dst[m];
}
if(a[j]!=stk[m])panic("Oops, the program logic is screwed up");
acc-= k;
}
}
return acc;
}

/*:5*//*15:*/
#line 307 "./polyenum.w"

patt*lookup(int len,char*a,int m)
{
patkey key;
register int j,l;
register unsigned int h;
register patt*p;
register unsigned char*q;
/*17:*/
#line 344 "./polyenum.w"

a[len]= 0,h= len<<(hash_width-4);
for(l= 1;l+l<=len;l++){
key.bytes[l]= (a[l+l-1]<<4)+a[l+l];
h+= hash[l][key.bytes[l]];
}
length(key)= len;
h&= hash_mask;

/*:17*/
#line 315 "./polyenum.w"
;
p= hash_table[h];
if(!p)hash_table[h]= patt_ptr;
else while(1){
for(j= 0,q= &(p->key.bytes[0]);j<l;j++,q++)
if(*q!=key.bytes[j])goto mismatch;
return p;
mismatch:if(!p->link){
p->link= patt_ptr;break;
}
p= p->link;
}
/*16:*/
#line 330 "./polyenum.w"

if(patt_ptr==bad_patt)panic("Pattern memory overflow");
patt_ptr->key= key;
patt_ptr->next= patt_list[m];
patt_list[m]= patt_ptr;
patt_count[m]++;
return patt_ptr++;

/*:16*/
#line 327 "./polyenum.w"
;
}

/*:15*//*35:*/
#line 626 "./polyenum.w"

void merge(int j,int k)
{
register int p,t;
if(j<k){
for(p= t= circ[k];p!=k;p= circ[p])leader[p]= j;
leader[p]= j;
circ[p]= circ[j],circ[j]= t;
}else{
for(p= t= circ[j];p!=j;p= circ[p])leader[p]= k;
leader[p]= k;
circ[p]= circ[k],circ[k]= t;
}
}

/*:35*//*52:*/
#line 831 "./polyenum.w"

void open_it()
{
sprintf(filename,"%.90s.%d",base_name,file_extension);
out_file= fopen(filename,"wb");
if(!out_file){
fprintf(stderr,"I can't open file %s",filename);
panic(" for output");
}
words_out= 0;
}

/*:52*//*53:*/
#line 843 "./polyenum.w"

void close_it()
{
if(fclose(out_file)!=0)panic("I couldn't close the output file");
printf("[%d bytes written on file %s.]\n",4*words_out,filename);
}

/*:53*//*54:*/
#line 850 "./polyenum.w"

int out_it()
{
if(words_out==filelength_threshold){
close_it();
file_extension++;
open_it();
}
words_out++;
return fwrite(&buf,sizeof(unsigned int),1,out_file)==1;
}

/*:54*//*59:*/
#line 928 "./polyenum.w"

void print_slice(patt*p,int m,int death)
{
register int j;
for(j= 0;j<length(p->key);j++)
printf("%x",j==0?1:j&1?p->key.bytes[(j+1)>>1]>>4:
p->key.bytes[j>>1]&0xf);
if(death)printf(":%d..%d\n",m,death);
else printf(":%d\n",m);
}

/*:59*/
#line 67 "./polyenum.w"


main(int argc,char*argv[])
{
/*20:*/
#line 367 "./polyenum.w"

register int j,k,l,m;

/*:20*//*23:*/
#line 446 "./polyenum.w"

register patt*p;

/*:23*//*45:*/
#line 730 "./polyenum.w"

register patt*q;
register succ*s;

/*:45*/
#line 71 "./polyenum.w"
;
/*4:*/
#line 89 "./polyenum.w"

if(argc!=2){
fprintf(stderr,"Usage: %s outfilename\n",argv[0]);
}
base_name= argv[1];

/*:4*/
#line 72 "./polyenum.w"
;
/*19:*/
#line 360 "./polyenum.w"

m= 314159265;
for(j= 1;j<8;j++)for(k= 0;k<256;k++){
m= 69069*m+1;
hash[j][k]= m>>(32-hash_width);
}

/*:19*//*24:*/
#line 449 "./polyenum.w"

dig[16]= 1;
for(j= 1;j<16;j++)leader[j]= j;
for(j= 31;j<50;j++)leader[j]= j;

/*:24*//*55:*/
#line 862 "./polyenum.w"

open_it();

/*:55*/
#line 73 "./polyenum.w"
;
/*49:*/
#line 785 "./polyenum.w"

/*7:*/
#line 155 "./polyenum.w"

for(k= 1;k<(1<<maxlen);k+= 2)
/*47:*/
#line 758 "./polyenum.w"

{
m= k;
for(j= l= 0;m;j++,m>>= 1)
if(m&1)newpat[j]= ++l;
else newpat[j]= 0;
len= j;
weight= l;
for(j--;j>=0;j--,m++)
if(newpat[j])backpat[m]= l+1-newpat[j];
else backpat[m]= 0;
mult= 1;
for(j= 1;j<len;j++)
if(newpat[j]<backpat[j]){
mult= 2;break;
}else if(newpat[j]> backpat[j])goto bypass;
/*57:*/
#line 904 "./polyenum.w"

l= nmax-conn_distance(len,newpat);
q= lookup(len,newpat,weight);
if(patt_code(q)!=++prev_pat)panic("Out of sync");
if(verbose){
printf("-%s>",mult==2?"2":mult!=1?"?":"");
print_slice(q,weight,l);
}
buf= (mult<<27)+(weight<<8)+l;
if(!out_it())panic("Bad write of initial transition");
slices+= l-weight+1;

/*:57*/
#line 774 "./polyenum.w"
;
bypass:
}

/*:47*/
#line 157 "./polyenum.w"
;

/*:7*/
#line 786 "./polyenum.w"
;
for(m= 1;;m++){
printf(" %d new patterns on level %d (%d,%d)\n",patt_count[m],m,
patt_ptr-patt_table,arcs);
/*56:*/
#line 895 "./polyenum.w"

buf= (new_level_code<<27)+m;
if(!out_it())panic("Bad write of newlevel message");

/*:56*/
#line 790 "./polyenum.w"
;
if(m==nmax)break;
for(p= patt_list[m];p;p= p->next){
/*27:*/
#line 498 "./polyenum.w"

{
succ_ptr= succ_table;
/*22:*/
#line 426 "./polyenum.w"

l= length(p->key);
for(j= 2;j<=l;j+= 2){
k= p->key.bytes[(j>>1)];
dig[j+15]= k>>4,dig[j+16]= k&0xf;
}
dig[l+16]= 0;
rightend= l+17;
for(j= rightend;j<=31;j++)leader[j]= j;
for(j= 1;j<=l;j++)touched[j]= appeared[j]= 0;
for(j= 16,k= l= 0;j<rightend;j++){
first[j]= 0;
if(dig[j]){
if(dig[j-1]==0)adjcell[k++]= j;
adjcell[k++]= j+1;
if(!appeared[dig[j]])first[j]= dig[j],appeared[dig[j]]= 1;
}else if(dig[j-1]==0)intcell[l++]= j,leader[j]= j;
}
adjcell[k]= intcell[l]= 0;

/*:22*/
#line 501 "./polyenum.w"
;
touched[0]= 128;
/*26:*/
#line 482 "./polyenum.w"

for(k--;k>=0;k--){
j= adjcell[k];
if(!touched[first[j]])touched[dig[j-1]]++,touched[dig[j]]++,occ[j]= 1;
}

/*:26*/
#line 503 "./polyenum.w"
;
while(1){
/*41:*/
#line 692 "./polyenum.w"

/*32:*/
#line 590 "./polyenum.w"

for(j= 16;!occ[j];j++);
leftbound= j;
for(j= rightend-1;!occ[j];j--);
rightbound= j;

/*:32*/
#line 693 "./polyenum.w"
;
if(rightbound-leftbound>=maxlen){
l= nmax;goto done;
}
/*33:*/
#line 606 "./polyenum.w"

for(j= leftbound-1,l= m;j<=rightbound;j++){
if(occ[j])l++,circ[j]= leader[j]= j;
if(dig[j]){
if(occ[j])appeared[dig[j]]= j;
else if(occ[j+1])appeared[dig[j]]= j+1;
}
}
if(l> nmax)goto done;
for(j= leftbound-1;j<=rightbound;j++)if((k= dig[j])){
if(occ[j]&&leader[j]!=leader[appeared[k]])
merge(leader[j],leader[appeared[k]]);
if(occ[j+1]&&leader[j+1]!=leader[appeared[k]])
merge(leader[j+1],leader[appeared[k]]);
}

/*:33*/
#line 697 "./polyenum.w"
;
/*39:*/
#line 668 "./polyenum.w"

for(j= leftbound,l= 0;j<=rightbound;j++)
if(occ[j])l++;
if(m+l<=nmax){
weight= l;
/*36:*/
#line 644 "./polyenum.w"

for(j= leftbound,k= l= 0;j<=rightbound;j++,k++)
if(!occ[j])newpat[k]= 0;
else if(leader[j]<j)newpat[k]= class[leader[j]];
else newpat[k]= class[j]= ++l;

/*:36*/
#line 673 "./polyenum.w"
;
/*38:*/
#line 659 "./polyenum.w"

for(j= rightbound,k= l= 0;j>=leftbound;j--,k++)
if(!occ[j])backpat[k]= 0;
else if(class[leader[j]]&0x10)backpat[k]= class[leader[j]]&0xf;
else backpat[k]= ++l,class[leader[j]]= l+0x10;

/*:38*/
#line 674 "./polyenum.w"
;
len= k;
bestpat= newpat;
for(j= 1;j<k;j++)
if(newpat[j]<backpat[j])break;
else if(newpat[j]> backpat[j]){
bestpat= backpat;break;
}
k= conn_distance(k,bestpat);
l= weight+k;
}

/*:39*/
#line 698 "./polyenum.w"
;
done:

/*:41*/
#line 505 "./polyenum.w"
;
if(m+l> nmax)goto move;
/*44:*/
#line 718 "./polyenum.w"

q= lookup(len,bestpat,m+weight);
if(q->aux)q->aux->degree++;
else{
s= succ_ptr++;
s->pat= q;
q->aux= s;
s->degree= 1;
s->weight= weight;
s->death= nmax-l+weight;
}

/*:44*/
#line 507 "./polyenum.w"
;
/*28:*/
#line 514 "./polyenum.w"

while(1){
/*29:*/
#line 535 "./polyenum.w"

save_leftbound= leftbound;
while(1){
/*30:*/
#line 553 "./polyenum.w"

save_rightbound= rightbound;
rightbound= rightend;
occ[rightend]= 1;
while(1){
if(rightbound-leftbound>=maxlen)break;
/*43:*/
#line 710 "./polyenum.w"

/*39:*/
#line 668 "./polyenum.w"

for(j= leftbound,l= 0;j<=rightbound;j++)
if(occ[j])l++;
if(m+l<=nmax){
weight= l;
/*36:*/
#line 644 "./polyenum.w"

for(j= leftbound,k= l= 0;j<=rightbound;j++,k++)
if(!occ[j])newpat[k]= 0;
else if(leader[j]<j)newpat[k]= class[leader[j]];
else newpat[k]= class[j]= ++l;

/*:36*/
#line 673 "./polyenum.w"
;
/*38:*/
#line 659 "./polyenum.w"

for(j= rightbound,k= l= 0;j>=leftbound;j--,k++)
if(!occ[j])backpat[k]= 0;
else if(class[leader[j]]&0x10)backpat[k]= class[leader[j]]&0xf;
else backpat[k]= ++l,class[leader[j]]= l+0x10;

/*:38*/
#line 674 "./polyenum.w"
;
len= k;
bestpat= newpat;
for(j= 1;j<k;j++)
if(newpat[j]<backpat[j])break;
else if(newpat[j]> backpat[j]){
bestpat= backpat;break;
}
k= conn_distance(k,bestpat);
l= weight+k;
}

/*:39*/
#line 711 "./polyenum.w"
;

/*:43*/
#line 559 "./polyenum.w"
;
if(m+l> nmax)break;
/*44:*/
#line 718 "./polyenum.w"

q= lookup(len,bestpat,m+weight);
if(q->aux)q->aux->degree++;
else{
s= succ_ptr++;
s->pat= q;
q->aux= s;
s->degree= 1;
s->weight= weight;
s->death= nmax-l+weight;
}

/*:44*/
#line 561 "./polyenum.w"
;
for(j= rightend;;j++){
if(occ[j])occ[j]= 0;
else break;
}
occ[j]= 1;
if(j> rightbound)rightbound= j;
}
occ[rightbound]= 0;
rightbound= save_rightbound;

/*:30*/
#line 538 "./polyenum.w"
;
for(j= 15;;j--){
if(occ[j])occ[j]= 0;
else break;
}
occ[j]= 1;
if(j<leftbound)leftbound= j;
if(rightbound-leftbound>=maxlen)break;
/*43:*/
#line 710 "./polyenum.w"

/*39:*/
#line 668 "./polyenum.w"

for(j= leftbound,l= 0;j<=rightbound;j++)
if(occ[j])l++;
if(m+l<=nmax){
weight= l;
/*36:*/
#line 644 "./polyenum.w"

for(j= leftbound,k= l= 0;j<=rightbound;j++,k++)
if(!occ[j])newpat[k]= 0;
else if(leader[j]<j)newpat[k]= class[leader[j]];
else newpat[k]= class[j]= ++l;

/*:36*/
#line 673 "./polyenum.w"
;
/*38:*/
#line 659 "./polyenum.w"

for(j= rightbound,k= l= 0;j>=leftbound;j--,k++)
if(!occ[j])backpat[k]= 0;
else if(class[leader[j]]&0x10)backpat[k]= class[leader[j]]&0xf;
else backpat[k]= ++l,class[leader[j]]= l+0x10;

/*:38*/
#line 674 "./polyenum.w"
;
len= k;
bestpat= newpat;
for(j= 1;j<k;j++)
if(newpat[j]<backpat[j])break;
else if(newpat[j]> backpat[j]){
bestpat= backpat;break;
}
k= conn_distance(k,bestpat);
l= weight+k;
}

/*:39*/
#line 711 "./polyenum.w"
;

/*:43*/
#line 546 "./polyenum.w"
;
if(m+l> nmax)break;
/*44:*/
#line 718 "./polyenum.w"

q= lookup(len,bestpat,m+weight);
if(q->aux)q->aux->degree++;
else{
s= succ_ptr++;
s->pat= q;
q->aux= s;
s->degree= 1;
s->weight= weight;
s->death= nmax-l+weight;
}

/*:44*/
#line 548 "./polyenum.w"
;
}
occ[leftbound]= 0;
leftbound= save_leftbound;

/*:29*/
#line 516 "./polyenum.w"
;
advance:for(k= 0;;k++){
j= intcell[k];
if(occ[j])occ[j]= 0;
else break;
}
if(!j)break;
occ[j]= 1;
/*42:*/
#line 704 "./polyenum.w"

/*32:*/
#line 590 "./polyenum.w"

for(j= 16;!occ[j];j++);
leftbound= j;
for(j= rightend-1;!occ[j];j--);
rightbound= j;

/*:32*/
#line 705 "./polyenum.w"
;
/*39:*/
#line 668 "./polyenum.w"

for(j= leftbound,l= 0;j<=rightbound;j++)
if(occ[j])l++;
if(m+l<=nmax){
weight= l;
/*36:*/
#line 644 "./polyenum.w"

for(j= leftbound,k= l= 0;j<=rightbound;j++,k++)
if(!occ[j])newpat[k]= 0;
else if(leader[j]<j)newpat[k]= class[leader[j]];
else newpat[k]= class[j]= ++l;

/*:36*/
#line 673 "./polyenum.w"
;
/*38:*/
#line 659 "./polyenum.w"

for(j= rightbound,k= l= 0;j>=leftbound;j--,k++)
if(!occ[j])backpat[k]= 0;
else if(class[leader[j]]&0x10)backpat[k]= class[leader[j]]&0xf;
else backpat[k]= ++l,class[leader[j]]= l+0x10;

/*:38*/
#line 674 "./polyenum.w"
;
len= k;
bestpat= newpat;
for(j= 1;j<k;j++)
if(newpat[j]<backpat[j])break;
else if(newpat[j]> backpat[j]){
bestpat= backpat;break;
}
k= conn_distance(k,bestpat);
l= weight+k;
}

/*:39*/
#line 706 "./polyenum.w"
;

/*:42*/
#line 524 "./polyenum.w"
;
if(m+l> nmax)goto advance;
/*44:*/
#line 718 "./polyenum.w"

q= lookup(len,bestpat,m+weight);
if(q->aux)q->aux->degree++;
else{
s= succ_ptr++;
s->pat= q;
q->aux= s;
s->degree= 1;
s->weight= weight;
s->death= nmax-l+weight;
}

/*:44*/
#line 526 "./polyenum.w"
;
}

/*:28*/
#line 508 "./polyenum.w"
;
move:/*25:*/
#line 463 "./polyenum.w"

touched[0]= 128;
for(k= 0;;k++){
j= adjcell[k];
if(occ[j])touched[dig[j-1]]--,touched[dig[j]]--,occ[j]= 0;
else break;
}
if(!j)break;
touched[dig[j-1]]++,touched[dig[j]]++,occ[j]= 1;
/*26:*/
#line 482 "./polyenum.w"

for(k--;k>=0;k--){
j= adjcell[k];
if(!touched[first[j]])touched[dig[j-1]]++,touched[dig[j]]++,occ[j]= 1;
}

/*:26*/
#line 472 "./polyenum.w"
;

/*:25*/
#line 509 "./polyenum.w"
;
}
/*46:*/
#line 739 "./polyenum.w"

if(!appeared[2]){
s= succ_ptr++;
s->pat= NULL,s->degree= 1,s->weight= 0,s->death= m;
}

/*:46*/
#line 511 "./polyenum.w"
;
}

/*:27*/
#line 793 "./polyenum.w"
;
/*50:*/
#line 798 "./polyenum.w"

/*60:*/
#line 939 "./polyenum.w"

{
if(verbose)print_slice(p,m,0);
buf= (new_pred_code<<27)+patt_code(p);
if(!out_it())panic("Bad write of predecessor pattern");
}

/*:60*/
#line 799 "./polyenum.w"
;
for(s= succ_table;s<succ_ptr;s++){
/*61:*/
#line 946 "./polyenum.w"

{
if(verbose){
if(s->degree==1)printf("->");
else printf("-%d>",s->degree);
if(!s->pat)printf("0:%d\n",m);
else print_slice(s->pat,m+s->weight,
patt_code(s->pat)> prev_pat?s->death:0);
}
if(s->degree> 30)panic("Surprisingly large arc multiplicity");
if(!s->pat)buf= (1<<27)+(1<<26);
else if(patt_code(s->pat)<=prev_pat)
buf= (s->degree<<27)+(1<<26)+patt_code(s->pat);
else{
prev_pat++,buf= (s->degree<<27)+((s->weight+m)<<8)+s->death;
slices+= s->death-(s->weight+m)+1;
}
if(!out_it())panic("Bad write of transition");
arcs++;
}

/*:61*/
#line 801 "./polyenum.w"
;
if(s->pat)s->pat->aux= NULL;
}

/*:50*/
#line 794 "./polyenum.w"
;
}
}

/*:49*/
#line 74 "./polyenum.w"
;
/*62:*/
#line 969 "./polyenum.w"

if(patt_ptr!=patt_table+prev_pat)panic("Output out of sync");
printf("All done!\n");
printf(" %d patterns generated,",prev_pat);
printf(" %d slices,",slices);
printf(" %d arcs.\n",arcs);
close_it();

/*:62*/
#line 75 "./polyenum.w"
;
}

/*:1*/
