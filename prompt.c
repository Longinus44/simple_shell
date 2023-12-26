#include "main.h"

void display_prompt()
{
	printf("simple_shell$ ");
	fflush(stdout);
}

void run_command(char *command)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		execlp(command, command, NULL);
		perror("exec");
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
}

int main()
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		display_prompt();

		if (fgets(command, sizeof(command), stdin) == NULL)
		{
			printf("\nExiting simple_shell.\n");
			break;
		}
		command[strcspn(command, "\n")] = '\0';

		run_command(command);
	}

	return 0;
}