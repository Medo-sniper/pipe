#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	pid_t childPID1, childPID2;
	int pipes[2];

	if (pipe(pipes)) {
		fprintf(stderr, "ERROR: cannot create pipe\n");
		return (-1);
	}
/*
	childPID1 = fork();
	if (childPID1 < 0) {
		fprintf(stderr, "ERROR: cannot fork process\n");
		return (-1);
	} else if (childPID1 == 0) {
		close(pipes[1]);
		printf("Child1: ready for input\n");
        dup2(pipes[0], STDIN_FILENO);
        execl("/bin/cat", "/bin/cat", NULL);
        printf("ERROR: Child1: cannot exec\n");
		return 1;
	}
*/
	childPID2 = fork();
	if (childPID2 < 0) {
		fprintf(stderr, "ERROR: cannot fork process\n");
		return (-1);
	} else if (childPID2 == 0) {
		close(pipes[0]);
		printf("Child2: ready for output\n");
        dup2(pipes[1], STDOUT_FILENO);
        execl("/bin/echo", "/bin/echo", "Hello", NULL);
        printf("ERROR: Child2: cannot exec\n");
		return 1;
	}

	printf("Parent: closing pipes\n");
	close(pipes[0]);
	close(pipes[1]);

	printf("Parent: waiting on child processes\n");
//	waitpid(childPID1, NULL, 0);
	printf("Parent: Child1 returned\n");
	waitpid(childPID2, NULL, 0);
	printf("Parent: Child2 returned\n");
	printf("Parent: exiting\n");

	return 0;
}
