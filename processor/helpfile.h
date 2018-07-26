#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define OK 1
#define ERR 0
#define EPS DBL_EPSILON
#define REG 5
#define STACK_SIZE 30
#define REGISTR 100
#define RET_HELP 3
#define JUMP_HELP 1
#define LABEL_MAX 20


const double D_POISON = tan(M_PI / 2);
const int I_POISON = INT_MAX;
const int canary1 = 5;
const int canary2 = 6;

struct _Mark_t
{
	char Name[LABEL_MAX];
	int Num;
};
typedef struct _Mark_t Mark_t;

typedef double StackElem_t;

	struct _stack_t
	{
		int first;
		StackElem_t* data;
		int count;
		int hash;
		int size;
		int last;
	};

typedef struct _stack_t stack_t;
	
struct _struct_t
{
	stack_t* stack;
	StackElem_t* registr;
};

typedef struct _struct_t ACPU_t;



stack_t* c_tor (size_t size);
ACPU_t* StructConstruct ();
int* givememory_int (size_t size);
char** givememory_str (size_t size);
char* givememory_char (size_t size);
double* givememory_double (size_t size);
void free_memory (char** array, int size);
void Write (double* MainArr, int* trueheight, FILE* assem);
void Write_str (char** MainArr, int* height, FILE* out);
void Dump (stack_t* stack);
int StackOK (const stack_t* stack);
int Push (stack_t* stack,  StackElem_t value);
void StackDestruct (stack_t* stack);
void StructDestruct (ACPU_t* Struct);
int Hash (stack_t* stack);
int Pop (stack_t* stack, StackElem_t* buf);
int Pop_reg (stack_t* stack, StackElem_t* ptr_num);
int FinalCheck (stack_t* stack);
int Add (stack_t* stack);
int Sub (stack_t* stack);
int Mul (stack_t* stack);
int Div (stack_t* stack);
int Pow (stack_t* stack);
int Sqrt (stack_t* stack);
int Jmp (int num);
int Jbe (stack_t* stack, int num, int count);
int Jae (stack_t* stack, int num, int count);
int Jne (stack_t* stack, int num, int count);
int Je (stack_t* stack, int num, int count);
int Ja (stack_t* stack, int num, int count);
int Jb (stack_t* stack, int num, int count);
int Out (stack_t* stack);
int FileSize (FILE* file);
char** readtxt(FILE* text, int* height, char* FileString);
void check (FILE* text);
void ShowRegistr (ACPU_t* my_struct);
void StructArr_create (Mark_t** Arr, int* height);



void StructArr_create (Mark_t** Arr, int* height)
{
	assert(Arr);
	assert(height);
	if ((*Arr = (Mark_t*) calloc ((size_t) *height, sizeof(Mark_t))) == NULL)
	{
		perror("error in MarkArr calloc");
		exit(EXIT_FAILURE);
	}
}

stack_t* c_tor (size_t size)
{
	stack_t* stack;
	if (!(stack = (stack_t*) calloc (1, sizeof(stack_t))))
	{
		perror("stack_t: calloc error\n");
		exit(EXIT_FAILURE);
	}
	stack->data = givememory_double(size);
	stack->data[0] = canary1;
	int end = ((int) size) - 1;
	stack->data[end] = canary2;
	stack->count = 0;	
	stack->size = (int) size;
	stack->first = canary1;
	stack->last = canary2;
	return stack;
}

int Push (stack_t* stack,  StackElem_t value)
{
	assert(StackOK(stack));
	if (stack->count < ((stack->size) - 2))
	{
		stack->count++;
		stack->data[stack->count] = value;
		assert(StackOK(stack));
		return OK;
	}
	else
	{
		return ERR;
	}
}

int Pop (stack_t* stack, StackElem_t* buf)
{
	assert(StackOK(stack));
	if (stack->count > 0)
	{
		*buf = stack->data[stack->count];
		stack->data[stack->count] = 0;
		stack->count--;
		return OK;
	}
	else
	{
		return ERR;
	}
}


void StackDestruct (stack_t* stack)
{
	assert(StackOK(stack));
	assert(FinalCheck(stack));
	int i;
	for (i = 0; i < stack->size; i++)
		stack->data[i] = D_POISON;
	free (stack->data);
	stack->data = NULL;
	stack->count = I_POISON;
	stack->size = I_POISON;
	stack->hash = I_POISON;
	free (stack);
}

void Dump (stack_t* stack)
{
	int i = 0;
	assert(FinalCheck(stack));
	printf("Stack pointer is [%p]\n", stack);
	printf("data pointer is [%p]\n", stack->data);
	while (i < stack->size)
	{
		printf("data [%d] is %lg\n", i, stack->data[i]);
		i++;
	}
	printf("canary1 (must be equal to data[0]) is %d\n", canary1);
	printf("canary2 (must be equal to data[%d]) is %d\n", (stack->size - 1), canary2);
	printf("count is %d\n", stack->count);
	printf("hash is %d\n", stack->hash);
	printf("size is %d\n", stack->size);
}

