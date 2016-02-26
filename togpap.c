/*1:*/
#line 16 "togpap.w"

#include <gb_graph.h> 
#include <gb_lisa.h> 
#include <math.h> 
#include <time.h> 
#define m 360
#define n 250
#define lisacode 1
#define spherecode 2
#define fscode 1
#define odithcode 2
#define ddiffcode 3
#define sdiffcode 4
#define ariescode 5 \

#define row u.I
#define col v.I
#define weight w.I
#define del_i a.I
#define del_j b.I \

#define alpha 0.4375
#define beta 0.1875
#define gamma 0.3125
#define delta 0.0625
#define check(i,j) {if(A[i][j]<lo_A) lo_A= A[i][j]; \
if(A[i][j]> hi_A) hi_A= A[i][j];} \


#line 21 "togpap.w"

time_t clock;
double A[m+2][256];
int board[10][10];
Graph*gg;
int kk;
/*6:*/
#line 120 "togpap.w"

int di[4]= {0,1,0,1};
int dj[4]= {0,1,1,0};

/*:6*//*8:*/
#line 145 "togpap.w"

char name_buffer[]= "99";

/*:8*//*14:*/
#line 224 "togpap.w"

double edge_accum;
double lo_A= 100000.0,hi_A= -100000.0;

/*:14*//*16:*/
#line 241 "togpap.w"

double accum;
double block_err[(m+7)>>3][(n+7)>>3];
int bad_blocks;

/*:16*//*19:*/
#line 274 "togpap.w"

int barons;
int bad_barons;
double lo_err= 100000.0,hi_err= -100000.0;

/*:19*//*26:*/
#line 352 "togpap.w"

int ll;
int inxi[32],inxj[32];
double val[32];

/*:26*/
#line 27 "togpap.w"

/*7:*/
#line 124 "togpap.w"

void store(i,j)
int i,j;
{
Vertex*v;
if(i<1)i+= 8;else if(i> 8)i-= 8;
if(j<1)j+= 8;else if(j> 8)j-= 8;
board[i][j]= kk;v= gg->vertices+kk;
sprintf(name_buffer,"%d",kk);
v->name= gb_save_string(name_buffer);
v->row= i;v->col= j;
kk++;
}

void store_eight(i,j)
int i,j;
{
store(i,j);store(i-4,j+4);store(1-j,i-4);store(5-j,i);
store(j,5-i);store(4+j,1-i);store(5-i,5-j);store(1-i,1-j);
}

/*:7*//*25:*/
#line 340 "togpap.w"

rank(i,j)
int i,j;
{
register double key= A[i][j]-board[i&7][j&7]/32.0;
register int l;
for(l= ll;l> 0;l--)
if(key>=val[l-1])break;
else inxi[l]= inxi[l-1],inxj[l]= inxj[l-1],val[l]= val[l-1];
inxi[l]= i;inxj[l]= j;val[l]= key;ll++;
}

/*:25*/
#line 28 "togpap.w"


