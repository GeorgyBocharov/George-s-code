#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>

#define MAX_LENGTH 50

char* strscan ();
void Read (char* fifo_name);
void Write (char* fifo_name);


int main(int argc, char* argv[])
{
	char* fifo_name1 = argv[1];
	char* fifo_name2 = argv[2];
	mknod (fifo_name1, S_IFIFO | 0666, 0);
	mknod (fifo_name2, S_IFIFO | 0666, 0);
	pid_t result = fork();
	if (result == 0)
		Read(fifo_name1);
	else
		Write(fifo_name2);
	return 0;
}

void Read (char* fifo_name)
{
	long int size, bytesread;
	int fd = open(fifo_name, O_RDONLY);
	char* readstring = (char*) calloc (MAX_LENGTH, sizeof(char));
	while (1)
	{
		if ((bytesread = read(fd, readstring, MAX_LENGTH * sizeof(char))) == -1)
		{
			perror("error of reading");
			exit(EXIT_FAILURE);
		}
		if (bytesread  == 0)
		{
			printf("The second file exited");
			exit(EXIT_FAILURE);
		}
		printf("-  %s\n", readstring);
	}
	free(readstring);
}
	
void Write (char* fifo_name)
{
	long int size, bytesread;
	int fd = open(fifo_name, O_WRONLY);
	while (1)
	{
		char* str = strscan();
		if ((size = write(fd, str, strlen(str))) == -1)
		{
			perror("error of writing");
			exit(EXIT_FAILURE);
		}
		free(str);
	}
}




char* strscan ()
{
  int i = 0, counter = 1;
  char *s;
  if ( (s = (char*) calloc (1, sizeof(char))) == NULL)
    {
	  printf("strscan: s: calloc: %s\n", strerror(errno));
	  exit(-1);
	}
  while ((s[i] = getchar()) != '\n')
  {          
    if (i + 1 == counter)
    {
      counter *= 2;
	  if ( (s = (char*) realloc (s, counter * sizeof(char))) == NULL)
	  {
	    printf("strscan: s: realloc: %s\n", strerror(errno));
		exit(-1);
	  }
    }
    i++;
  }
  s[i] = '\0';
  return s;
}