ACPU_t* StructConstruct ()
{
	ACPU_t* Struct;
	if (!(Struct = (ACPU_t*) calloc (1, sizeof(ACPU_t))))
	{
		perror("ACPU_t: calloc error\n");
		exit(EXIT_FAILURE);
	}
	Struct->registr = givememory_double(REG);
	(Struct->registr)[REG - 1] = '\0';
	return Struct;
}

void StructDestruct (ACPU_t* Struct)
{
	assert(Struct);
	assert(Struct->registr);
	int i;
	for (i = 0; i < REG; i++)
		(Struct->registr)[i] = D_POISON;
	free (Struct->registr);
	Struct->registr = NULL;
	free (Struct);
}


int StackOK (const stack_t* stack)
{
	if (stack == NULL)
	{
		printf("StackPointer is NULL\n");
		return ERR;
	}
	else if (stack->count < 0)
	{
		printf("Counter of stack failed\n");
		return ERR;
	}
	else if (stack->data == NULL)
	{
		printf("data pointer is NULL\n");
		return ERR;
	}
	else if (fabs(stack->data[0] - canary1) > EPS)
	{
		printf("canary1 was changed\n");
		return ERR;
	}
	else if (fabs(stack->data[stack->size - 1] - canary2) > EPS)
	{
		printf("canary2 was changed\n");
		return ERR;
	}
	else if (stack->first != canary1)
	{
		printf("first canary was changed\n");
		return ERR;
	}
	else if (stack->last != canary2)
	{
		printf("last canary was changed\n");
		return ERR;
	}
	else
		return OK;
}

int Hash (stack_t* stack)
{
	int i = 0;
	int sum = 0;
	assert(StackOK(stack));
	for (i = 0; i < stack->size; i++)
		sum += (int) stack->data[i];
	sum += (stack->first + stack->last + stack->count + stack->size);
	return sum;
}

int FinalCheck (stack_t* stack)
{
	assert(StackOK(stack));
	if (Hash(stack) - stack->hash > EPS)
	{
		printf("hash is incorrect\n");
		return ERR;
	}
	else
		return OK;
}

int FileSize (FILE* file)
{
	assert(file);	
	long int length;	
	if (fseek(file, 0, SEEK_END) == -1)
	{
		perror("fseek crashed");
		exit(EXIT_FAILURE);
	}
	length = ftell(file);
	rewind(file);
	if (length <= 0)
	{
		printf("error of reading");
		exit(EXIT_FAILURE);
	}
	return (int) length;
}

char** readtxt(FILE* text, int* height, char* FileString)
{
	assert(text);
	assert(height);
	assert(FileString);
	int i, j = 1, count = 0;
	size_t k;
	char** PtrArr;	
	if ((k = fread(FileString, sizeof(char), (size_t) *height, text)) != (size_t) *height)
	{
		printf("read wrong");
		exit(EXIT_FAILURE);
	}
	FileString[*height] = '\n';
	for (i = 0;i < *height + 1; i++)
	{
		if ((FileString[i] == '\n') || (FileString[i] == ' '))
		{
			count++;
		}
	}
	FileString[*height] = '\0';
	if (!(PtrArr = (char**) calloc((size_t) count, sizeof(char*))))
	{
		perror("calloc readtxt: error:\n");
		exit(EXIT_FAILURE);
	}
	PtrArr[0] = &(FileString[0]);
	for (i = 0; i < *height; i++)
	{
		if ((FileString[i] == '\n') || (FileString[i] == ' '))
		{
			FileString[i] = '\0';
			PtrArr[j] = &(FileString[i + 1]);
			j++;
		}
	}
	*height = count;
	return PtrArr;
}

void check (FILE* text)
{
	if (text == NULL)
	{
		printf("error of file pointer\n");
		exit(EXIT_FAILURE);
	}
}

