#define m 64
#define n 55
#define r 64 \

#define panic(s) {fprintf(stderr,s) ;exit(-1) ;} \

#define alpha 0.4375
#define beta 0.1875
#define gamma 0.3125
#define delta 0.0625 \

/*1:*/
#line 30 "./halftone.w"

#include <stdio.h> 

float a[m+m+2][n+r];
/*4:*/
#line 88 "./halftone.w"

int llx,lly,urx,ury;

/*:4*//*8:*/
#line 202 "./halftone.w"

float d[65]= {0.000,0.060,0.114,0.162,0.205,0.243,0.276,0.306,0.332,0.355,
0.375,0.393,0.408,0.422,0.435,0.446,0.456,0.465,0.474,0.482,
0.490,0.498,0.505,0.512,0.520,0.527,0.535,0.543,0.551,0.559,
0.568,0.577,0.586,0.596,0.605,0.615,0.625,0.635,0.646,0.656,
0.667,0.677,0.688,0.699,0.710,0.720,0.731,0.742,0.753,0.764,
0.775,0.787,0.798,0.810,0.822,0.835,0.849,0.863,0.878,0.894,
0.912,0.931,0.952,0.975,1.000};

/*:8*/
#line 34 "./halftone.w"
;

main(argc,argv)
int argc;char*argv[];
{
register int i,j,k,l,p;
int levels,trash,ii,jj;
float dampening= 1.0,brightness= 1.0;
/*2:*/
#line 55 "./halftone.w"

if(argc> 1&&sscanf(argv[1],"%g",&dampening)==1){
fprintf(stderr,"Using dampening factor %g\n",dampening);
if(argc> 2&&sscanf(argv[2],"%g",&brightness)==1)
fprintf(stderr,"  and brightness factor %g\n",brightness);
}

/*:2*/
#line 42 "./halftone.w"
;
/*3:*/
#line 74 "./halftone.w"

k= 0;
scan:if(k++> 1000)panic("Couldn't find the bounding box info!\n");
if(getchar()!='B')goto scan;
if(getchar()!='o')goto scan;
if(getchar()!='x')goto scan;
if(getchar()!=':')goto scan;
if(scanf("%d %d %d %d",&llx,&lly,&urx,&ury)!=4||llx!=0||lly!=0)
panic("Bad bounding box data!\n");
if(urx==n&&ury==m)levels= 65;
else if(urx==n+n&&ury==m+m)levels= 17;
else if(urx==m+n-1&&ury==urx)levels= 33;
else panic("Bounding box doesn't match the formats I know!\n");

/*:3*/
#line 43 "./halftone.w"
;
fprintf(stderr,"Making %d lines of %d-level data\n",
(levels<65?m+m:m),levels);
printf("\\begin%shalftone\n",levels==33?"alt":"");
/*5:*/
#line 94 "./halftone.w"

k= 0;
skan:if(k++> 10000)panic("Couldn't find the pixel data!\n");
if(getchar()!='b')goto skan;
if(getchar()!='e')goto skan;
if(getchar()!='g')goto skan;
if(getchar()!='i')goto skan;
if(getchar()!='n')goto skan;
if(getchar()!='i')goto skan;
if(getchar()!='m')goto skan;
if(getchar()!='a')goto skan;
if(getchar()!='g')goto skan;
if(getchar()!='e')goto skan;
if(getchar()!='\r')goto skan;
if(levels==33)/*7:*/
#line 166 "./halftone.w"

{
for(i= 0;i<ury;i++)
for(j= 0;j<urx;j++){
ii= m+i-j;jj= i+j+1-m;
if(ii>=0&&ii<m+m&&jj>=0&&jj<n+n)
a[ii][i]= 1.0-brightness*getchar()/255.0;
else trash= getchar();
}
a[0][n-1]= 1.0-brightness;
}

/*:7*/
#line 108 "./halftone.w"

else/*6:*/
#line 121 "./halftone.w"

for(i= 1;i<=ury;i++)for(j= 0;j<urx;j++)
a[i][j]= 1.0-brightness*getchar()/255.0;

/*:6*/
#line 109 "./halftone.w"
;
if(getchar()!='\r')panic("Wrong amount of pixel data!\n");

/*:5*/
#line 47 "./halftone.w"
;
/*12:*/
#line 278 "./halftone.w"

p= 64/(levels-1);
if(p!=2){
for(j= 0;j<urx;j++)for(i= 0;i<=ury+1;i++)
/*10:*/
#line 238 "./halftone.w"

{register float err;
if(i==0||i> ury)l= 0;
else/*9:*/
#line 216 "./halftone.w"

if(a[i][j]<=0.0)l= 0;
else if(a[i][j]>=1.0)l= 64;
else{register int lo_l= 0,hi_l= 64;
while(hi_l-lo_l> p){register int mid_l= (lo_l+hi_l)>>1;

if(a[i][j]>=d[mid_l])lo_l= mid_l;
else hi_l= mid_l;
}
if(a[i][j]-d[lo_l]<=d[hi_l]-a[i][j])l= lo_l;
else l= hi_l;
}

/*:9*/
#line 241 "./halftone.w"
;
err= a[i][j]-d[l];
a[i][j]= (float)(l/p);
if(i<=ury)a[i+1][j]+= alpha*dampening*err;
if(j<urx-1){
if(i> 0)a[i-1][j+1]+= beta*dampening*err;
a[i][j+1]+= gamma*dampening*err;
if(i<=ury)a[i+1][j+1]+= delta*dampening*err;
}
}

/*:10*/
#line 282 "./halftone.w"
;
for(i= 1;i<=ury;i++){
for(j= 0;j<urx;j++)
printf("%c",(p==1?'0':((i+j)&1)?'a':'A')+(int)a[i][j]);
printf(".\n");
}
}else{
for(j= 0;j<m+n-1;j++)for(i= 0;i<m+m;i++)
/*11:*/
#line 261 "./halftone.w"

{register float err;
if((i>>1)<=j-n||(i>>1)> j)
l= 0;
else/*9:*/
#line 216 "./halftone.w"

if(a[i][j]<=0.0)l= 0;
else if(a[i][j]>=1.0)l= 64;
else{register int lo_l= 0,hi_l= 64;
while(hi_l-lo_l> p){register int mid_l= (lo_l+hi_l)>>1;

if(a[i][j]>=d[mid_l])lo_l= mid_l;
else hi_l= mid_l;
}
if(a[i][j]-d[lo_l]<=d[hi_l]-a[i][j])l= lo_l;
else l= hi_l;
}

/*:9*/
#line 265 "./halftone.w"
;
err= a[i][j]-d[l];
a[i][j]= (float)(l/p);
if(i<m+m-1)a[i+1][j]+= alpha*dampening*err;
if(j<m+n-2){
a[i][j+1]+= beta*dampening*err;
if(i<m+m-1)a[i+1][j+1]+= gamma*dampening*err;
if(i<m+m-2)a[i+2][j+1]+= delta*dampening*err;
}
}

/*:11*/
#line 290 "./halftone.w"
;
for(i= 0;i<m+m;i++){
for(j= 0;j<n;j++)printf("%c",'0'+(int)a[i][j+(i>>1)]);
printf(".\n");
}
}
printf("\\endhalftone\n");

/*:12*/
#line 48 "./halftone.w"
;
}

/*:1*/
