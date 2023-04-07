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
#include <poll.h>
#include <time.h>

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

void get_loads(){

}
int main(int argc, char *argv[])
{
	int sockfd,csockfd, newsockfd; /* Socket descriptors */
	int clilen;
	struct sockaddr_in cli_addr, serv_addr,serv_addr2;

	int i;
	char buf[400];
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Cannot create socket\n");
		exit(0);
	}
    
    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);

	serv_addr2.sin_family = AF_INET;
	serv_addr2.sin_addr.s_addr = INADDR_ANY;
	//serv_addr.sin_port = htons(20000);
	serv_addr2.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr *)&serv_addr2,
			 sizeof(serv_addr2)) < 0)
	{
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5);
    struct pollfd timer;
    timer.fd = sockfd;
    timer.events = POLLIN;
    long long int t2=1;
    int load1=1,load2=2;
	while (1)
	{
		long long int t1=1000*time(NULL);
        int r = poll(&timer, 1, t2);
        t2-=(long long int)(1000*time(NULL)-t1);
        
        if(t2<=0){
            //getting load
            t2=5000;
            if ((csockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror(":( socket not created!\n");
                exit(0);
            }
            

            serv_addr.sin_port = htons(atoi(argv[2]));
            if (connect(csockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                perror(":( Unable to connect to the server!\n");
                exit(0);
            }
            
            memset(buf,100,'\0');
            strcpy(buf,"SEND LOAD\0");
            sendstr(csockfd,buf);
            memset(buf,100,'\0');
            recievestr(csockfd,buf);
            load1=atoi(buf);
            printf("Load recived from %s ,%s ,S1: %d\n",inet_ntoa(serv_addr.sin_addr),argv[2],load1);
            close(csockfd);


            serv_addr.sin_port = htons(atoi(argv[3]));
            if ((csockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror(":( socket not created!\n");
                exit(0);
            }
            if (connect(csockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                perror(":( Unable to connect to the server!\n");
                exit(0);
            }
            
            memset(buf,100,'\0');
            strcpy(buf,"SEND LOAD\0");
            sendstr(csockfd,buf);
            memset(buf,100,'\0');
            recievestr(csockfd,buf);
            load2=atoi(buf);
            printf("Load recived from %s ,%s ,S2: %d\n",(inet_ntoa(serv_addr.sin_addr)),argv[3],load2);
            close(csockfd);
        }
        if(r>0){
            //recieve req from client
            if (fork() == 0)
            {
                clilen = sizeof(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
                if (newsockfd < 0)
                {
                    printf("Accept error\n");
                    exit(0);
                }
                // else
                // {
                //     printf("Accepted\n");
                // }
                close(sockfd);
                
                
                    
                    if(load1>load2){
                        serv_addr.sin_port = htons(atoi(argv[3]));
                        printf("Sending client request to %s ,%s ,S2\n",(inet_ntoa(serv_addr.sin_addr)),argv[3]);
                    }
                    else{
                        serv_addr.sin_port = htons(atoi(argv[2]));
                        printf("Sending client request to %s ,%s ,S1\n",(inet_ntoa(serv_addr.sin_addr)),argv[2]);
                    }

                    if ((csockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                    {
                        perror(":( socket not created!\n");
                        exit(0);
                    }
                    if (connect(csockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                    {
                        perror(":( Unable to connect to the server!\n");
                        exit(0);
                    }
                    // else
                    // {
                    //     printf(":) connected to the server!\n");
                    // }
                    strcpy(buf,"SEND TIME\0");
                    sendstr(csockfd,buf);
                    memset(buf,100,'\0');
                    recievestr(csockfd,buf);
                    
                    close(csockfd);

                    sendstr(newsockfd,buf);

                
                close(newsockfd);
                exit(0);
            }
        }
		
	}
	return 0;
}
