#define maxm 100
#define maxn 100
#define bufsize 5*maxn+2 \

#define eqbit(k,kk) (((i>>k) ^(ii>>kk) ) &1)  \

#define pop(i,j,k) i= s>>16,j= (s>>4) &0xfff,k= s&0x7,s= inout[i][j][k]
#define push(ii,jj,kk) {if(inout[ii][jj][(kk) &0x7]==0)  \
inout[ii][jj][(kk) &0x7]= s, \
s= ((ii) <<16) +((jj) <<4) +((kk) &0x7) ;} \

/*3:*/
#line 109 "./celtic-paths.w"

#include <stdio.h> 
#include <stdlib.h> 
char buf[bufsize],entry[8];
int a[maxm][maxn];
int b[maxm][maxn];
int codetable[0x7778];
char bw[maxm][maxn][8];
int inout[maxm][maxn][8];
/*11:*/
#line 363 "./celtic-paths.w"

char magic[56]= {
1,3,5,7,
7,1,3,5,
1,3,5,0,
7,2,3,5,
1,4,5,7,
7,1,3,6,
0,3,5,7,
7,1,2,5,
1,3,4,7,
6,1,3,5,
1,4,5,0,
7,2,3,6,
0,3,4,7,
6,1,2,5};

/*:11*//*22:*/
#line 495 "./celtic-paths.w"

char offset[16]= {0,1,2,0,2,3,0,0,1,0,3,0,0,0,0,0};

