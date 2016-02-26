#define nn 10 \

#define S 0
#define R 10
#define D 20 \

/*1:*/
#line 6 "co-debruijn.w"

#include <stdio.h> 
int p[nn];
int x[nn],y[nn],t[nn],xp[nn],yp[nn],tp[nn];
int n[nn];
/*2:*/
#line 46 "co-debruijn.w"

void init(int r){
register q= r-1;
n[q]= r;
if(r==2)p[q]= S+1;
else if(r&1){
p[q]= R;
x[q]= 0;
init(q);
}else{
register int k,qq;
qq= q>>1;
p[q]= D+1;
x[q]= xp[q]= 2;
init(qq+1);
for(k= q-1;k> qq;k--)
p[k]= p[k-qq],x[k]= x[k-qq],xp[k]= xp[k-qq],n[k]= n[k-qq];
}
}

/*:2*//*4:*/
#line 71 "co-debruijn.w"

int co(int q){
switch(p[q]){
/*5:*/
#line 81 "co-debruijn.w"

case S+1:p[q]= S+2;return 0;
case S+2:p[q]= S+3;return 0;
case S+3:p[q]= S+4;return 1;
case S+4:p[q]= S+1;return 1;

/*:5*//*6:*/
#line 92 "co-debruijn.w"

R1:case R:p[q]= R+1;return x[q];
case R+1:if(x[q]!=0&&t[q]>=n[q-1])goto R3;
R2:y[q]= co(q-1);
R3:t[q]= (y[q]==1?t[q]+1:0);
R4:if(t[q]==n[q-1]&&x[q]!=0)goto R2;
R5:x[q]= (x[q]+y[q])&1;goto R1;

/*:6*//*7:*/
#line 103 "co-debruijn.w"

D1:case D+1:if(t[q]!=n[q-1]||x[q]>=2)y[q]= co(q-(n[q]>>1));
D2:if(x[q]!=y[q])x[q]= y[q],t[q]= 1;else t[q]++;
D3:p[q]= D+4;return x[q];
D4:case D+4:yp[q]= co(q-1);
D5:if(xp[q]!=yp[q])xp[q]= yp[q],tp[q]= 1;else tp[q]++;
D6:if(tp[q]==n[q-1]&&xp[q]<2){
if(t[q]<n[q-1]||xp[q]<x[q])goto D4;
if(xp[q]==x[q])goto D3;
}
D7:p[q]= D+8;return(xp[q]);
case D+8:if(tp[q]==n[q-1]&&xp[q]<2)goto D3;
goto D1;

/*:7*/
#line 74 "co-debruijn.w"

}
}

/*:4*/
#line 11 "co-debruijn.w"
;
main(){
register int k,kp;
/*3:*/
#line 66 "co-debruijn.w"

init(nn);

/*:3*/
#line 14 "co-debruijn.w"
;
for(k= 0;k<(1<<nn);k++)
printf("%d",co(nn-1));
printf("\n");
}

/*:1*/
