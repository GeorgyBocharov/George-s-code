#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <errno.h>

char**	readtxt(FILE*);
char**	split(char*, int*);

int main(int argc, char *argv[])
{
	int words = 0, i = 0, maxtime = 0;
	FILE* text;
	char** ptrarr;
	pid_t pid = -1; 
	text = fopen("text", "r");
	ptrarr = readtxt(text);
	fclose(text);
	int amount = atoi(ptrarr[0]);
	for (i = 1;i <= amount; i++)
	{
		char** string = split(ptrarr[i], &words);
		int  time = atoi(string[words - 1]);
		string[words - 1] = (char*) NULL;
		pid = fork();
		if (pid == 0)
		{
			sleep(time);
			printf("process %d, running new programm\n", i);
			execvp(string[0], string);
			perror("exec failed");
			exit(-1);
		}
		else if (pid == -1)
		{
			perror("fork failed");
			exit(-1);
		}
		else 
			waitpid (pid, 0, 0);
		free(string);
	}
	free(ptrarr);
	return 0;
}



char** split (char* s, int* length)
{
	int counter = 1, i = 0;
	char** head;
	char* point;
	errno = 0;
	if (!(head = (char**) calloc (1, sizeof(char*))))
	{
		printf("realloc: error: %s\n", strerror(errno));
		exit(-1);
	}
	point = strtok (s, " ");
	while (point != NULL)
	{
		if (i + 2 >= counter)
		{
			counter *= 2;
			if (!(head = (char**) realloc (head,  counter * sizeof(char*))))
			{
				printf("realloc: error: %s\n", strerror(errno));
				exit(-1);
			}
		}
		head[i] = point;
		i++;
		point = strtok (NULL, " ");
	}
	*length = i;
	head[i] = (char*) NULL;
	return head;
}

char** readtxt(FILE* text)
{
	int i = 0, j = 1, count = 0;
	long int length = 0;
	size_t k;
	char** ptrarr;	
	char* filestring;
	errno = 0;
	if (fseek(text, 0, SEEK_END) == -1)
	{
		perror("fseek crush");
		exit(-1);
	}
	length = ftell(text);
	rewind(text);
	if (!(filestring = (char*) calloc (length + 1, sizeof(char))))
	{
		printf("calloc: error: %s\n", strerror(errno));
		exit(-1);
	}
	if ((k = fread(filestring, sizeof(char), length,text)) != length)
	{
		printf("read wrong");
		exit(-1);
	}
	filestring[length] = '\n';
	for (i = 0;i < length + 1; i++)
	{
		if (filestring[i] == '\n')
		{
			count++;
		}
	}
	filestring[length] = '\0';
	if (!(ptrarr = (char**) calloc(count, sizeof(char*))))
	{
		printf("calloc: error: %s\n", strerror(errno));
		exit(-1);
	}
	ptrarr[0] = &(filestring[0]);
	for (i = 0; i < length; i++)
	{
		if (filestring[i] == '\n')
		{
			filestring[i] = '\0';
			ptrarr[j] = &(filestring[i + 1]);
			j++;
		}
	}
	return ptrarr;
}
