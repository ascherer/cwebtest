#define maxm (1<<15) 
#define maxd (1<<8) 
#define maxp 100
#define bufsize 1024
#define verbose_echo (1<<0) 
#define verbose_folds (1<<1) 
#define verbose_dirs (1<<2) 
#define metapost_name "/tmp/dragon-calc.mp" \

#define must_see(c) while(*p==' ') p++;if(*p++!=c) goto bad_command
#define check_tile(v) if(((v.x+v.y) &0x1) ==0) { \
printf("Bad tile (%ld,%ld)!\n",v.x,v.y) ;break;} \

#define typ(w) (((w.x&0x1) +((w.x+w.y) &0x2) +3) &0x3)  \

#define norm(z) (z.x*z.x+z.y*z.y)  \

#define classof(w) congclass[w.y>>1][w.x] \

/*2:*/
#line 101 "dragon-calc.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
int vbose;
FILE*infile,*outfile;
char buf[bufsize];
char dir[maxm],fold[maxm];
int s;
typedef struct pair_struct{
long x,y;
}pair;
pair e,u,v,w,z,uu,vv;
pair ipower[4]= {{1,0},{0,1},{-1,0},{0,-1}};
pair sqrt8i= {2,2};
pair poly[maxm];
int congclass[maxd][4*maxm];
int fill[maxm];
pair cyc[maxp];
int cycptr;
int count;
/*23:*/
#line 355 "dragon-calc.w"

pair sum(pair a,pair b){
pair res;
res.x= a.x+b.x;
res.y= a.y+b.y;
return res;
}

pair diff(pair a,pair b){
pair res;
res.x= a.x-b.x;
res.y= a.y-b.y;
return res;
}

pair prod(pair a,pair b){
pair res;
res.x= a.x*b.x-a.y*b.y;
res.y= a.x*b.y+a.y*b.x;
return res;
}

/*:23*//*24:*/
#line 381 "dragon-calc.w"

pair quot(pair a,pair b){
pair res;
long n= norm(b);
res.x= (a.x*b.x+a.y*b.y)/n;
res.y= (-a.x*b.y+a.y*b.x)/n;
return res;
}

/*:24*/
#line 122 "dragon-calc.w"
;

