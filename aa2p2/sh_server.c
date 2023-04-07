/*
Name : Budda Mohan Chandu
Roll No: 20CS10017
Assignment-2 -- part-2
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>

void sendstr(int newsockfd, char *buf)
{
	int l = strlen(buf);
	int x = 0;
	char temp[20];
	memset(temp, '\0', 20);
	for (int i = 0; i < l + 1; i++)
	{
		temp[x] = buf[i];
		x++;
		if (buf[i] == '\0')
		{
			send(newsockfd, temp, strlen(temp) + 1, 0);
			break;
		}
		if (x == 10)
		{
			send(newsockfd, temp, 10, 0);
			x = 0;
			memset(temp, '\0', 20);
		}
	}
	// printf("-");
}

void recievestr(int newsockfd, char buffer[])
{
	char buf[400];
	char comm[400];
	comm[0] = '\0';
	int sz;
	sz = recv(newsockfd, buf, 20, 0);
	strcat(comm, buf);
	while (buf[sz - 1] != '\0')
	{
		memset(buf, '\0', 100);
		sz = recv(newsockfd, buf, 20, 0);
		strcat(comm, buf);
	}
	strcpy(buffer, comm);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd; /* Socket descriptors */
	int clilen;
	struct sockaddr_in cli_addr, serv_addr;

	int i;
	char buf[400];
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Cannot create socket\n");
		exit(0);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(20000);
	// serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr *)&serv_addr,
			 sizeof(serv_addr)) < 0)
	{
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5);
	while (1)
	{
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0)
		{
			printf("Accept error\n");
			exit(0);
		}
		else
		{
			printf("Accepted\n");
		}
		if (fork() == 0)
		{
			char use[30];
			char *A;
			close(sockfd);
			A = "LOGIN\0";
			sendstr(newsockfd, A);
			memset(buf, '\0', 100);
			recievestr(newsockfd, buf);
			FILE *file;
			char str[30];
			file = fopen("users.txt", "r");
			int flag = 0;
			while (fgets(str, 30, file) != NULL)
			{
				str[strlen(str) - 1] = '\0';
				if (strcmp(str, buf) == 0)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				strcpy(use, buf);
				printf("%s hopped into the server\n", buf);
				A = "FOUND\0";
				sendstr(newsockfd, A);
			}
			else
			{

				A = "NOT-FOUND\0";
				sendstr(newsockfd, A);
				close(newsockfd);
				exit(0);
			}
			fclose(file);
			while (1)
			{

				char comm[400];
				comm[0] = '\0';
				int sz;
				if ((sz = recv(newsockfd, buf, 20, 0)) < 0)
					break;
				strcat(comm, buf);

				while (buf[sz - 1] != '\0')
				{
					memset(buf, '\0', 100);
					sz = recv(newsockfd, buf, 20, 0);
					strcat(comm, buf);
				}
				printf("+ %s tried to use command : %s\n", use, comm);
				int tc = 0;
				char *token;
				char *token1;
				char *token2;
				token = strtok(comm, " ");
				while (token != NULL)
				{
					tc++;
					if (tc == 1)
						token1 = token;
					if (tc == 2)
						token2 = token;

					token = strtok(NULL, " ");
				}
				memset(buf, 100, '\0');
				if (strcmp(token1, "exit\0") == 0)
				{
					printf("%s logged out!\n", use);
					break;
				}
				else if (tc == 0 || tc > 2)
				{
					strcpy(buf, "command invalid####\0");
				}
				else if (strcmp(token1, "pwd") == 0)
				{

					if (tc == 1)
					{

						if (getcwd(buf, sizeof(buf)) == NULL)
						{
							strcpy(buf, "error in getcwd\0");
						}
					}
					else
					{
						strcpy(buf, "command invalid####\0");
					}
				}
				else if (strcmp(token1, "dir") == 0)
				{
					if (tc == 1)
					{
						token2 = ".";
					}

					DIR *d;
					struct dirent *dir;
					d = opendir(token2);
					if (d)
					{
						while ((dir = readdir(d)) != NULL)
						{
							strcat(buf, dir->d_name);
							strcat(buf, "\n");
						}
						closedir(d);
					}
					else
					{
						strcpy(buf, "command invalid####\0");
					}
				}
				else if (strcmp(token1, "cd") == 0)
				{
					if (tc == 1)
					{
						strcpy(buf, "####invalid directory");
					}
					else if (chdir(token2) != 0)
					{
						strcpy(buf, "####invalid directory");
					}
					else
					{
						strcpy(buf, "changed directory successfully\0");
					}
				}
				else
				{
					strcpy(buf, "command invalid####\0");
				}

				//////////////////////////////////////////////

				A = buf;
				sendstr(newsockfd, A);
				// printf("\n");
				memset(buf, '\0', 400);
			}
			close(newsockfd);
			exit(0);
		}

		close(newsockfd);
	}
	return 0;
}
