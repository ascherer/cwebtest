#define maxn 105 \

/*1:*/
#line 31 "./commafree-eastman.w"

#include <stdio.h> 
#include <stdlib.h> 
int x[maxn+maxn+maxn];
int b[maxn+maxn+maxn];
int bb[maxn];
/*5:*/
#line 119 "./commafree-eastman.w"

int compare(register int i){
register int j;
if(b[i]-b[i-1]==b[i+1]-b[i]){
for(j= 0;b[i]+j<b[i+1];j++){
if(x[b[i-1]+j]==x[b[i]+j])continue;
return(x[b[i-1]+j]> x[b[i]+j]);
}
return 0;
}
return(b[i]-b[i-1]> b[i+1]-b[i]);
}

/*:5*/
#line 37 "./commafree-eastman.w"
;
main(int argc,char*argv[]){
register int i,j,k,n,p,q,t,tt;
/*2:*/
#line 44 "./commafree-eastman.w"

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
#line 40 "./commafree-eastman.w"
;
/*3:*/
#line 106 "./commafree-eastman.w"

/*4:*/
#line 111 "./commafree-eastman.w"

for(j= n;j<n+n+n;j++)x[j]= x[j-n];
for(j= 0;j<n+n+n;j++)b[j]= j;
t= n;

/*:4*/
#line 107 "./commafree-eastman.w"
;
for(p= 1,t= n;t> 1;t= tt,p++)
/*6:*/
#line 132 "./commafree-eastman.w"

{
for(tt= 0,i= 1;i<=t;i++)if(compare(i))break;
if(i> t){
fprintf(stderr,"The input is cyclic!\n");
exit(-666);
}
for(;compare(i+1);i++);
for(;i<=t;i= j){
for(q= i+1;compare(q+1)==0;q++);
for(j= q+1;compare(j+1);j++);
if((j-i)&1)/*7:*/
#line 151 "./commafree-eastman.w"

{
if((q-i)&1)q++;
if(q<t)bb[tt++]= b[q];
else{
for(k= tt++;k> 0;k--)bb[k]= bb[k-1];
bb[0]= b[q-t];
}
}

/*:7*/
#line 143 "./commafree-eastman.w"
;
}
printf("Phase %d leaves",p);
for(k= 0;k<tt;k++)b[k]= bb[k],printf(" %d",bb[k]);
printf("\n");
for(;b[k-tt]<n+n;k++)b[k]= b[k-tt]+n;
}

/*:6*/
#line 109 "./commafree-eastman.w"
;

/*:3*/
#line 41 "./commafree-eastman.w"
;
}

/*:1*/
