#include <stdio.h>

int main(){
	FILE *fp;
	char buffer[100];
	
	fp = fopen("test.txt", "r");
	if(fp){
		while(fgets(buffer, 100, (FILE *)fp))
		printf("%s", buffer);
	}
	
	int ret = system("mkdir testdir");

	if(!ret)
		printf("command Success!!\n");
	else
		printf("command Failed!\n");

	return 0;
}
