#define max_pieces 100
#define buf_size 36*36*3+8 \

#define panic(m) {fprintf(stderr,"%s!\n%s",m,buf) ;exit(-1) ;} \

/*1:*/
#line 54 "polyominoes.w"

#include <stdio.h> 
#include <ctype.h> 
/*4:*/
#line 105 "polyominoes.w"

char buf[buf_size];
int board[36][36];
int bxmin,bxmax,bymin,bymax;

/*:4*//*7:*/
#line 150 "polyominoes.w"

char name[4];
int s,t;
int xx[36*36],yy[36*36];
int xmin,xmax,ymin,ymax;

/*:7*/
#line 57 "polyominoes.w"

/*3:*/
#line 93 "polyominoes.w"

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
#line 195 "polyominoes.w"

char encode(x)
int x;
{
if(x<10)return'0'+x;
return'a'-10+x;
}

/*:12*/
#line 58 "polyominoes.w"
;

main()
{
register char*p,*q;
register int j,k,n,x,y,z;
/*2:*/
#line 71 "polyominoes.w"

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
if(!isspace(*(p+2)))panic("Bad board position");
if(board[x][y])panic("Duplicate board position");
if(x<bxmin)bxmin= x;
if(x> bxmax)bxmax= x;
if(y<bymin)bymin= y;
if(y> bymax)bymax= y;
board[x][y]= 1;
}
if(bxmin> bxmax)panic("Empty board");
fwrite(buf,1,strlen(buf)-1,stdout);

/*:2*/
#line 64 "polyominoes.w"
;
/*5:*/
#line 110 "polyominoes.w"

if(!fgets(buf,buf_size,stdin))panic("No piece names");
printf(" %s",buf);

/*:5*/
#line 65 "polyominoes.w"
;
/*6:*/
#line 114 "polyominoes.w"

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
if((s!=1&&s!=2&&s!=4)||!isspace(*(p+1)))panic("Bad s value");
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
if(!isspace(*(p+2)))panic("Bad board position");
if(n==36*36)panic("Pigeonhole principle says you repeated a position");
xx[n]= x,yy[n]= y;
if(x<xmin)xmin= x;
if(x> xmax)xmax= x;
if(y<ymin)ymin= y;
if(y> ymax)ymax= y;
}
if(n==0)panic("Empty piece");
/*8:*/
#line 156 "polyominoes.w"

while(t){
for(k= 1;k<=4;k++){
if(k<=s)/*11:*/
#line 183 "polyominoes.w"

for(x= bxmin-xmin;x<=bxmax-xmax;x++)
for(y= bymin-ymin;y<=bymax-ymax;y++){
for(j= 0;j<n;j++)
if(!board[x+xx[j]][y+yy[j]])goto nope;
printf(name);
for(j= 0;j<n;j++)
printf(" %c%c",encode(x+xx[j]),encode(y+yy[j]));
printf("\n");
nope:;
}

/*:11*/
#line 159 "polyominoes.w"
;
/*10:*/
#line 175 "polyominoes.w"

for(j= 0;j<n;j++){
z= xx[j];
xx[j]= 35-yy[j];
yy[j]= z;
}
z= xmin;xmin= 35-ymax;ymax= xmax;xmax= 35-ymin;ymin= z;

/*:10*/
#line 160 "polyominoes.w"
;
}
/*9:*/
#line 166 "polyominoes.w"

for(j= 0;j<n;j++){
z= xx[j];
xx[j]= yy[j];
yy[j]= z;
}
z= xmin;xmin= ymin;ymin= z;
z= xmax;xmax= ymax;ymax= z;

/*:9*/
#line 162 "polyominoes.w"
;
t--;
}

/*:8*/
#line 147 "polyominoes.w"
;
}

/*:6*/
#line 66 "polyominoes.w"
;
}

/*:1*/
