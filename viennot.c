#define n 17
#define nn (n+n)  \

#define check(s) {y-= d[--p]; \
if(d[p]!=s)  \
fprintf(stderr,"Rejection at position %d, case %d!\n",p,serial) ;} \

/*2:*/
#line 66 "viennot.w"

#include <stdio.h> 
int d[nn+1];
int x[nn+1];
char ring[n][n+3],ringcount[n];
int wall[n];
int serial;
int count[10];

main()
{
register int i,j,k,m,p,w,ww,y,mode;
printf("Checking Kepler towers with %d bricks...\n",n);
/*3:*/
#line 95 "viennot.w"

for(k= 0;k<nn;k+= 2)d[k]= +1,d[k+1]= -1;
d[nn]= -1,i= nn-2;

/*:3*/
#line 79 "viennot.w"
;
while(1){
/*7:*/
#line 230 "viennot.w"

w= 1,ww= 2,m= 0;
ring[0][1]= ring[0][3]= 1;
for(y= p= 0;y!=-ww;){
if(y==0)mode= -d[++p],y-= mode;
else if(y==ww)/*8:*/
#line 243 "viennot.w"

{
wall[w++]= ++m;
ww+= ww;
for(k= 0;k<=ww;k+= 2)ring[m][k+1]= 1;
y= 0;
}

/*:8*/
#line 235 "viennot.w"

else{
y+= d[++p];
if(d[p]==mode)/*9:*/
#line 251 "viennot.w"

{
k= y+(mode<0?1:ww);
for(j= m;ring[j][k-1]==0&&ring[j][k]==0&&ring[j][k+1]==0;j--);
if(j==m)m++;
ring[j+1][k]= 1;
if(k==1)ring[j+1][ww+1]= 1;
else if(k==ww)ring[j+1][0]= 1;
ringcount[j+1]++;
}

/*:9*/
#line 238 "viennot.w"
;
}
}
wall[w]= m+1;

/*:7*/
#line 81 "viennot.w"
;
/*5:*/
#line 112 "viennot.w"

for(m= j= k= 1;k<nn-1;j+= d[k],k++)
if(j>=((1<<m)-1))m++;
m--;
count[m]++,serial++;
if(w!=m){
fprintf(stderr,"I goofed on case %d.\n",serial);
}

/*:5*/
#line 82 "viennot.w"
;
/*10:*/
#line 271 "viennot.w"

m= wall[w]-1,mode= +1;
for(y= -ww+1,p= nn;p;){
if(y==1-ww||y==ww-1)check(-mode)
else/*11:*/
#line 278 "viennot.w"

{
look:k= y+(mode<0?1:ww);
for(j= m;ring[j][k]==0;j--)
if(ring[j][k-1]||ring[j][k+1])goto notfound;
if(j==wall[w-1])goto notfound;
ring[j][k]= 0;
if(k==1)ring[j][ww+1]= 0;
else if(k==ww)ring[j][0]= 0;
ringcount[j]--;
if(ringcount[j]==0)m--;
check(mode);continue;
notfound:if(y==0){
if(m==wall[w-1])/*12:*/
#line 297 "viennot.w"

{
for(k= 0;k<=ww;k+= 2)ring[m][k+1]= 0;
m--,ww>>= 1,w--;
y= ww,mode= -1;
}

/*:12*/
#line 291 "viennot.w"

else/*13:*/
#line 310 "viennot.w"

{
mode= -mode;goto look;
}

/*:13*/
#line 292 "viennot.w"
;
}
check(-mode);
}

/*:11*/
#line 275 "viennot.w"
;
}

/*:10*/
#line 83 "viennot.w"
;
/*4:*/
#line 101 "viennot.w"

d[i]= -1;
if(d[i-1]<0)d[i-1]= 1,i--;
else{
for(j= i-1,k= nn-2;d[j]> 0;j--,k-= 2){
d[j]= -1,d[k]= +1;
if(j==0)goto done;
}
d[j]= +1,i= nn-2;
}

/*:4*/
#line 84 "viennot.w"
;
}
done:for(w= 1;count[w];w++)
printf("Altogether %d cases with %d wall%s.\n",
count[w],w,w> 1?"s":"");
}


/*:2*/
