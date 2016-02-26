#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define AA (1<<A) 
#define BB (1<<B) 
#define CC (1<<C) 
#define DD (1<<D) 
#define EE (1<<E) 
#define o mems++
#define oo mems+= 2
#define stacksize 1000000
#define delta 10000000000 \

#define tag 20 \

#define pack(u,q,x) (((u) <<8) +((q) <<3) +(x) )  \

#define deny(q,x) {if(!remov(q,x) ) goto bad; \
if(q==loguy&&!remov(higuy,x) ) goto bad; \
if(q==higuy&&!remov(loguy,x) ) goto bad;}
#define force(q,x) {if(!innerforce(q,x) ) goto bad;} \

#define isvowel(q) (o,(mem[q]&0x11) ?1:0)  \

/*1:*/
#line 27 "./back-20q.w"

#include <stdio.h> 
#include <stdlib.h> 
typedef unsigned long long ull;
ull mems;
ull nodes;
ull count;
ull thresh= delta;
ull profile[22];
int false1,false2;
int score;
char falsity[21];
char believe3;
char mem[41];
char vbose;
/*6:*/
#line 112 "./back-20q.w"

int order[21]= {0,
3,
15,
20,
19,
2,
1,
17,
10,
5,
4,
16,
11,
13,
14,
7,
18,
6,
8,
12,
9};
int loguy,higuy;
int rho[32]= {-1,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0};
int stack[stacksize],frame[21];
int stackptr;
int pstack[21];
int pstackptr;

/*:6*//*31:*/
#line 526 "./back-20q.w"

int dist[5];
int tie[5];
int trick;

/*:31*//*52:*/
#line 782 "./back-20q.w"

int magic[]= {(1<<2)+(1<<3)+(1<<5)+(1<<7),
(1<<0)+(1<<1)+(1<<4),
(1<<1)+(1<<3)+(1<<5)+(1<<7),
(1<<0)+(1<<2)+(1<<4)+(1<<6)+(1<<8),
(1<<0)}

/*:52*/
#line 42 "./back-20q.w"
;
/*7:*/
#line 149 "./back-20q.w"

int remov(int q,int x){
register int t,b,bb;
t= 1<<x;
o,b= mem[q];
if(b&t){
if(vbose> 2)fprintf(stderr,"(%d: not %d%c)\n",stackptr,q,x+(falsity[q]?'a':'A'));
o,mem[q]= bb= b-t;
o,stack[stackptr++]= (q<<8)+b;
if(believe3&&bb&&!(bb&(bb-1)))o,pstack[pstackptr++]= q;
return bb;
}
return b;
}

/*:7*//*8:*/
#line 166 "./back-20q.w"

int innerforce(int q,int x){
register int t,b;
t= 1<<x;
o,b= mem[q];
if((b&t)==0)return 0;
if(b!=t){
if(vbose> 2)fprintf(stderr,"(%d: %d%c)\n",stackptr,q,x+(falsity[q]?'a':'A'));
o,mem[q]= t;
o,stack[stackptr++]= (q<<8)+b;
if(believe3)o,pstack[pstackptr++]= q;
}
return t;
}

/*:8*//*9:*/
#line 183 "./back-20q.w"

void print_state(void){
register int b,c,q,z;
for(q= 1;q<=20;q++)
if(q<10)fprintf(stderr,"%d    ",q);
else fprintf(stderr,"%d   ",q);
printf("\n");
for(q= 1;q<=20;q++){
for(b= mem[q],c= z= 0;c<5;c++){
if(b&(1<<c))fprintf(stderr,"%c",c+(falsity[q]?'a':'A'));
else z++;
}
if(mem[q+tag])z--,fprintf(stderr,"*");
for(c= 0;c<z;c++)fprintf(stderr," ");
}
printf("\n");
}

/*:9*//*10:*/
#line 201 "./back-20q.w"

void print_cols(void){
register int c,q;
for(c= 0;c<5;c++){
fprintf(stderr,"%c:",'A'+c);
for(q= 1;q<=20;q++)if(mem[q]&(1<<c))fprintf(stderr," %d",q);
fprintf(stderr,"\n");
}
}

