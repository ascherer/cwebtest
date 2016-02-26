#define m 512
#define n 440 \

#define panic(s) {fprintf(stderr,s) ;exit(-1) ;} \

#define white 0
#define gray 1
#define black 2 \

/*1:*/
#line 22 "dot-diff.w"

#include <stdio.h> 

float a[m+2][n+2];
/*4:*/
#line 76 "dot-diff.w"

int llx,lly,urx,ury;

/*:4*//*8:*/
#line 150 "dot-diff.w"

char aa[m+2][n+2];

/*:8*//*11:*/
#line 194 "dot-diff.w"

char class_row[64],class_col[64];
char class_number[10][10];
int kk= 0;
int start[65];
int del_i[256],del_j[256];
float alpha[256];

/*:11*/
#line 26 "dot-diff.w"

/*12:*/
#line 208 "dot-diff.w"

void store(i,j)
int i,j;
{
if(i<1)i+= 8;else if(i> 8)i-= 8;
if(j<1)j+= 8;else if(j> 8)j-= 8;
class_number[i][j]= kk;
class_row[kk]= i;class_col[kk]= j;
kk++;
}

void store_eight(i,j)
int i,j;
{
store(i,j);store(i-4,j+4);store(1-j,i-4);store(5-j,i);
store(j,5-i);store(4+j,1-i);store(5-i,5-j);store(1-i,1-j);
}

/*:12*/
#line 27 "dot-diff.w"


main(argc,argv)
int argc;char*argv[];
{
register int i,j,k,l,ii,jj,w;
register float err;
float zeta= 0.2,sharpening= 0.9;
/*2:*/
#line 45 "dot-diff.w"

if(argc> 1&&sscanf(argv[1],"%g",&zeta)==1){
fprintf(stderr,"Using zeta = %g\n",zeta);
if(argc> 2&&sscanf(argv[2],"%g",&sharpening)==1)
fprintf(stderr,"  and sharpening factor %g\n",sharpening);
}

/*:2*/
#line 35 "dot-diff.w"
;
/*3:*/
#line 64 "dot-diff.w"

k= 0;
scan:if(k++> 1000)panic("Couldn't find the bounding box info!\n");
if(getchar()!='B')goto scan;
if(getchar()!='o')goto scan;
if(getchar()!='x')goto scan;
if(getchar()!=':')goto scan;
if(scanf("%d %d %d %d",&llx,&lly,&urx,&ury)!=4||llx!=0||lly!=0)
panic("Bad bounding box data!\n");
if(urx!=n||ury!=m)
panic("The image doesn't have the corect width and height!\n");

/*:3*/
#line 36 "dot-diff.w"
;
/*5:*/
#line 82 "dot-diff.w"

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
/*6:*/
#line 108 "dot-diff.w"

for(i= 1;i<=ury;i++)for(j= 1;j<=urx;j++)a[i][j]= 1.0-getchar()/255.0;

/*:6*/
#line 96 "dot-diff.w"
;
if(getchar()!='\r')panic("Wrong amount of pixel data!\n");

/*:5*/
#line 37 "dot-diff.w"
;
/*15:*/
#line 256 "dot-diff.w"

/*10:*/
#line 190 "dot-diff.w"

/*13:*/
#line 226 "dot-diff.w"

store_eight(7,2);store_eight(8,3);store_eight(8,2);store_eight(8,1);
store_eight(1,4);store_eight(1,3);store_eight(1,2);store_eight(2,3);
for(i= 1;i<=8;i++)
class_number[i][0]= class_number[i][8],class_number[i][9]= class_number[i][1];
for(j= 0;j<=9;j++)
class_number[0][j]= class_number[8][j],class_number[9][j]= class_number[1][j];

/*:13*/
#line 191 "dot-diff.w"
;
/*14:*/
#line 238 "dot-diff.w"

for(k= 0,l= 0;k<64;k++){
start[k]= l;
i= class_row[k];j= class_col[k];w= 0;
for(ii= i-1;ii<=i+1;ii++)for(jj= j-1;jj<=j+1;jj++)
if(class_number[ii][jj]> k){
del_i[l]= ii-i;del_j[l]= jj-j;l++;
if(ii!=i&&jj!=j)w++;
else w+= 2;
}
for(jj= start[k];jj<l;jj++)
if(del_i[jj]!=0&&del_j[jj]!=0)alpha[jj]= 1.0/w;
else alpha[jj]= 2.0/w;
}
start[64]= l;

/*:14*/
#line 192 "dot-diff.w"
;

/*:10*/
#line 257 "dot-diff.w"
;
if(sharpening)/*16:*/
#line 278 "dot-diff.w"

{
for(i= 1;i<=m;i++)for(j= 1;j<=n;j++){float abar;
abar= (a[i-1][j-1]+a[i-1][j]+a[i-1][j+1]+a[i][j-1]+
a[i][j]+a[i][j+1]+a[i+1][j-1]+a[i+1][j]+a[i+1][j+1])/9.0;
a[i-1][j-1]= (a[i][j]-sharpening*abar)/(1.0-sharpening);
}
for(i= m;i> 0;i--)for(j= n;j> 0;j--)
a[i][j]= (a[i-1][j-1]<=0.0?0.0:a[i-1][j-1]>=1.0?1.0:a[i-1][j-1]);
}

/*:16*/
#line 258 "dot-diff.w"
;
/*7:*/
#line 124 "dot-diff.w"

for(k= 0;k<64;k++)
for(i= class_row[k];i<=m;i+= 8)
for(j= class_col[k];j<=n;j+= 8){
/*9:*/
#line 168 "dot-diff.w"

if(aa[i][j]==white)err= a[i][j]-1.0-4*zeta;
else{
err= a[i][j]-1.0+zeta;
if(aa[i-1][j]==white)err-= zeta;
if(aa[i+1][j]==white)err-= zeta;
if(aa[i][j-1]==white)err-= zeta;
if(aa[i][j+1]==white)err-= zeta;
}
if(err+a[i][j]> 0){
aa[i][j]= black;
if(aa[i-1][j]==white)aa[i-1][j]= gray;
if(aa[i+1][j]==white)aa[i+1][j]= gray;
if(aa[i][j-1]==white)aa[i][j-1]= gray;
if(aa[i][j+1]==white)aa[i][j+1]= gray;
}else err= a[i][j];

/*:9*/
#line 128 "dot-diff.w"
;
for(l= start[k];l<start[k+1];l++)
a[i+del_i[l]][j+del_j[l]]+= err*alpha[l];
}

/*:7*/
#line 259 "dot-diff.w"
;

/*:15*/
#line 38 "dot-diff.w"
;
/*17:*/
#line 291 "dot-diff.w"

for(i= 1;i<=m;i++){
printf("row %d; data \"",i);
for(j= 1;j<=n;j+= 4){
for(k= 0,w= 0;k<4;k++)w= w+w+(aa[i][j+k]==black?1:0);
printf("%x",w);
}
printf("\";\n");
}

/*:17*/
#line 39 "dot-diff.w"
;
}

/*:1*/
