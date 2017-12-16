#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <string.h>

#define MAX_SIZE 100
#define MAX_SYM 10


void RaseDir(char* path);
void add(char* path, char* next_dir);
char* givememory_char (size_t size);



enum FeedBack {
  Success,
  Failure
};

enum FeedBack search(char *path, int deep, const char *file, char* answer) 
{
	DIR* dir;
	struct dirent *active;
	int result;
	if (deep < 0) 
		return Failure;
	if((dir = opendir(path)) == NULL) 
	{
		perror("Can't open directory");
		exit(1);
    }
	while((active = readdir(dir)) != NULL) 
	{
		if(!strcmp(active->d_name, file))
		{
            		strcpy(answer, path);
            		return Success;
       	}
    }	
	rewinddir(dir);
    	while((active = readdir(dir)) != NULL)
    	{
        	if(active->d_type == 4 && strcmp(".", active -> d_name) && strcmp("..", active -> d_name)) 
        	{
        		add(path, active->d_name);
				result = search(path, deep - 1, file, answer);
				if(result == Success)
					return Success;
				else
					RaseDir(path);
        	}
    	}	
    	return Failure;
}



int main(int argc, char** argv) 
{
 	if (argc == 4) 
 	{ 
        int deep = atoi(argv[2]);
        char* directory;
        char* file;
        char* answer;
        int res = 0; 
        directory = givememory_char(MAX_SIZE);
        file = givememory_char(MAX_SIZE);
        answer = givememory_char(MAX_SIZE);
        strcpy(directory, argv[1]);
        strcpy(file, argv[3]);
		if (search(directory, deep, file, answer) == Success)
			printf("File %s was founded in %s directory\n", file, answer);
		else
			printf("File %s was not founded\n", file);
		free(directory);
        free(answer);
        free(file);
    	}
    	else 
			printf("Wrong number of arguments\n");
}



void RaseDir(char* path) 
{
	int len = 0;
	if (path == NULL) 
		return;
	len = strlen(path);
	do 
   	{
        path [len] = '\0';
		len--;
    }
	while (len != 0 && path [len] != '/');
		return;
}

void add(char* path, char* next_dir) 
{ 
	strcat(path, "/");
	strcat(path, next_dir);
	return;
}

char* givememory_char (size_t size)
{
	char* CharArr;
	if (!(CharArr = (char*) calloc (size + 1, sizeof(char))))
	{
		perror("CharArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	CharArr[size] = '\0';
	return CharArr;
}


