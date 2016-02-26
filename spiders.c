#define maxn 100
#define abort(f,d,n) {fprintf(stderr,f,d) ;exit(-n) ;} \

/*1:*/
#line 20 "spiders.w"

#include <stdio.h> 
/*4:*/
#line 113 "spiders.w"

int par[maxn];
int sign[maxn];
int scope[maxn];
int stack[maxn];
int rchild[maxn],lsib[maxn];
int verbose;

/*:4*//*9:*/
#line 288 "spiders.w"

int ppro[maxn],npro[maxn];
int prev[maxn];
int ptr[maxn];
int umax[maxn],vmax[maxn];

/*:9*//*12:*/
#line 406 "spiders.w"

int umin[maxn],vmin[maxn];
int ueven[maxn],veven[maxn];
int umaxbit[maxn],vmaxbit[maxn];
int bit[maxn];

/*:12*//*18:*/
#line 611 "spiders.w"

int left[maxn],right[maxn];
int bstart[maxn];
int umaxscope[maxn],vmaxscope[maxn];
int flag[maxn];
int focus[maxn];

/*:18*//*25:*/
#line 753 "spiders.w"

int count;
int been_there_and_done_that;

/*:25*//*32:*/
#line 832 "spiders.w"

int asleep[maxn];

/*:32*/
#line 22 "spiders.w"

/*13:*/
#line 434 "spiders.w"

void setlast(register int k);
void setmid(register int k,int b);
void setfirst(register int k)
{
register int j;
bit[k]= 0;
for(j= rchild[k];j;j= lsib[j])
if(sign[j]==0){
if(ueven[k]>=j)setfirst(j);
else setlast(j);
}else if(ueven[k]>=j)setmid(j,0);
else setfirst(j);
}
void setlast(register int k)
{
register int j;
bit[k]= 1;
for(j= rchild[k];j;j= lsib[j])
if(sign[j]==1){
if(veven[k]>=j)setlast(j);
else setfirst(j);
}else if(veven[k]>=j)setmid(j,1);
else setlast(j);
}
void setmid(register int k,int b)
{
register int j;
bit[k]= b;
for(j= rchild[k];j;j= lsib[j])
if(sign[j]==0)setlast(j);else setfirst(j);
}

/*:13*//*19:*/
#line 633 "spiders.w"

void fixup(register int k,register int l)
{
register int i,j;
flag[l]= 0;
if(k> 0)/*20:*/
#line 648 "spiders.w"

{
j= bstart[k],i= lsib[j];
left[j]= left[l],right[left[l]]= j;
left[l]= k,right[k]= l;
if(i){
if(sign[k]==1){
if(sign[i]==0){
if(vmin[i]<maxn)j= vmin[i];
i= -i;
}else j= umin[i];
}else{
if(sign[i]==1){
if(umin[i]<maxn)j= umin[i];
i= -i;
}else j= vmin[i];
}
flag[j]= i;
}
return;
}

/*:20*/
#line 638 "spiders.w"

/*21:*/
#line 674 "spiders.w"

k= -k,j= bstart[k],i= lsib[j];
if(left[l]!=k)
printf("Oops, fixup(%d,%d) is confused!\n",-k,l);
if(i&&sign[i]!=sign[k]){
if((sign[i]==0&&vmax[i]==0)||(sign[i]==1&&umax[i]==0))
/*22:*/
#line 694 "spiders.w"

{
left[l]= i,right[i]= l;
k= bstart[i],left[k]= left[j],right[left[k]]= k;
i= lsib[k];
if(i){
if(sign[k]==0){
if(sign[i]==1){
if(umin[i]<maxn)k= umin[i];
i= -i;
}else k= vmin[i];
}else{
if(sign[i]==0){
if(vmin[i]<maxn)k= vmin[i];
i= -i;
}else k= umin[i];
}
flag[k]= i;
}
return;
}

/*:22*/
#line 680 "spiders.w"
;
}
left[l]= left[j],right[left[j]]= l;
if(i){
if(sign[k]==0){
if(sign[i]==1)j= umin[i];
else j= vmin[i],i= -i;
}else{
if(sign[i]==0)j= vmin[i];
else j= umin[i],i= -i;
}
flag[j]= i;
}

/*:21*/
#line 639 "spiders.w"
;
}

/*:19*/
#line 23 "spiders.w"

