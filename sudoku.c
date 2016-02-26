#define panic(m) {fprintf(stderr,"%s!\n%s",m,buf) ;exit(-1) ;} \

/*1:*/
#line 16 "./sudoku.w"

#include <stdio.h> 
char buf[11];
int row[9][10],col[9][10],box[9][10];
int board[9][9];

main()
{
register int j,k,d,x;
for(k= 0;k<9;k++)/*2:*/
#line 37 "./sudoku.w"

{
fgets(buf,11,stdin);
if(buf[9]!='\n')
panic("Input line should have 9 characters exactly!\n");
for(j= 0;j<9;j++)if(buf[j]!='.'){
if(buf[j]<'1'||buf[j]> '9')
panic("Illegal character in input!\n");
d= buf[j]-'0';
if(row[k][d])panic("Two identical digits in a row!\n");
row[k][d]= 1;
if(col[j][d])panic("Two identical digits in a column!\n");
col[j][d]= 1;
x= ((int)(k/3))*3+((int)(j/3));
if(box[x][d])panic("Two identical digits in a box!\n");
box[x][d]= 1;
board[k][j]= 1;
}
}

/*:2*/
#line 25 "./sudoku.w"
;
/*3:*/
#line 60 "./sudoku.w"

for(k= 0;k<9;k++)for(j= 0;j<9;j++)
if(!board[k][j])printf(" p%d%d",k,j);
for(k= 0;k<9;k++)for(d= 1;d<=9;d++){
if(!row[k][d])printf(" r%d%d",k,d);
if(!col[k][d])printf(" c%d%d",k,d);
if(!box[k][d])printf(" b%d%d",k,d);
}
printf("\n");

/*:3*/
#line 26 "./sudoku.w"
;
for(j= 0;j<9;j++)for(k= 0;k<9;k++)if(!board[k][j])
/*4:*/
#line 72 "./sudoku.w"

{
x= ((int)(k/3))*3+((int)(j/3));
for(d= 1;d<=9;d++)if(!row[k][d]&&!col[j][d]&&!box[x][d])
printf("p%d%d r%d%d c%d%d b%d%d\n",k,j,k,d,j,d,x,d);
}

/*:4*/
#line 28 "./sudoku.w"
;
}

/*:1*/
