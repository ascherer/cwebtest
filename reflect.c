#define swap(j) {int tmp= a[j];a[j]= a[j+1];a[j+1]= tmp;}
#define npairs 120
#define ncycle 240 \

#define abort(s) {fprintf(stderr,s) ;exit(1) ;}
/*1:*/
#line 37 "reflect.w"

#include <stdio.h> 

/*:1*//*3:*/
#line 66 "reflect.w"

/*2:*/
#line 50 "reflect.w"

int n;
int a[10];
int k;
int j;
int l;
int c[npairs];
int t[npairs];
int i,ii,iii;
int bn,cn,dn,en;
int smin,smax;
float stot;

/*:2*//*8:*/
#line 148 "reflect.w"

int b[ncycle];
int r,rr;
int d[npairs];
int rrr;

/*:8*//*13:*/
#line 223 "reflect.w"

int x[ncycle];
int m;
int y[npairs];
int jj;
int s,ss;
int e[npairs];
int rep;
int ref;

/*:13*/
#line 67 "reflect.w"

main(argc,argv)
int argc;
char**argv;
{
if(argc!=2)
abort("Usage: reflect n\n");
if(sscanf(argv[1],"%d",&n)!=1||n<2||n> 10)
abort("n should be in the range 2..10!\n");
/*4:*/
#line 82 "reflect.w"

for(j= 1;j<=n;j++)a[j]= n+1-j;
k= n*(n-1);k/= 2;
c[0]= 0;
l= 1;
j= n;
bn= cn= dn= en= smax= 0;
stot= 0.0;
smin= 1000000000;

/*:4*/
#line 76 "reflect.w"
;
/*5:*/
#line 92 "reflect.w"

moveleft:j--;
loop:if(j==0){
if(k==0)/*7:*/
#line 119 "reflect.w"

{
#ifdef DEBUG
for(i= 1;i<l;i++)
putchar('0'+c[i]);
putchar(':');
#endif
#ifndef NOPRINT
for(i= 1;i<l;i++)
if(c[i])putchar('0'-1+t[i]);
#endif
/*9:*/
#line 154 "reflect.w"

for(rr= 0;rr<n;rr++)b[rr]= rr+1;
for(i= 1;i<l;i++)
if(c[i]){
b[rr]= d[rr]= t[i];
rr++;
}
d[rr]= 1;
rrr= rr;
r= 0;
while(1){
/*10:*/
#line 169 "reflect.w"

j= n-b[r++];
for(i= rr++;b[i-1]<j;i--)
b[i]= b[i-1];
b[i]= j+1;

/*:10*/
#line 165 "reflect.w"
;
if(b[r]==1)/*11:*/
#line 175 "reflect.w"

{
b[rr]= 0;
for(i= r+n;b[i]==d[i-r];i++);
if(b[i]<d[i-r]){
if(i==rr){
#ifndef NOPRINT
putchar('*');
#endif
cn++;
/*12:*/
#line 198 "reflect.w"

/*14:*/
#line 237 "reflect.w"

rr= i-r;
for(i= n;i<rr;i++)b[i]= d[i];
for(;i<rr+rr;i++)b[i]= n+1-b[i-rr];

/*:14*/
#line 199 "reflect.w"
;
/*15:*/
#line 245 "reflect.w"

for(i= 0;i<rrr;i++)x[rrr-1-i]= n+1-b[i];
s= 0;ss= rrr;
while(x[s]> 1)/*19:*/
#line 303 "reflect.w"

{
j= n-x[s++];
for(i= ss++;x[i-1]<j;i--)
x[i]= x[i-1];
x[i]= j+1;
}

/*:19*/
#line 248 "reflect.w"
;
for(i= s+n;i<ss;i++)e[i-s]= x[i];
e[rrr]= 1;
while(1){
x[ss]= 0;
for(i= s+n;x[i]==d[i-s];i++);
if(i==ss)break;
if(x[i]<d[i-s]){
#ifndef NOPRINT
putchar('>');
for(i= s+n;i<ss;i++)putchar(x[i]+'0'-1);
#endif
goto done;
}
do/*19:*/
#line 303 "reflect.w"

{
j= n-x[s++];
for(i= ss++;x[i-1]<j;i--)
x[i]= x[i-1];
x[i]= j+1;
}

/*:19*/
#line 262 "reflect.w"

while(x[s]> 1);
x[ss]= 0;
for(i= s+n;x[i]==e[i-s];i++);
if(i==ss)break;
}

/*:15*/
#line 201 "reflect.w"
;
/*22:*/
#line 373 "reflect.w"

dn++;
rr= rrr+rrr;
for(i= 0;i<rr;i++)x[i]= 1;
for(j= 2;j<=n;j++)
/*23:*/
#line 393 "reflect.w"

{int acc= 0;
int p;
ii= rr;
for(i= 0;i<rr;i++){register int delta= j-b[i];
if(delta==0){
x[ii]= acc;
ii= i;
acc= p;
}else if(delta==1){
p= x[i];
acc+= p;
}
}
x[ii]= acc+x[rr];
}

/*:23*/
#line 378 "reflect.w"
;
{register int score= 0;
for(i= 0;i<rr;i++)
if(b[i]==n)score+= x[i];
stot+= (float)score;
if(score> smax)smax= score;
if(score<smin)smin= score;
#ifndef NOPRINT
printf(" [%d]",score);
#endif
}

/*:22*/
#line 202 "reflect.w"
;
for(r= 0;r<rrr;r++){
/*20:*/
#line 317 "reflect.w"

/*24:*/
#line 410 "reflect.w"

#ifdef DEBUG
printf("\n>");
for(m= r;m<r+rrr;m++)putchar(b[m]+'0'-1);
#endif

/*:24*/
#line 318 "reflect.w"
;
s= 0;ss= rrr;
iii= jj= 0;
x[0]= m= rep= b[r];
rr= r+rrr;
for(i= r+1;i<rr;i++){
j= b[i]-1;
/*21:*/
#line 360 "reflect.w"

if(j> m)y[jj++]= j;
else{
if(j==m)m++;
for(ii= ++iii;x[ii-1]<j;ii--)
x[ii]= x[ii-1];
x[ii]= j+1;
}

/*:21*/
#line 325 "reflect.w"
;
}
for(i= 0;iii<rrr-1;i++){
j= n-1-y[i];
/*21:*/
#line 360 "reflect.w"

if(j> m)y[jj++]= j;
else{
if(j==m)m++;
for(ii= ++iii;x[ii-1]<j;ii--)
x[ii]= x[ii-1];
x[ii]= j+1;
}

/*:21*/
#line 329 "reflect.w"
;
}
/*25:*/
#line 416 "reflect.w"

#ifdef DEBUG
printf("\n  ");
for(m= s;m<ss;m++)putchar(x[m]+'0'-1);
#endif/*:25*/
#line 331 "reflect.w"
;
while(rep--){
m= 0;
for(i= 0;x[i]!=1;i++){
x[i]--;
if(x[i]> m)m= x[i];
}
iii= i-1;
jj= 0;
for(j= n-1;j>=0;j--)
if(j==0&&i==0){
x[0]= m= 1;
iii= 0;
}else/*21:*/
#line 360 "reflect.w"

if(j> m)y[jj++]= j;
else{
if(j==m)m++;
for(ii= ++iii;x[ii-1]<j;ii--)
x[ii]= x[ii-1];
x[ii]= j+1;
}

/*:21*/
#line 344 "reflect.w"
;
for(i+= n;i<rrr;i++){
j= x[i];
/*21:*/
#line 360 "reflect.w"

if(j> m)y[jj++]= j;
else{
if(j==m)m++;
for(ii= ++iii;x[ii-1]<j;ii--)
x[ii]= x[ii-1];
x[ii]= j+1;
}

/*:21*/
#line 347 "reflect.w"
;
}
for(i= 0;iii<ss-1;i++){
j= n-1-y[i];
/*21:*/
#line 360 "reflect.w"

if(j> m)y[jj++]= j;
else{
if(j==m)m++;
for(ii= ++iii;x[ii-1]<j;ii--)
x[ii]= x[ii-1];
x[ii]= j+1;
}

/*:21*/
#line 351 "reflect.w"
;
}
/*25:*/
#line 416 "reflect.w"

#ifdef DEBUG
printf("\n  ");
for(m= s;m<ss;m++)putchar(x[m]+'0'-1);
#endif/*:25*/
#line 353 "reflect.w"
;
}

/*:20*/
#line 204 "reflect.w"
;
for(ref= 0;ref<2;ref++){
if(ref==0)
for(i= 0;i<rrr;i++)y[i]= x[i];
else/*16:*/
#line 269 "reflect.w"

{
for(i= 0;i<rrr;i++)x[rrr-1-i]= n+1-y[i];
s= 0;ss= rrr;
while(x[s]> 1)/*19:*/
#line 303 "reflect.w"

{
j= n-x[s++];
for(i= ss++;x[i-1]<j;i--)
x[i]= x[i-1];
x[i]= j+1;
}

/*:19*/
#line 273 "reflect.w"
;
#ifdef DEBUG
putchar('/');
/*25:*/
#line 416 "reflect.w"

#ifdef DEBUG
printf("\n  ");
for(m= s;m<ss;m++)putchar(x[m]+'0'-1);
#endif/*:25*/
#line 276 "reflect.w"
;
#endif
}

/*:16*/
#line 208 "reflect.w"
;
/*17:*/
#line 281 "reflect.w"

for(i= s+n;i<ss;i++)e[i-s]= x[i];
while(1){
/*18:*/
#line 297 "reflect.w"

x[ss]= 0;
for(i= s+n;x[i]==d[i-s];i++);
if(i==ss)goto okay;
if(x[i]<d[i-s])goto done;

/*:18*/
#line 285 "reflect.w"
;
do/*19:*/
#line 303 "reflect.w"

{
j= n-x[s++];
for(i= ss++;x[i-1]<j;i--)
x[i]= x[i-1];
x[i]= j+1;
}

/*:19*/
#line 286 "reflect.w"

while(x[s]> 1);
/*25:*/
#line 416 "reflect.w"

#ifdef DEBUG
printf("\n  ");
for(m= s;m<ss;m++)putchar(x[m]+'0'-1);
#endif/*:25*/
#line 288 "reflect.w"
;
x[ss]= 0;
for(i= s+n;x[i]==e[i-s];i++);
if(i==ss)
break;
}
okay:;

/*:17*/
#line 210 "reflect.w"
;
}
}
#ifndef NOPRINT
putchar('#');
#endif
en++;
done:;

/*:12*/
#line 185 "reflect.w"
;
}
break;
}
}

/*:11*/
#line 166 "reflect.w"
;
}

/*:9*/
#line 130 "reflect.w"
;
#ifndef NOPRINT
putchar('\n');
#endif
bn++;
}

/*:7*/
#line 95 "reflect.w"
;
/*6:*/
#line 105 "reflect.w"

while(c[--l]){
j= t[l];
swap(j);
k++;
}
if(l==0)goto finished;
j= t[l];
c[l++]= 1;
swap(j);
k--;
if(++j==n)j--;
goto loop;

/*:6*/
#line 97 "reflect.w"
;
}
if(a[j]<a[j+1])goto moveleft;
t[l]= j;
c[l++]= 0;
goto moveleft;
finished:;

/*:5*/
#line 77 "reflect.w"
;
printf("B=%d, C=%d, D=%d, E=%d\n",bn,cn,dn,en);
printf("scores min=%d, max=%d, mean=%.1f\n",smin,smax,stot/(float)dn);
}

/*:3*/
