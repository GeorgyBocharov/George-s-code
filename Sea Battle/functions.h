#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <wait.h>
#include <ncurses.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 11
#define UNITS 10
#define MAX_QUEUE_SIZE 10

typedef int sct_t;
typedef struct sockaddr_in Addr;


int*	AllocateMemoryInt (size_t size);
char**	AllocateMemoryStr (size_t size);
char*	AllocateMemoryChar (size_t size);
char**	AllocateMemoryField ();
int		FileSize (FILE* file);
void	Check (FILE* text);
char**	ReadTxt(FILE* text, int* height, char* fileString);
void	CheckStr (char* word);
void	FreeMemoryField (char** field);
int		Shoot (char** field, char** ptrArr);
int		CheckCoord (char hor, int y);
void	ServerPlay(sct_t* clients);
int		CheckVal (char sym);
void	ServerExch(sct_t* clients);
void	History (char** field);
void	Server ();
void	ShowField (char** field);
int		CheckDestruction(char** field, int x, int y);
int		HaveGot (char* msg);
void	SetAddrAny(Addr* addr);
int		SendAll(int s, char* buf, int len, int flags);
int		AddClients(sct_t* clients, sct_t newone);
void	Send (int sockfd, char* str, struct sockaddr * servptr);
unsigned int StrToIp(const char* ipstr);




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

char** ReadTxt(FILE* text, int* height, char* fileString)
{
	Check (text);
	assert(height);
	assert(fileString);
	int i, j = 1, count = 0;
	size_t k;
	char** ptrArr;	
	if ((k = fread(fileString, sizeof(char), (size_t) *height, text)) != (size_t) *height)
	{
		printf("read wrong");
		exit(EXIT_FAILURE);
	}
	fileString[*height] = '\n';
	for (i = 0; i < *height + 1; i++)
	{
		if (fileString[i] == '\n') 
		{
			count++;
		}
	}
	fileString[*height] = '\0';
	ptrArr = AllocateMemoryStr ((size_t) count);
	ptrArr[0] = &(fileString[0]);
	for (i = 0; i < *height; i++)
	{
		if (fileString[i] == '\n')
		{
			fileString[i] = '\0';
			ptrArr[j] = &(fileString[i + 1]);
			j++;
		}
	}
	*height = count - 1;
	return ptrArr;
}

