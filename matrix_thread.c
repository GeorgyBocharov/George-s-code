#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define ARG 5
#define MAX_THREADS 20
int th_amount;
int height;
int rest_height;

struct _struct_t
{
	/*
	можете комментарием пояснить, зачем вам нужно так много звездочек здесь?)
	что пойдет не так, если по одной звездочке в каждой строке убрать?*/
	/*
	-Звёздочки нужны, чтобы выделять память только на саму структуру, а не на каждый её элемент
	*/
	pthread_t** idptr;
	double*** matrixptr_two;
	double*** matrixptr_ex;
	double*** matrixptr_one;
	int* horptr_two;
	int* horptr_one;
};

typedef struct _struct_t struct_t;

void* th_multiply (void* structure);
double** allocate_memory (int vert, int hor);
void create_matrix (double*** matrix, int vert, int hor);
void print_matrix (double** matrix, int vert, int hor);
void free_memory (double** matrix, int vert);
void multiply (double*** matrixptr_one, int vert_one, int hor_one, 
			   double*** matrixptr_two, int hor_two,  double*** matrixptr_ex);

int main(int argc, char* argv[])
{
	
	int count = 0, i = 0;
	double **matrix_one, **matrix_two, **matrix_ex;
	int vert_one = -1, hor_one = -1, vert_two = -1, hor_two = -1;
	int vert_ex, hor_ex, amount, reply = 0;
	double result;
	struct_t* Info;
	pthread_t* id;
	
	printf("max amount of threads is %d\n", MAX_THREADS);
	printf("enter amount of processes\n(it should be less than amount of threads)\n");
	scanf("%d", &amount);
	clock_t time[amount], simple_time;

	if (argc != ARG)
	{
		printf("you entered too few arguments\n");
		exit(EXIT_FAILURE);
	}
	vert_one = atoi(argv[1]);
	hor_one = atoi(argv[2]);
	vert_two = atoi(argv[3]);
	hor_two = atoi(argv[4]);
	if (amount > MAX_THREADS)
	{
		printf("ERROR:\namount of processes is greater than amount of threads\n");
		exit(EXIT_FAILURE);
	}
	
	th_amount = 2;

	if (vert_one <= 0 || vert_two <= 0 || hor_one <= 0 || hor_two <= 0)
	{
		printf("wrong value of some size\n");
		exit(EXIT_FAILURE);
	}
	if (hor_one != vert_two)
	{
		printf("wrong sizes of matrix, multiply isn't available\n");
		exit(EXIT_FAILURE);
	}
	if ((Info = (struct_t*) calloc (1, sizeof(struct_t))) == NULL)
	{
		perror("error of struct Info calloc");
		exit(EXIT_FAILURE);
	}
	if ((id = (pthread_t*) calloc (MAX_THREADS, sizeof(pthread_t))) == NULL)
	{
		perror("error in calloc of id array");
		exit(EXIT_FAILURE);
	}
	vert_ex = vert_one;
	hor_ex = hor_two;
	matrix_one = allocate_memory(vert_one, hor_one);
	matrix_two = allocate_memory(vert_two, hor_two);
	matrix_ex  = allocate_memory(vert_ex, hor_ex);

	create_matrix(&matrix_one, vert_one, hor_one);
	create_matrix(&matrix_two, vert_two, hor_two);

	Info->idptr	     		= &id;
	Info->horptr_two		= &hor_two;
	Info->matrixptr_two  	= &matrix_two;
	Info->matrixptr_ex		= &matrix_ex;
	Info->horptr_one		= &hor_one;
	Info->matrixptr_one	    = &matrix_one;
	
	
	for (i = 0; i < amount; i++)
	{
		time[i] = clock();
		if ((vert_one / th_amount) >= 1)
		{
			height = vert_one / th_amount;
			rest_height = vert_one -  (th_amount - 1) * height ;
			for (count = 0; count < th_amount; count++)
			{
				if (pthread_create(&id[count], NULL, th_multiply, ((void*) Info)) != 0)
				{
					perror("thread <<number>>: error of thread creating");
					printf("number is:%d\n", count);
					exit(EXIT_FAILURE);
				}
			}
			for (count = 0; count < th_amount; count++)
				pthread_join(id[count],	NULL);
			time[i] = clock() - time[i];
			printf("time[%d] is %ld\n", i, time[i]);
			th_amount ++;
		}
		else
			break;
	}
	th_amount--;
	simple_time = clock();
	
	multiply(&matrix_one, vert_one, hor_one,
				 &matrix_two, vert_two, &matrix_ex);
	
	simple_time = clock() - simple_time;
	printf("simple_time is %ld\n", simple_time);
	printf("enter 1 to print the result of multiply\n");
	scanf("%d", &reply);
	if (reply == 1)
		print_matrix (matrix_ex, vert_ex, hor_ex);
	printf(" ______________\n");
	printf("|N ||| boost   |\n");
	for (i = amount - 1; i >= 0; i--)
	{
		result = (double) (simple_time) / (time[i]);
		printf("|__|||_________|\n");
		printf("|%d ||| %lg|\n", th_amount, result);
		th_amount--;
	}
	printf("|__|||_________|\n");
	free_memory(matrix_one, vert_one);
	free_memory(matrix_two, vert_two);
	free_memory(matrix_ex, vert_ex);
	free(Info);
	return 0;
}


double** allocate_memory (int vert, int hor)
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

void create_matrix (double*** matr, int vert, int hor)
{
	int i, j;
	double **matrix = *matr;
	for (i = 0; i < vert; i++)
	{
		for (j = 0; j < hor; j++)
		{
			matrix[i][j] = (double) (random() % 4);
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
	struct_t* Info = ((struct_t*) structure);
	int i, j, k, need = 0, tall;
	int hor_two = *(Info->horptr_two);
	int hor_one = *(Info->horptr_one);
	double** matrix_two = *(Info->matrixptr_two);
	pthread_t* id = *(Info->idptr);
	double** matrix_one = *(Info->matrixptr_one);
	double sum = 0;
	double** matrix_ex = *(Info->matrixptr_ex);
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
