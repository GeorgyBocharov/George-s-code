#define FUNC 200
#define LABEL 300

void labels_funcs (Mark_t* Arr, int max, char* word, int id);
void pop_push (char** Arr, int place);
void check_str (char* word);
void check_prevcom(char** PtrArr, int count, int func_num);
void check_order(int* Ret_Arr, int max);
void check_regs (char* token);


int end = 0;

void labels_funcs (Mark_t* Arr, int max, char* word, int id)
{
	int count = 0;
	char symbol = '0';
	if (id == FUNC)
	{
		symbol = word[strlen(word) - 1];
		word[strlen(word) - 1] = '\0';
		if (end == 0)
		{
			printf("ERRROR, the body of function shuld be after command 'end'; function is '%s'\n", word);
			exit(EXIT_FAILURE);
		}

	}
	else if (id != LABEL)
	{
		printf("error of function labels_funcs\n");
		exit(EXIT_FAILURE);
	}
	for (count = 0; count < max; count++)
	{
		if (strcmp(Arr[count].Name, word) == 0)
		{
			printf("ERROR: try to duplicate %s\n",  word);
			exit(EXIT_FAILURE);
		}
	}
	if (id == FUNC)
		word[strlen(word)] = symbol;
}

void pop_push (char** Arr, int place)
{
	int count = -1;
	place++;
	check_str(Arr[place]);
	if (fabs(1  - atof(Arr[place])) < EPS)
	{
		count = place + 1;
		check_regs(Arr[count]);
	}
	else if (fabs(atof(Arr[place])) > EPS)
	{
		printf("first argument of push or pop must be '1' or '0', but it is '%s';token number %d\n", Arr[place], place);
		exit(EXIT_FAILURE);
	}
}

void check_str (char* word)
{
	int count = 0, dot = 0;
	while (word[count] != '\0')
	{
		if ((isdigit(word[count]) == 0) && (word[count] != '.'))
		{
			printf("arguments should have format double, but it is '%s'\n", word);
			exit(EXIT_FAILURE);
		}
		else if (word[count] == '.')
			dot++;
		count++;
	}
	if (dot > 1)
	{
		printf("double arguments, couldn't have more than one dot, but it is '%s'\n", word);
		exit(EXIT_FAILURE);
	}
}

void check_prevcom(char** PtrArr, int count, int func_num)
{
	count--;
	if ((PtrArr[count][0] != 'e') && (func_num == 0))
	{
		printf("before the  body of first function should be command 'end', but there is '%s'\n", PtrArr[count]);
		exit(EXIT_FAILURE);
	}
	else if ((PtrArr[count][0] != 'r') && (func_num > 0))
	{
		printf("after body of function there should be command 'ret', but there is the body of another function\n token number is %d\n", count);
		exit(EXIT_FAILURE);
	}
}

void check_order(int* Ret_Arr, int max)
{
	int count = 0;
	if (Ret_Arr[0] == 1)
	{
		printf("ERROR: command 'ret' couldn't be before function's body\n");
		exit(EXIT_FAILURE);
	}
	else if (Ret_Arr[max - 1] == 0)
	{
		printf("ERROR: last function doesn't have 'ret' command in the end of body\n");
		exit(EXIT_FAILURE);
	}
	for (count = 0; count < max - 1; count++)
	{
		if (Ret_Arr[count] == Ret_Arr[count + 1])
		{
			printf("ERROR: wrong oder of 'ret' and functions\n");
			exit(EXIT_FAILURE);
		}
	}
}

void check_regs (char* token)
{
	int check = 0;
	#define REGS(name, num)			\
	if (strcmp(token, #name) == 0)  \
		check++;
	#include"register.h"
	#undef REGS
	if (check != 1)
	{
		printf("ERROR: no such register '%s'\n", token);
		exit(EXIT_FAILURE);
	}
}