/*:10*//*11:*/
#line 211 "./back-20q.w"

void print_stack(int p){
register int i,l,b,ii;
for(i= 0,l= 1;i<p;i++){
ii= stack[i]>>8;
fprintf(stderr,"mem[%d%s]:",ii> 20?ii-tag:ii,ii> 20?"*":"");
for(b= 0;b<5;b++)if(stack[i]&(1<<b))fprintf(stderr,"%c",b+'A');
while(i==frame[l]){
fprintf(stderr," l%dq%d%s",l,order[l],falsity[l]?"-":"");
l++;
}
fprintf(stderr,"\n");
}
}

/*:11*/
#line 43 "./back-20q.w"
;
main(int argc,char*argv[]){
register int i,j,k,l,p,q,t,u,x,y,really_bad;
/*3:*/
#line 60 "./back-20q.w"

if(argc<3||sscanf(argv[1],"%d",&false1)!=1||
sscanf(argv[2],"%d",&false2)!=1){
fprintf(stderr,"Usage: %s false1 false2 [verbose]\n",argv[0]);
exit(-1);
}
score= 20;
if(false1> 0&&false1<=20)falsity[false1]= 1,score--;
if(false2> 0&&false2<=20&&false2!=false1)falsity[false2]= 1,score--;
believe3= !falsity[3];
vbose= argc-3;
if(falsity[6]){
if(vbose)fprintf(stderr,"Question 6 can't be wrong.\n");
goto done;
}

/*:3*/
#line 46 "./back-20q.w"
;
/*5:*/
#line 99 "./back-20q.w"

for(q= 1;q<=20;q++)o,mem[q]= AA+BB+CC+DD+EE;

/*:5*/
#line 47 "./back-20q.w"
;
/*12:*/
#line 231 "./back-20q.w"

b1:l= 1,stackptr= 0;
b2:nodes++,profile[l]++;
if(mems>=thresh)/*14:*/
#line 283 "./back-20q.w"

{
fprintf(stderr,"After %llu mems, l=%d, stackptr=%d\n",mems,l,stackptr);
print_state();
thresh+= delta;
}

/*:14*/
#line 234 "./back-20q.w"
;
if(l> 20)/*16:*/
#line 321 "./back-20q.w"

{
/*30:*/
#line 507 "./back-20q.w"

{
register int dA= 0,dB= 0,dC= 0,dD= 0,dE= 0;
for(q= 1;q<=20;q++){
o,y= mem[q];
if(y==AA)dA++;
else if(y==BB)dB++;
else if(y==CC)dC++;
else if(y==DD)dD++;
else dE++;
}
o,dist[A]= dA;
o,dist[B]= dB;
o,dist[C]= dC;
o,dist[D]= dD;
o,dist[E]= dE;
trick= (1<<dA)+(1<<dB)+(1<<dC)+(1<<dD)+(1<<dE);
}

/*:30*/
#line 323 "./back-20q.w"
;
for(q= 1;q<=20;q++)if(o,mem[tag+q]){
oo,x= rho[mem[q]];
o,u= falsity[q];
if(vbose> 1)fprintf(stderr,"Checking %d%c\n",q,x+(u?'a':'A'));
switch(pack(u,q,x)){
/*20:*/
#line 383 "./back-20q.w"

case pack(1,1,E):if((o,mem[4]!=AA)&&(o,mem[5]==AA))goto b5;break;

/*:20*//*22:*/
#line 404 "./back-20q.w"

case pack(1,2,B):if((o,mem[4]!=BB)&&(o,mem[5]!=BB)&&(o,mem[6]==BB))goto b5;
break;
case pack(1,2,C):if((o,mem[4]!=CC)&&(o,mem[5]!=CC)&&(o,mem[6]!=CC)&&
(o,mem[7]!=CC)&&(o,mem[8]==CC))goto b5;
break;
case pack(1,2,D):if((o,mem[4]!=DD)&&(o,mem[5]!=DD)&&(o,mem[6]!=DD)&&
(o,mem[7]!=DD)&&(o,mem[8]!=DD)&&(o,mem[9]!=DD)&&(o,mem[10]==DD))goto b5;
break;
case pack(1,2,E):if((o,mem[4]!=EE)&&(o,mem[5]!=EE)&&(o,mem[6]!=EE)&&
(o,mem[7]!=EE)&&(o,mem[8]!=EE)&&(o,mem[9]!=EE)&&(o,mem[10]!=EE)&&
(o,mem[11]!=EE)&&(o,mem[12]==EE))goto b5;
break;

/*:22*//*24:*/
#line 430 "./back-20q.w"

case pack(1,3,A):if((oo,mem[15]==mem[16])&&(o,mem[16]!=mem[17])&&
(o,mem[17]!=mem[18])&&(o,mem[18]!=mem[19])&&(o,mem[19]!=mem[20]))
goto test3;break;
case pack(1,3,B):if((oo,mem[15]!=mem[16])&&(o,mem[16]==mem[17])&&
(o,mem[17]!=mem[18])&&(o,mem[18]!=mem[19])&&(o,mem[19]!=mem[20]))
goto test3;break;
case pack(1,3,C):if((oo,mem[15]!=mem[16])&&(o,mem[16]!=mem[17])&&
(o,mem[17]==mem[18])&&(o,mem[18]!=mem[19])&&(o,mem[19]!=mem[20]))
goto test3;break;
case pack(1,3,D):if((oo,mem[15]!=mem[16])&&(o,mem[16]!=mem[17])&&
(o,mem[17]!=mem[18])&&(o,mem[18]==mem[19])&&(o,mem[19]!=mem[20]))
goto test3;break;
case pack(1,3,E):if((oo,mem[15]!=mem[16])&&(o,mem[16]!=mem[17])&&
(o,mem[17]!=mem[18])&&(o,mem[18]!=mem[19])&&(o,mem[19]==mem[20]))
goto test3;break;
test3:if((oo,mem[1]!=mem[2])&&(o,mem[2]!=mem[3])&&(o,mem[3]!=mem[4])&&
(o,mem[4]!=mem[5])&&(o,mem[5]!=mem[6])&&(o,mem[6]!=mem[7])&&
(o,mem[7]!=mem[8])&&(o,mem[8]!=mem[9])&&(o,mem[9]!=mem[10])&&
(o,mem[10]!=mem[11])&&(o,mem[11]!=mem[12])&&(o,mem[12]!=mem[13])&&
(o,mem[13]!=mem[14])&&(o,mem[14]!=mem[15]))goto b5;break;

/*:24*//*26:*/
#line 468 "./back-20q.w"

case pack(1,4,A):if((o,mem[13]==AA))goto b5;break;
case pack(1,4,B):if((o,mem[14]==BB)&&(o,mem[16]==BB))goto b5;break;
case pack(1,4,C):if(o,mem[7]==CC)goto b5;break;
case pack(1,4,E):if((o,mem[8]==EE)&&(o,mem[12]==EE))goto b5;break;

/*:26*//*32:*/
#line 531 "./back-20q.w"

case pack(0,7,A):case pack(0,7,B):case pack(0,7,C):case pack(0,7,D):
case pack(0,7,E):
case pack(1,7,A):case pack(1,7,B):case pack(1,7,C):case pack(1,7,D):
case pack(1,7,E):for(o,i= 0,j= dist[x];i<5;i++)if(o,dist[i]> j)break;
if(!u&&i<5)goto b5;
if(u&&i==5)goto b5;break;

/*:32*//*34:*/
#line 548 "./back-20q.w"

case pack(0,8,A):case pack(0,8,B):case pack(0,8,C):case pack(0,8,D):
case pack(0,8,E):
case pack(1,8,A):case pack(1,8,B):case pack(1,8,C):case pack(1,8,D):
case pack(1,8,E):for(i= 0;i<5;i++)o,tie[i]= 0;
for(i= 0;i<5;i++)for(j= i+1;j<5;j++)
if(oo,dist[i]==dist[j])oo,tie[i]= tie[j]= 1;
for(i= 0,j= 100;i<5;i++)if((o,tie[i]==0)&&(o,dist[i]<j))j= dist[i];

if(!u&&j!=dist[x])goto b5;
if(u&&j==dist[x])goto b5;break;

/*:34*//*36:*/
#line 571 "./back-20q.w"

case pack(0,9,A):case pack(0,9,B):case pack(0,9,C):case pack(0,9,D):
case pack(0,9,E):
case pack(1,9,A):case pack(1,9,B):case pack(1,9,C):case pack(1,9,D):
case pack(1,9,E):for(j= 0,i= 1;i<=20;i++)
if((o,falsity[i]==0)&&(o,mem[i]==(1<<x)))j+= i;
switch(x){
case A:i= (j>=59&&j<=62);break;
case B:i= (j>=52&&j<=55);break;
case C:i= (j>=44&&j<=49);break;
case D:i= (j>=59&&j<=67);break;
case E:i= (j>=44&&j<=53);break;
}
if(!u&&!i)goto b5;
if(u&&i)goto b5;break;

/*:36*//*39:*/
#line 611 "./back-20q.w"

case pack(0,11,A):case pack(0,11,B):case pack(0,11,C):case pack(0,11,D):
case pack(0,11,E):
case pack(1,11,A):case pack(1,11,B):case pack(1,11,C):case pack(1,11,D):
case pack(1,11,E):
if(!u&&(o,dist[D]!=x+2))goto b5;
if(u&&(o,dist[D]==x+2))goto b5;break;

/*:39*//*41:*/
#line 642 "./back-20q.w"

case pack(0,12,A):case pack(0,12,B):case pack(0,12,C):case pack(0,12,D):
if(oo,dist[x]-1!=dist[x+1])goto b5;break;
case pack(0,12,E):if((oo,dist[E]-1==dist[B])||(o,dist[E]-1==dist[C])||
(o,dist[E]-1==dist[D]))goto b5;break;
case pack(1,12,A):case pack(1,12,B):case pack(1,12,C):case pack(1,12,D):
if(oo,dist[x]-1==dist[x+1])goto b5;break;
case pack(1,12,E):if((oo,dist[E]-1!=dist[B])&&(o,dist[E]-1!=dist[C])&&
(o,dist[E]-1!=dist[D]))goto b5;break;

/*:41*//*43:*/
#line 661 "./back-20q.w"

case pack(0,13,A):case pack(0,13,B):case pack(0,13,C):case pack(0,13,D):
case pack(0,13,E):
case pack(1,13,A):case pack(1,13,B):case pack(1,13,C):case pack(1,13,D):
case pack(1,13,E):
if(!u&&(o,dist[E]!=5-x))goto b5;
if(u&&(o,dist[E]==5-x))goto b5;break;

/*:43*//*45:*/
#line 684 "./back-20q.w"

case pack(0,14,A):case pack(0,14,B):case pack(0,14,C):case pack(0,14,D):
for(i= 0;i<5;i++)if(o,dist[i]==x+2)goto b5;
break;
case pack(0,14,E):if(trick!=0x7c)goto b5;break;
case pack(1,14,A):case pack(1,14,B):case pack(1,14,C):case pack(1,14,D):
for(i= 0;i<5;i++)if(o,dist[i]==x+2)break;
if(i==5)goto b5;
break;
case pack(1,14,E):if(trick==0x7c)goto b5;break;

/*:45*//*47:*/
#line 712 "./back-20q.w"

case pack(1,15,B):if((o,mem[9]==AA)&&(o,mem[11]!=AA)&&(o,mem[13]!=AA))
goto test_odd;break;
case pack(0,15,C):case pack(1,15,C):if((o,mem[1]!=AA)&&(o,mem[3]!=AA)&&
(o,mem[5]!=AA)&&(o,mem[7]!=AA)&&(o,mem[9]!=AA)&&(o,mem[11]==AA)&&
(o,mem[13]!=AA)&&(o,mem[17]!=AA)&&(o,mem[19]!=AA))goto b5;break;
case pack(1,15,D):if((o,mem[9]!=AA)&&(o,mem[11]!=AA)&&(o,mem[13]==AA))
goto test_odd;break;
test_odd:if((o,mem[1]!=AA)&&(o,mem[5]!=AA)&&(o,mem[7]!=AA)&&
(o,mem[17]!=AA)&&(o,mem[19]!=AA))goto b5;
break;

/*:47*//*49:*/
#line 739 "./back-20q.w"

case pack(0,16,C):if(o,mem[8]!=mem[13])goto b5;break;
case pack(0,16,D):if(o,mem[8]!=mem[18])goto b5;break;
case pack(1,16,C):if(o,mem[8]==mem[13])goto b5;break;
case pack(1,16,D):if(o,mem[8]==mem[18])goto b5;break;

/*:49*//*54:*/
#line 805 "./back-20q.w"

case pack(1,19,A):if((o,mem[14]==BB)&&(o,mem[15]!=BB)&&(o,mem[16]!=BB)&&
(o,mem[17]!=BB)&&(o,mem[18]!=BB))goto b5;break;
case pack(1,19,C):if((o,mem[16]==BB)&&(o,mem[17]!=BB)&&(o,mem[18]!=BB))goto b5;break;
case pack(1,19,D):if((o,mem[17]==BB)&(o,mem[18]!=BB))goto b5;break;
case pack(1,19,E):if(o,mem[18]==BB)goto b5;break;

/*:54*/
#line 329 "./back-20q.w"

default:fprintf(stderr,"Impossible deferred case %d%c!\n",q,x+(u?'a':'A'));
exit(-31);
}
}
/*17:*/
#line 338 "./back-20q.w"

count++;
printf("%lld: ",count);
for(q= 1;q<=20;q++)
printf("%c",(falsity[q]?'a':'A')+rho[mem[q]]);
printf("\n");

/*:17*/
#line 334 "./back-20q.w"
;
goto b5;
}

/*:16*/
#line 235 "./back-20q.w"
;
oo,q= order[l],u= falsity[q];
o,y= mem[q];
b3:if(y==0){
fprintf(stderr,"I'm confused!\n");exit(-69);
}
p= stackptr;
o,x= rho[y];
pstackptr= 0;
if(vbose> 1)fprintf(stderr,"Level %d(%d), trying %d%c\n",l,p,q,x+(u?'a':'A'));
switch(pack(u,q,x)){
/*19:*/
#line 367 "./back-20q.w"

case pack(0,1,A):goto okay;
case pack(0,1,B):deny(1,A);force(2,A);goto okay;
case pack(0,1,C):deny(1,A);deny(2,A);force(3,A);goto okay;
case pack(0,1,D):deny(1,A);deny(2,A);deny(3,A);force(4,A);goto okay;
case pack(0,1,E):deny(1,A);deny(2,A);deny(3,A);deny(4,A);force(5,A);
goto okay;
case pack(1,1,A):goto bad;
case pack(1,1,B):deny(2,A);goto okay;
case pack(1,1,C):if(o,mem[2]!=AA)deny(3,A);goto okay;
case pack(1,1,D):if((o,mem[2]!=AA)&&(o,mem[3]!=AA))deny(4,A);goto okay;
case pack(1,1,E):if((o,mem[2]==AA)||(o,mem[3]==AA))goto okay;
goto defer;

/*:19*//*21:*/
#line 389 "./back-20q.w"

case pack(0,2,A):deny(3,A);force(4,A);goto okay;
case pack(0,2,B):deny(3,B);deny(4,B);deny(5,B);force(6,B);goto okay;
case pack(0,2,C):deny(3,C);deny(4,C);deny(5,C);deny(6,C);deny(7,C);
force(8,C);goto okay;
case pack(0,2,D):deny(3,D);deny(4,D);deny(5,D);deny(6,D);deny(7,D);
deny(8,D);deny(9,D);force(10,D);goto okay;
case pack(0,2,E):deny(3,E);deny(4,E);deny(5,E);deny(6,E);deny(7,E);
deny(8,E);deny(9,E);deny(10,E);deny(11,E);force(12,E);goto okay;
case pack(1,2,A):if(o,mem[3]!=AA)deny(4,A);goto okay;
case pack(1,2,B):if(o,mem[3]==BB)goto okay;goto defer;
case pack(1,2,C):if(o,mem[3]==CC)goto okay;goto defer;
case pack(1,2,D):if(o,mem[3]==DD)goto okay;goto defer;
case pack(1,2,E):if(o,mem[3]==EE)goto okay;goto defer;

/*:21*//*23:*/
#line 421 "./back-20q.w"

case pack(0,3,A):loguy= 15,higuy= 16;goto okay;
case pack(0,3,B):loguy= 16,higuy= 17;goto okay;
case pack(0,3,C):loguy= 17,higuy= 18;goto okay;
case pack(0,3,D):loguy= 18,higuy= 19;goto okay;
case pack(0,3,E):loguy= 19,higuy= 20;goto okay;
case pack(1,3,A):case pack(1,3,B):case pack(1,3,C):case pack(1,3,D):
case pack(1,3,E):goto defer;

/*:23*//*25:*/
#line 457 "./back-20q.w"

case pack(0,4,A):force(10,A);force(13,A);goto okay;
case pack(0,4,B):force(14,B);force(16,B);goto okay;
case pack(0,4,C):force(7,C);force(20,C);goto okay;
case pack(0,4,D):force(1,D);force(15,D);goto okay;
case pack(0,4,E):force(8,E);force(12,E);goto okay;
case pack(1,4,A):if(o,mem[10]!=AA)goto okay;goto defer;
case pack(1,4,B):case pack(1,4,E):goto defer;
case pack(1,4,C):if(o,mem[20]!=CC)goto okay;goto defer;
case pack(1,4,D):if((o,mem[1]!=DD)||(o,mem[15]!=DD))goto okay;goto bad;

/*:25*//*27:*/
#line 477 "./back-20q.w"

case pack(0,5,A):force(14,B);goto okay;
case pack(0,5,B):force(14,E);goto okay;
case pack(0,5,C):force(14,C);goto okay;
case pack(0,5,D):force(14,A);goto okay;
case pack(0,5,E):force(14,D);goto okay;
case pack(1,5,A):deny(14,B);goto okay;
case pack(1,5,B):deny(14,E);goto okay;
case pack(1,5,C):deny(14,C);goto okay;
case pack(1,5,D):deny(14,A);goto okay;
case pack(1,5,E):deny(14,D);goto okay;

/*:27*//*28:*/
#line 492 "./back-20q.w"

case pack(0,6,A):case pack(0,6,B):case pack(0,6,C):case pack(0,6,D):
case pack(0,6,E):goto okay;
case pack(1,6,A):case pack(1,6,B):case pack(1,6,C):case pack(1,6,D):
case pack(1,6,E):goto bad;

/*:28*//*29:*/
#line 501 "./back-20q.w"

case pack(0,7,A):case pack(0,7,B):case pack(0,7,C):case pack(0,7,D):
case pack(0,7,E):
case pack(1,7,A):case pack(1,7,B):case pack(1,7,C):case pack(1,7,D):
case pack(1,7,E):goto defer;

/*:29*//*33:*/
#line 542 "./back-20q.w"

case pack(0,8,A):case pack(0,8,B):case pack(0,8,C):case pack(0,8,D):
case pack(0,8,E):
case pack(1,8,A):case pack(1,8,B):case pack(1,8,C):case pack(1,8,D):
case pack(1,8,E):goto defer;

/*:33*//*35:*/
#line 565 "./back-20q.w"

case pack(0,9,A):case pack(0,9,B):case pack(0,9,C):case pack(0,9,D):
case pack(0,9,E):
case pack(1,9,A):case pack(1,9,B):case pack(1,9,C):case pack(1,9,D):
case pack(1,9,E):goto defer;

/*:35*//*37:*/
#line 590 "./back-20q.w"

case pack(0,10,A):force(17,D);goto okay;
case pack(0,10,B):force(17,B);goto okay;
case pack(0,10,C):force(17,A);goto okay;
case pack(0,10,D):force(17,E);goto okay;
case pack(0,10,E):if(o,falsity[17]==1)goto okay;goto bad;
case pack(1,10,A):deny(17,D);goto okay;
case pack(1,10,B):deny(17,B);goto okay;
case pack(1,10,C):deny(17,A);goto okay;
case pack(1,10,D):deny(17,E);goto okay;
case pack(1,10,E):if(o,falsity[17]==1)goto bad;goto okay;

/*:37*//*38:*/
#line 605 "./back-20q.w"

case pack(0,11,A):case pack(0,11,B):case pack(0,11,C):case pack(0,11,D):
case pack(0,11,E):
case pack(1,11,A):case pack(1,11,B):case pack(1,11,C):case pack(1,11,D):
case pack(1,11,E):goto defer;

/*:38*//*40:*/
#line 636 "./back-20q.w"

case pack(0,12,A):case pack(0,12,B):case pack(0,12,C):case pack(0,12,D):
case pack(0,12,E):
case pack(1,12,A):case pack(1,12,B):case pack(1,12,C):case pack(1,12,D):
case pack(1,12,E):goto defer;

/*:40*//*42:*/
#line 655 "./back-20q.w"

case pack(0,13,A):case pack(0,13,B):case pack(0,13,C):case pack(0,13,D):
case pack(0,13,E):
case pack(1,13,A):case pack(1,13,B):case pack(1,13,C):case pack(1,13,D):
case pack(1,13,E):goto defer;

/*:42*//*44:*/
#line 678 "./back-20q.w"

case pack(0,14,A):case pack(0,14,B):case pack(0,14,C):case pack(0,14,D):
case pack(0,14,E):
case pack(1,14,A):case pack(1,14,B):case pack(1,14,C):case pack(1,14,D):
case pack(1,14,E):goto defer;

/*:44*//*46:*/
#line 701 "./back-20q.w"

case pack(0,15,A):case pack(0,15,E):goto bad;
case pack(0,15,B):force(9,A);deny(11,A);deny(13,A);goto odd_denials;
case pack(0,15,D):deny(9,A);deny(11,A);force(13,A);goto odd_denials;
odd_denials:deny(1,A);deny(3,A);deny(5,A);deny(7,A);
deny(17,A);deny(19,A);goto okay;
case pack(1,15,A):case pack(1,15,E):goto okay;
case pack(0,15,C):case pack(1,15,B):case pack(1,15,D):
if(o,mem[3]==AA)goto okay;goto defer;
case pack(1,15,C):goto defer;

/*:46*//*48:*/
#line 729 "./back-20q.w"

case pack(0,16,A):oo;force(8,rho[mem[3]]);goto okay;
case pack(0,16,B):oo;force(8,rho[mem[2]]);goto okay;
case pack(0,16,E):oo;force(8,rho[mem[20]]);goto okay;
case pack(0,16,C):case pack(0,16,D):goto defer;
case pack(1,16,A):oo;deny(8,rho[mem[3]]);goto okay;
case pack(1,16,B):oo;deny(8,rho[mem[2]]);goto okay;
case pack(1,16,E):oo;deny(8,rho[mem[20]]);goto okay;
case pack(1,16,C):case pack(1,16,D):goto defer;

/*:48*//*50:*/
#line 753 "./back-20q.w"

case pack(0,17,A):force(10,C);goto okay;
case pack(0,17,B):force(10,D);goto okay;
case pack(0,17,C):force(10,B);goto okay;
case pack(0,17,D):force(10,A);goto okay;
case pack(0,17,E):deny(10,E);if(o,falsity[10]==0)goto okay;goto bad;
case pack(1,17,A):deny(10,C);goto okay;
case pack(1,17,B):deny(10,D);goto okay;
case pack(1,17,C):deny(10,B);goto okay;
case pack(1,17,D):deny(10,A);goto okay;
case pack(1,17,E):deny(10,E);if(o,falsity[10]==0)goto bad;goto okay;

/*:50*//*51:*/
#line 772 "./back-20q.w"

case pack(0,18,A):case pack(0,18,B):case pack(0,18,C):case pack(0,18,D):
case pack(0,18,E):
case pack(1,18,A):case pack(1,18,B):case pack(1,18,C):case pack(1,18,D):
case pack(1,18,E):j= isvowel(2)+isvowel(3)+isvowel(5)+isvowel(7)+
isvowel(11)+isvowel(13)+isvowel(17)+isvowel(19);
j= (1<<j)&magic[x];
if(!u&&j)goto okay;goto bad;
if(u&&!j)goto okay;goto bad;

/*:51*//*53:*/
#line 794 "./back-20q.w"

case pack(0,19,A):force(14,B);deny(15,B);deny(16,B);deny(17,B);
deny(18,B);deny(20,B);goto okay;
case pack(0,19,B):goto bad;
case pack(0,19,C):force(16,B);deny(17,B);deny(18,B);deny(20,B);goto okay;
case pack(0,19,D):force(17,B);deny(18,B);deny(20,B);goto okay;
case pack(0,19,E):force(18,B);deny(20,B);goto okay;
case pack(1,19,B):goto okay;
case pack(1,19,A):case pack(1,19,C):case pack(1,19,D):case pack(1,19,E):
if(o,mem[20]==BB)goto okay;goto defer;

/*:53*//*55:*/
#line 823 "./back-20q.w"

case pack(0,20,A):case pack(0,20,B):case pack(0,20,C):
if(score==18+x)goto okay;goto bad;
case pack(0,20,D):case pack(0,20,E):goto bad;
case pack(1,20,A):case pack(1,20,B):case pack(1,20,C):
if(score!=18+x)goto okay;goto bad;
case pack(1,20,D):case pack(1,20,E):goto okay;

/*:55*/
#line 246 "./back-20q.w"

default:fprintf(stderr,"Impossible case %d%c!\n",q,x+(u?'a':'A'));
exit(-30);
}
defer:oo,mem[tag+q]= 1,stack[stackptr++]= (q+tag)<<8;
if(vbose> 1)fprintf(stderr,"(%d: deferring %d%c)\n",stackptr-1,q,x+(u?'a':'A'));
okay:force(q,x);
/*15:*/
#line 308 "./back-20q.w"

while(pstackptr){
o,t= pstack[--pstackptr];
oo,j= rho[mem[t]];
if(vbose> 3)fprintf(stderr,"(propagating from %d%c)\n",t,j+'A');
if(t==loguy)force(t+1,j)
else if(t<20)deny(t+1,j);
if(t==higuy)force(t-1,j)
else if(t> 1)deny(t-1,j);
}

/*:15*/
#line 253 "./back-20q.w"
;
o,frame[l]= p;
l++;goto b2;
bad:if(really_bad){
really_bad= 0;
goto b5;
}
while(stackptr> p)/*13:*/
#line 277 "./back-20q.w"

{
o,t= stack[--stackptr];
o,mem[t>>8]= t&0x1f;
}

/*:13*/
#line 260 "./back-20q.w"
;
b4:really_bad= 1;
pstackptr= 0;
deny(q,x);
/*15:*/
#line 308 "./back-20q.w"

while(pstackptr){
o,t= pstack[--pstackptr];
oo,j= rho[mem[t]];
if(vbose> 3)fprintf(stderr,"(propagating from %d%c)\n",t,j+'A');
if(t==loguy)force(t+1,j)
else if(t<20)deny(t+1,j);
if(t==higuy)force(t-1,j)
else if(t> 1)deny(t-1,j);
}

/*:15*/
#line 264 "./back-20q.w"
;
really_bad= 0;
y-= 1<<x;
if(y==mem[q])goto b3;
fprintf(stderr,"I screwed up!\n");exit(-666);
b5:if(--l){
oo,q= order[l],u= falsity[q];
o,p= frame[l];
while(stackptr> p)/*13:*/
#line 277 "./back-20q.w"

{
o,t= stack[--stackptr];
o,mem[t>>8]= t&0x1f;
}

/*:13*/
#line 272 "./back-20q.w"
;
oo,y= mem[q],x= rho[y];
goto b4;
}

/*:12*/
#line 48 "./back-20q.w"
;
done:fprintf(stderr,"Altogether %llu solutions (%llu mems, %llu nodes).\n",
count,mems,nodes);
if(vbose)/*2:*/
#line 54 "./back-20q.w"

{
fprintf(stderr,"Profile:          1\n");
for(k= 2;k<=21;k++)fprintf(stderr,"%19lld\n",profile[k]);
}

/*:2*/
#line 51 "./back-20q.w"
;
}

/*:1*/