main(){
register int c,d,j,k,neg;
register char*p,*q;
long qq;
int including= 0;
/*3:*/
#line 141 "dragon-calc.w"

s= 1,z.x= 1,z.y= 0;
/*4:*/
#line 149 "dragon-calc.w"

poly[0].x= 0,congclass[0][0]= -1;fill[0]= -1;

/*:4*/
#line 143 "dragon-calc.w"
;

/*:3*/
#line 129 "dragon-calc.w"
;
while(1){
if(including)/*6:*/
#line 158 "dragon-calc.w"

{
if(!fgets(buf,bufsize,infile)){
including= 0;
continue;
}
}

/*:6*/
#line 131 "dragon-calc.w"

else/*5:*/
#line 152 "dragon-calc.w"

{
printf("> ");fflush(stdout);
fgets(buf,bufsize,stdin);
}

/*:5*/
#line 132 "dragon-calc.w"
;
/*7:*/
#line 166 "dragon-calc.w"

for(p= buf;*p==' ';p++);
if(*p=='\n'){
if(!including)printf("Please type a command, or say q to quit.\n");
continue;
}
if(including&&(vbose&verbose_echo))printf("%s",buf);
switch(*p){
case'q':goto done;
case'i':if(!including){
for(p= buf+1;*p==' ';p++);
for(q= p+1;*q!='\n';q++);
*q= '\0';
if(infile= fopen(p,"r"))including= 1;
else printf("Sorry --- I couldn't open file `%s' for reading!\n",p);
}else printf("Sorry; you can't include one file in another.\n");
case'%':continue;
case'v':p++;
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 184 "dragon-calc.w"
;
vbose= k;break;

/*9:*/
#line 199 "dragon-calc.w"

case'p':for(s= 0,z.x= z.y= 0,p++;*p>='0'&&*p<='3';s++,p++){
if(s==0&&*p!='0'){
printf("A path must start in direction 0!\n");
goto bad_path;
}else if((*p^s)&0x1){
printf("Direction %c in this path has bad parity!\n",*p);
bad_path:/*3:*/
#line 141 "dragon-calc.w"

s= 1,z.x= 1,z.y= 0;
/*4:*/
#line 149 "dragon-calc.w"

poly[0].x= 0,congclass[0][0]= -1;fill[0]= -1;

/*:4*/
#line 143 "dragon-calc.w"
;

/*:3*/
#line 206 "dragon-calc.w"
;
break;
}
/*11:*/
#line 229 "dragon-calc.w"

if(s<maxm)dir[s]= *p-'0';
switch(*p){
case'0':z.x++;break;
case'1':z.y++;break;
case'2':z.x--;break;
case'3':z.y--;break;
}

/*:11*/
#line 209 "dragon-calc.w"
;
}
if(s> maxm){
too_long:
printf("Sorry, I can't deal with paths longer than %d; recompile me!\n",
maxm);
goto bad_path;
}
/*13:*/
#line 248 "dragon-calc.w"

for(j= k= 0;j<s-1;j++)
fold[j]= ((dir[j+1]-dir[j])&0x2?'U':'D');
fold[j]= '\0';

/*:13*/
#line 217 "dragon-calc.w"
;
finish_dirs:/*10:*/
#line 226 "dragon-calc.w"

if(vbose&verbose_folds)printf(" %s,",fold);

/*:10*/
#line 218 "dragon-calc.w"
;
print_path_params:
printf(" s=%d, z=",s);
/*12:*/
#line 238 "dragon-calc.w"

if(z.x)printf("%ld",z.x);
else if(!z.y)printf("0");
if(z.y){
if(z.y==1)printf("+i");
else if(z.y> 0)printf("+%ldi",z.y);
else if(z.y==-1)printf("-i");
else printf("-%ldi",-z.y);
}

/*:12*/
#line 221 "dragon-calc.w"
;
printf("\n");
/*4:*/
#line 149 "dragon-calc.w"

poly[0].x= 0,congclass[0][0]= -1;fill[0]= -1;

/*:4*/
#line 223 "dragon-calc.w"
;
break;

/*:9*//*14:*/
#line 253 "dragon-calc.w"

case'D':case'U':for(s= 0;*p=='D'||*p=='U';s++,p++)if(s<maxm)fold[s]= *p;
if(++s> maxm)goto too_long;
finish_folds:/*16:*/
#line 266 "dragon-calc.w"

for(j= k= 0,z.x= z.y= 0;k<s;k++){
dir[k]= j;
switch(j){
case 0:z.x++;break;
case 1:z.y++;break;
case 2:z.x--;break;
case 3:z.y--;break;
}
j= (j+(fold[k]=='D'?1:-1))&0x3;
}

/*:16*/
#line 256 "dragon-calc.w"
;
/*15:*/
#line 260 "dragon-calc.w"

if(vbose&verbose_dirs){
printf(" ");
for(k= 0;k<s;k++)printf("%d",dir[k]);
}

/*:15*/
#line 257 "dragon-calc.w"
;
goto print_path_params;

/*:14*//*17:*/
#line 278 "dragon-calc.w"

case'*':p++;
if(*p=='D'||*p=='U')/*18:*/
#line 287 "dragon-calc.w"

{
for(k= j= s-1;*p=='D'||*p=='U';p++){
if(k+s>=maxm)goto too_long;
fold[k++]= *p;
if(j)for(;j;j--)fold[k++]= 'U'+'D'-fold[j-1];
else for(;j<s-1;j++)fold[k++]= fold[j];
}
fold[k]= '\0',s= k+1;
/*10:*/
#line 226 "dragon-calc.w"

if(vbose&verbose_folds)printf(" %s,",fold);

/*:10*/
#line 296 "dragon-calc.w"
;
goto finish_folds;
}

/*:18*/
#line 280 "dragon-calc.w"

else if(*p=='0')/*19:*/
#line 300 "dragon-calc.w"

{
for(k= j= s-1,p++;*p>='0'&&*p<='3'&&((*p^*(p-1))&0x1);p++){
if(k+s>=maxm)goto too_long;
fold[k++]= (*p-*(p-1))&0x2?'U':'D';
if(j)for(;j;j--)fold[k++]= 'U'+'D'-fold[j-1];
else for(;j<s-1;j++)fold[k++]= fold[j];
}
fold[k]= '\0',s= k+1;
/*16:*/
#line 266 "dragon-calc.w"

for(j= k= 0,z.x= z.y= 0;k<s;k++){
dir[k]= j;
switch(j){
case 0:z.x++;break;
case 1:z.y++;break;
case 2:z.x--;break;
case 3:z.y--;break;
}
j= (j+(fold[k]=='D'?1:-1))&0x3;
}

/*:16*/
#line 309 "dragon-calc.w"
;
/*15:*/
#line 260 "dragon-calc.w"

if(vbose&verbose_dirs){
printf(" ");
for(k= 0;k<s;k++)printf("%d",dir[k]);
}

/*:15*/
#line 310 "dragon-calc.w"
;
goto finish_dirs;
}

/*:19*/
#line 281 "dragon-calc.w"

else{
printf("Improper multiplication!\n");
break;
}

/*:17*//*20:*/
#line 318 "dragon-calc.w"

default:/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 319 "dragon-calc.w"
;
v.x= k;
while(*p==' ')p++;
if(*p++!=','){
bad_command:p--;
if(including&&!(vbose&verbose_echo))
printf("Sorry, I don't understand the command %s",buf);
else printf("Sorry, I don't understand that command!\n");
break;
}
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 329 "dragon-calc.w"
;
v.y= k;
check_tile(v);
must_see('*');
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 333 "dragon-calc.w"
;
w.x= k;
must_see(',');
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 336 "dragon-calc.w"
;
w.y= k;
check_tile(w);
/*21:*/
#line 343 "dragon-calc.w"

/*22:*/
#line 349 "dragon-calc.w"

d= typ(w);
e= sum(w,ipower[(2-d)&0x3]);

/*:22*/
#line 344 "dragon-calc.w"
;
u= sum(prod(ipower[(-d)&0x3],v),prod(z,e));

/*:21*/
#line 339 "dragon-calc.w"
;
printf(" %ld,%ld\n",u.x,u.y);
break;

/*:20*//*25:*/
#line 390 "dragon-calc.w"

case'a':/*26:*/
#line 408 "dragon-calc.w"

if(!poly[0].x){
for(k= 0,u.x= u.y= 0;k<s;k++){
v= u;
switch(dir[k]){
case 0:u.x++;break;
case 1:u.y++;break;
case 2:u.x--;break;
case 3:u.y--;break;
}
poly[k]= sum(u,v);
}
}

/*:26*/
#line 391 "dragon-calc.w"
;
p++;
must_see('*');
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 394 "dragon-calc.w"
;
w.x= k;
must_see(',');
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 397 "dragon-calc.w"
;
w.y= k;
check_tile(w);
for(k= 0;k<s;k++){
v= poly[k];
/*21:*/
#line 343 "dragon-calc.w"

/*22:*/
#line 349 "dragon-calc.w"

d= typ(w);
e= sum(w,ipower[(2-d)&0x3]);

/*:22*/
#line 344 "dragon-calc.w"
;
u= sum(prod(ipower[(-d)&0x3],v),prod(z,e));

/*:21*/
#line 402 "dragon-calc.w"
;
printf(" %ld,%ld",u.x,u.y);
}
printf("\n");
break;

/*:25*//*30:*/
#line 486 "dragon-calc.w"

case'c':/*27:*/
#line 438 "dragon-calc.w"

if(congclass[0][0]<0){
/*28:*/
#line 458 "dragon-calc.w"

uu= prod(z,sqrt8i),vv.x= -uu.y,vv.y= uu.x;
if(uu.y<0)uu.x= -uu.x,uu.y= -uu.y;
if(vv.y<0)vv.x= -vv.x,vv.y= -vv.y;
while(uu.y){
while(vv.y>=uu.y)vv= diff(vv,uu);
w= vv,vv= uu,uu= w;
}
if(uu.x<0)uu.x= -uu.x;

/*:28*/
#line 440 "dragon-calc.w"
;
for(j= 0;j<vv.y>>1;j++)for(k= 0;k<uu.x;k++)congclass[j][k]= -1;
for(c= j= 0;j<vv.y>>1;j++)for(k= 0;k<uu.x;k++)if(congclass[j][k]<0){
congclass[j][k]= c;
v.x= k,v.y= 2*j+1-(k&0x1);
for(d= 1;d<4;d++){
w= prod(v,ipower[d]);
/*29:*/
#line 468 "dragon-calc.w"

{
if(w.y<0){
qq= (vv.y-1-w.y)/vv.y;
w.x+= qq*vv.x,w.y+= qq*vv.y;
}else{
qq= w.y/vv.y;
w.x-= qq*vv.x,w.y-= qq*vv.y;
}
if(w.x<0){
qq= (uu.x-1-w.x)/uu.x;
w.x+= qq*uu.x;
}else{
qq= w.x/uu.x;
w.x-= qq*uu.x;
}
}

/*:29*/
#line 447 "dragon-calc.w"
;
classof(w)= c;
}
c++;
}
}

/*:27*/
#line 487 "dragon-calc.w"
;
p++;
while(*p==' ')p++;
if(*p=='\n')/*32:*/
#line 506 "dragon-calc.w"

{
/*26:*/
#line 408 "dragon-calc.w"

if(!poly[0].x){
for(k= 0,u.x= u.y= 0;k<s;k++){
v= u;
switch(dir[k]){
case 0:u.x++;break;
case 1:u.y++;break;
case 2:u.x--;break;
case 3:u.y--;break;
}
poly[k]= sum(u,v);
}
}

/*:26*/
#line 508 "dragon-calc.w"
;
for(k= 0;k<s;k++){
w= poly[k];
/*31:*/
#line 501 "dragon-calc.w"

v= w;
/*29:*/
#line 468 "dragon-calc.w"

{
if(w.y<0){
qq= (vv.y-1-w.y)/vv.y;
w.x+= qq*vv.x,w.y+= qq*vv.y;
}else{
qq= w.y/vv.y;
w.x-= qq*vv.x,w.y-= qq*vv.y;
}
if(w.x<0){
qq= (uu.x-1-w.x)/uu.x;
w.x+= qq*uu.x;
}else{
qq= w.x/uu.x;
w.x-= qq*uu.x;
}
}

/*:29*/
#line 503 "dragon-calc.w"
;
printf(" %ld,%ld is %d_%d\n",v.x,v.y,classof(w),typ(v));

/*:31*/
#line 511 "dragon-calc.w"
;
}
}

/*:32*/
#line 490 "dragon-calc.w"

else{
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 492 "dragon-calc.w"
;
w.x= k;
must_see(',');
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 495 "dragon-calc.w"
;
w.y= k;
/*31:*/
#line 501 "dragon-calc.w"

v= w;
/*29:*/
#line 468 "dragon-calc.w"

{
if(w.y<0){
qq= (vv.y-1-w.y)/vv.y;
w.x+= qq*vv.x,w.y+= qq*vv.y;
}else{
qq= w.y/vv.y;
w.x-= qq*vv.x,w.y-= qq*vv.y;
}
if(w.x<0){
qq= (uu.x-1-w.x)/uu.x;
w.x+= qq*uu.x;
}else{
qq= w.x/uu.x;
w.x-= qq*uu.x;
}
}

/*:29*/
#line 503 "dragon-calc.w"
;
printf(" %ld,%ld is %d_%d\n",v.x,v.y,classof(w),typ(v));

/*:31*/
#line 497 "dragon-calc.w"
;
}
break;

/*:30*//*34:*/
#line 534 "dragon-calc.w"

case'f':case'F':q= p++;
/*33:*/
#line 519 "dragon-calc.w"

if(fill[0]<0&&(norm(z)==s)){
/*26:*/
#line 408 "dragon-calc.w"

if(!poly[0].x){
for(k= 0,u.x= u.y= 0;k<s;k++){
v= u;
switch(dir[k]){
case 0:u.x++;break;
case 1:u.y++;break;
case 2:u.x--;break;
case 3:u.y--;break;
}
poly[k]= sum(u,v);
}
}

/*:26*/
#line 521 "dragon-calc.w"
;
/*27:*/
#line 438 "dragon-calc.w"

if(congclass[0][0]<0){
/*28:*/
#line 458 "dragon-calc.w"

uu= prod(z,sqrt8i),vv.x= -uu.y,vv.y= uu.x;
if(uu.y<0)uu.x= -uu.x,uu.y= -uu.y;
if(vv.y<0)vv.x= -vv.x,vv.y= -vv.y;
while(uu.y){
while(vv.y>=uu.y)vv= diff(vv,uu);
w= vv,vv= uu,uu= w;
}
if(uu.x<0)uu.x= -uu.x;

/*:28*/
#line 440 "dragon-calc.w"
;
for(j= 0;j<vv.y>>1;j++)for(k= 0;k<uu.x;k++)congclass[j][k]= -1;
for(c= j= 0;j<vv.y>>1;j++)for(k= 0;k<uu.x;k++)if(congclass[j][k]<0){
congclass[j][k]= c;
v.x= k,v.y= 2*j+1-(k&0x1);
for(d= 1;d<4;d++){
w= prod(v,ipower[d]);
/*29:*/
#line 468 "dragon-calc.w"

{
if(w.y<0){
qq= (vv.y-1-w.y)/vv.y;
w.x+= qq*vv.x,w.y+= qq*vv.y;
}else{
qq= w.y/vv.y;
w.x-= qq*vv.x,w.y-= qq*vv.y;
}
if(w.x<0){
qq= (uu.x-1-w.x)/uu.x;
w.x+= qq*uu.x;
}else{
qq= w.x/uu.x;
w.x-= qq*uu.x;
}
}

/*:29*/
#line 447 "dragon-calc.w"
;
classof(w)= c;
}
c++;
}
}

/*:27*/
#line 522 "dragon-calc.w"
;
for(j= 1;j<s;j++)fill[j]= -1;
for(k= 0;k<s;k++){
w= poly[k];
/*29:*/
#line 468 "dragon-calc.w"

{
if(w.y<0){
qq= (vv.y-1-w.y)/vv.y;
w.x+= qq*vv.x,w.y+= qq*vv.y;
}else{
qq= w.y/vv.y;
w.x-= qq*vv.x,w.y-= qq*vv.y;
}
if(w.x<0){
qq= (uu.x-1-w.x)/uu.x;
w.x+= qq*uu.x;
}else{
qq= w.x/uu.x;
w.x-= qq*uu.x;
}
}

/*:29*/
#line 526 "dragon-calc.w"
;
if(fill[classof(w)]>=0){
fill[0]= -1;break;
}
fill[classof(w)]= k;
}
}

/*:33*/
#line 536 "dragon-calc.w"
;
if(fill[0]<0){
printf("Sorry, the current path isn't plane-filling!\n");
break;
}
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 541 "dragon-calc.w"
;
u.x= k;
must_see(',');
/*8:*/
#line 190 "dragon-calc.w"

{
while(*p==' ')p++;
if(*p=='-')neg= 1,p++;
else neg= 0;
for(k= 0;*p>='0'&&*p<='9';p++)k= 10*k+*p-'0';
if(neg)k= -k;
}

/*:8*/
#line 544 "dragon-calc.w"
;
u.y= k;
check_tile(u);
cyc[0]= u,cycptr= 1;
while(1){
/*35:*/
#line 558 "dragon-calc.w"

w= u;
/*29:*/
#line 468 "dragon-calc.w"

{
if(w.y<0){
qq= (vv.y-1-w.y)/vv.y;
w.x+= qq*vv.x,w.y+= qq*vv.y;
}else{
qq= w.y/vv.y;
w.x-= qq*vv.x,w.y-= qq*vv.y;
}
if(w.x<0){
qq= (uu.x-1-w.x)/uu.x;
w.x+= qq*uu.x;
}else{
qq= w.x/uu.x;
w.x-= qq*uu.x;
}
}

/*:29*/
#line 560 "dragon-calc.w"
;
v= poly[fill[classof(w)]];
k= (typ(u)-typ(v))&0x3;
e= quot(diff(u,prod(v,ipower[(-k)&0x3])),z);
w= sum(e,ipower[(-k)&0x3]);
printf(" %ld,%ld = %ld,%ld * %ld,%ld\n",u.x,u.y,v.x,v.y,w.x,w.y);

/*:35*/
#line 549 "dragon-calc.w"
;
if(*q=='f')break;
/*36:*/
#line 570 "dragon-calc.w"

if(norm(w)==1)break;
if(norm(w)<norm(cyc[0]))cyc[0]= w,cycptr= 1;
else{
for(k= 0,cyc[cycptr]= w;w.x!=cyc[k].x||w.y!=cyc[k].y;k++);
if(k<cycptr)break;
cycptr++;
}

/*:36*/
#line 551 "dragon-calc.w"
;
u= w;
}
break;

/*:34*//*37:*/
#line 582 "dragon-calc.w"

case'm':/*38:*/
#line 593 "dragon-calc.w"

if(!outfile){
outfile= fopen(metapost_name,"w");
if(!outfile){
fprintf(stderr,"Oops, I can't open %s for output! Have to quit...\n",
metapost_name);
exit(-99);
}
fprintf(outfile,"%% Output from DRAGON-CALC\n");
fprintf(outfile,
"numeric dd; pair rr,ww,zz; rr=(10bp,0); %% adjust rr if desired!\n");
fprintf(outfile,
"def D = dd:=dd+90; ww:=zz; zz:=ww+rr rotated dd; draw ww--zz; enddef;\n");
fprintf(outfile,
"def U = dd:=dd-90; ww:=zz; zz:=ww+rr rotated dd; draw ww--zz; enddef;\n");
fprintf(outfile,
"def O = zz:=origin; dd:=-90; D; enddef;\n");
}

/*:38*/
#line 583 "dragon-calc.w"
;
count++,p++;
fprintf(outfile,"\nbeginfig(%d)\n O",count);
for(k= 0;k<s-1;k++){
if(k%32==31)fprintf(outfile,"\n");
fprintf(outfile," %c",fold[k]);
}
fprintf(outfile,";\nendfig;\n");
break;

/*:37*/
#line 187 "dragon-calc.w"
;
}

/*:7*/
#line 133 "dragon-calc.w"
;
while(*p==' ')p++;
if(*p!='\n')
printf("Junk at end of command has been ignored: %s",p);
}
done:/*39:*/
#line 612 "dragon-calc.w"

if(outfile){
fprintf(outfile,"\nbye.\n");
fclose(outfile);
fprintf(stderr,"METAPOST output for %d paths written on %s.\n",
count,metapost_name);
outfile= NULL;
}

/*:39*/
#line 138 "dragon-calc.w"

}

/*:2*/
