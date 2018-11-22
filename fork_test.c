#include <stdio.h>

int main() {
	int pid;		// 프로세스 id.
	int a = 10;
	int b = 20;

	a = a + 1;

	pid = fork();		// 자식 프로세스를 생성하고 그 아이디를 리턴 받음.

	if(pid > 0) {		// 부모 프로세스
		a = a + 1;
		printf("[PARENT] a = %d, b = %d\n", a, b);
		sleep(30);
	} else {		// (pid =< 0) 자식 프로세스이거나 fork함수가 실패한 경우, 주로 전자
		b = b + 1;
		printf("[CHILD] a = %d, b = %d\n", a, b);
		sleep(10);
	}
	printf("[%d] a= %d, b = \%d\n", getpid(), a, b);
	return 0;
}
