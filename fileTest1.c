#include <stdio.h>

int main()
{
	FILE *fp;
	char buffer[100];
	
	fp = fopen("test.txt", "r");
	if(fp)
	{
	      while(fgets(buffer, 100, (FILE *)fp))
		printf("%s" ,buffer);

	}
	
	fclose(fp);


	int ret = system ("mkdir testdir");

	if(!ret)
		printf("command Success");
	else
		printf("command failed");


	return 0;
}