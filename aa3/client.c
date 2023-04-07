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
    // printf("\n");
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
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[100];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(":( socket not created!\n");
        exit(0);
    }
    // else
    // {
    //     printf(":) socket created successfully!\n");
    // }

    server_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &server_addr.sin_addr);
    //server_addr.sin_port = htons(20000);
    server_addr.sin_port=htons(atoi(argv[1]));

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror(":( Unable to connect to the server!\n");
        exit(0);
    }
    // else
    // {
    //     printf(":) connected to the server!\n");
    // }
    
        recievestr(sockfd,buffer);
        buffer[strlen(buffer)-7]=' ';
        printf("Time is :%s",buffer);
    

    close(sockfd);
    return 0;
}