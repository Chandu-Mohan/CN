#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(){
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[100];
    char temp[11];
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(":( socket not created!\n");
        exit(0);
    }
    else
    {
        printf(":) socket created successfully!\n");
    }

    server_addr.sin_family=AF_INET;
    inet_aton("127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port=htons(20000);

    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
        perror(":( Unable to connect to the server!\n");
        exit(0);
    }
    else {
        printf(":) connected to the server!\n");
    }
    printf("Note: Give '?' after each expression\n");
    while(1){

        printf("\nGive next expression:::");
        memset(buffer,'\0',100);
        gets(buffer);
        
        int len=strlen(buffer);
        int x=0;
        while(x<=len){
            memset(temp,'\0',11);
            for(int i=0;i<10;i++){
                temp[i]=buffer[x+i];
            }
            x=x+10;
            send(sockfd, temp, strlen(temp) + 1, 0);
        }
        memset(temp,'\0',11);
        send(sockfd, temp, strlen(temp) + 1, 0);
        if(strcmp(buffer,"-1")==0){
            break;
        }
        recv(sockfd,buffer,100,0);
        printf("ans= %s",buffer);
    }

    close(sockfd);
    return 0;
}