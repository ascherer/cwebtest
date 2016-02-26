#define n 17 \

/*2:*/
#line 65 "./zeilberger.w"

#include <stdio.h> 
int l[n+2],r[n+1];
int ll[n+1],rr[n+1];
int lll[n+1],rrr[n+1];
int q[n+1],s[n+1];
int serial;
int count[10];

/*5:*/
#line 130 "./zeilberger.w"

void label(register int x)
{
register int p,qr;
if(l[x])label(l[x]);
if(r[x])label(r[x]);
p= q[l[x]]+(s[l[x]]> 1);
qr= q[r[x]];
if(p> qr)q[x]= p,s[x]= 1;
else q[x]= qr,s[x]= s[r[x]]+(p==qr);
}

/*:5*//*8:*/
#line 164 "./zeilberger.w"

void zeil(register int x)
{
register int k,ql,qr,p,y,yy,z,zz,ss;
k= q[x],ql= q[ll[x]],qr= q[rr[x]];
if(ql==k)/*10:*/
#line 216 "./zeilberger.w"

{
if(qr==k)ss= s[rr[x]]+1;else ss= 1;
for(y= ll[x];q[y]==k;yy= y,y= rr[yy])s[y]= ss;
rr[yy]= rr[x],rr[x]= y;
}

/*:10*/
#line 169 "./zeilberger.w"

else if(qr<k-1)/*9:*/
#line 194 "./zeilberger.w"

{
for(y= ll[x],ss= s[y];s[y]==ss;yy= y,y= rr[yy])s[y]= 1;

for(z= yy,ss--;s[rr[y]]==ss&&q[rr[y]]==ql;
yy= z,z= y,y= rr[z],rr[z]= yy);


if(z==yy)rr[z]= 0;
else for(zz= 0;rr[z]!=zz;yy= zz,zz= z,z= rr[zz],rr[zz]= yy){
p= q[ll[z]]+(s[ll[z]]> 1);
if(p> q[zz])q[z]= p,s[z]= 1;
else q[z]= q[zz],s[z]= s[zz]+(p==q[zz]);
}
for(zz= x,z= rr[zz];z;zz= z,z= rr[zz])q[z]= ql,s[z]= ss;
rr[zz]= y;
}

/*:9*/
#line 170 "./zeilberger.w"
;

if(ll[x])zeil(ll[x]);
if(rr[x])zeil(rr[x]);
}

/*:8*//*12:*/
#line 231 "./zeilberger.w"

int strahler(register int x)
{
register int sl,sr,s;
if(ll[x])sl= strahler(ll[x]);else sl= 0;
if(rr[x])sr= strahler(rr[x]);else sr= 0;
s= (sl> sr?sl:sl<sr?sr:sl+1);
if(q[x]!=s)
fprintf(stderr,"I goofed at binary tree node %d, case %d.\n",x,serial);
return s;
}

/*:12*//*15:*/
#line 273 "./zeilberger.w"

void unzeil(register int x)
{
register int ql,qr,p,y,yy,z,zz,ss;
if(rrr[x])unzeil(rrr[x]);
if(lll[x])unzeil(lll[x]);
ql= q[lll[x]],qr= q[rrr[x]];
if(ql> qr)/*16:*/
#line 288 "./zeilberger.w"

{
if(s[lll[x]]> 1){
for(y= lll[x];s[rrr[y]]==s[y];y= rrr[y])s[y]= 1;
s[y]= 1;
}else for(y= lll[x];q[rrr[y]]==q[y];y= rrr[y]);
yy= rrr[y],rrr[y]= rrr[x],rrr[x]= yy;
qr= q[yy];
}

/*:16*/
#line 280 "./zeilberger.w"

else if(ql==qr&&s[lll[x]]==1)/*17:*/
#line 303 "./zeilberger.w"

{
for(z= rrr[x],zz= x;s[rrr[z]]==s[z]&&q[rrr[z]]==q[z];
yy= zz,zz= z,z= rrr[zz],rrr[zz]= yy);
if(zz==x)yy= rrr[x]= 0;
else for(yy= 0;zz!=x;y= zz,zz= rrr[y],rrr[y]= yy,yy= y){
p= q[lll[zz]]+(s[lll[zz]]> 1);
if(p> q[yy])q[zz]= p,s[zz]= 1;
else q[zz]= q[yy],s[zz]= s[yy]= (p==q[yy]);
}
qr= q[yy],ss= s[z]+1;
for(yy= lll[x],y= rrr[yy];y;yy= y,y= rrr[yy]){
s[yy]= ss;
if(q[yy]==ql)ss--;
}
s[yy]= ss,rrr[yy]= z;
}

/*:17*/
#line 281 "./zeilberger.w"
;

p= ql+(s[lll[x]]> 1);
if(p> qr)q[x]= p,s[x]= 1;
else q[x]= qr,s[x]= s[rrr[x]]+(p==qr);
}

/*:15*/
#line 74 "./zeilberger.w"


main()
{
register int j,k,y,p;
printf("Checking all forests with %d nodes...\n",n);
q[0]= 0,s[0]= 1000000;
for(k= 1;k<n;k++)l[k]= k+1,r[k]= 0;
l[n]= r[n]= 0;
l[n+1]= 1;
while(1){
/*6:*/
#line 146 "./zeilberger.w"

for(k= 1;k<=n;k++)ll[k]= l[k],rr[k]= r[k];
label(1);
zeil(1);

/*:6*/
#line 85 "./zeilberger.w"
;
/*13:*/
#line 243 "./zeilberger.w"

count[strahler(1)]++;
serial++;

/*:13*/
#line 86 "./zeilberger.w"
;
/*14:*/
#line 263 "./zeilberger.w"

for(k= 1;k<=n;k++)lll[k]= ll[k],rrr[k]= rr[k],q[k]= s[k]= 0;

unzeil(1);
for(k= 1;k<=n;k++)if(lll[k]!=l[k]||rrr[k]!=r[k])
fprintf(stderr,"Rejection at node %d of case %d!\n",k,serial);

/*:14*/
#line 87 "./zeilberger.w"
;
/*3:*/
#line 94 "./zeilberger.w"

for(j= 1;!l[j];j++)r[j]= 0,l[j]= j+1;
if(j> n)break;
for(k= 0,y= l[j];r[y];k= y,y= r[y]);
if(k> 0)r[k]= 0;else l[j]= 0;
r[y]= r[j],r[j]= y;

/*:3*/
#line 88 "./zeilberger.w"

}
for(p= 1;count[p];p++)
printf("Altogether %d cases with pruning order %d.\n",count[p],p);
}

/*:2*/
