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
#include <time.h>
#include <stdlib.h>  
#

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
    srand(time(NULL));
    int sockfd, new_sockfd, client_len, server_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[200];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(":( socket not created!\n");
        exit(0);
    }
    else
    {
        printf(" socket created successfully!\n");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    //server_addr.sin_port = htons(20000);
    server_addr.sin_port=htons(atoi(argv[1]));
    
    server_len = 0;
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror(":( unable to bind the local address!\n");
        exit(0);
    }
    else
    {
        printf(" binding successful!\n");
    }
    listen(sockfd, 5);
    printf(" Im listening!\n");

    while (1)
    {
        client_len = sizeof(client_addr);
        if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len)) < 0)
        {
            perror(":( unable to accept\n");
            exit(0);
        }
        // else
        // {
        //     printf(":) connection accepted!\n");
        // }

        
            memset(buffer,100,'\0');
            recievestr(new_sockfd,buffer);
            if(strcmp(buffer,"SEND LOAD\0")==0){
                int load;
                
                load=rand()%100+1;
                
                memset(buffer,100,'\0');
                sprintf(buffer, "%d", load);
                sendstr(new_sockfd,buffer);
                printf(" Load sent: %d\n",load);
                
            }
            else if(strcmp(buffer,"SEND TIME\0")==0){
                time_t t; // not a primitive datatype
                time(&t);
                memset(buffer,100,'\0');
                strcpy(buffer, ctime(&t));
                sendstr(new_sockfd,buffer);
                //printf(" Time sent!\n");
                
            }
            
        
        close(new_sockfd);
    }

    close(sockfd);
    return 0;
}