#define maxn 105 \

/*1:*/
#line 45 "./commafree-eastman-new.w"

#include <stdio.h> 
#include <stdlib.h> 
int x[maxn+maxn];
int b[maxn+maxn];
int bb[maxn];
/*5:*/
#line 140 "./commafree-eastman-new.w"

int less(register int i){
register int j;
if(b[i]-b[i-1]==b[i+1]-b[i]){
for(j= 0;b[i]+j<b[i+1];j++){
if(x[b[i-1]+j]==x[b[i]+j])continue;
return(x[b[i-1]+j]<x[b[i]+j]);
}
return 0;
}
return(b[i]-b[i-1]<b[i+1]-b[i]);
}

/*:5*/
#line 51 "./commafree-eastman-new.w"
;
main(int argc,char*argv[]){
register int i,i0,j,k,n,p,q,t,tt;
/*2:*/
#line 59 "./commafree-eastman-new.w"

if(argc<4){
fprintf(stderr,"Usage: %s x1 x2 ... xn\n",argv[0]);
exit(-1);
}
n= argc-1;
if((n&1)==0){
fprintf(stderr,"The number of items, n, should be odd, not %d!\n",n);
exit(-2);
}
for(j= 1;j<argc;j++){
if(sscanf(argv[j],"%d",&x[j-1])!=1||x[j-1]<0){
fprintf(stderr,"Argument %d should be a nonnegative integer, not `%s'!\n",
j,argv[j]);
exit(-3);
}
}

/*:2*/
#line 54 "./commafree-eastman-new.w"
;
/*3:*/
#line 125 "./commafree-eastman-new.w"

/*4:*/
#line 132 "./commafree-eastman-new.w"

for(j= n;j<n+n;j++)x[j]= x[j-n];
for(j= 0;j<n+n;j++)b[j]= j;
t= n;

/*:4*/
#line 126 "./commafree-eastman-new.w"
;
for(p= 1,t= n;t> 1;t= tt,p++)
/*6:*/
#line 153 "./commafree-eastman-new.w"

{
for(i= 1;;i++)if(!less(i))break;

for(i+= 2;i<=t+2;i++)if(less(i-1))break;
if(i> t+2){
fprintf(stderr,"The input is cyclic!\n");
exit(-666);
}

if(i<t)i0= i;else i= i0= i-t;
for(tt= 0;i<i0+t;i= j){
for(j= i+2;;j++)if(less(j-1))break;
if((j-i)&1)/*7:*/
#line 180 "./commafree-eastman-new.w"

{
if(i<t)bb[tt++]= b[i];
else{
for(k= tt++;k> 0;k--)bb[k]= bb[k-1];
bb[0]= b[i-t];
}
}

/*:7*/
#line 166 "./commafree-eastman-new.w"
;
}
printf("Phase %d leaves",p);
for(k= 0;k<tt;k++)b[k]= bb[k],printf(" %d",bb[k]);
printf("\n");
for(;b[k-tt]<n+n;k++)b[k]= b[k-tt]+n;
}

/*:6*/
#line 128 "./commafree-eastman-new.w"
;

/*:3*/
#line 55 "./commafree-eastman-new.w"
;
/*8:*/
#line 189 "./commafree-eastman-new.w"

for(j= b[0];j<b[0]+n;j++)printf(" %d",
x[j]);
printf("\n");

/*:8*/
#line 56 "./commafree-eastman-new.w"
;
}

/*:1*/
