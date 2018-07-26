#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_LENGTH 50

int main (int argc, char** argv)
{
	int sockfd;
	int n, len;
	char sendline[MAX_LENGTH], recvline[MAX_LENGTH];
	struct sockaddr_in servaddr, cliaddr;
	
	if (argc != 2)
	{
		printf("ERROR: too few args, arg number must be 2, but it is %d\n", argc);
		exit(1);
	}
	if ((sockfd = socket (PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*) &cliaddr, sizeof(cliaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP ADDRESS\n");
		close(sockfd);
		exit(1);
	}
	int count = 0;
	while(count < 25){
		count ++;
	printf("enter your string\n");
	fgets(sendline, MAX_LENGTH, stdin);
	if(strcmp(sendline, "system(exit)") == 0){
		close(sockfd);
		//exit(EXIT_SUCCESS);
		return 0;
	}	
	if (sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	if ((n = recvfrom (sockfd, recvline, MAX_LENGTH, 0, (struct sockaddr *) NULL, NULL)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	printf("%s\n", recvline);
	}
	close(sockfd);
	

	return 0;
}
