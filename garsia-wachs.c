#define size 64 \

/*1:*/
#line 14 "./garsia-wachs.w"

#include <stdio.h> 

int w[size];
int l[size],r[size];
int d[size];
int q[size];
int v[size];
int t;
int m;

/*4:*/
#line 72 "./garsia-wachs.w"

combine(register int k)
{
register int j,d,x;
m++;l[m]= v[k-1];r[m]= v[k];w[m]= x= q[k-1]+q[k];
printf(" node %d(%d)=%d(%d)+%d(%d)\n",m,x,l[m],w[l[m]],r[m],w[r[m]]);
t--;
for(j= k;j<=t;j++)q[j]= q[j+1],v[j]= v[j+1];
for(j= k-2;q[j]<x;j--)q[j+1]= q[j],v[j+1]= v[j];
q[j+1]= x;v[j+1]= m;
for(d= 1;d<=t;d++)printf("%d ",q[d]);
printf("\n");
while(j> 0&&q[j-1]<=x){
d= t-j;combine(j);j= t-d;
}
}

/*:4*//*6:*/
#line 95 "./garsia-wachs.w"

mark(k,p)
int k;
int p;
{
printf(" node %d(%d) has depth %d\n",k,w[k],p);
d[k]= p;
if(l[k]>=0)mark(l[k],p+1);
if(r[k]>=0)mark(r[k],p+1);
}

/*:6*//*8:*/
#line 115 "./garsia-wachs.w"

build(b)
int b;
{
register int j= m;
if(d[t]==b)l[j]= t++;
else{
m--;l[j]= m;build(b+1);
}
if(d[t]==b)r[j]= t++;
else{
m--;r[j]= m;build(b+1);
}
printf(" node %d = %d + %d\n",j,l[j],r[j]);
}

/*:8*/
#line 25 "./garsia-wachs.w"


main(argc,argv)
int argc;char*argv[];
{
register int i,j,k,n;
/*2:*/
#line 37 "./garsia-wachs.w"

n= argc-2;
if(n<0){
fprintf(stderr,"Usage: %s wt0 ... wtn\n",argv[0]);exit(0);
}
if(n+n>=size){
fprintf(stderr,"Recompile me with a larger tree size!\n");exit(0);
}
for(j= 0;j<=n;j++){
if(sscanf(argv[j+1],"%d",&m)!=1){
fprintf(stderr,"Couldn't read wt%d!\n",j);exit(0);
}
w[j]= m;l[j]= r[j]= -1;
}

/*:2*/
#line 31 "./garsia-wachs.w"
;
/*3:*/
#line 52 "./garsia-wachs.w"

printf("Phase I:\n");
m= n;
t= 1;
q[0]= 1000000000;
q[1]= w[0];v[1]= 0;
for(k= 1;k<=n;k++){
while(q[t-1]<=w[k])combine(t);
t++;q[t]= w[k];v[t]= k;
for(j= 1;j<=t;j++)printf("%d ",q[j]);printf("\n");
}
while(t> 1)combine(t);

/*:3*/
#line 32 "./garsia-wachs.w"
;
/*5:*/
#line 89 "./garsia-wachs.w"

printf("Phase II:\n");
mark(v[1],0);

/*:5*/
#line 33 "./garsia-wachs.w"
;
/*7:*/
#line 106 "./garsia-wachs.w"

printf("Phase III:\n");
t= 0;m= 2*n;
build(1);

/*:7*/
#line 34 "./garsia-wachs.w"
;
}

/*:1*/
