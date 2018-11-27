#include <stdio.h>

int main(){
	FILE *fp;
	char buffer[100];

	fp=fopen("test.txt", "r");
	if(fp){
		while(fgets(buffer, 100, (FILE *)fp))
		printf("%s", buffer);
	}
	fclose(fp);
	int ret=system("mkdir testdir");
	if(!ret) //ret가 0이면
		printf("command Success!!\n");
	else
		printf("command Failed!!\n");
	return 0;
}
