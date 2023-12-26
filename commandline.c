#include "main.h"

#define MAX_INPUT_LENGTH 100
#define MAX_ARGS 10

void display_prompt()
{
	printf("simple_shell$ ");
	fflush(stdout);
}

void parse_command(char *input, char **args)
{
	int i = 0;
	args[i] = strtok(input, " ");

	while (args[i] != NULL && i < MAX_ARGS - 1)
	{
		i++;
		args[i] = strtok(NULL, " ");
	}
}

void run_command(char **args)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		// Child process
		execvp(args[0], args);
		// If execvp fails
		perror("exec");
		exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		int status;
		waitpid(pid, &status, 0);
	}
}

int main()
{
	char input[MAX_INPUT_LENGTH];
	char *args[MAX_ARGS];

	while (1)
	{
		display_prompt();

		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			// Handle end of file (Ctrl+D)
			printf("\nExiting simple_shell.\n");
			break;
		}

		// Remove the newline character from the input
		input[strcspn(input, "\n")] = '\0';

		parse_command(input, args);
		run_command(args);
	}

	return 0;
}