/*:22*/
#line 118 "./celtic-paths.w"
;
main(){
register int i,j,k,ii,jj,kk,m,n,s,t;
/*4:*/
#line 133 "./celtic-paths.w"

for(i= 1;i<0x7778;i++)codetable[i]= -1;
codetable[0x1537]= 0;codetable[0]= 040;
codetable[0x0300]= 044,
codetable[0x0500]= 046,
codetable[0x1300]= 050,
codetable[0x1400]= 052,
codetable[0x1500]= 054,
codetable[0x1600]= 056,
codetable[0x1700]= 060,
codetable[0x2500]= 062,
codetable[0x2700]= 064,
codetable[0x3500]= 066,
codetable[0x3600]= 070,
codetable[0x3700]= 072,
codetable[0x4700]= 074,
codetable[0x5700]= 076,
codetable[0x1357]= 0100,
codetable[0x1735]= 0104,
codetable[0x0513]= 0110,
codetable[0x2735]= 0114,
codetable[0x1457]= 0120,
codetable[0x1736]= 0124,
codetable[0x0357]= 0130,
codetable[0x1725]= 0134,
codetable[0x1347]= 0140,
codetable[0x1635]= 0144,
codetable[0x0514]= 0150,
codetable[0x2736]= 0154,
codetable[0x0347]= 0160,
codetable[0x1625]= 0164,
codetable[0x0314]= 0170,
codetable[0x2536]= 0174,
codetable[0x0547]= 0200,
codetable[0x1627]= 0204,
codetable[0x0315]= 0210,
codetable[0x2537]= 0214,
codetable[0x1547]= 0220,
codetable[0x1637]= 0224,
codetable[0x0537]= 0230,
codetable[0x1527]= 0234,
codetable[0x1437]= 0240,
codetable[0x1536]= 0244,
codetable[0x0316]= 0250,
codetable[0x0325]= 0254,
codetable[0x2547]= 0260,
codetable[0x1647]= 0264,
codetable[0x0527]= 0270,
codetable[0x1425]= 0274,
codetable[0x1436]= 0300,
codetable[0x0536]= 0304,
codetable[0x0317]= 0310,
codetable[0x1325]= 0314,
codetable[0x3547]= 0320,
codetable[0x1657]= 0324,
codetable[0x0517]= 0330,
codetable[0x1327]= 0334,
codetable[0x1435]= 0340,
codetable[0x3657]= 0344,
codetable[0x0516]= 0350,
codetable[0x0327]= 0354,
codetable[0x1425]= 0360,
codetable[0x3647]= 0364;

/*:4*/
#line 121 "./celtic-paths.w"
;
/*5:*/
#line 203 "./celtic-paths.w"

for(m= 0;;m++){
if(!fgets(buf,bufsize,stdin))break;
for(j= 0;;j++){
k= 5*j;
if(j==n&&m> 0){
fprintf(stderr,"Missing `.' at the end of row %d!\n",m);
exit(-1);
}
/*6:*/
#line 237 "./celtic-paths.w"

for(jj= 0;jj<4;jj++)entry[jj]= buf[5*j+jj];
if(entry[0]==' '){
if(entry[1]!=' '||entry[2]!=' '||entry[3]!=' ')goto badentry;
else a[m][j]= 0;
}else{
if(entry[0]<'0'||entry[0]> '7')goto badentry;
if(entry[1]<'0'||entry[1]> '7')goto badentry;
a[m][j]= ((entry[0]-'0')<<12)+((entry[1]-'0')<<8);
b[m][j]= (1<<(entry[0]-'0'))+(1<<(entry[1]-'0'));
if(entry[2]==' '){
if(entry[3]!=' ')goto badentry;
}else{
if(entry[2]<'0'||entry[2]> '7')goto badentry;
if(entry[3]<'0'||entry[3]> '7')goto badentry;
a[m][j]+= ((entry[2]-'0')<<4)+(entry[3]-'0');
b[m][j]+= (1<<(entry[2]-'0'))+(1<<(entry[3]-'0'));
}
}
if(codetable[a[m][j]]<0){
fprintf(stderr,"Sorry, %s isn't a legal tile (row %d, col %d)!\n",
entry,m,j);
exit(-4);
}

/*:6*/
#line 212 "./celtic-paths.w"
;
if(buf[k+4]=='.'){
if(m==0)n= j+1;
else if(n!=j+1){
fprintf(stderr,"Premature `.' in row %d!\n",m);
exit(-2);
}
break;
}else if(buf[k+4]!=' '){
fprintf(stderr,"Tile spec in row %d, col %d not followed by blank!\n",
m,j);
exit(-5);
}
}
continue;
badentry:fprintf(stderr,"Bad entry (%s) in row %d and column %d!\n",
entry,m,j);
exit(-3);
}
if(m==0){
fprintf(stderr,"There was no input!\n");exit(-4);
}
fprintf(stderr,"OK, I've successfully read %d rows and %d columns.\n",m,n);
/*7:*/
#line 264 "./celtic-paths.w"

t= 0;
for(j= 0;j<=m;j++)for(jj= 0;jj<n;jj++){
i= (j> 0?b[j-1][jj]:0);
ii= (j<m?b[j][jj]:0);
if(eqbit(4,2)+eqbit(5,1)+eqbit(6,0)){
fprintf(stderr,"Inconsistent tiles %04x/%04x (row %d, col %d)!\n",
j> 0?a[j-1][jj]:0,a[j][jj],j,jj);
t++;
}
}
for(jj= 0;jj<=n;jj++)for(j= 0;j<m;j++){
i= (jj> 0?b[j][jj-1]:0);
ii= (jj<n?b[j][jj]:0);
if(eqbit(2,0)+eqbit(3,7)+eqbit(4,6)){
fprintf(stderr,"Inconsistent tiles %04x,%04x (row %d, col %d)!\n",
jj> 0?a[j][jj-1]:0,a[j][jj],j,jj);
t++;
}
}
if(t){
fprintf(stderr,"Sorry, I can't go on (errs=%d).\n",t);
exit(-69);
}

/*:7*/
#line 235 "./celtic-paths.w"
;

/*:5*/
#line 122 "./celtic-paths.w"
;
/*8:*/
#line 310 "./celtic-paths.w"

for(i= 0;i<m;i++)for(j= 0;j<n;j++){
bw[i][j][0]= (i==0?0:bw[i-1][j][5]);
for(k= 1;k<8;k++)
bw[i][j][k]= (b[i][j]&(1<<k)?1:0)^bw[i][j][k-1];
}

/*:8*/
#line 123 "./celtic-paths.w"
;
/*9:*/
#line 331 "./celtic-paths.w"

inout[0][0][0]= -1;
for(s= 0;s>=0;){
pop(i,j,k);
/*10:*/
#line 341 "./celtic-paths.w"

switch(k){
case 0:case 1:if(i> 0)push(i-1,j,5-k);break;
case 2:case 3:if(j<n-1)push(i,j+1,9-k);break;
case 4:case 5:if(i<m-1)push(i+1,j,5-k);break;
case 6:case 7:if(j> 0)push(i,j-1,9-k);break;
}
if((b[i][j]&(1<<k))==0)push(i,j,k+7);
kk= (k+1)&0x7;
if((b[i][j]&(1<<kk))==0)push(i,j,kk);
/*12:*/
#line 380 "./celtic-paths.w"

kk= codetable[a[i][j]]-0100;
if(kk<070&&kk>=0){
if(k==magic[kk+1])push(i,j,magic[kk]+7);
if(k==((magic[kk]+7)&0x7))push(i,j,magic[kk+1]);
if(k==magic[kk+3])push(i,j,magic[kk+2]+7);
if(k==((magic[kk+2]+7)&0x7))push(i,j,magic[kk+3]);
}

/*:12*/
#line 351 "./celtic-paths.w"
;

/*:10*/
#line 335 "./celtic-paths.w"
;
}

/*:9*/
#line 124 "./celtic-paths.w"
;
/*13:*/
#line 412 "./celtic-paths.w"

/*14:*/
#line 417 "./celtic-paths.w"

printf("%% begin output of CELTIC-PATHS\n");
printf("\\font\\celtica=celtica13 \\font\\celticb=celticb13\n\n");
printf("\\begingroup\\celtica\n");
printf("\\catcode`\\|=\\active \\def|#1|{\\hskip#1em}\n");
printf("\\catcode`\\-=\\active \\def-#1#2#3{\\celtica\\char'#1#2#3}\n");
printf("\\catcode`\\+=\\active \\def+#1#2#3{\\celticb\\char'#1#2#3}\n");
printf("\\offinterlineskip\\baselineskip=1em\n");
printf("\\let\\par=\\cr \\obeylines \\halign{#\\hfil\n");

/*:14*/
#line 413 "./celtic-paths.w"
;
for(i= 0;i<m;i++)/*16:*/
#line 430 "./celtic-paths.w"

{
s= 0;
for(j= 0;j<n;j++)/*17:*/
#line 437 "./celtic-paths.w"

{
kk= codetable[a[i][j]];
ii= bw[i][j][0];
if(kk>=0100)/*21:*/
#line 485 "./celtic-paths.w"

{
t= (inout[i][j][0]?8:0);
for(k= 1,jj= 4;jj;k++)if(b[i][j]&(1<<k)){
t+= (inout[i][j][k]?jj:0);
jj>>= 1;
}
kk+= offset[t];
}

/*:21*/
#line 441 "./celtic-paths.w"

else if(kk==040)/*18:*/
#line 452 "./celtic-paths.w"

{
if(inout[i][j][0]){
s++;continue;
}
}

/*:18*/
#line 442 "./celtic-paths.w"

else if(kk==0)/*20:*/
#line 471 "./celtic-paths.w"

{
kk= (inout[i][j][1]?1:0)+
(inout[i][j][3]?8:0)+
(inout[i][j][5]?4:0)+
(inout[i][j][7]?2:0);
}

/*:20*/
#line 443 "./celtic-paths.w"

else/*19:*/
#line 459 "./celtic-paths.w"

{
for(k= 0;k<8;k++)if(inout[i][j][k]){
kk++;break;
}
}

/*:19*/
#line 444 "./celtic-paths.w"
;
if(s){
printf("|%d|",s);
s= 0;
}
printf("%c%03o",ii?'+':'-',kk);
}

/*:17*/
#line 433 "./celtic-paths.w"
;
printf("\n");
}

/*:16*/
#line 414 "./celtic-paths.w"
;
/*15:*/
#line 427 "./celtic-paths.w"

printf("}\\endgroup\n");

/*:15*/
#line 415 "./celtic-paths.w"
;

/*:13*/
#line 125 "./celtic-paths.w"
;
}

/*:3*/
