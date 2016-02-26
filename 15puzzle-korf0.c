#define row(x) ((x) >>2) 
#define col(x) ((x) &0x3)  \

/*1:*/
#line 40 "./15puzzle-korf0.w"

#include <stdio.h> 
#include <time.h> 
char board[16];
char start[16];
short tim[100];
char dir[100],pos[100];
int timer;

main(int argc,char*argv[])
{
register int j,k,s,t,l,d,p,q,del,piece,moves;
/*2:*/
#line 87 "./15puzzle-korf0.w"

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
#line 52 "./15puzzle-korf0.w"
;
/*4:*/
#line 174 "./15puzzle-korf0.w"

/*5:*/
#line 188 "./15puzzle-korf0.w"

for(j= moves= 0;j<16;j++)if(start[j]>=0){
del= row(start[j])-row(j);
moves+= (del<0?-del:del);
del= col(start[j])-col(j);
moves+= (del<0?-del:del);
}

/*:5*/
#line 175 "./15puzzle-korf0.w"
;
if(moves==0)goto win;
while(1){
timer= time(0);
t= moves;

/*6:*/
#line 214 "./15puzzle-korf0.w"

for(j= 0;j<16;j++){
board[j]= start[j];
if(board[j]<0)p= j;
}
pos[0]= 16,l= 1;
newlevel:d= 0,tim[l]= t,pos[l]= p,q= pos[l-1];
trymove:switch(d){
case 0:if(col(p)<=2&&q!=p+1)/*7:*/
#line 248 "./15puzzle-korf0.w"

{
q= p+1,piece= board[q];
del= (col(piece)<col(q)?0x1:0x100);
break;
}

/*:7*/
#line 222 "./15puzzle-korf0.w"
;
d++;
case 1:if(row(p)>=1&&q!=p-4)/*8:*/
#line 255 "./15puzzle-korf0.w"

{
q= p-4,piece= board[q];
del= (row(piece)> row(q)?0x1:0x100);
break;
}

/*:8*/
#line 224 "./15puzzle-korf0.w"
;
d++;
case 2:if(col(p)>=1&&q!=p-1)/*9:*/
#line 262 "./15puzzle-korf0.w"

{
q= p-1,piece= board[q];
del= (col(piece)> col(q)?0x1:0x100);
break;
}

/*:9*/
#line 226 "./15puzzle-korf0.w"
;
d++;
case 3:if(row(p)<=2&&q!=p+4)/*10:*/
#line 269 "./15puzzle-korf0.w"

{
q= p+4,piece= board[q];
del= (row(piece)<row(q)?0x1:0x100);
break;
}

/*:10*/
#line 228 "./15puzzle-korf0.w"
;
d++;
case 4:goto backtrack;
}


if(t<=del){
if(t==del)goto win;
d++;goto trymove;
}
dir[l]= d,board[p]= board[q],t-= del,p= q,l++;
goto newlevel;
backtrack:if(l> 1){
l--;
q= pos[l],board[p]= board[q],p= q,q= pos[l-1],t= tim[l],d= dir[l]+1;
goto trymove;
}

/*:6*/
#line 181 "./15puzzle-korf0.w"
;
printf(" ... no solution with %d+%d moves (%d sec)\n",
moves&0xff,moves>>8,time(0)-timer);
moves+= 0x101;
}
win:

/*:4*/
#line 53 "./15puzzle-korf0.w"
;
/*11:*/
#line 280 "./15puzzle-korf0.w"

pos[l+1]= q;
printf("Solution in %d+%d moves: ",moves&0xff,moves>>8);
if(moves> 0){
for(j= 0;j<16;j++)board[j]= start[j];
for(k= 1;k<=l;k++){
printf("%x",board[pos[k+1]]+1);
board[pos[k]]= board[pos[k+1]];
}
printf(" (%d sec)\n",time(0)-timer);
}

/*:11*/
#line 54 "./15puzzle-korf0.w"
;
}

/*:1*/
