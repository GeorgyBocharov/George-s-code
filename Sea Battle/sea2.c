#include "functions.h"



int main ()
{
	FILE* input = fopen("SeaBattle", "r");
	char** field = AllocateMemoryField();
	int height = 0, kills = 0, allies = UNITS, count = 0, get = -1;
	char* fileString, ipStr[] = "192.168.1.8";//"127.0.0.1";
	in_addr_t ip = inet_addr(ipStr);


	char **ptrArr, **enemyField;
	char indicator[1], reply[1];
	ssize_t n;
	Check(input);
	height = FileSize(input);
	printf("height is %d\n", height);
	fileString = AllocateMemoryChar ((size_t) height);
	ptrArr = ReadTxt (input, &height, fileString);
	printf("height is %d\n", height);
	enemyField = AllocateMemoryStr(SIZE);

	for (count = 0; count < SIZE; count++)
		enemyField[count] = AllocateMemoryChar(SIZE);

	
	int sockfd;
	Addr servaddr = {}, cliaddr = {};
	
	if ((sockfd = socket (PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("client: socket");
		exit(1);
	}
	/*cliaddr.sin_family		= AF_INET;
	cliaddr.sin_port		= htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*) &cliaddr, sizeof(Addr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}*/

	servaddr.sin_family		 = AF_INET;
	servaddr.sin_port		 = htons(51000);
	if (inet_aton(ipStr, &servaddr.sin_addr) == -1)
	{
		perror("inet_aton");
		exit(EXIT_FAILURE);
	}
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(Addr)) == -1)
	{
		perror("client: connect");
		close(sockfd);
		exit(1);
	}

	printf("Client: connected\n");
	//ShowField(ptrArr);
	for (count = 0; count < SIZE; count++)
		printf("%s\n", ptrArr[count]);


	for (count = 0; count < SIZE; count++)
	{
		if (sendto(sockfd, ptrArr[count], SIZE, 0, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0)
		{
			printf("count is %d\n", count);
			perror("sendto");
			close(sockfd);
			exit(1);
		}
	}

	for (count = 0; count < SIZE; count++)
	{
		if ((n = recvfrom (sockfd, enemyField[count], SIZE, 0, (struct sockaddr *) NULL, NULL)) < 0)
		{
		perror("recvfrom");
		close(sockfd);
		exit(1);
		}
	}
	printf("\nVRAZHYE POLE\n\n");
	ShowField (enemyField);
	printf("before main while\n");
	char startFlag[1];
	recv(sockfd, startFlag, 1, 0);
	if (startFlag[0] == '9') goto label;
	while (kills != UNITS)
	{
		do {
			get = Shoot(field, enemyField);
			if (get == 1)
				kills++;
			indicator[0] = (char) ('0' + get);
			Send (sockfd, indicator, (struct sockaddr *) &servaddr);
			printf("i ve sent smth\n");
		} while (get != 0);
		if (kills == UNITS)
			break;
		label:
		do {
			printf("please, wait for your turn\n");
			if ((n = recvfrom (sockfd, reply, 1, 0, (struct sockaddr *) NULL, NULL)) < 0)
			{
				perror("recvfrom:");
				close(sockfd);
				exit(1);
			}
			printf("i have got reply %c\n", reply[0]);
			if (reply[0] == '1')
				allies--;
			if (allies < 1)
			{
				indicator[0] = '3';
				Send (sockfd, indicator, (struct sockaddr *) &servaddr);
				close(sockfd);
				printf("your fleet is destroyed, welcome to the bottom\n");
				break;
			}
		}  while (reply[0] != '0'); 
		if (allies < 1)
			break;
	}
	if ((kills == UNITS) && (allies > 0))
	{
		indicator[0] = '3';
		Send (sockfd, indicator, (struct sockaddr *) &servaddr);
		printf("congratulations, you've won in this battle\n");
		close(sockfd);
	}
	ShowField(field);
	free (fileString);
	free (ptrArr);
	FreeMemoryField (field);
	return 0;
}



