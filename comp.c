#include "helpfile.h"
#include "check_functions.h"

#define COM(name, num, execution) \
			CMD_##name = num,
#define IF_DUMMY if(int ayy = 0)	\
			ayy = 0;
enum {
	#include "commands.h"
	ENUM_DUMMY
 };
#undef COM

double* test(char** PtrArr, int* height, int* trueheight);
void fill (double** MainArr, int* count, char** PtrArr, int* count1, int arg);
int ifjump (char* string, Mark_t* Arr, int max);

int main(int argc, char* argv[])
{
	FILE* body, *assem;
	int height, trueheight;
	char *FileString;
	char** PtrArr;
	double* MainArr;
	if (argc != 2)
	{
		printf("wrong number of arguments; it must be 2, but it is %d\n", argc);
		exit(EXIT_FAILURE);
	}
	body = fopen(argv[1], "r");
	assem = fopen("asm.txt", "w");
	check(body);
	check(assem);
	height = FileSize(body);
	FileString = givememory_char((size_t) height + 1);
	PtrArr = readtxt(body, &height, FileString);
	fclose(body);
	MainArr = test(PtrArr, &height, &trueheight);
	Write (MainArr, &trueheight, assem);
	free(FileString);
	free(PtrArr);
	free(MainArr);
	fclose(assem);
	printf("Done\n");
	return 0;
}

double* test (char **PtrArr, int* height, int* trueheight)
{
	assert(PtrArr); 
	assert(trueheight);
	assert(height);
	double *MainArr = givememory_double((size_t) *height);
	Mark_t* Mark_Arr;
	Mark_t* Func_Arr;
	int* Ret_Arr;
	int count1 = 0, count2 = 0, count = 0, count3 = 0, ret_count = 0;
	StructArr_create(&Mark_Arr, height);
	StructArr_create(&Func_Arr, height);
		#define COM(name, num, execution)			  \
			if (strcmp(PtrArr[count1], #name) == 0)   \
			{								          \
				if (num == 7)						  \
					ret_count++;					  \
				if (num == 6)				          \
					end++;							  \
				if (CMD_##name < 10)		          \
					count++;				          \
				else if (CMD_##name < 20)	          \
					count += 2;                       \
				else if (CMD_##name < 30)             \
					count += 3;                       \
			}                                     
	for (count1 = 0; count1 < *height; count1++) 
	{
		#include "commands.h"
		else if (PtrArr[count1][0] == '.')                     
		{						                           
			labels_funcs(Mark_Arr, count2, &PtrArr[count1][1], LABEL);
			strcpy(Mark_Arr[count2].Name, &PtrArr[count1][1]); 
			Mark_Arr[count2].Num = count;                      
			count2++;                                            
		}
		else if (PtrArr[count1][(strlen(PtrArr[count1]) - 1)] == ':')
		{
			check_prevcom(PtrArr, count1, count3);
			labels_funcs(Func_Arr, count3, PtrArr[count1], FUNC);
			strcpy(Func_Arr[count3].Name, PtrArr[count1]);
			(Func_Arr[count3].Name)[(strlen(Func_Arr[count3].Name) - 1)] = '\0';
			Func_Arr[count3].Num = count;
			count3++;
		}
	}
	if (end != 1)
	{
		printf("the program must contain only one command 'end', but there are (is) %d such commands\n", end);
		exit(EXIT_FAILURE);
	}
	if (ret_count != count3)
	{
		printf("ERROR: amount of 'ret' commands isn't equal to amount of functions\n");
		exit(EXIT_FAILURE);
	}
	else 
		ret_count = 0;
	if (count3 != 0)
		Ret_Arr = givememory_int((size_t) (2 * count3));
	else
		Ret_Arr = NULL;
	count = 0;
	count1 = 0;
	#undef COM
		#define COM(name, num, execution)											      \
			else if (strcmp(PtrArr[count1], #name) == 0)								  \
			{																	          \
				MainArr[count] = CMD_##name;                                              \
				count++;                                                                  \
				if (num == 7)															  \
				{																		  \
					Ret_Arr[ret_count] = 1;												  \
					ret_count++;														  \
				}																		  \
				if ((num < 17) && (num > 9))											  \
				{                                                                         \
					MainArr[count] = ifjump(PtrArr[count1 + 1], Mark_Arr, count2);        \
					count1++;															  \
					count++;                                                              \
				}																          \
				else if (num == 17)			                                              \
				{																	      \
					MainArr[count] = ifjump(PtrArr[count1 + 1], Func_Arr, count3);        \
					count1++;															  \
					count++;														      \
				}																		  \
				else if ((CMD_##name < 20) && (CMD_##name > 9))                           \
					fill (&MainArr, &count, PtrArr, &count1, 1);                          \
				else if ((CMD_##name < 30) && (CMD_##name > 19))                          \
					fill (&MainArr, &count, PtrArr, &count1, 2);                          \
			}																			  
	for (count1 = 0; count1 < (*height - 1); count1++)
	{
		IF_DUMMY	
		#include"commands.h"
		else if ((PtrArr[count1][0] != '.') && (PtrArr[count1][(strlen(PtrArr[count1]) - 1)] != ':'))
		{
			printf("wrong number of arguments: token number %d\n", count1);
			exit(EXIT_FAILURE);
		}
		else if (PtrArr[count1][0] != '.')
		{
			Ret_Arr[ret_count] = 0;
			ret_count++;
		}
	}
		#undef COM	
	if (count3 != 0)
	{
		check_order(Ret_Arr, ret_count);
		free(Ret_Arr);
	}
	*trueheight = count;
	free(Func_Arr);
	free(Mark_Arr);
	return MainArr;
}

void fill (double** MainArr, int* count, char** PtrArr, int* count1, int arg)
{
	assert(MainArr);
	assert(count);
	assert(PtrArr);
	int i = 0;
	if  (strcmp(PtrArr[*count1], "push") == 0)
		pop_push(PtrArr, *count1);
	else if (strcmp(PtrArr[*count1], "pop_reg") == 0)
		check_regs(PtrArr[*count1 + 1]);
	#define REGS(name, num)							     \
		if (strcmp(PtrArr[*count1 + i + 1], #name) == 0) \
			(*MainArr)[*count + i] = num;                \
		else
	for (i = 0; i < arg; i++)
	{
		#include"register.h"
		{
			check_str(PtrArr[*count1 + i + 1]);
			(*MainArr)[*count + i] = atof(PtrArr[*count1 + i + 1]);
		}
	}
	#undef REGS
	*count = *count + arg;
	*count1 += arg;
}

int ifjump (char* string, Mark_t* Arr, int max)
{
	assert(string);
	assert(Arr);
	int count = 0;
	for (count = 0; count < max; count++)
	{
		if (strcmp(Arr[count].Name, string) == 0)
			return Arr[count].Num;
	}
    printf("ERROR of function ifjump\n");
	exit(EXIT_FAILURE);
}


