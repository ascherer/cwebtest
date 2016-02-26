#define buf_size 1024 \

/*1:*/
#line 17 "dimacs-to-sat.w"

#include <stdio.h> 
#include <stdlib.h> 
char buf[buf_size];
int m,n,v;
main(){
register int j,k,l,p;
/*2:*/
#line 32 "dimacs-to-sat.w"

while(1){
if(!fgets(buf,buf_size,stdin))break;
if(buf[0]=='c')/*3:*/
#line 45 "dimacs-to-sat.w"

{
printf("~ %s",buf+1);
continue;
}

/*:3*/
#line 35 "dimacs-to-sat.w"
;
if(buf[0]!='p'||buf[1]!=' '||
buf[2]!='c'||buf[3]!='n'||buf[4]!='f'){
fprintf(stderr,"Unrecognized input line: %s\n",buf);
exit(-2);
}
sscanf(buf+5,"%i %i",&n,&m);
break;
}

/*:2*/
#line 24 "dimacs-to-sat.w"
;
if(m==0||n==0){
fprintf(stderr,"I didn't find m or n!\n");
exit(-1);
}
/*4:*/
#line 51 "dimacs-to-sat.w"

j= k= l= p= 0;
while(fscanf(stdin,"%i",&v)==1){
if(v==0){
if(j==0)
fprintf(stderr,"Warning: Clause %d is empty!\n",k+1);
printf("\n");
if(k==m){
fprintf(stderr,"Too many clauses (more than %d)!\n",m);
exit(-3);
}
k++,j= 0;
}else if(v> 0){
if(v> n){
fprintf(stderr,"Too many variables (%d > %d)!\n",v,n);
exit(-4);
}
printf(" %d",v);
if(v> p)p= v;
j++,l++;
}else{
if(v<-n){
fprintf(stderr,"Too many variables (%d < -%d)!\n",v,n);
exit(-5);
}
printf(" ~%d",-v);
if(-v> p)p= -v;
j++,l++;
}
}
if(j){
fprintf(stderr,"The last clause didn't end with 0!\n");
printf("\n");
k++;
}
if(k<m)fprintf(stderr,"Too few clauses (%d < %d)!\n",k,m);
fprintf(stderr,"OK, I wrote out %d literals in %d clauses on %d variables.\n",
l,k,p);

/*:4*/
#line 29 "dimacs-to-sat.w"
;
}

/*:1*/
