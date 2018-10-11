#include <stdio.h>
int main(){
	FILE *fp;
	char buffer[100];
	fp = fopen("test.txt", "r");
	if(fp){
		while(fgets(buffer, 100, (FILE *)fp))
		printf("%s", buffer);//버퍼 내에 개행문자가 있으므로 \n은 필요없음
	}
	fclose(fp);
	int ret = system("mkdir testdir");//성공하면 0, 실패하면 0이 아닌값
	if(!ret)//ret가 0이면
		printf("command Success\n");
	else
		printf("command Failed\n");
	return 0;
}
