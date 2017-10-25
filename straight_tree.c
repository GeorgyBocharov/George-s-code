#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{	
	int  i = 0, n, *id;
	pid_t father, pid;
	id = (int*) calloc (n, sizeof (int));
	/*
	fixit: не кажется странным, что вы выделили память до того, как узнали количество процессов?
	*/
	printf ("enter amount of processes  ");
	scanf ("%d", &n);
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
			break;
			/*
			если я правильно понял, то здесь вместо break вы как раз можете написать exit.
			и тогда цикл ниже можно будет удалить
			*/
		}
	}
	for (i = 0;i < n; i++)
	{
		if (id[i] == getpid ())
			exit (i);
	}
	return 0;
}