main(argc,argv)
int argc;char*argv[];
{
register int i,j,k,l,ii,jj;
register double err;
register Graph*g;
register Vertex*u,*v;
register Arc*a;
int imagecode,sharpcode,methodcode;
/*2:*/
#line 48 "togpap.w"

if(argc!=4||sscanf(argv[1],"%d",&imagecode)!=1||
sscanf(argv[2],"%d",&sharpcode)!=1||
sscanf(argv[3],"%d",&methodcode)!=1){
usage:
fprintf(stderr,"Usage: %s imagecode sharpcode methodcode\n",argv[0]);
fprintf(stderr," Mona Lisa = %d, Sphere = %d\n",lisacode,spherecode);
fprintf(stderr," unretouched = 0, edges enhanced = 1\n");
fprintf(stderr," Floyd-Steinberg = %d, ordered dither = %d,\n",
fscode,odithcode);
fprintf(stderr," dot diffusion = %d, smooth dot diffusion = %d,\n",
ddiffcode,sdiffcode);
fprintf(stderr," ARIES = %d\n",ariescode);
exit(0);
}

/*:2*/
#line 39 "togpap.w"
;
/*3:*/
#line 64 "togpap.w"

if(imagecode==lisacode){Area workplace;
register int*mtx= lisa(m,n,255,0,0,0,0,0,0,workplace);
for(i= 0;i<m;i++)for(j= 0;j<n;j++)
A[i+1][j+1]= pow(1.0-(*(mtx+i*n+j)+0.5)/256.0,2.0);
fprintf(stderr,"(Mona Lisa image loaded)\n");
}else if(imagecode==spherecode){
for(i= 1;i<=m;i++)for(j= 1;j<=n;j++){
register double x= (i-120.0)/111.5,y= (j-120.0)/111.5;
if(x*x+y*y>=1.0)A[i][j]= (1500.0*i+j*j)/1000000.0;
else A[i][j]= (9.0+x-4.0*y-8.0*sqrt(1.0-x*x-y*y))/18.0;
}
fprintf(stderr,"(Sphere image loaded)\n");
}else goto usage;

/*:3*/
#line 40 "togpap.w"
;
/*4:*/
#line 79 "togpap.w"

if(sharpcode==1){
for(i= 1;i<=m;i++)for(j= 1;j<=n;j++)
A[i-1][j-1]= 9*A[i][j]-
(A[i-1][j-1]+A[i-1][j]+A[i-1][j+1]+A[i][j-1]+
A[i][j+1]+A[i+1][j-1]+A[i+1][j]+A[i+1][j+1]);
for(i= m;i> 0;i--)for(j= n;j> 0;j--)
A[i][j]= (A[i-1][j-1]<=0.0?0.0:A[i-1][j-1]>=1.0?1.0:A[i-1][j-1]);
for(i= 0;i<m;i++)A[i][0]= 0.0;
for(j= 1;j<n;j++)A[0][j]= 0.0;
fprintf(stderr,"(with enhanced edges)\n");
}else if(sharpcode==0)fprintf(stderr,"(no sharpening)\n");
else goto usage;

/*:4*/
#line 41 "togpap.w"
;
/*5:*/
#line 93 "togpap.w"

switch(methodcode){
case fscode:fprintf(stderr,"(using Floyd-Steinberg error diffusion)\n");goto done;
case odithcode:fprintf(stderr,"(using ordered dithering)\n");
for(i= 0;i<4;i++)for(j= 0;j<4;j++)for(k= 0;k<4;k++){
ii= 4*di[k]+2*di[j]+di[i]+2;
jj= 4*dj[k]+2*dj[j]+dj[i]+2;
kk= 16*i+4*j+k;
board[8-(jj&7)][1+(ii&7)]= kk;
}
goto finishit;
case ddiffcode:fprintf(stderr,"(using dot diffusion)\n");break;
case sdiffcode:fprintf(stderr,"(using smooth dot diffusion)\n");break;
case ariescode:fprintf(stderr,"(using ARIES)\n");break;
default:goto usage;
}
/*9:*/
#line 154 "togpap.w"

kk= 0;gg= g= gb_new_graph(64);
store_eight(7,2);store_eight(8,3);store_eight(8,2);store_eight(8,1);
store_eight(1,4);store_eight(1,3);store_eight(1,2);store_eight(2,3);

/*:9*/
#line 109 "togpap.w"
;
finishit:
for(i= 1;i<=8;i++)
board[i][0]= board[i][8],board[i][9]= board[i][1];
for(j= 0;j<=9;j++)
board[0][j]= board[8][j],board[9][j]= board[1][j];
if(methodcode>=ddiffcode)
/*11:*/
#line 165 "togpap.w"

if(methodcode==ddiffcode){
for(v= g->vertices;v<g->vertices+64;v++){
i= v->row;j= v->col;v->weight= 0;
for(ii= i-1;ii<=i+1;ii++)for(jj= j-1;jj<=j+1;jj++){
u= g->vertices+board[ii][jj];
if(u> v){
gb_new_arc(v,u,0);
v->arcs->del_i= ii-i;v->arcs->del_j= jj-j;
v->weight+= 3-(ii-i)*(ii-i)-(jj-j)*(jj-j);
}
}
}
}else{
for(v= g->vertices;v<g->vertices+64;v++){
i= v->row;j= v->col;
for(jj= j-3;jj<=j+3;jj++){register int del= (jj<j?j-jj:jj-j);
for(ii= i-3+del;ii<=i+4-del;ii++){
u= g->vertices+board[ii&7][jj&7];
if(u> v){
gb_new_arc(v,u,0);
v->arcs->del_i= ii-i;v->arcs->del_j= jj-j;
}
}
}
}
for(i= 0;i<10;i++)for(j= 0;j<10;j++)board[i][j]>>= 1;
}

/*:11*/
#line 116 "togpap.w"
;
/*10:*/
#line 159 "togpap.w"

for(i= 1;i<=8;i++){
for(j= 1;j<=8;j++)fprintf(stderr," %2d",board[i][j]);
fprintf(stderr,"\n");
}

/*:10*/
#line 117 "togpap.w"
;
done:

/*:5*/
#line 42 "togpap.w"
;
/*33:*/
#line 411 "togpap.w"

switch(methodcode){
case fscode:/*12:*/
#line 205 "togpap.w"

for(i= 1;i<=m;i++)for(j= 1;j<=n;j++){
err= A[i][j];
if(err>=.5)err-= 1.0;
A[i][j]-= err;
A[i][j+1]+= err*alpha;check(i,j+1);
A[i+1][j-1]+= err*beta;check(i+1,j-1);
A[i+1][j]+= err*gamma;check(i+1,j);
A[i+1][j+1]+= err*delta;check(i+1,j+1);
}

/*:12*/
#line 413 "togpap.w"
;break;
case odithcode:/*15:*/
#line 231 "togpap.w"

for(i= 1;i<=m;i++)for(j= 1;j<=n;j++){
k= board[i&7][j&7];
err= A[i][j];
if(err>=(k+0.5)/64.0)err-= 1.0;
A[i][j]-= err;
accum+= fabs(err);
block_err[(i-1)>>3][(j-1)>>3]+= err;
}

/*:15*/
#line 414 "togpap.w"
;break;
case ddiffcode:/*18:*/
#line 258 "togpap.w"

for(v= g->vertices;v<g->vertices+64;v++)
for(i= v->row;i<=m;i+= 8)for(j= v->col;j<=n;j+= 8){
err= A[i][j];
if(err>=.5)err-= 1.0;
A[i][j]-= err;
if(v->arcs)/*20:*/
#line 279 "togpap.w"

for(a= v->arcs;a;a= a->next){
ii= i+a->del_i;jj= j+a->del_j;
A[ii][jj]+= err*(double)(3-a->del_i*a->del_i-a->del_j*a->del_j)
/(double)v->weight;
check(ii,jj);
}

/*:20*/
#line 264 "togpap.w"

else{
accum+= fabs(err);
barons++;
if(fabs(err)> 0.5)bad_barons++;
if(err<lo_err)lo_err= err;
if(err> hi_err)hi_err= err;
}
}

/*:18*/
#line 415 "togpap.w"
;break;
case sdiffcode:/*21:*/
#line 290 "togpap.w"

for(v= g->vertices;v<g->vertices+64;v++)
for(i= v->row;i<=m;i+= 8)for(j= v->col;j<=n;j+= 8){
k= (v-g->vertices)>>1;
err= A[i][j];
if(err>=.5/(double)(32-k))err-= 1.0;
A[i][j]-= err;
if(v->arcs)/*22:*/
#line 310 "togpap.w"

for(a= v->arcs;a;a= a->next){
ii= i+a->del_i;jj= j+a->del_j;
if(ii> 0&&ii<=m&&jj> 0&&jj<=n){
A[ii][jj]+= err/(double)(31-k);check(ii,jj);
}else edge_accum+= fabs(err);
}

/*:22*/
#line 297 "togpap.w"

else{
accum+= fabs(err);
barons++;
if(fabs(err)> 0.5)bad_barons++;
if(err<lo_err)lo_err= err;
if(err> hi_err)hi_err= err;
}
}

/*:21*/
#line 416 "togpap.w"
;break;
case ariescode:/*24:*/
#line 326 "togpap.w"

for(i= -1;i<=m+3;i+= 4)
for(j= (i&4)?2:-2;j<=n+3;j+= 8){double s= 0.5;
ll= 0;
for(jj= j-3;jj<=j+3;jj++){register int del= (jj<j?j-jj:jj-j);
for(ii= i-3+del;ii<=i+4-del;ii++)if(ii> 0&&ii<=m&&jj> 0&&jj<=n)
s+= A[ii][jj],rank(ii,jj);
}
/*27:*/
#line 359 "togpap.w"

if(ll){barons++;accum+= fabs(s-0.5-(int)s);}
while(ll> 0){
ll--;s-= 1.0;
ii= inxi[ll];jj= inxj[ll];
err= A[ii][jj];
if(s>=0.0)err-= 1.0;
A[ii][jj]-= err;
}

/*:27*/
#line 334 "togpap.w"
;
}

/*:24*/
#line 417 "togpap.w"
;break;
}

/*:33*/
#line 43 "togpap.w"
;
/*29:*/
#line 377 "togpap.w"

/*30:*/
#line 383 "togpap.w"

printf("%%!PS\n");
printf("%%%%BoundingBox: 0 0 %d %d\n",n,m);
printf("%%%%Creator: togpap\n");
clock= time(0);
printf("%%%%CreationDate: %s",ctime(&clock));
printf("%%%%Pages: 1\n");
printf("%%%%EndProlog\n");
printf("%%%%Page: 1 1\n");
printf("/picstr %d string def\n",(n+7)>>3);
printf("%d %d scale\n",n,m);
printf("%d %d true [%d 0 0 -%d 0 %d]\n",n,m,n,m,m);
printf(" {currentfile picstr readhexstring pop} imagemask\n");

/*:30*/
#line 378 "togpap.w"
;
/*31:*/
#line 397 "togpap.w"

for(i= 1;i<=m;i++){
for(j= 1;j<=n;j+= 8){
for(k= 0,l= 0;k<8;k++)l= l+l+(A[i][j+k]?1:0);
printf("%02x",l);
}
printf("\n");
}

/*:31*/
#line 379 "togpap.w"
;
/*32:*/
#line 406 "togpap.w"

printf("%%%%EOF\n");

/*:32*/
#line 380 "togpap.w"
;


/*:29*/
#line 44 "togpap.w"
;
/*34:*/
#line 420 "togpap.w"

switch(methodcode){
case odithcode:/*17:*/
#line 246 "togpap.w"

fprintf(stderr,"Total undiffused error: %.20g\n",accum);
for(i= 0,accum= 0.0;i<m;i+= 8)for(j= 0;j<n;j+= 8){
if(fabs(block_err[i>>3][j>>3])> 1.0)bad_blocks++;
accum+= fabs(block_err[i>>3][j>>3]);
}
fprintf(stderr,"Total block error: %.20g (%d bad)\n",accum,bad_blocks);

/*:17*/
#line 422 "togpap.w"
;break;
case ariescode:/*28:*/
#line 369 "togpap.w"

fprintf(stderr,"Total lossage %.20g in %d dots\n",accum,barons);

/*:28*/
#line 423 "togpap.w"
;break;
case ddiffcode:
case sdiffcode:/*23:*/
#line 318 "togpap.w"

fprintf(stderr,"Total undiffused error %.20g at %d barons\n",accum,barons);
fprintf(stderr,"  (%d bad, min %.20g, max %.20g)\n",bad_barons,lo_err,hi_err);

/*:23*/
#line 425 "togpap.w"
;
case fscode:/*13:*/
#line 216 "togpap.w"

if(methodcode!=sdiffcode){
for(i= 0;i<=m+1;i++)edge_accum+= fabs(A[i][0])+fabs(A[i][n+1]);
for(j= 1;j<=n;j++)edge_accum+= fabs(A[0][j])+fabs(A[m+1][j]);
}
fprintf(stderr,"Total leakage at boundaries: %.20g\n",edge_accum);
fprintf(stderr,"Data remained between %.20g and %.20g\n",lo_A,hi_A);

/*:13*/
#line 426 "togpap.w"
;break;
}

/*:34*/
#line 45 "togpap.w"
;
}

/*:1*/
