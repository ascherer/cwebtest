#define max_pieces 100
#define buf_size 3*36*36*4+10 \

#define panic(m) {fprintf(stderr,"%s!\n%s",m,buf) ;exit(-1) ;} \

/*1:*/
#line 34 "./polysticks.w"

#include <stdio.h> 
#include <ctype.h> 
/*5:*/
#line 97 "./polysticks.w"

char buf[buf_size];
int board[36][36][3];
int bxmin,bxmax,bymin,bymax;

/*:5*//*7:*/
#line 142 "./polysticks.w"

char name[4];
int s,t;
int xx[36*36*3],yy[36*36*3],zz[36*36*3];
int xmin,xmax,ymin,ymax;

/*:7*/
#line 37 "./polysticks.w"

/*4:*/
#line 85 "./polysticks.w"

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

/*:4*//*12:*/
#line 198 "./polysticks.w"

char codeletter[3]= {'H','I','V'};
char encode(x)
int x;
{
if(x<10)return'0'+x;
return'a'-10+x;
}

/*:12*/
#line 38 "./polysticks.w"
;

main()
{
register char*p,*q;
register int j,k,n,x,y,z,bar;
/*2:*/
#line 51 "./polysticks.w"

if(!fgets(buf,buf_size,stdin))panic("No piece names");
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
fwrite(buf,1,strlen(buf)-1,stdout);

/*:2*/
#line 44 "./polysticks.w"
;
/*3:*/
#line 56 "./polysticks.w"

if(!fgets(buf,buf_size,stdin))panic("No board");
if(buf[strlen(buf)-1]!='\n')panic("Input line too long");
bxmin= bymin= 35;bxmax= bymax= 0;
for(p= buf,bar= 0;*p;p+= 4){
while(isspace(*p))p++;
if(!*p)break;
if(*p=='|'&&isspace(*(p+1))){
bar= 1;p-= 2;continue;
}
x= decode(*(p+1));
if(x<0)panic("Bad x coordinate");
y= decode(*(p+2));
if(y<0)panic("Bad y coordinate");
if(!isspace(*(p+3)))panic("Bad board position");
if(*p=='H'&&!bar)z= 0;
else if(*p=='V'&&!bar)z= 2;
else if(*p=='I'&&bar)z= 1;
else panic("Illegal board position");
if(board[x][y][z])panic("Duplicate board position");
if(x<bxmin)bxmin= x;
if(x> bxmax)bxmax= x;
if(y<bymin)bymin= y;
if(y> bymax)bymax= y;
board[x][y][z]= 1;
}
if(bxmin> bxmax)panic("Empty board");
printf(" %s",buf);

/*:3*/
#line 45 "./polysticks.w"
;
/*6:*/
#line 102 "./polysticks.w"

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
for(p+= 2;*p;p+= 4,n++){
while(isspace(*p))p++;
if(!*p)break;
x= decode(*(p+1));
if(x<0)panic("Bad x coordinate");
y= decode(*(p+2));
if(y<0)panic("Bad y coordinate");
if(!isspace(*(p+3)))panic("Bad board position");
if(*p=='H')z= 0;
else if(*p=='V')z= 2;
else if(*p=='I')z= 1;
else panic("Illegal board position");
if(n==36*36*2)panic("Pigeonhole principle says you repeated a position");
xx[n]= x,yy[n]= y,zz[n]= z;
if(x<xmin)xmin= x;
if(x> xmax)xmax= x;
if(y<ymin)ymin= y;
if(y> ymax)ymax= y;
}
if(n==0)panic("Empty piece");
/*8:*/
#line 148 "./polysticks.w"

while(t){
for(k= 1;k<=4;k++){
if(k<=s)/*11:*/
#line 185 "./polysticks.w"

for(x= bxmin-xmin;x<=bxmax-xmax;x++)
for(y= bymin-ymin;y<=bymax-ymax;y++){
for(j= 0;j<n;j++)
if(zz[j]!=1&&!board[x+xx[j]][y+yy[j]][zz[j]])goto nope;
printf(name);
for(j= 0;j<n;j++)if(board[x+xx[j]][y+yy[j]][zz[j]]){
printf(" %c%c%c",codeletter[zz[j]],encode(x+xx[j]),encode(y+yy[j]));
}
printf("\n");
nope:;
}

/*:11*/
#line 151 "./polysticks.w"
;
/*10:*/
#line 168 "./polysticks.w"

xmin= ymin= 1000;xmax= ymax= -1000;
for(j= 0;j<n;j++){
z= xx[j];
xx[j]= -yy[j];
if(zz[j]==2)xx[j]--;
yy[j]= z;
zz[j]= 2-zz[j];
if(xx[j]<xmin)xmin= xx[j];
if(xx[j]> xmax)xmax= xx[j];
if(yy[j]<ymin)ymin= yy[j];
if(yy[j]> ymax)ymax= yy[j];
}

/*:10*/
#line 152 "./polysticks.w"
;
}
/*9:*/
#line 158 "./polysticks.w"

for(j= 0;j<n;j++){
z= xx[j];
xx[j]= yy[j];
yy[j]= z;
zz[j]= 2-zz[j];
}
z= xmin;xmin= ymin;ymin= z;
z= xmax;xmax= ymax;ymax= z;

/*:9*/
#line 154 "./polysticks.w"
;
t--;
}

/*:8*/
#line 139 "./polysticks.w"
;
}

/*:6*/
#line 46 "./polysticks.w"
;
}

/*:1*/
