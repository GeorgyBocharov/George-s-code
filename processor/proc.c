#include "helpfile.h"


void convert(char** PtrArr, StackElem_t** MainArr, int count);
void execute(stack_t* my_stack, StackElem_t* MainArr, int height, ACPU_t* my_struct);



int main()
{
	stack_t* my_stack;
	ACPU_t* my_struct; 
	size_t size = STACK_SIZE;
	FILE *assem;
	int height;
	char *FileString;
	char **PtrArr;
	StackElem_t* MainArr;
	my_stack = c_tor(size);
	my_struct = StructConstruct();
	my_struct->stack = my_stack;
	assem = fopen("asm.txt", "r");
	check(assem);
	height = FileSize(assem);
	FileString = givememory_char((size_t) height + 1);
	PtrArr = readtxt(assem, &height, FileString);
	fclose(assem);
	height--;
	MainArr = givememory_double((size_t) height);
	convert(PtrArr, &MainArr, height);
	execute(my_stack, MainArr, height, my_struct);
	ShowRegistr(my_struct);
	my_stack->hash = Hash(my_stack);
	Dump(my_stack);
	StackDestruct(my_stack);
	StructDestruct(my_struct);
	free(MainArr);
	free(FileString);
	free(PtrArr);
	return 0;
}

void convert(char** PtrArr, StackElem_t** MainArr, int height)
{
	assert(PtrArr);
	assert(MainArr);
	int count;
	for (count = 0; count < height; count++)
	{
		(*MainArr)[count] = atof(PtrArr[count]);
	}
}

void execute(stack_t* my_stack, StackElem_t* MainArr, int height, ACPU_t* my_struct)
{
	assert(my_stack);
	assert(MainArr);
	assert(my_struct);
	int count = 0;
	StackElem_t buf = 0;
	size_t size = STACK_SIZE;
	stack_t*  adress_stack;
	adress_stack = c_tor(size);
	#define COM(name, num, execution)                               \
		case num:													\
			execution												
	for (count = 0; count < height; count++)
	{
		switch ((int) MainArr[count]){
		#include"commands.h"
		default:
			{	
				printf("no such (%lg) command\n", MainArr[count]); 
				exit(EXIT_FAILURE);								
				break;											
			}
		}
	}
	#undef COM
	adress_stack->hash = Hash(adress_stack);
	StackDestruct(adress_stack);

}







