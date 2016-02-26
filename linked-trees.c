#define maxn 100
#define abort(m) {fprintf(stderr,"%s!\n",m) ;exit(j) ;} \

/*1:*/
#line 21 "./linked-trees.w"

#include <stdio.h> 
int h[maxn];
int l[maxn][maxn];
int count;

main(int argc,char*argv[])
{
register int j,k,n,r,x,y;
/*2:*/
#line 46 "./linked-trees.w"

n= argc-1;
if(n==0){
fprintf(stderr,"Usage: %s t1 t2 ... tn\n",argv[0]);
exit(0);
}
if(n>=maxn)abort("I can't handle that many degrees");
for(j= 1;j<=n;j++){
if(sscanf(argv[j],"%d",&h[j])!=1)abort("unreadable degree");
h[j]--;
if(h[j]<0)abort("Each degree must be positive");
if(h[j]>=maxn)abort("Degree is too large");
}


/*:2*/
#line 30 "./linked-trees.w"
;
for(j= 1;j<n;j++)l[j][h[j]]= j+1;
while(1){
/*3:*/
#line 63 "./linked-trees.w"

count++;
printf("%d:",count);
for(j= 1;j<=n;j++){
for(k= h[j];k>=0;k--)printf(" %d",l[j][k]);
if(j<n)printf(";");
}
printf("\n");

/*:3*/
#line 33 "./linked-trees.w"
;
for(j= 1,x= l[1][0];x==j+1;j= x,x= l[j][0])
l[j][0]= 0,l[j][h[j]]= x;
if(j==n)break;
for(r= 1;l[j][r]==0;r++);
for(k= 0,y= l[j][r];l[y][0];k= y,y= l[y][0]);
if(k)l[k][0]= 0;else l[j][r]= 0;
l[j][0]= 0,l[j][r-1]= y,l[y][0]= x;
}
}

/*:1*/
