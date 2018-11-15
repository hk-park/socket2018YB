#include <stdio.h>
main(){
int pid;
int a=10;
int b=20;
a=a+1;
pid=fork();
if(pid>0){
a=a+1;
printf("parent");
}else{
b=b+1;
printf("child");
}
}
