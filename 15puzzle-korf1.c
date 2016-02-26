#define row(x) ((x) >>2) 
#define col(x) ((x) &0x3)  \

#define code(r,c,d,p) (((((r<<2) +c) <<2) +d) <<2) +p
#define tailcode(r,c,d) 0x100+(code(0,r,c,d) ) 
#define bottom code(0,0,1,1) 
#define bord(r,c) board[((r) <<2) +(c) ]
#define east(r,c) piece= bord(r,c+1) ;del= (((c-col(piece) ) >>2) &0xff) +1
#define west(r,c) piece= bord(r,c-1) ;del= (((col(piece) -c) >>2) &0xff) +1
#define north(r,c) piece= bord(r-1,c) ;del= (((row(piece) -r) >>2) &0xff) +1
#define south(r,c) piece= bord(r+1,c) ;del= (((r-row(piece) ) >>2) &0xff) +1 \

/*1:*/
#line 41 "15puzzle-korf1.w"

#include <stdio.h> 
#include <time.h> 
char board[16];
char start[16];
int stack[100];
int timer;

main(int argc,char*argv[])
{
register int j,k,s,t,del,piece,moves;
/*2:*/
#line 87 "15puzzle-korf1.w"

if(argc!=2){
fprintf(stderr,"Usage: %s startposition\n",argv[0]);
exit(-1);
}
for(j= 0;k= argv[1][j];j++){
if(k>='0'&&k<='9')k-= '0';
else if(k>='a'&&k<='f')k-= 'a'-10;
else{
fprintf(stderr,
"The start position should use only hex digits (0123456789abcdef)!\n");
exit(-2);
}
if(start[k]){
fprintf(stderr,"Your start position uses %x twice!\n",k);
exit(-3);
}
start[k]= 1;
}
for(k= 0;k<16;k++)if(start[k]==0){
fprintf(stderr,"Your start position doesn't use %x!\n",k);
exit(-4);
}
for(del= j= 0;k= argv[1][j];j++){
if(k>='0'&&k<='9')k-= '0';else k-= 'a'-10;
start[j]= k-1;
for(s= 0;s<j;s++)if(start[s]> start[j])del++;
if(k==0)t= j;
}
if(((row(t)+col(t)+del)&0x1)==0){
printf("Sorry ... the goal is unreachable from that start position!\n");
exit(0);
}

/*:2*/
#line 52 "15puzzle-korf1.w"
;
/*5:*/
#line 265 "15puzzle-korf1.w"

/*6:*/
#line 279 "15puzzle-korf1.w"

for(j= moves= 0;j<16;j++)if(start[j]>=0){
del= row(start[j])-row(j);
moves+= (del<0?-del:del);
del= col(start[j])-col(j);
moves+= (del<0?-del:del);
}

/*:6*/
#line 266 "15puzzle-korf1.w"
;
if(moves==0)goto win;
while(1){
timer= time(0);
t= moves;

/*23:*/
#line 1190 "15puzzle-korf1.w"

for(j= 0;j<16;j++){
board[j]= start[j];
if(board[j]<0)k= j;
}
stack[0]= (t<<16)+bottom,s= 1;
if(col(k)!=3)
stack[s++]= (t<<16)+tailcode(row(k),col(k),0);
if(row(k)!=0)
stack[s++]= (t<<16)+tailcode(row(k),col(k),1);
if(col(k)!=0)
stack[s++]= (t<<16)+tailcode(row(k),col(k),2);
if(row(k)!=3)
stack[s++]= (t<<16)+tailcode(row(k),col(k),3);
/*8:*/
#line 293 "15puzzle-korf1.w"

switcher:t= stack[--s]>>16;
switch(stack[s]&0xffff){
/*10:*/
#line 335 "15puzzle-korf1.w"

case tailcode(0,0,0):
case code(0,0,0,3):
r0c0d0p3:east(0,0);
if(t<=del){if(t==del)goto win;else goto r0c0d3p3;}
bord(0,0)= piece,stack[s++]= (t<<16)+code(0,0,3,3),t-= del;
goto r0c1d0p2;

case code(1,0,0,1):
r1c0d0p1:east(1,0);
if(t<=del){if(t==del)goto win;else goto r1c0d3p1;}
bord(1,0)= piece,stack[s++]= (t<<16)+code(1,0,3,1),t-= del;
goto r1c1d1p2;

case tailcode(1,0,0):
case code(1,0,0,3):
r1c0d0p3:east(1,0);
if(t<=del){if(t==del)goto win;else goto r1c0d3p3;}
bord(1,0)= piece,stack[s++]= (t<<16)+code(1,0,3,3),t-= del;
goto r1c1d1p2;

case code(2,0,0,1):
r2c0d0p1:east(2,0);
if(t<=del){if(t==del)goto win;else goto r2c0d3p1;}
bord(2,0)= piece,stack[s++]= (t<<16)+code(2,0,3,1),t-= del;
goto r2c1d1p2;

case tailcode(2,0,0):
case code(2,0,0,3):
r2c0d0p3:east(2,0);
if(t<=del){if(t==del)goto win;else goto r2c0d3p3;}
bord(2,0)= piece,stack[s++]= (t<<16)+code(2,0,3,3),t-= del;
goto r2c1d1p2;

case tailcode(3,0,0):
case code(3,0,0,1):
r3c0d0p1:east(3,0);
if(t<=del){if(t==del)goto win;else goto r3c0d1p1;}
bord(3,0)= piece,stack[s++]= (t<<16)+code(3,0,1,1),t-= del;
goto r3c1d1p2;

/*:10*/
#line 296 "15puzzle-korf1.w"

/*11:*/
#line 376 "15puzzle-korf1.w"

case code(0,1,0,2):
r0c1d0p2:east(0,1);
if(t<=del){if(t==del)goto win;else goto r0c1d3p2;}
bord(0,1)= piece,stack[s++]= (t<<16)+code(0,1,3,2),t-= del;
goto r0c2d0p2;

case tailcode(0,1,0):
case code(0,1,0,3):
r0c1d0p3:east(0,1);
if(t<=del){if(t==del)goto win;else goto r0c1d3p3;}
bord(0,1)= piece,stack[s++]= (t<<16)+code(0,1,3,3),t-= del;
goto r0c2d0p2;

case code(1,1,0,1):
r1c1d0p1:east(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d3p1;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,3,1),t-= del;
goto r1c2d1p2;

case code(1,1,0,2):
r1c1d0p2:east(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d3p2;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,3,2),t-= del;
goto r1c2d1p2;

case tailcode(1,1,0):
case code(1,1,0,3):
r1c1d0p3:east(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d3p3;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,3,3),t-= del;
goto r1c2d1p2;

case code(2,1,0,1):
r2c1d0p1:east(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d3p1;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,3,1),t-= del;
goto r2c2d1p2;

case code(2,1,0,2):
r2c1d0p2:east(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d3p2;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,3,2),t-= del;
goto r2c2d1p2;

case tailcode(2,1,0):
case code(2,1,0,3):
r2c1d0p3:east(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d3p3;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,3,3),t-= del;
goto r2c2d1p2;

case code(3,1,0,1):
r3c1d0p1:east(3,1);
if(t<=del){if(t==del)goto win;else goto r3c1d2p1;}
bord(3,1)= piece,stack[s++]= (t<<16)+code(3,1,2,1),t-= del;
goto r3c2d1p2;

case tailcode(3,1,0):
case code(3,1,0,2):
r3c1d0p2:east(3,1);
if(t<=del){if(t==del)goto win;else goto r3c1d2p2;}
bord(3,1)= piece,stack[s++]= (t<<16)+code(3,1,2,2),t-= del;
goto r3c2d1p2;

/*:11*/
#line 297 "15puzzle-korf1.w"

/*12:*/
#line 441 "15puzzle-korf1.w"

case code(0,2,0,2):
r0c2d0p2:east(0,2);
if(t<=del){if(t==del)goto win;else goto r0c2d3p2;}
bord(0,2)= piece,stack[s++]= (t<<16)+code(0,2,3,2),t-= del;
goto r0c3d3p2;

case tailcode(0,2,0):
case code(0,2,0,3):
r0c2d0p3:east(0,2);
if(t<=del){if(t==del)goto win;else goto r0c2d3p3;}
bord(0,2)= piece,stack[s++]= (t<<16)+code(0,2,3,3),t-= del;
goto r0c3d3p2;

case code(1,2,0,1):
r1c2d0p1:east(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d3p1;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,3,1),t-= del;
goto r1c3d1p2;

case code(1,2,0,2):
r1c2d0p2:east(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d3p2;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,3,2),t-= del;
goto r1c3d1p2;

case tailcode(1,2,0):
case code(1,2,0,3):
r1c2d0p3:east(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d3p3;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,3,3),t-= del;
goto r1c3d1p2;

case code(2,2,0,1):
r2c2d0p1:east(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d3p1;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,3,1),t-= del;
goto r2c3d1p2;

case code(2,2,0,2):
r2c2d0p2:east(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d3p2;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,3,2),t-= del;
goto r2c3d1p2;

case tailcode(2,2,0):
case code(2,2,0,3):
r2c2d0p3:east(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d3p3;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,3,3),t-= del;
goto r2c3d1p2;

case code(3,2,0,1):
r3c2d0p1:east(3,2);
if(t<=del){if(t==del)goto win;else goto r3c2d2p1;}
bord(3,2)= piece,stack[s++]= (t<<16)+code(3,2,2,1),t-= del;
goto r3c3d1p2;

case tailcode(3,2,0):
case code(3,2,0,2):
r3c2d0p2:east(3,2);
if(t<=del){if(t==del)goto win;else goto r3c2d2p2;}
bord(3,2)= piece,stack[s++]= (t<<16)+code(3,2,2,2),t-= del;
goto r3c3d1p2;

/*:12*/
#line 298 "15puzzle-korf1.w"

/*13:*/
#line 506 "15puzzle-korf1.w"

case tailcode(0,1,2):
case code(0,1,2,0):
r0c1d2p0:west(0,1);
if(t<=del){if(t==del)goto win;else goto r0c1d0p0;}
bord(0,1)= piece,stack[s++]= (t<<16)+code(0,1,0,0),t-= del;
goto r0c0d3p0;

case code(0,1,2,3):
r0c1d2p3:west(0,1);
if(t<=del){if(t==del)goto win;else goto r0c1d0p3;}
bord(0,1)= piece,stack[s++]= (t<<16)+code(0,1,0,3),t-= del;
goto r0c0d3p0;

case tailcode(1,1,2):
case code(1,1,2,1):
r1c1d2p1:west(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d1p1;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,1,1),t-= del;
goto r1c0d3p0;

case code(1,1,2,0):
r1c1d2p0:west(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d1p0;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,1,0),t-= del;
goto r1c0d3p0;

case code(1,1,2,3):
r1c1d2p3:west(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d1p3;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,1,3),t-= del;
goto r1c0d3p0;

case tailcode(2,1,2):
case code(2,1,2,1):
r2c1d2p1:west(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d1p1;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,1,1),t-= del;
goto r2c0d3p0;

case code(2,1,2,0):
r2c1d2p0:west(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d1p0;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,1,0),t-= del;
goto r2c0d3p0;

case code(2,1,2,3):
r2c1d2p3:west(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d1p3;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,1,3),t-= del;
goto r2c0d3p0;

case tailcode(3,1,2):
case code(3,1,2,1):
r3c1d2p1:west(3,1);
if(t<=del){if(t==del)goto win;else goto r3c1d1p1;}
bord(3,1)= piece,stack[s++]= (t<<16)+code(3,1,1,1),t-= del;
goto r3c0d1p0;

case code(3,1,2,0):
r3c1d2p0:west(3,1);
if(t<=del){if(t==del)goto win;else goto r3c1d1p0;}
bord(3,1)= piece,stack[s++]= (t<<16)+code(3,1,1,0),t-= del;
goto r3c0d1p0;


/*:13*/
#line 299 "15puzzle-korf1.w"

/*14:*/
#line 572 "15puzzle-korf1.w"

case tailcode(0,2,2):
case code(0,2,2,0):
r0c2d2p0:west(0,2);
if(t<=del){if(t==del)goto win;else goto r0c2d0p0;}
bord(0,2)= piece,stack[s++]= (t<<16)+code(0,2,0,0),t-= del;
goto r0c1d3p0;

case code(0,2,2,3):
r0c2d2p3:west(0,2);
if(t<=del){if(t==del)goto win;else goto r0c2d0p3;}
bord(0,2)= piece,stack[s++]= (t<<16)+code(0,2,0,3),t-= del;
goto r0c1d3p0;

case tailcode(1,2,2):
case code(1,2,2,1):
r1c2d2p1:west(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d1p1;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,1,1),t-= del;
goto r1c1d3p0;

case code(1,2,2,0):
r1c2d2p0:west(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d1p0;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,1,0),t-= del;
goto r1c1d3p0;

case code(1,2,2,3):
r1c2d2p3:west(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d1p3;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,1,3),t-= del;
goto r1c1d3p0;

case tailcode(2,2,2):
case code(2,2,2,1):
r2c2d2p1:west(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d1p1;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,1,1),t-= del;
goto r2c1d3p0;

case code(2,2,2,0):
r2c2d2p0:west(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d1p0;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,1,0),t-= del;
goto r2c1d3p0;

case code(2,2,2,3):
r2c2d2p3:west(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d1p3;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,1,3),t-= del;
goto r2c1d3p0;

case tailcode(3,2,2):
case code(3,2,2,1):
r3c2d2p1:west(3,2);
if(t<=del){if(t==del)goto win;else goto r3c2d1p1;}
bord(3,2)= piece,stack[s++]= (t<<16)+code(3,2,1,1),t-= del;
goto r3c1d2p0;

case code(3,2,2,0):
r3c2d2p0:west(3,2);
if(t<=del){if(t==del)goto win;else goto r3c2d1p0;}
bord(3,2)= piece,stack[s++]= (t<<16)+code(3,2,1,0),t-= del;
goto r3c1d2p0;


/*:14*/
#line 300 "15puzzle-korf1.w"

/*15:*/
#line 638 "15puzzle-korf1.w"

case tailcode(0,3,2):
case code(0,3,2,3):
r0c3d2p3:west(0,3);
if(t<=del){if(t==del)goto win;else goto r0c3d3p3;}
bord(0,3)= piece,stack[s++]= (t<<16)+code(0,3,3,3),t-= del;
goto r0c2d3p0;

case tailcode(1,3,2):
case code(1,3,2,1):
r1c3d2p1:west(1,3);
if(t<=del){if(t==del)goto win;else goto r1c3d1p1;}
bord(1,3)= piece,stack[s++]= (t<<16)+code(1,3,1,1),t-= del;
goto r1c2d3p0;

case code(1,3,2,3):
r1c3d2p3:west(1,3);
if(t<=del){if(t==del)goto win;else goto r1c3d1p3;}
bord(1,3)= piece,stack[s++]= (t<<16)+code(1,3,1,3),t-= del;
goto r1c2d3p0;

case tailcode(2,3,2):
case code(2,3,2,1):
r2c3d2p1:west(2,3);
if(t<=del){if(t==del)goto win;else goto r2c3d1p1;}
bord(2,3)= piece,stack[s++]= (t<<16)+code(2,3,1,1),t-= del;
goto r2c2d3p0;

case code(2,3,2,3):
r2c3d2p3:west(2,3);
if(t<=del){if(t==del)goto win;else goto r2c3d1p3;}
bord(2,3)= piece,stack[s++]= (t<<16)+code(2,3,1,3),t-= del;
goto r2c2d3p0;

case tailcode(3,3,2):
case code(3,3,2,1):
r3c3d2p1:west(3,3);
if(t<=del){if(t==del)goto win;else goto r3c3d1p1;}
bord(3,3)= piece,stack[s++]= (t<<16)+code(3,3,1,1),t-= del;
goto r3c2d2p0;

/*:15*/
#line 301 "15puzzle-korf1.w"

/*16:*/
#line 679 "15puzzle-korf1.w"

case tailcode(1,0,1):
case code(1,0,1,0):
r1c0d1p0:north(1,0);
if(t<=del){if(t==del)goto win;else goto r1c0d0p0;}
bord(1,0)= piece,stack[s++]= (t<<16)+code(1,0,0,0),t-= del;
goto r0c0d0p3;

case code(1,0,1,3):
r1c0d1p3:north(1,0);
if(t<=del){if(t==del)goto win;else goto r1c0d0p3;}
bord(1,0)= piece,stack[s++]= (t<<16)+code(1,0,0,3),t-= del;
goto r0c0d0p3;

case tailcode(1,1,1):
case code(1,1,1,0):
r1c1d1p0:north(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d0p0;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,0,0),t-= del;
goto r0c1d2p3;

case code(1,1,1,2):
r1c1d1p2:north(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d0p2;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,0,2),t-= del;
goto r0c1d2p3;

case code(1,1,1,3):
r1c1d1p3:north(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d0p3;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,0,3),t-= del;
goto r0c1d2p3;

case tailcode(1,2,1):
case code(1,2,1,0):
r1c2d1p0:north(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d0p0;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,0,0),t-= del;
goto r0c2d2p3;

case code(1,2,1,2):
r1c2d1p2:north(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d0p2;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,0,2),t-= del;
goto r0c2d2p3;

case code(1,2,1,3):
r1c2d1p3:north(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d0p3;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,0,3),t-= del;
goto r0c2d2p3;

case code(1,3,1,2):
r1c3d1p2:north(1,3);
if(t<=del){if(t==del)goto win;else goto r1c3d3p2;}
bord(1,3)= piece,stack[s++]= (t<<16)+code(1,3,3,2),t-= del;
goto r0c3d2p3;

case tailcode(1,3,1):
case code(1,3,1,3):
r1c3d1p3:north(1,3);
if(t<=del){if(t==del)goto win;else goto r1c3d3p3;}
bord(1,3)= piece,stack[s++]= (t<<16)+code(1,3,3,3),t-= del;
goto r0c3d2p3;


/*:16*/
#line 302 "15puzzle-korf1.w"

/*17:*/
#line 745 "15puzzle-korf1.w"

case tailcode(2,0,1):
case code(2,0,1,0):
r2c0d1p0:north(2,0);
if(t<=del){if(t==del)goto win;else goto r2c0d0p0;}
bord(2,0)= piece,stack[s++]= (t<<16)+code(2,0,0,0),t-= del;
goto r1c0d1p3;

case code(2,0,1,3):
r2c0d1p3:north(2,0);
if(t<=del){if(t==del)goto win;else goto r2c0d0p3;}
bord(2,0)= piece,stack[s++]= (t<<16)+code(2,0,0,3),t-= del;
goto r1c0d1p3;

case tailcode(2,1,1):
case code(2,1,1,0):
r2c1d1p0:north(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d0p0;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,0,0),t-= del;
goto r1c1d2p3;

case code(2,1,1,2):
r2c1d1p2:north(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d0p2;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,0,2),t-= del;
goto r1c1d2p3;

case code(2,1,1,3):
r2c1d1p3:north(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d0p3;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,0,3),t-= del;
goto r1c1d2p3;

case tailcode(2,2,1):
case code(2,2,1,0):
r2c2d1p0:north(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d0p0;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,0,0),t-= del;
goto r1c2d2p3;

case code(2,2,1,2):
r2c2d1p2:north(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d0p2;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,0,2),t-= del;
goto r1c2d2p3;

case code(2,2,1,3):
r2c2d1p3:north(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d0p3;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,0,3),t-= del;
goto r1c2d2p3;

case code(2,3,1,2):
r2c3d1p2:north(2,3);
if(t<=del){if(t==del)goto win;else goto r2c3d3p2;}
bord(2,3)= piece,stack[s++]= (t<<16)+code(2,3,3,2),t-= del;
goto r1c3d2p3;

case tailcode(2,3,1):
case code(2,3,1,3):
r2c3d1p3:north(2,3);
if(t<=del){if(t==del)goto win;else goto r2c3d3p3;}
bord(2,3)= piece,stack[s++]= (t<<16)+code(2,3,3,3),t-= del;
goto r1c3d2p3;


/*:17*/
#line 303 "15puzzle-korf1.w"

/*18:*/
#line 811 "15puzzle-korf1.w"

case tailcode(3,0,1):
case code(3,0,1,0):
r3c0d1p0:north(3,0);
if(t<=del){if(t==del)goto win;else goto r3c0d0p0;}
bord(3,0)= piece,stack[s++]= (t<<16)+code(3,0,0,0),t-= del;
goto r2c0d1p3;

case tailcode(3,1,1):
case code(3,1,1,0):
r3c1d1p0:north(3,1);
if(t<=del){if(t==del)goto win;else goto r3c1d0p0;}
bord(3,1)= piece,stack[s++]= (t<<16)+code(3,1,0,0),t-= del;
goto r2c1d2p3;

case code(3,1,1,2):
r3c1d1p2:north(3,1);
if(t<=del){if(t==del)goto win;else goto r3c1d0p2;}
bord(3,1)= piece,stack[s++]= (t<<16)+code(3,1,0,2),t-= del;
goto r2c1d2p3;

case tailcode(3,2,1):
case code(3,2,1,0):
r3c2d1p0:north(3,2);
if(t<=del){if(t==del)goto win;else goto r3c2d0p0;}
bord(3,2)= piece,stack[s++]= (t<<16)+code(3,2,0,0),t-= del;
goto r2c2d2p3;

case code(3,2,1,2):
r3c2d1p2:north(3,2);
if(t<=del){if(t==del)goto win;else goto r3c2d0p2;}
bord(3,2)= piece,stack[s++]= (t<<16)+code(3,2,0,2),t-= del;
goto r2c2d2p3;

case tailcode(3,3,1):
case code(3,3,1,2):
r3c3d1p2:north(3,3);
if(t<=del){if(t==del)goto win;else goto r3c3d2p2;}
bord(3,3)= piece,stack[s++]= (t<<16)+code(3,3,2,2),t-= del;
goto r2c3d2p3;

/*:18*/
#line 304 "15puzzle-korf1.w"

/*19:*/
#line 852 "15puzzle-korf1.w"

case tailcode(0,0,3):
case code(0,0,3,0):
r0c0d3p0:south(0,0);
if(t<=del){if(t==del)goto win;else goto r0c0d0p0;}
bord(0,0)= piece,stack[s++]= (t<<16)+code(0,0,0,0),t-= del;
goto r1c0d0p1;

case code(0,1,3,0):
r0c1d3p0:south(0,1);
if(t<=del){if(t==del)goto win;else goto r0c1d2p0;}
bord(0,1)= piece,stack[s++]= (t<<16)+code(0,1,2,0),t-= del;
goto r1c1d0p1;

case tailcode(0,1,3):
case code(0,1,3,2):
r0c1d3p2:south(0,1);
if(t<=del){if(t==del)goto win;else goto r0c1d2p2;}
bord(0,1)= piece,stack[s++]= (t<<16)+code(0,1,2,2),t-= del;
goto r1c1d0p1;

case code(0,2,3,0):
r0c2d3p0:south(0,2);
if(t<=del){if(t==del)goto win;else goto r0c2d2p0;}
bord(0,2)= piece,stack[s++]= (t<<16)+code(0,2,2,0),t-= del;
goto r1c2d0p1;

case tailcode(0,2,3):
case code(0,2,3,2):
r0c2d3p2:south(0,2);
if(t<=del){if(t==del)goto win;else goto r0c2d2p2;}
bord(0,2)= piece,stack[s++]= (t<<16)+code(0,2,2,2),t-= del;
goto r1c2d0p1;

case tailcode(0,3,3):
case code(0,3,3,2):
r0c3d3p2:south(0,3);
if(t<=del){if(t==del)goto win;else goto r0c3d2p2;}
bord(0,3)= piece,stack[s++]= (t<<16)+code(0,3,2,2),t-= del;
goto r1c3d3p1;

/*:19*/
#line 305 "15puzzle-korf1.w"

/*20:*/
#line 893 "15puzzle-korf1.w"

case code(1,0,3,0):
r1c0d3p0:south(1,0);
if(t<=del){if(t==del)goto win;else goto r1c0d1p0;}
bord(1,0)= piece,stack[s++]= (t<<16)+code(1,0,1,0),t-= del;
goto r2c0d0p1;

case tailcode(1,0,3):
case code(1,0,3,1):
r1c0d3p1:south(1,0);
if(t<=del){if(t==del)goto win;else goto r1c0d1p1;}
bord(1,0)= piece,stack[s++]= (t<<16)+code(1,0,1,1),t-= del;
goto r2c0d0p1;

case code(1,1,3,0):
r1c1d3p0:south(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d2p0;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,2,0),t-= del;
goto r2c1d0p1;

case code(1,1,3,1):
r1c1d3p1:south(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d2p1;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,2,1),t-= del;
goto r2c1d0p1;

case tailcode(1,1,3):
case code(1,1,3,2):
r1c1d3p2:south(1,1);
if(t<=del){if(t==del)goto win;else goto r1c1d2p2;}
bord(1,1)= piece,stack[s++]= (t<<16)+code(1,1,2,2),t-= del;
goto r2c1d0p1;

case code(1,2,3,0):
r1c2d3p0:south(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d2p0;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,2,0),t-= del;
goto r2c2d0p1;

case code(1,2,3,1):
r1c2d3p1:south(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d2p1;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,2,1),t-= del;
goto r2c2d0p1;

case tailcode(1,2,3):
case code(1,2,3,2):
r1c2d3p2:south(1,2);
if(t<=del){if(t==del)goto win;else goto r1c2d2p2;}
bord(1,2)= piece,stack[s++]= (t<<16)+code(1,2,2,2),t-= del;
goto r2c2d0p1;

case code(1,3,3,1):
r1c3d3p1:south(1,3);
if(t<=del){if(t==del)goto win;else goto r1c3d2p1;}
bord(1,3)= piece,stack[s++]= (t<<16)+code(1,3,2,1),t-= del;
goto r2c3d3p1;

case tailcode(1,3,3):
case code(1,3,3,2):
r1c3d3p2:south(1,3);
if(t<=del){if(t==del)goto win;else goto r1c3d2p2;}
bord(1,3)= piece,stack[s++]= (t<<16)+code(1,3,2,2),t-= del;
goto r2c3d3p1;


/*:20*/
#line 306 "15puzzle-korf1.w"

/*21:*/
#line 961 "15puzzle-korf1.w"

case code(2,0,3,0):
r2c0d3p0:south(2,0);
if(t<=del){if(t==del)goto win;else goto r2c0d1p0;}
bord(2,0)= piece,stack[s++]= (t<<16)+code(2,0,1,0),t-= del;
goto r3c0d0p1;

case tailcode(2,0,3):
case code(2,0,3,1):
r2c0d3p1:south(2,0);
if(t<=del){if(t==del)goto win;else goto r2c0d1p1;}
bord(2,0)= piece,stack[s++]= (t<<16)+code(2,0,1,1),t-= del;
goto r3c0d0p1;

case code(2,1,3,0):
r2c1d3p0:south(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d2p0;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,2,0),t-= del;
goto r3c1d0p1;

case code(2,1,3,1):
r2c1d3p1:south(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d2p1;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,2,1),t-= del;
goto r3c1d0p1;

case tailcode(2,1,3):
case code(2,1,3,2):
r2c1d3p2:south(2,1);
if(t<=del){if(t==del)goto win;else goto r2c1d2p2;}
bord(2,1)= piece,stack[s++]= (t<<16)+code(2,1,2,2),t-= del;
goto r3c1d0p1;

case code(2,2,3,0):
r2c2d3p0:south(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d2p0;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,2,0),t-= del;
goto r3c2d0p1;

case code(2,2,3,1):
r2c2d3p1:south(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d2p1;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,2,1),t-= del;
goto r3c2d0p1;

case tailcode(2,2,3):
case code(2,2,3,2):
r2c2d3p2:south(2,2);
if(t<=del){if(t==del)goto win;else goto r2c2d2p2;}
bord(2,2)= piece,stack[s++]= (t<<16)+code(2,2,2,2),t-= del;
goto r3c2d0p1;

case code(2,3,3,1):
r2c3d3p1:south(2,3);
if(t<=del){if(t==del)goto win;else goto r2c3d2p1;}
bord(2,3)= piece,stack[s++]= (t<<16)+code(2,3,2,1),t-= del;
goto r3c3d2p1;

case tailcode(2,3,3):
case code(2,3,3,2):
r2c3d3p2:south(2,3);
if(t<=del){if(t==del)goto win;else goto r2c3d2p2;}
bord(2,3)= piece,stack[s++]= (t<<16)+code(2,3,2,2),t-= del;
goto r3c3d2p1;

/*:21*/
#line 307 "15puzzle-korf1.w"

/*22:*/
#line 1029 "15puzzle-korf1.w"


case code(0,0,0,0):
r0c0d0p0:bord(0,0)= bord(0,1);goto switcher;

case code(0,0,3,3):
r0c0d3p3:bord(0,0)= bord(1,0);goto switcher;

case code(0,1,0,0):
r0c1d0p0:bord(0,1)= bord(0,2);goto switcher;

case code(0,1,2,2):
r0c1d2p2:bord(0,1)= bord(0,0);goto switcher;

case code(0,1,3,3):
r0c1d3p3:bord(0,1)= bord(1,1);goto switcher;

case code(0,2,0,0):
r0c2d0p0:bord(0,2)= bord(0,3);goto switcher;

case code(0,2,2,2):
r0c2d2p2:bord(0,2)= bord(0,1);goto switcher;

case code(0,2,3,3):
r0c2d3p3:bord(0,2)= bord(1,2);goto switcher;

case code(0,3,2,2):
r0c3d2p2:bord(0,3)= bord(0,2);goto switcher;

case code(0,3,3,3):
r0c3d3p3:bord(0,3)= bord(1,3);goto switcher;

case code(1,0,0,0):
r1c0d0p0:bord(1,0)= bord(1,1);goto switcher;

case code(1,0,1,1):
r1c0d1p1:bord(1,0)= bord(0,0);goto switcher;

case code(1,0,3,3):
r1c0d3p3:bord(1,0)= bord(2,0);goto switcher;

case code(1,1,0,0):
r1c1d0p0:bord(1,1)= bord(1,2);goto switcher;

case code(1,1,1,1):
r1c1d1p1:bord(1,1)= bord(0,1);goto switcher;

case code(1,1,2,2):
r1c1d2p2:bord(1,1)= bord(1,0);goto switcher;

case code(1,1,3,3):
r1c1d3p3:bord(1,1)= bord(2,1);goto switcher;

case code(1,2,0,0):
r1c2d0p0:bord(1,2)= bord(1,3);goto switcher;

case code(1,2,1,1):
r1c2d1p1:bord(1,2)= bord(0,2);goto switcher;

case code(1,2,2,2):
r1c2d2p2:bord(1,2)= bord(1,1);goto switcher;

case code(1,2,3,3):
r1c2d3p3:bord(1,2)= bord(2,2);goto switcher;

case code(1,3,1,1):
r1c3d1p1:bord(1,3)= bord(0,3);goto switcher;

case code(1,3,2,2):
r1c3d2p2:bord(1,3)= bord(1,2);goto switcher;

case code(1,3,3,3):
r1c3d3p3:bord(1,3)= bord(2,3);goto switcher;

case code(2,0,0,0):
r2c0d0p0:bord(2,0)= bord(2,1);goto switcher;

case code(2,0,1,1):
r2c0d1p1:bord(2,0)= bord(1,0);goto switcher;

case code(2,0,3,3):
r2c0d3p3:bord(2,0)= bord(3,0);goto switcher;

case code(2,1,0,0):
r2c1d0p0:bord(2,1)= bord(2,2);goto switcher;

case code(2,1,1,1):
r2c1d1p1:bord(2,1)= bord(1,1);goto switcher;

case code(2,1,2,2):
r2c1d2p2:bord(2,1)= bord(2,0);goto switcher;

case code(2,1,3,3):
r2c1d3p3:bord(2,1)= bord(3,1);goto switcher;

case code(2,2,0,0):
r2c2d0p0:bord(2,2)= bord(2,3);goto switcher;

case code(2,2,1,1):
r2c2d1p1:bord(2,2)= bord(1,2);goto switcher;

case code(2,2,2,2):
r2c2d2p2:bord(2,2)= bord(2,1);goto switcher;

case code(2,2,3,3):
r2c2d3p3:bord(2,2)= bord(3,2);goto switcher;

case code(2,3,1,1):
r2c3d1p1:bord(2,3)= bord(1,3);goto switcher;

case code(2,3,2,2):
r2c3d2p2:bord(2,3)= bord(2,2);goto switcher;

case code(2,3,3,3):
r2c3d3p3:bord(2,3)= bord(3,3);goto switcher;

case code(3,0,0,0):
r3c0d0p0:bord(3,0)= bord(3,1);goto switcher;

case code(3,0,1,1):
r3c0d1p1:bord(3,0)= bord(2,0);goto switcher;

case code(3,1,0,0):
r3c1d0p0:bord(3,1)= bord(3,2);goto switcher;

case code(3,1,1,1):
r3c1d1p1:bord(3,1)= bord(2,1);goto switcher;

case code(3,1,2,2):
r3c1d2p2:bord(3,1)= bord(3,0);goto switcher;

case code(3,2,0,0):
r3c2d0p0:bord(3,2)= bord(3,3);goto switcher;

case code(3,2,1,1):
r3c2d1p1:bord(3,2)= bord(2,2);goto switcher;

case code(3,2,2,2):
r3c2d2p2:bord(3,2)= bord(3,1);goto switcher;

case code(3,3,1,1):
r3c3d1p1:bord(3,3)= bord(2,3);goto switcher;

case code(3,3,2,2):
r3c3d2p2:bord(3,3)= bord(3,2);goto switcher;

/*:22*/
#line 308 "15puzzle-korf1.w"

case bottom:break;
default:fprintf(stderr,"Oops, I'm confused about case %x!\n",stack[s]);
}

/*:8*/
#line 1204 "15puzzle-korf1.w"
;

/*:23*/
#line 272 "15puzzle-korf1.w"
;
printf(" ... no solution with %d+%d moves (%d sec)\n",
moves&0xff,moves>>8,time(0)-timer);
moves+= 0x101;
}
win:

/*:5*/
#line 53 "15puzzle-korf1.w"
;
/*24:*/
#line 1216 "15puzzle-korf1.w"

printf("Solution in %d+%d moves: ",moves&0xff,moves>>8);
if(moves> 1){
for(k= 1;(stack[k]&0xffff)>=tailcode(0,0,0);k++);
for(j= 0;j<16;j++)board[j]= start[j];
j= (stack[k]&0xffff)>>4;
for(k++;k<s;k++){
del= (stack[k]&0xffff)>>4;
printf("%x",board[del]+1);
board[j]= board[del],j= del;
}
if(j==0xd)printf("ef\n");
else if(j==0x7)printf("8c\n");
else printf("b%x\n",bord(3,3)+1);
}else{
if(moves==1)printf("%x",start[0xf]+1);
printf(" (%d sec)\n",time(0)-timer);
}

/*:24*/
#line 54 "15puzzle-korf1.w"
;
}

/*:1*/
