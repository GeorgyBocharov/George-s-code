#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

const int th_amount = 4;
int height;
int rest_height;

struct struct_t
{
	pthread_t** idptr;
	double*** matrixptr_two;
	double*** matrixptr_ex;
	double*** matrixptr_one;
	int* horptr_two;
	int* horptr_one;
};


void* th_multiply (void* structure);
double** give_memory (int vert, int hor);
void scan_matrix (double*** matrix, int vert, int hor);
void print_matrix (double** matrix, int vert, int hor);
void free_memory (double** matrix, int vert);
void multiply (double*** matrixptr_one, int vert_one, int hor_one, 
			   double*** matrixptr_two, int hor_two,  double*** matrixptr_ex);

int main()
{
	int count, i = 0;
	double **matrix_one, **matrix_two, **matrix_ex;
	int vert_one = -1, hor_one = -1, vert_two = -1, hor_two = -1;
	int vert_ex, hor_ex;
	struct_t* my_struct;
	pthread_t* id;
	printf ("enter size of the first matrix (vert, then hor)\n");
	scanf ("%d%d", &vert_one, &hor_one);
	printf("enter size of the second matrix (vert, then hor)\n");
	scanf ("%d%d", &vert_two, &hor_two);
	if ((vert_one <= 0) || (vert_two <= 0) || (hor_one <= 0) || (hor_two <= 0))
	{
		printf("wrong value of some size\n");
		exit(EXIT_FAILURE);
	}
	if (hor_one != vert_two)
	{
		printf("wrong sizes of matrix, multiply isn't available\n");
		exit(EXIT_FAILURE);
	}
	if ((id = (pthread_t*) calloc (((size_t) th_amount), sizeof(pthread_t))) == NULL)
		{
			perror("<<number>> string of matrix: error in calloc");
			printf("\nnumber is:%d\n", count);
			exit(EXIT_FAILURE);
		}
	vert_ex = vert_one;
	hor_ex = hor_two;
	matrix_one = give_memory(vert_one, hor_one);
	matrix_two = give_memory(vert_two, hor_two);
	matrix_ex  = give_memory(vert_ex, hor_ex);

	printf("get ready to enter first matrix\n");
	scan_matrix(&matrix_one, vert_one, hor_one);

	printf("get ready to enter second matrix\n");
	scan_matrix(&matrix_two, vert_two, hor_two);


	my_struct->idptr			= &id;
	my_struct->horptr_two		= &hor_two;
	my_struct->matrixptr_two	= &matrix_two;
	my_struct->matrixptr_ex		= &matrix_ex;
	my_struct->horptr_one		= &hor_one;
	my_struct->matrixptr_one	= &matrix_one;

	long int start_time = clock();
	if ((vert_one / th_amount) >= 1)
	{
		height = vert_one / th_amount;
		rest_height = vert_one -  (th_amount - 1) * height ;
		printf("height = %d\n", height);
		printf("rest = %d\n", rest_height);
		for (count = 0; count < th_amount; count++)
		{
			if (pthread_create(&id[count], NULL, th_multiply, ((void*) my_struct)) != 0)
			{
				perror("thread <<number>>: error of thread creating");
				printf("number is:%d\n", count);
				exit(EXIT_FAILURE);
			}
			
		}
	}
	else
	{
		multiply(&matrix_one, vert_one, hor_one,
				 &matrix_two, vert_two, &matrix_ex);
	}
	for (i = 0; i < th_amount; i++)
		pthread_join(id[i],	NULL);
	printf("multiplied matrix is:\n");
	print_matrix (matrix_ex, vert_ex, hor_ex);
	free_memory(matrix_one, vert_one);
	free_memory(matrix_two, vert_two);
	free_memory(matrix_ex, vert_ex);
	long int end_time = clock();
	printf("time of multiply is %li\n", (end_time - start_time));
	return 0;
}


double** give_memory (int vert, int hor)
{
	int count;
	double** matrix;
	if ((matrix = (double**) calloc (((size_t) vert), sizeof(double*))) == NULL)
	{
		perror("matrix : error in calloc");
		exit(EXIT_FAILURE);
	}
	for (count = 0; count < vert; count++)
	{
		if ((matrix[count] = (double*) calloc (((size_t) hor), sizeof(double))) == NULL)
		{
			perror("<<number>> string of matrix: error in calloc");
			printf("\nnumber is:%d\n", count);
			exit(EXIT_FAILURE);
		}
	}
	return matrix;
}

void scan_matrix (double*** matr, int vert, int hor)
{
	int i, j;
	double **matrix = *matr;
	for (i = 0; i < vert; i++)
	{
		for (j = 0; j < hor; j++)
		{
			scanf("%lg", &matrix[i][j]);
		}
	}
	matr = &matrix;
}

void print_matrix (double** matrix, int vert, int hor)
{
	int i, j;
	for (i = 0; i < vert; i++)
	{
		for (j = 0; j < hor; j++)
		{
			printf("%lg\t", matrix[i][j]);
		}
		printf("\n");
	}
}

void multiply (double*** matrixptr_one, int vert_one, int hor_one, 
			   double*** matrixptr_two, int hor_two,  double*** matrixptr_ex)
{
	int i, j, k;
	double sum = 0;
	double** matrix_one = *matrixptr_one;
	double** matrix_two = *matrixptr_two;
	double** matrix_ex = *matrixptr_ex;
	for (i = 0; i < vert_one; i++)
	{
		for (k = 0; k < hor_two; k++)
		{
			sum = 0;
			for (j = 0; j < hor_one; j++)
			{
				sum += matrix_one[i][j] * matrix_two[j][k];
			}
			matrix_ex[i][k] = sum;
		}
	}
	matrixptr_ex = &matrix_ex;
}

void free_memory (double** matrix, int vert)
{
	int i;
	for (i = 0; i < vert; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
	matrix = NULL;
}

void* th_multiply (void* structure)
{
	struct_t* my_struct = ((struct_t*) structure);
	int i, j, k, need = 0, tall;
	int hor_two = *(my_struct->horptr_two);
	int hor_one = *(my_struct->horptr_one);
	double** matrix_two = *(my_struct->matrixptr_two);
	pthread_t* id = *(my_struct->idptr);
	double** matrix_one = *(my_struct->matrixptr_one);
	double sum = 0;
	double** matrix_ex = *(my_struct->matrixptr_ex);
	for (i = 0; i < th_amount; i++)
	{
		if (pthread_self() == id[i])
			need = i * height;
	}
	if (pthread_self() == id[th_amount - 1])
		tall = rest_height;
	else
		tall = height;
	for (i = 0; i < tall; i++)
	{			
		for (k = 0; k < hor_two; k++)
		{
			sum = 0;
			for (j = 0; j < hor_one; j++)
			{
				sum += matrix_one[need + i][j] * matrix_two[j][k];
			}
			matrix_ex[need + i][k] = sum;
		}
	}
	return NULL;
}
