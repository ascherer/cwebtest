#define max_pieces 100
#define buf_size 36*36*3+8 \

#define panic(m) {fprintf(stderr,"%s!\n%s",m,buf) ;exit(-1) ;} \

/*1:*/
#line 31 "polyiamonds.w"

#include <stdio.h> 
#include <ctype.h> 
/*4:*/
#line 83 "polyiamonds.w"

char buf[buf_size];
int board[36][36][2];
int bxmin,bxmax,bymin,bymax;

/*:4*//*7:*/
#line 129 "polyiamonds.w"

char name[4];
int s,t;
int xx[36*36*2],yy[36*36*2],zz[36*36*2];
int xmin,xmax,ymin,ymax;

/*:7*/
#line 34 "polyiamonds.w"

/*3:*/
#line 71 "polyiamonds.w"

int decode(c)
char c;
{
if(c<='9'){
if(c>='0')return c-'0';
}else if(c>='a'){
if(c<='z')return c+10-'a';
}
return-1;
}

/*:3*//*12:*/
#line 181 "polyiamonds.w"

char encode(x)
int x;
{
if(x<10)return'0'+x;
return'a'-10+x;
}

/*:12*/
#line 35 "polyiamonds.w"
;

main()
{
register char*p,*q;
register int j,k,n,x,y,z;
/*2:*/
#line 48 "polyiamonds.w"

fgets(buf,buf_size,stdin);
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
bxmin= bymin= 35;bxmax= bymax= 0;
for(p= buf;*p;p+= 3){
while(isspace(*p))p++;
if(!*p)break;
x= decode(*p);
if(x<0)panic("Bad x coordinate");
y= decode(*(p+1));
if(y<0)panic("Bad y coordinate");
if(*(p+2)=='*')p++,z= 1;else z= 0;
if(!isspace(*(p+2)))panic("Bad board position");
if(board[x][y][z])panic("Duplicate board position");
if(x<bxmin)bxmin= x;
if(x> bxmax)bxmax= x;
if(y<bymin)bymin= y;
if(y> bymax)bymax= y;
board[x][y][z]= 1;
}
if(bxmin> bxmax)panic("Empty board");
fwrite(buf,1,strlen(buf)-1,stdout);

/*:2*/
#line 41 "polyiamonds.w"
;
/*5:*/
#line 88 "polyiamonds.w"

if(!fgets(buf,buf_size,stdin))panic("No piece names");
printf(" %s",buf);

/*:5*/
#line 42 "polyiamonds.w"
;
/*6:*/
#line 92 "polyiamonds.w"

while(fgets(buf,buf_size,stdin)){
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
for(p= buf;isspace(*p);p++);
if(!*p)panic("Empty line");
for(q= p+1;!isspace(*q);q++);
if(q> p+3)panic("Piece name too long");
for(q= name;!isspace(*p);p++,q++)*q= *p;
*q= '\0';
for(p++;isspace(*p);p++);
s= *p-'0';
if((s!=1&&s!=2&&s!=3&&s!=6)||!isspace(*(p+1)))panic("Bad s value");
for(p+= 2;isspace(*p);p++);
t= *p-'0';
if((t!=1&&t!=2)||!isspace(*(p+1)))panic("Bad t value");
n= 0;
xmin= ymin= 35;xmax= ymax= 0;
for(p+= 2;*p;p+= 3,n++){
while(isspace(*p))p++;
if(!*p)break;
x= decode(*p);
if(x<0)panic("Bad x coordinate");
y= decode(*(p+1));
if(y<0)panic("Bad y coordinate");
if(*(p+2)=='*')p++,z= 1;else z= 0;
if(!isspace(*(p+2)))panic("Bad board position");
if(n==36*36*2)panic("Pigeonhole principle says you repeated a position");
xx[n]= x,yy[n]= y,zz[n]= z;
if(x<xmin)xmin= x;
if(x> xmax)xmax= x;
if(y<ymin)ymin= y;
if(y> ymax)ymax= y;
}
if(n==0)panic("Empty piece");
/*8:*/
#line 135 "polyiamonds.w"

while(t){
for(k= 1;k<=6;k++){
if(k<=s)/*11:*/
#line 167 "polyiamonds.w"

for(x= bxmin-xmin;x<=bxmax-xmax;x++)
for(y= bymin-ymin;y<=bymax-ymax;y++){
for(j= 0;j<n;j++)
if(!board[x+xx[j]][y+yy[j]][zz[j]])goto nope;
printf(name);
for(j= 0;j<n;j++){
printf(" %c%c",encode(x+xx[j]),encode(y+yy[j]));
if(zz[j])printf("*");
}
printf("\n");
nope:;
}

/*:11*/
#line 138 "polyiamonds.w"
;
/*10:*/
#line 154 "polyiamonds.w"

xmin= ymin= 1000;xmax= ymax= -1000;
for(j= 0;j<n;j++){
z= xx[j];
xx[j]= z+yy[j]+zz[j];
yy[j]= -z;
zz[j]= 1-zz[j];
if(xx[j]<xmin)xmin= xx[j];
if(xx[j]> xmax)xmax= xx[j];
if(yy[j]<ymin)ymin= yy[j];
if(yy[j]> ymax)ymax= yy[j];
}

/*:10*/
#line 139 "polyiamonds.w"
;
}
/*9:*/
#line 145 "polyiamonds.w"

for(j= 0;j<n;j++){
z= xx[j];
xx[j]= yy[j];
yy[j]= z;
}
z= xmin;xmin= ymin;ymin= z;
z= xmax;xmax= ymax;ymax= z;

/*:9*/
#line 141 "polyiamonds.w"
;
t--;
}

/*:8*/
#line 126 "polyiamonds.w"
;
}

/*:6*/
#line 43 "polyiamonds.w"
;
}

/*:1*/