int main(int argc,char*argv[])
{
/*5:*/
#line 121 "spiders.w"

register int j,k,l= 0;
int n;

/*:5*/
#line 26 "spiders.w"
;
/*3:*/
#line 87 "spiders.w"

{
register char*c;
if(argc<2||argc> 3||(argc==3&&sscanf(argv[2],"%d",&verbose)!=1))
abort("Usage: %s graphspecification [verbosity]\n",argv[0],1);
for(c= argv[1],j= n= 0;*c;c++)switch(*c){
case'.':if(n==maxn-1)
abort("Sorry, I can only handle %d vertices!\n",maxn-1,2);
stack[j++]= ++n;break;
case'+':case'-':if(j<2)
abort("Parsing error: `%s' should start with `.'!\n",c,3);
j--,k= stack[j],l= stack[j-1];
sign[k]= (*c=='+'?1:0);
par[k]= l,lsib[k]= rchild[l],rchild[l]= k;
scope[k]= n;
break;
default:abort("Parsing error: `%s' should start with `.' or `+' or `-'!\n",
c,4);
}
scope[0]= n,sign[0]= 1,rchild[0]= stack[--j];
for(k= n;j>=0;j--){
l= stack[j],scope[l]= k,k= l-1;
if(j> 0)lsib[l]= stack[j-1];
}
}

/*:3*/
#line 27 "spiders.w"
;
/*7:*/
#line 226 "spiders.w"

for(j= 1;j<=n;j++){
k= par[j];
if(sign[j]==0){
ppro[j]= j,npro[j]= npro[k];
if(k)prev[j]= umax[ppro[k]],umax[ppro[k]]= j;
else prev[j]= lsib[j];
}else{
npro[j]= j,ppro[j]= ppro[k];
prev[j]= vmax[npro[k]],vmax[npro[k]]= j;
}
}
/*8:*/
#line 242 "spiders.w"

lsib[0]= -1;
ptr[0]= vmax[0];
umax[0]= rchild[0];
for(j= rchild[0];;){
if(sign[j]==0){
ptr[j]= umax[j];
k= npro[j],l= ptr[k];
while(l> scope[j])l= prev[l];
ptr[k]= l;
if(l> j)vmax[j]= l;
}else{
ptr[j]= vmax[j];
k= ppro[j],l= ptr[k];
while(l> scope[j])l= prev[l];
ptr[k]= l;
if(l> j)umax[j]= l;
}
if(rchild[j])j= rchild[j];
else{
while(!lsib[j])j= par[j];
j= lsib[j];
if(j<0)break;
}
}

/*:8*/
#line 238 "spiders.w"
;

/*:7*//*10:*/
#line 333 "spiders.w"

for(k= 0;k<=n;k++)ueven[k]= veven[k]= umin[k]= vmin[k]= maxn;
for(j= n;j> 0;j--){
k= ppro[j];
if(umin[k]<=scope[j])umin[j]= umin[k];
if(ueven[k]<=scope[j])ueven[j]= ueven[k];
k= npro[j];
if(vmin[k]<=scope[j])vmin[j]= vmin[k];
if(veven[k]<=scope[j])veven[j]= veven[k];
l= (ueven[j]<maxn)^(veven[j]<maxn);
k= par[j];
if(sign[j]==0){
umin[ppro[k]]= j;
if(l==0)ueven[ppro[k]]= j;
}else{
vmin[npro[k]]= j;
if(l==0)veven[npro[k]]= j;
}
}
ueven[0]= maxn;

/*:10*//*11:*/
#line 358 "spiders.w"

for(k= n;k> 0;k--){
l= par[k];
if(k==umax[l])umaxbit[l]= 1;
else{
j= umax[k];
if(j&&umax[l]==j){
if(ueven[k]<j)umaxbit[l]= umaxbit[k];
else umaxbit[l]= 1^umaxbit[k];
}
}
if(k==vmax[l])vmaxbit[l]= 0;
else{
j= vmax[k];
if(j&&vmax[l]==j){
if(veven[k]<j)vmaxbit[l]= vmaxbit[k];
else vmaxbit[l]= 1^vmaxbit[k];
}
}
}

/*:11*//*16:*/
#line 591 "spiders.w"

for(k= n;k;k--){
j= lsib[k];
if(j)left[k]= j,right[j]= k;
else/*17:*/
#line 604 "spiders.w"

for(j= l= k;j;j= right[j]){
if(right[j]&&sign[j]==sign[right[j]]&&
((sign[j]==0&&!vmax[j])||(sign[j]==1&&!umax[j])))continue;
bstart[j]= l,l= right[j];
}

/*:17*/
#line 595 "spiders.w"
;
j= umax[k];
if(!j)umaxscope[k]= k;
else umaxscope[k]= (umaxbit[k]==1?(vmax[j]?vmax[j]:j):umaxscope[j]);
j= vmax[k];
if(!j)vmaxscope[k]= k;
else vmaxscope[k]= (vmaxbit[k]==0?(umax[j]?umax[j]:j):vmaxscope[j]);
}

/*:16*/
#line 28 "spiders.w"
;
/*24:*/
#line 733 "spiders.w"

/*23:*/
#line 719 "spiders.w"

setfirst(0);
for(l= k= 0;k<=n;k++){
focus[k]= k;
if(sign[k]==bit[par[k]])right[l]= k,left[k]= l,l= k;
}
right[l]= 0,left[0]= l;

/*:23*/
#line 734 "spiders.w"
;
if(verbose> 1)/*33:*/
#line 838 "spiders.w"

{
for(k= 0;k<=n;k++){
printf("%d(%c): scope=%d, par=%d, rchild=%d, lsib=%d,",
k,sign[k]?'-':'+',scope[k],par[k],rchild[k],lsib[k]);
printf(" ppro=%d, npro=%d, prev=%d, bstart=%d\n",
ppro[k],npro[k],prev[k],bstart[k]);
printf(" umin=%d, ueven=%d, umax=%d, umaxbit=%d, umaxscope=%d\n",
umin[k],ueven[k],umax[k],umaxbit[k],umaxscope[k]);
printf(" vmin=%d, veven=%d, vmax=%d, vmaxbit=%d, vmaxscope=%d\n",
vmin[k],veven[k],vmax[k],vmaxbit[k],vmaxscope[k]);
}
}

/*:33*/
#line 735 "spiders.w"
;
while(1){
count++;
if(verbose>=0)/*30:*/
#line 798 "spiders.w"

{
for(k= 1;k<=n;k++)putchar('0'+bit[k]);
if(verbose> 0)/*31:*/
#line 812 "spiders.w"

{
for(k= left[0];;k--){
for(j= k,k= focus[k];j> k;j--){
asleep[j]= 1;
if(flag[j])printf("\nOops, flag[%d] is wrong!\n",j);
}
if(k==0)break;
asleep[k]= 0;
}
for(k= 1,j= 0;k<=left[0];k++)if(sign[k]==bit[par[k]]){
if(asleep[k])printf(" (%d)",k);
else if(flag[k])printf(" !%d",k);
else printf(" %d",k);
if((k!=right[j]||left[k]!=j)&&k> l)
printf("[oops]");
j= k;
}
}

/*:31*/
#line 801 "spiders.w"
;
putchar('\n');
}

/*:30*/
#line 738 "spiders.w"
;
/*26:*/
#line 757 "spiders.w"

j= left[0],k= focus[j],focus[j]= j;

/*:26*/
#line 739 "spiders.w"
;
if(k){
if(flag[k])fixup(flag[k],k);
if(bit[k]==0)/*28:*/
#line 766 "spiders.w"

{
bit[k]= 1,j= rchild[k];
if(j){
if(sign[j]==0){
l= vmin[j];
if(l<maxn)fixup(-j,l);
else fixup(-j,right[j]);
}else{
l= umin[j];
if(l<maxn)fixup(j,l);
else fixup(j,right[umaxscope[k]]);
}
}
}

/*:28*/
#line 742 "spiders.w"

else/*29:*/
#line 782 "spiders.w"

{
bit[k]= 0,j= rchild[k];
if(j){
if(sign[j]==1){
l= umin[j];
if(l<maxn)fixup(-j,l);
else fixup(-j,right[j]);
}else{
l= vmin[j];
if(l<maxn)fixup(j,l);
else fixup(j,right[vmaxscope[k]]);
}
}
}

/*:29*/
#line 743 "spiders.w"
;
}else if(been_there_and_done_that)break;
else{
printf("...%d so far; now we generate in reverse:\n",count);
been_there_and_done_that= 1;continue;
}
/*27:*/
#line 763 "spiders.w"

j= left[k],focus[k]= focus[j],focus[j]= j;

/*:27*/
#line 749 "spiders.w"
;
}
printf("Altogether %d/2 labelings.\n",count);

/*:24*/
#line 29 "spiders.w"
;
return 0;
}

/*:1*/
