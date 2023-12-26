#include "main.h"

char *custom_getline()
{
	static char buffer[BUFFER_SIZE];
	static char *line = NULL;
	static size_t line_size = 0;
	size_t len = 0;
	int c;

	if (line == NULL || line_size == 0)
	{
		line = (char *)malloc(BUFFER_SIZE);
		if (line == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		line_size = BUFFER_SIZE;
	}

	while ((c = getchar()) != EOF && c != '\n')
	{
		if (len + 1 >= line_size)
		{
			line_size *= 2;
			line = (char *)realloc(line, line_size);
			if (line == NULL)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}

		line[len++] = (char)c;
	}

	if (len >= line_size)
	{
		line_size++;
		line = (char *)realloc(line, line_size);
		if (line == NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
	}
	line[len] = '\0';

	if (c == EOF && len == 0)
	{
		free(line);
		line = NULL;
	}

	return line;
}

int main()
{
	char *line;

	while ((line = custom_getline()) != NULL)
	{
		printf("You entered: %s\n", line);
		free(line);
	}

	return 0;
}
