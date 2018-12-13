#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

	pid_t childPID1, childPID2;
	int pipes[2];

	if (pipe(pipes)) {
		fprintf(stderr, "ERROR: cannot create pipe\n");
		return (-1);
	}

	childPID1 = fork();
	if (childPID1 < 0) {
		fprintf(stderr, "ERROR: cannot fork process\n");
		return (-1);
	} else if (childPID1 == 0) {

		close(pipes[1]);
		
		FILE *input;
		int c;
		input = fdopen(pipes[0], "r");
		printf("Child1: ready for input\n");
		while ((c=fgetc(input))!=EOF)
			putchar(c);
		printf("Child1: done with input\n");
		fclose(input);

		printf("Child1: exiting\n");
		return 0;
	}

	childPID2 = fork();
	if (childPID2 < 0) {
		fprintf(stderr, "ERROR: cannot fork process\n");
		return (-1);
	} else if (childPID2 == 0) {

		close(pipes[0]);

		FILE *output;
		output = fdopen(pipes[1], "w");
		printf("Child2: ready for output\n");
		fprintf(output, "Hello\n");
		printf("Child2: done with output\n");
		fclose(output);

		printf("Child2: exiting\n");
		return 0;
	}

	printf("Parent: closing pipes\n");
	close(pipes[0]);
	close(pipes[1]);

	printf("Parent: waiting on child processes\n");
	waitpid(childPID1, NULL, 0);
	waitpid(childPID2, NULL, 0);
	
	printf("Parent: exiting\n");

	return 0;
}
