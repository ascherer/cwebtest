#define maxx 20
#define maxy 20 \

#define pp(xx,yy) ((xx) >=0&&(yy) >=0?p[xx][yy]:0)  \

#define taut (2<<25) 
#define sign (1U<<31)  \

#define newlit(x,y,c,k) clause[clauseptr++]= ((c) <<28) +((k) <<25) +((x) <<12) +(y) 
#define newcomplit(x,y,c,k)  \
clause[clauseptr++]= sign+((c) <<28) +((k) <<25) +((x) <<12) +(y)  \

/*2:*/
#line 94 "./sat-life.w"

#include <stdio.h> 
#include <stdlib.h> 
char p[maxx+2][maxy+2];
char have_b[maxx+2][maxy+2];
char have_d[maxx+2][maxy+2];
char have_e[maxx+2][maxy+4];
char have_f[maxx+4][maxy+2];
int tt;
int xmax,ymax;
int xmin= maxx,ymin= maxy;
char timecode[]= "abcdefghijklmnopqrstuvwxyz"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

char buf[maxy+2];
unsigned int clause[4];
int clauseptr;
/*6:*/
#line 191 "./sat-life.w"

void outclause(void){
register int c,k,x,y,p;
for(p= 0;p<clauseptr;p++)
if(clause[p]==taut)goto done;
for(p= 0;p<clauseptr;p++)if(clause[p]!=taut+sign){
if(clause[p]>>31)printf(" ~");else printf(" ");
c= (clause[p]>>28)&0x7;
k= (clause[p]>>25)&0x7;
x= (clause[p]>>12)&0xfff;
y= clause[p]&0xfff;
if(c)printf("%d%c%d%c%d",
x,timecode[tt],y,c+'@',k);
else if(k==7)printf("%d%c%dx",
x,timecode[tt],y);
else printf("%d%c%d",
x,timecode[tt+k],y);
}
printf("\n");
done:clauseptr= 0;
}

/*:6*//*7:*/
#line 215 "./sat-life.w"

void applit(int x,int y,int bar,int k){
if(k==0&&(x<xmin||x> xmax||y<ymin||y> ymax||p[x][y]==0))
clause[clauseptr++]= (bar?0:sign)+taut;
else clause[clauseptr++]= (bar?sign:0)+(k<<25)+(x<<12)+y;
}

/*:7*//*8:*/
#line 232 "./sat-life.w"

void d(int x,int y){
register x1= x-1,x2= x,yy= y+1;
if(have_d[x][y]!=tt+1){
applit(x1,yy,1,0),newlit(x,y,4,1),outclause();
applit(x2,yy,1,0),newlit(x,y,4,1),outclause();
applit(x1,yy,1,0),applit(x2,yy,1,0),newlit(x,y,4,2),outclause();
applit(x1,yy,0,0),applit(x2,yy,0,0),newcomplit(x,y,4,1),outclause();
applit(x1,yy,0,0),newcomplit(x,y,4,2),outclause();
applit(x2,yy,0,0),newcomplit(x,y,4,2),outclause();
have_d[x][y]= tt+1;
}
}

void e(int x,int y){
register x1= x-1,x2= x,yy= y-1;
if(have_e[x][y]!=tt+1){
applit(x1,yy,1,0),newlit(x,y,5,1),outclause();
applit(x2,yy,1,0),newlit(x,y,5,1),outclause();
applit(x1,yy,1,0),applit(x2,yy,1,0),newlit(x,y,5,2),outclause();
applit(x1,yy,0,0),applit(x2,yy,0,0),newcomplit(x,y,5,1),outclause();
applit(x1,yy,0,0),newcomplit(x,y,5,2),outclause();
applit(x2,yy,0,0),newcomplit(x,y,5,2),outclause();
have_e[x][y]= tt+1;
}
}

/*:8*//*9:*/
#line 262 "./sat-life.w"

void f(int x,int y){
register xx= x-1,y1= y,y2= y+1;
if(have_f[x][y]!=tt+1){
applit(xx,y1,1,0),newlit(x,y,6,1),outclause();
applit(xx,y2,1,0),newlit(x,y,6,1),outclause();
applit(xx,y1,1,0),applit(xx,y2,1,0),newlit(x,y,6,2),outclause();
applit(xx,y1,0,0),applit(xx,y2,0,0),newcomplit(x,y,6,1),outclause();
applit(xx,y1,0,0),newcomplit(x,y,6,2),outclause();
applit(xx,y2,0,0),newcomplit(x,y,6,2),outclause();
have_f[x][y]= tt+1;
}
}

/*:9*//*10:*/
#line 280 "./sat-life.w"

void g(int x,int y){
register x1,x2,y1,y2;
if(x&1)x1= x-1,y1= y,x2= x+1,y2= y^1;
else x1= x+1,y1= y,x2= x-1,y2= y-1+((y&1)<<1);
applit(x1,y1,1,0),newlit(x,y,7,1),outclause();
applit(x2,y2,1,0),newlit(x,y,7,1),outclause();
applit(x1,y1,1,0),applit(x2,y2,1,0),newlit(x,y,7,2),outclause();
applit(x1,y1,0,0),applit(x2,y2,0,0),newcomplit(x,y,7,1),outclause();
applit(x1,y1,0,0),newcomplit(x,y,7,2),outclause();
applit(x2,y2,0,0),newcomplit(x,y,7,2),outclause();
}

