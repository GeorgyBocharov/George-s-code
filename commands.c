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
	int height, i, father = 0;
	FILE* text;
	char** PtrArr;
	pid_t pid = -1; 
	text = fopen("text", "r");
	PtrArr = readtxt(text);
	fclose(text);
	int amount = atoi(PtrArr[0]);
	for (i = 1;i <= amount; i++)
	{
		char** string = split(PtrArr[i], &height);
		int  time = atoi(string[height - 1]);
		string[height - 1] = (char*) NULL;
		if (father < time)
			father = time;
		pid = fork();
		if (pid == 0)
		{
			sleep(time);
			printf("process %d, running new programm\n", i);
			execvp(string[0], string);
			perror("exec failed");
			exit(-1);
		}
		if (pid == -1)
		{
			perror("fork failed");
			exit(-1);
		}
		free(string);
	}
	sleep(father + 1);
	free(PtrArr);
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
	int i, j = 1, count = 0;
	long int length;
	size_t k;
	char** PtrArr;	
	char* FileString;
	errno = 0;
	if (fseek(text, 0, SEEK_END) == -1)
	{
		perror("fseek crush");
		exit(-1);
	}
	length = ftell(text);
	rewind(text);
	if (!(FileString = (char*) calloc (length + 1, sizeof(char))))
	{
		printf("calloc: error: %s\n", strerror(errno));
		exit(-1);
	}
	if ((k = fread(FileString, sizeof(char), length,text)) != length)
	{
		printf("read wrong");
		exit(-1);
	}
	FileString[length] = '\n';
	for (i = 0;i < length + 1; i++)
	{
		if (FileString[i] == '\n')
		{
			count++;
		}
	}
	FileString[length] = '\0';
	if (!(PtrArr = (char**) calloc(count, sizeof(char*))))
	{
		printf("calloc: error: %s\n", strerror(errno));
		exit(-1);
	}
	PtrArr[0] = &(FileString[0]);
	for (i = 0; i < length; i++)
	{
		if (FileString[i] == '\n')
		{
			FileString[i] = '\0';
			PtrArr[j] = &(FileString[i + 1]);
			j++;
		}
	}
	return PtrArr;
}
