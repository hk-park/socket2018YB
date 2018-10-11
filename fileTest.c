#include<stdio.h>
#include<string.h>


int main(){
	FILE *fp;
	char buffer[100];

	fp = fopen("test.txt", "r");
	if(fp){

		while(fgets(buffer, 100, (FILE *)fp)){
		printf("%s\n", buffer);}

	}
	fclose(fp);
	int ret = system("mkdir testdir");
	if(!ret )
	printf("Command Success!\n");
	else
	printf("Command failed\n");
	return 0;

}