/*:10*//*11:*/
#line 296 "./sat-life.w"

void b(int x,int y){
register j,k,xx= x,y1= y-(y&2),y2= y+(y&2);
if(have_b[x][y]!=tt+1){
d(xx,y1);
e(xx,y2);
for(j= 0;j<3;j++)for(k= 0;k<3;k++)if(j+k){
if(j)newcomplit(xx,y1,4,j);
if(k)newcomplit(xx,y2,5,k);
newlit(x,y,2,j+k);
outclause();
if(j)newlit(xx,y1,4,3-j);
if(k)newlit(xx,y2,5,3-k);
newcomplit(x,y,2,5-j-k);
outclause();
}
have_b[x][y]= tt+1;
}
}

/*:11*//*12:*/
#line 319 "./sat-life.w"

void c(int x,int y){
register j,k,x1,y1;
if(x&1)x1= x+2,y1= (y-1)|1;
else x1= x,y1= y&-2;
f(x1,y1);
g(x,y);
for(j= 0;j<3;j++)for(k= 0;k<3;k++)if(j+k){
if(j)newcomplit(x1,y1,6,j);
if(k)newcomplit(x,y,7,k);
newlit(x,y,3,j+k);
outclause();
if(j)newlit(x1,y1,6,3-j);
if(k)newlit(x,y,7,3-k);
newcomplit(x,y,3,5-j-k);
outclause();
}
}

/*:12*//*13:*/
#line 341 "./sat-life.w"

void a(int x,int y){
register j,k,xx= x|1;
b(xx,y);
c(x,y);
for(j= 0;j<5;j++)for(k= 0;k<5;k++)if(j+k> 1&&j+k<5){
if(j)newcomplit(xx,y,2,j);
if(k)newcomplit(x,y,3,k);
newlit(x,y,1,j+k);
outclause();
}
for(j= 0;j<5;j++)for(k= 0;k<5;k++)if(j+k> 2&&j+k<6&&j*k){
if(j)newlit(xx,y,2,j);
if(k)newlit(x,y,3,k);
newcomplit(x,y,1,j+k-1);
outclause();
}
}

/*:13*//*14:*/
#line 366 "./sat-life.w"

void zprime(int x,int y){
newcomplit(x,y,1,4),applit(x,y,1,1),outclause();
newlit(x,y,1,2),applit(x,y,1,1),outclause();
newlit(x,y,1,3),applit(x,y,0,0),applit(x,y,1,1),outclause();

newcomplit(x,y,1,3),newlit(x,y,1,4),applit(x,y,0,1),outclause();

applit(x,y,0,7),newcomplit(x,y,1,2),newlit(x,y,1,4),outclause();

applit(x,y,1,7),applit(x,y,1,0),applit(x,y,0,1),outclause();

}

/*:14*/
#line 112 "./sat-life.w"

main(int argc,char*argv[]){
register int j,k,x,y;
/*3:*/
#line 124 "./sat-life.w"

if(argc!=2||sscanf(argv[1],"%d",&tt)!=1){
fprintf(stderr,"Usage: %s t\n",argv[0]);
exit(-1);
}
if(tt<0||tt> 82){
fprintf(stderr,"The time should be between 0 and 82 (not %d)!\n",tt);
exit(-2);
}

/*:3*/
#line 115 "./sat-life.w"
;
/*4:*/
#line 134 "./sat-life.w"

for(x= 1;;x++){
if(!fgets(buf,maxy+2,stdin))break;
if(x> maxx){
fprintf(stderr,"Sorry, the pattern should have at most %d rows!\n",maxx);
exit(-3);
}
for(y= 1;buf[y-1]!='\n';y++){
if(y> maxy){
fprintf(stderr,"Sorry, the pattern should have at most %d columns!\n",
maxy);
exit(-4);
}
if(buf[y-1]=='*'){
p[x][y]= 1;
if(y> ymax)ymax= y;
if(y<ymin)ymin= y;
if(x> xmax)xmax= x;
if(x<xmin)xmin= x;
}else if(buf[y-1]!='.'){
fprintf(stderr,"Unexpected character `%c' found in the pattern!\n",
buf[y-1]);
exit(-5);
}
}
}

/*:4*/
#line 116 "./sat-life.w"
;
for(x= xmin-1;x<=xmax+1;x++)for(y= ymin-1;y<=ymax+1;y++){
/*5:*/
#line 163 "./sat-life.w"

if(pp(x-1,y-1)+pp(x-1,y)+pp(x-1,y+1)+
pp(x,y-1)+p[x][y]+p[x][y+1]+
pp(x+1,y-1)+p[x+1][y]+p[x+1][y+1]<3)continue;

/*:5*/
#line 118 "./sat-life.w"
;
a(x,y);
zprime(x,y);
}
}

/*:2*/
