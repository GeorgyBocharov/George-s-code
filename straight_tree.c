#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{	
	int  i = 0, n, *id;
	pid_t father, pid;
	printf ("enter amount of processes  ");
	scanf ("%d", &n);
	id = (int*) calloc (n, sizeof (int));
	father = getpid();
	printf ("First process: %d\n", father);
	while (i != n)
	{
		pid = fork ();
		if (pid == -1)
		{
			printf ("ERROR");
			exit (-2);
			break;
		}
		else if (pid == 0)
		{
			printf ("father %d  me %d\n", getppid (), getpid ());
			id[i] = getpid ();
			i++;
		}
		else
		{
			waitpid (pid, &i, 0);
			printf ("I am %d,  my child %d died\n", getpid (),  pid);
			exit(-1);
		}
	}
	return 0;
}
