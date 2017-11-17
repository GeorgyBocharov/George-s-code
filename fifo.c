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

char* strscan ();
void func_one (char* fifo_name);
void func_two (char* fifo_name);


int main(int argc, char* argv[])
{
	char* fifo_name1 = argv[1];
	char* fifo_name2 = argv[2];
	long int size, bytesread;
	mknod (fifo_name1, S_IFIFO | 0666, 0);
	mknod (fifo_name2, S_IFIFO | 0666, 0);
	pid_t result = fork();
	if (result == 0)
		func_one(fifo_name1);
	else
		func_two(fifo_name2);
	return 0;
}

void func_one (char* fifo_name)
{
	long int size, bytesread;
	int fd = open(fifo_name, O_RDONLY);
	while (1)
	{
		char* readstring = (char*) calloc (50, sizeof(char));
		if ((bytesread = read(fd, readstring, 50 * sizeof(char))) == -1)
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
		free(readstring);
	}
}
	
void func_two (char* fifo_name)
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

