#define n 17
#define nn (n+n)  \

#define check(s) {if(d[++p]!=s)  \
fprintf(stderr,"Rejection at position %d of case %d!\n",p,serial) ;} \

/*1:*/
#line 16 "francon.w"

#include <stdio.h> 
int d[nn+1];
int l[n+1],r[n+1];
int h[nn+1],q[n+1],qm[n+1];

int serial;
int count[10];

/*5:*/
#line 71 "francon.w"

int strahler(int x)
{
register int sl,sr;
if(l[x])sl= strahler(l[x]);else sl= 0;
if(r[x])sr= strahler(r[x]);else sr= 0;
return(sl> sr?sl:sl<sr?sr:sl+1);
}

/*:5*/
#line 25 "francon.w"


main()
{
register int i,j,k,jj,kk,m,p,s;
printf("Checking binary trees with %d nodes...\n",n);
/*2:*/
#line 45 "francon.w"

for(k= 0;k<nn;k+= 2)d[k]= +1,d[k+1]= -1;
d[nn]= -1,i= nn-2;

/*:2*/
#line 31 "francon.w"
;
while(1){
/*7:*/
#line 122 "francon.w"

h[1]= m= 1,k= 2,p= 0;
while(1){
while(h[k])k+= 2;
kk= h[k>>1];
if(d[++p]> 0)h[k]= l[kk]= ++m;else l[kk]= 0;
if(d[++p]> 0)h[k+1]= r[kk]= ++m;else r[kk]= 0;
if(h[k]){
if(h[k+1])continue;
kk= k;
}else if(h[k+1])kk= k+1;
else{
h[k>>1]= 0,kk= (k>>1)^1,k= kk&-2;
if(k==0)break;
}
/*8:*/
#line 144 "francon.w"

j= 0,jj= 1,q[0]= kk,qm[0]= 1;
while(j<jj){
kk= q[j];
h[((kk>>1)&-qm[j])+(kk&(qm[j]-1))]= h[kk];
if(h[kk+kk])q[jj]= kk+kk,q[jj+1]= kk+kk+1,qm[jj]= qm[jj+1]= qm[j]<<1,jj+= 2;
else h[kk]= 0;
j++;
}

/*:8*/
#line 137 "francon.w"
;
}

/*:7*/
#line 33 "francon.w"
;
/*4:*/
#line 62 "francon.w"

for(s= j= k= 1;k<nn-1;j+= d[k],k++)
if(j>=((1<<s)-1))s++;
s--;
count[s]++,serial++;
if(strahler(1)!=s){
fprintf(stderr,"I goofed on case %d.\n",serial);
}

/*:4*/
#line 34 "francon.w"
;
/*9:*/
#line 161 "francon.w"

h[1]= 1,k= 2,p= 0;
while(1){
while(h[k])k+= 2;
kk= h[k>>1];
if(l[kk]){
h[k]= l[kk];check(+1);
}else check(-1);
if(r[kk]){
h[k+1]= r[kk];check(+1);
}else check(-1);
if(h[k]){
if(h[k+1])continue;
kk= k;
}else if(h[k+1])kk= k+1;
else{
h[k>>1]= 0,kk= (k>>1)^1,k= kk&-2;
if(k==0)break;
}
/*8:*/
#line 144 "francon.w"

j= 0,jj= 1,q[0]= kk,qm[0]= 1;
while(j<jj){
kk= q[j];
h[((kk>>1)&-qm[j])+(kk&(qm[j]-1))]= h[kk];
if(h[kk+kk])q[jj]= kk+kk,q[jj+1]= kk+kk+1,qm[jj]= qm[jj+1]= qm[j]<<1,jj+= 2;
else h[kk]= 0;
j++;
}

/*:8*/
#line 180 "francon.w"
;
}



/*:9*/
#line 35 "francon.w"
;
/*3:*/
#line 51 "francon.w"

d[i]= -1;
if(d[i-1]<0)d[i-1]= 1,i--;
else{
for(j= i-1,k= nn-2;d[j]> 0;j--,k-= 2){
d[j]= -1,d[k]= +1;
if(j==0)goto done;
}
d[j]= +1,i= nn-2;
}

/*:3*/
#line 36 "francon.w"
;
}
done:for(s= 1;count[s];s++)
printf("Altogether %d cases with Strahler number %d.\n",count[s],s);
}

/*:1*/
