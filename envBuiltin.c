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

			printf("\nExiting simple_shell.\n");
			break;
		}

		input[strcspn(input, "\n")] = '\0';

		parse_command(input, args);

		if (strcmp(args[0], "exit") == 0)
		{

			printf("Exiting simple_shell.\n");
			break;
		}
		else if (strcmp(args[0], "env") == 0)
		{

			print_environment();
		}
		else
		{
			// Check if the command exists in PATH
			char command_path[MAX_PATH_LENGTH];
			int found = 0;

			// Iterate through each directory in PATH
			char *path_token = strtok(path, ":");
			while (path_token != NULL)
			{
				snprintf(command_path, sizeof(command_path), "%s/%s", path_token, args[0]);

				if (access(command_path, X_OK) == 0)
				{

					found = 1;
					break;
				}

				path_token = strtok(NULL, ":");
			}

			if (!found)
			{
				printf("simple_shell: command not found: %s\n", args[0]);
			}
			else
			{

				run_command(args);
			}
		}
	}

	return 0;
}
