#include "main.h"

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

int run_command(char **args)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		return -1;
	}
	else if (pid == 0)
	{
		execvp(args[0], args);

		perror("exec");
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		return status;
	}
}

void print_environment()
{
	extern char **environ;
	for (char **env = environ; *env != NULL; env++)
	{
		printf("%s\n", *env);
	}
}

int main()
{
	char input[MAX_INPUT_LENGTH];
	char *args[MAX_ARGS];
	char *path = getenv("PATH");

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

		// Split input into commands based on ';'
		char *separator = ";";
		char *command = strtok(input, separator);

		while (command != NULL)
		{
			parse_command(command, args);

			if (strcmp(args[0], "exit") == 0)
			{
				// Handle exit built-in with optional status argument
				if (args[1] != NULL)
				{
					int status = atoi(args[1]);
					printf("Exiting simple_shell with status %d.\n", status);
					exit(status);
				}
				else
				{
					printf("Exiting simple_shell.\n");
					exit(EXIT_SUCCESS);
				}
			}
			else if (strcmp(args[0], "env") == 0)
			{
				// Handle env built-in
				print_environment();
			}
			else if (strcmp(args[0], "setenv") == 0)
			{
				// Handle setenv built-in
				if (args[1] != NULL && args[2] != NULL)
				{
					if (setenv(args[1], args[2], 1) != 0)
					{
						fprintf(stderr, "setenv: Unable to set environment variable %s\n", args[1]);
					}
				}
				else
				{
					fprintf(stderr, "setenv: Invalid syntax. Usage: setenv VARIABLE VALUE\n");
				}
			}
			else if (strcmp(args[0], "unsetenv") == 0)
			{
				if (args[1] != NULL)
				{
					if (unsetenv(args[1]) != 0)
					{
						fprintf(stderr, "unsetenv: Unable to unset environment variable %s\n", args[1]);