int* AllocateMemoryInt (size_t size)
{
	int* intArr;
	if (!(intArr = (int*) calloc (size + 1, sizeof(int))))
	{
		perror("intArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	intArr[size] = '\0';
	return intArr;
}

char** AllocateMemoryStr (size_t size)
{
	char** ptrArr;
	if (!(ptrArr = (char**) calloc (size, sizeof(char*))))
	{
		perror("ptrArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	return ptrArr;
}

char* AllocateMemoryChar (size_t size)
{
	char* charArr;
	int count = 0;
	if (!(charArr = (char*) calloc (size + 1, sizeof(char))))
	{
		perror("charArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	for (count = 0; count < ((int)size); count++)
		charArr[count] = '0';
	charArr[size] = '\0';
	return charArr;
}

char** AllocateMemoryField ()
{
	char** fieldArr;
	int count = 0;
	if (!(fieldArr = (char**) calloc ((SIZE + 1), sizeof(char*))))
	{
		perror("fieldArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	fieldArr[SIZE] = NULL;
	for (count = 0; count < SIZE; count++)
		fieldArr[count] = AllocateMemoryChar (SIZE);
	for (count = 1; count < SIZE; count++)
	{
		fieldArr[0][count] = (char) ('A' + count - 1);
		fieldArr[count][0] = (char) ('0' + count);
	}
	fieldArr[SIZE - 1][0] = 'Z';
	return fieldArr;
}

void FreeMemoryField (char** field)
{
	int count = 0;
	for (count = 0; count < SIZE; count++)
		free(field[count]);
	free(field);
}

void Check (FILE* text)
{
	if (text == NULL)
	{
		printf("fopen: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void CheckStr (char* word)
{
	int count = 0;
	while (word[count] != '\0')
	{
		if (isdigit(word[count]) == 0)
		{
			printf("arguments should have format integer, but it is '%s'\n", word);
			exit(EXIT_FAILURE);
		}
		count++;
	}
}

int Shoot (char** field, char** ptrArr)
{
	int y = 0, x = 'A' - 1;
	char hor;
	printf("enter coordinates of the target (x and y)\n");
	while (scanf("%c %d", &hor, &y) < 2 || CheckCoord(hor, y) == 0)
		continue;
	x = ((int) hor) - x;
	if (ptrArr[y][x] == '0')
	{
		field[y][x] = '-';
		printf("you've missed\n");
		History (field);
		return 0;
	}
	else 
	{
		field[y][x] = 'x';
		ptrArr[y][x] = 'x';
		if (CheckDestruction(ptrArr, x, y) == 1)
		{
			printf("you've destroyed enemy's ship\n");
			History (field);
			return 1;
		}
		else
		{
			printf("you've damaged enemy's ship\n");
			History (field);
			return 2;
		}
	}
}

int CheckCoord (char hor, int y)
{
	if (('A' <= hor) && (hor <= 'A' + 9) && (y > 0) && (y <= 10))
		return 1;
	else
	{
		printf("error: sym1 = %c;sym2 = %d\n", hor, y);
		printf("wrong format of coordinates, try again\n");
		return 0;
	}
}

int CheckDestruction(char** field, int x, int y)
{
	if ((!CheckVal(field[y + 1][x])) || (!CheckVal(field[y - 1][x]))
	||	(!CheckVal(field[y][x + 1])) || (!CheckVal(field[y][x - 1])))
		return 0;
	else
		return 1;
}

int CheckVal (char sym)
{
	if ((sym == 'x') || (sym == '-') || (sym == '0'))
		return 1;
	else 
		return 0;
}

void History (char** field)
{
	int ans = 0;
	printf("do not forget history\n");
	scanf("%d", &ans);
	if (ans == 1)
		ShowField(field);
}

void ShowField (char** field)
{
	int vert = 0, hor = 0;
	//initscr();
	for (vert = 0; vert < SIZE; vert++)
	{
		for (hor = 0; hor < SIZE; hor++)
		{
			printf("%c", field[vert][hor]);
			//refresh();
		}
		printf("\n");
		//refresh(); 
	}
	//getch();
	//clear();
	//endwin();
}


void SetAddrAny(Addr* addr)
{
	addr->sin_family		= AF_INET;
	addr->sin_port			= htons(51000);
	addr->sin_addr.s_addr	= htons(INADDR_ANY);//htonl(INADDR_ANY);
}

void Server()
{
	printf("server: started...\n");
	sct_t clients[2] = {};
	sct_t newone = 0;
	sct_t lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock == -1)
	{
		perror("server: socket");
		exit(EXIT_FAILURE);
	}
	Addr addr = {};
	SetAddrAny(&addr);
	printf("server: gonna bind\n");
	if (bind(lsock, (struct sockaddr*) &addr, sizeof(addr)) == -1)
	{
		perror("server: bind");
		exit(EXIT_FAILURE);
	}
	listen(lsock, MAX_QUEUE_SIZE);

	printf("server: gonna find some clients\n");
	while (1)
	{
		printf("ya zaebalsya\n");
		if ((newone = accept(lsock, NULL, NULL)) == -1)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("newone is %d\n", newone);
		if (newone == -1)
		{
			perror("Server: accept");
			break;
		}
		if (AddClients(clients, newone) == 2)
		{
			ServerExch(clients);
			ServerPlay(clients);
		}
	}
	close(lsock);
	exit(EXIT_FAILURE);
}

void ServerExch(sct_t* clients)
{
	char** canvas1 = (char**) calloc (SIZE, sizeof(char*));
	char** canvas2 = (char**) calloc (SIZE, sizeof(char*));
	int i = 0;
	for (i = 0; i < SIZE; i++)
		canvas1[i] = (char*) calloc (SIZE, sizeof(char));

	for (i = 0; i < SIZE; i++)
		canvas2[i] = (char*) calloc (SIZE, sizeof(char));

	for (i = 0; i < SIZE; i++)
	{
		recv(clients[0], canvas1[i], SIZE, 0);
		//printf("%s\n", canvas1[i]);
	}

	for (i = 0; i < SIZE; i++)
	{
		recv(clients[1], canvas2[i], SIZE, 0);
		//printf("%s\n", canvas2[i]);
	}

	for (i = 0; i < SIZE; i++)
		send(clients[0], canvas2[i], SIZE, 0);

	for (i = 0; i < SIZE; i++)
		send(clients[1], canvas1[i], SIZE, 0);
}

void ServerPlay(sct_t* clients)
{
	char msg[1];
	int got_flg = 0;
	SendAll(clients[0], "8", 1, MSG_CONFIRM);	
	SendAll(clients[1], "9", 1, MSG_CONFIRM);	
	while (1)
	{
		do
		{
			recv(clients[0], msg, 1, 0);				//shot
			printf("msg from client1 is %c\n", msg[0]);
			SendAll(clients[1], msg, 1, MSG_CONFIRM);	// shot
			got_flg = HaveGot(msg);
			//recv(clients[1], msg, MSG_LEN, 0);				//result 
			//sendall(clients[0], msg, MSG_LEN, MSG_CONFIRM);	//result
			bzero(msg, 1);
		} while ((got_flg != 3) && (got_flg != 0));

		if (got_flg == 3) break;
		
		do
		{
			recv(clients[1], msg, 1, 0);
			printf("msg fro, client2 is %c\n", msg[0]);
			SendAll(clients[0], msg, 1, MSG_CONFIRM);
			got_flg = HaveGot(msg);
			//recv(clients[0], msg, MSG_LEN, 0);
			//sendall(clients[1], msg, MSG_LEN, MSG_CONFIRM);
			bzero(msg, 1);
		} while ((got_flg != 3) && (got_flg != 0));

		if (got_flg == 3) break;
	}
}

int HaveGot (char* msg)
{
	int got = msg[0] - '0';
	printf("got_flg = %d\n", got);
	return got;
}


int SendAll(int s, char* buf, int len, int flags)
{
	int total = 0;
	ssize_t n = 0;

	while (total < len)
	{
		n = send(s, buf + total, (size_t) (len - total), flags);
		if ((int) n == -1) break;
		total += (int) n;
	}

	return (n==-1? -1: total);
}

int AddClients(sct_t* clients, sct_t newone)
{
	if	(!clients[0]) clients[0] = newone;
	else if (!clients[1]) 
	{
		clients[1] = newone;
		return 2; //means server may proceed for plaing with those two clients
	}

	return 1;
}

unsigned int StrToIp(const char* ipstr)
{
	int i = 0;
	unsigned int ip = 0;
	unsigned char value[sizeof(int)] = {};
	while (*ipstr)
	{
		if (isdigit((unsigned char)*ipstr)) 
		{
			value[i] *= 10;
			value[i] += *ipstr - '0';
		} else i++;
		ipstr++;
	}

	for (i = 0; i < sizeof(int); i++)
		*((unsigned char*)(&ip) + i) = value[i];
	
	return ip;
}

void Send (int sockfd, char* str, struct sockaddr * servptr)
{
	if (sendto(sockfd, str, 1, 0, servptr, sizeof(*servptr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
}

