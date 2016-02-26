#define get_token gtok()  \

#define get_num gnum() 
#define panic(str) {fprintf(stderr,"Oops! %s:\n%s",str,buffer) ;exit(-1) ;} \

#define check(str,err) {get_token;if(strcmp(token,str) !=0) panic(err) ;} \

/*1:*/
#line 46 "./fog2mf.w"

#include <stdio.h> 
char buffer[100],*pos= buffer;
char token[100];
/*2:*/
#line 71 "./fog2mf.w"

gtok()
{
register char*p;
if(*pos==0||*pos=='\n'){
if(!fgets(buffer,100,stdin))exit(0);
pos= buffer;
}
for(;*pos==' ';pos++);
for(p= token;*pos!=' '&&*pos&&*pos!='\n';p++,pos++)*p= *pos;
*p= 0;
for(;*pos==' ';pos++);
}

/*:2*//*3:*/
#line 90 "./fog2mf.w"

double gnum()
{
double xx;
if(sscanf(token,"%lf",&xx)!=1)panic("Unreadable number");
return xx;
}

/*:3*/
#line 50 "./fog2mf.w"

main()
{
register int j,k;
double x,y,z;
register char*p,*q;
while(1){
/*4:*/
#line 112 "./fog2mf.w"

check("C","Expected `C'");
get_token;printf("beginchar(%s,",token);
check(";","Expected `;'");
check("WX","Expected `WX'")
get_token;printf("%su#,",token);
check(";","Expected `;'");
check("N","Expected `N'");
get_token;
check(";","Expected `;'");
check("B","Expected `B'");
get_token;get_token;
k= (int)(get_num+.5);if(k> 0)k= 0;else k= -k;
get_token;get_token;printf("%su#,%du#);\n",token,k);
check(";","Expected `;'");
get_token;
check("0","Expected `0'");
get_token;get_token;get_token;get_token;
check("Cache","Expected `Cache'");

/*:4*/
#line 57 "./fog2mf.w"
;
/*5:*/
#line 140 "./fog2mf.w"

while(1){
get_token;x= get_num;
get_token;if(strcmp(token,"FillStroke")==0)break;
y= get_num;check("moveto","Expected `moveto'");
printf("stroke (%d,%d)\n",(int)(x+.5),(int)(y+.5));
while(1){
get_token;
if(strcmp(token,"closepath")==0)break;
x+= get_num;get_token;y+= get_num;
get_token;
if(strcmp(token,"rlineto")==0)
printf(" --(%d,%d)\n",(int)(x+.5),(int)(y+.5));
else{
printf(" ...(%d,%d",(int)(x+.5),(int)(y+.5));
x+= get_num;get_token;y+= get_num;
printf(",%d,%d",(int)(x+.5),(int)(y+.5));
get_token;x+= get_num;get_token;y+= get_num;
printf(",%d,%d)\n",(int)(x+.5),(int)(y+.5));
check("rrcurveto","Expected `rrcurveto'");
}
};
printf(" --cycle;\n");
}
printf("endchar;\n");
check("}def","Expected `}def'");

/*:5*/
#line 58 "./fog2mf.w"
;
}
}

/*:1*/