int* givememory_int (size_t size)
{
	int* IntArr;
	if (!(IntArr = (int*) calloc (size + 1, sizeof(int))))
	{
		perror("IntArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	IntArr[size] = '\0';
	return IntArr;
}

char** givememory_str (size_t size)
{
	char** PtrArr;
	if (!(PtrArr = (char**) calloc (size, sizeof(char*))))
	{
		perror("PtrArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	return PtrArr;
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

double* givememory_double (size_t size)
{
	double* DobArr;
	if (!(DobArr = (double*) calloc (size + 1, sizeof(double))))
	{
		perror("CharArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	DobArr[size] = '\0';
	return DobArr;
}

void Write (double* MainArr, int* height, FILE* out)
{
	assert(MainArr);
	assert(height);
	assert(out);
	int i;
	for (i = 0; i < *height; i++)
	{
		fprintf(out, "%lg", MainArr[i]);
		if (1 != fwrite(&"\n", sizeof(char), 1, out))
		{
			printf("write wrong");
			exit(EXIT_FAILURE);
		}
	}
}

void Write_str (char** MainArr, int* height, FILE* out)
{
	assert(MainArr);
	assert(height);
	assert(out);
	int i;
	for (i = 0; i < *height; i++)
	{
		fprintf(out, "%s", MainArr[i]);
		if (1 != fwrite(&"\n", sizeof(char), 1, out))
		{
			printf("write wrong");
			exit(EXIT_FAILURE);
		}
	}
}

void free_memory (char** array, int size)
{
	assert(array);
	int count = 0;
	for (count = 0; count < size; count++)
	{
		free(array[count]);
	}
	free(array);
	array = NULL;
}

int Add (stack_t* stack)
{
	assert(StackOK(stack));
	StackElem_t result = 0;
	int i = 0;
	if (stack->count > 1)
	{
		while (i < 2)
		{
			result += stack->data[stack->count];
			stack->data[stack->count] = 0;
			stack->count--;
			i++;
		}
		stack->count++;
		stack->data[stack->count] = result;
		return OK;
	}
	else
	{
		return ERR;
	}
}

int Sub (stack_t* stack)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		stack->data[(stack->count) - 1] -= stack->data[stack->count];
		stack->data[stack->count] = 0;
		stack->count--;
		return OK;
	}
	else
	{
		return ERR;
	}
}

int Div (stack_t* stack)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{	
		if (fabs(stack->data[stack->count]) < EPS)
		{
			printf("try to divide by zero\n");
			return ERR;
		}
		else
		{
			stack->data[(stack->count) - 1] /= stack->data[stack->count];
			stack->data[stack->count] = 0;
			stack->count--;
			return OK;
		}
	}
	else
	{
		return ERR;
	}
}

int Mul (stack_t* stack)
{
	assert(StackOK(stack));
	StackElem_t result = 1;
	int i = 0;
	if (stack->count > 1)
	{
		while (i < 2)
		{
			result *= stack->data[stack->count];
			stack->data[stack->count] = 0;
			stack->count--;
			i++;
		}
		stack->count++;
		stack->data[stack->count] = result;
		return OK;
	}
	else
	{
		return ERR;
	}
}

int Sqrt (stack_t* stack)
{
	if (stack->count > 0)
	{
		stack->data[stack->count] = sqrt(stack->data[stack->count]);	
		return OK;
	}
	else
	{
		return ERR;
	}
}

int Pow (stack_t* stack)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		stack->data[stack->count - 1] = pow (stack->data[stack->count], stack->data[stack->count - 1]);
		stack->data[stack->count] = 0;
		stack->count--;
		return OK;
	}
	else
	{
		return ERR;
	}
}

int Je (stack_t* stack, int num, int count)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		if (fabs(stack->data[stack->count] - stack->data[(stack->count) - 1]) <= EPS)
			return num - 2;
		else
			return count;
	}
	else
	{
		printf("error in je\n");
		exit(EXIT_FAILURE);
	}
}

int Jne (stack_t* stack, int num, int count)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		if (fabs(stack->data[stack->count] - stack->data[(stack->count) - 1]) > EPS)
			return num - 2;
		else
			return count;
	}
	else
	{
		printf("error in jne\n");
		exit(EXIT_FAILURE);
	}
}

int Ja (stack_t* stack, int num, int count)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		if (stack->data[stack->count] > stack->data[(stack->count) - 1])
			return num - 2;
		else
			return count;
	}
	else
	{
		printf("error in ja\n");
		exit(EXIT_FAILURE);
	}
}

int Jb (stack_t* stack, int num, int count)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		if (stack->data[stack->count] < stack->data[(stack->count) - 1])
			return num - 2;
		else
			return count;
	}
	else
	{
		printf("error in jb\n");
		exit(EXIT_FAILURE);
	}
}

int Jae (stack_t* stack, int num, int count)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		if (stack->data[stack->count] >= stack->data[(stack->count) - 1])
			return num - 2;
		else
			return count;
	}
	else
	{
		printf("error in jae\n");
		exit(EXIT_FAILURE);
	}
}

int Jbe (stack_t* stack, int num, int count)
{
	assert(StackOK(stack));
	if (stack->count > 1)
	{
		if (stack->data[stack->count] <= stack->data[(stack->count) - 1])
			return num - 2;
		else
			return count;
	}
	else
	{
		printf("error in jbe\n");
		exit(EXIT_FAILURE);
	}
}

int Pop_reg (stack_t* stack, StackElem_t* ptr_num)
{
	assert(StackOK(stack));
	assert(ptr_num);
	if (stack->count > 0)
	{
		*ptr_num = stack->data[stack->count];
		stack->data[stack->count] = 0;
		stack->count--;
		return OK;
	}
	else
	{
		return ERR;
	}
}

void ShowRegistr (ACPU_t* my_struct)
{
	int count = 0;
	int letter = 'a';
	while (count < (REG - 1))
	{
		printf("register %cx  = %lg\n", (letter + count),  my_struct->registr[count]);
		count++;
	}
}

int Out (stack_t* stack)
{
	if (stack->count > 0)
	{
		printf("out: %lg\n", stack->data[stack->count]);
		return OK;
	}
	else
		return ERR;
}

