/*
Name : Budda Mohan Chandu
Roll No: 20CS10017
Name : Azad
Roll No: 20CS10032
Assignment-4
*/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_HEADER_LEN 200
#define MAX_CONTENT_LEN 10000
#define DEFAULT_CONTENT_TYPE "text/*"

void GETR(char *cmd, char *rsp, char *entity, char *extn)
{

    char response[MAX_HEADER_LEN + MAX_CONTENT_LEN];
    char content_type[MAX_HEADER_LEN];
    char expires[MAX_HEADER_LEN],current[MAX_HEADER_LEN];
    char cache_control[MAX_HEADER_LEN];
    char content_language[MAX_HEADER_LEN];
    int content_length;
    char last_modified[MAX_HEADER_LEN];

    time_t now = time(NULL);
    time_t expires_time = now + 3 * 24 * 60 * 60;
    time_t t2 = now - 2 * 24 * 60 * 60;
    struct tm *expires_gmt = gmtime(&expires_time);
    struct tm *current_gmt=gmtime(&now);
    strftime(current, MAX_HEADER_LEN, "%a, %d %b %Y %H:%M:%S GMT", current_gmt);
    strftime(expires, MAX_HEADER_LEN, "%a, %d %b %Y %H:%M:%S GMT", expires_gmt);
    strcpy(cache_control, "no-store");
    strcpy(content_language, "en-us");
    char *file_path;
    file_path = (char *)malloc(100 * sizeof(char));
    char *token = strtok(cmd, " ");
    if (strcmp(token, "GET") == 0)
    {
        token = strtok(NULL, " ");
        strcpy(file_path, token);
    }

    file_path += 1;
    printf("|%s|",file_path);
    FILE *file = fopen(file_path, "rb");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        if (errno == ENOENT) {
            strcpy(response,"HTTP/1.1 404 FILE NOT FOUND\r\n");
            strcat(response,"Time : ");
            strcat(response,current);
            strcat(response,"\r\n\r\n");
        }
        else if (errno == EACCES) {
            strcpy(response,"HTTP/1.1 403 FORBIDDEN\r\n");
            strcat(response,"Time : ");
            strcat(response,current);
            strcat(response,"\r\n\r\n");
        }

        strcpy(rsp,response);
        return;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    char *buffer = (char *)malloc(1000000 * sizeof(char));

    // Read the contents of the file into the buffer
    size_t bytes_read = fread(buffer, 1, file_size, file);

    // Null-terminate the buffer
    buffer[file_size] = '\0';
    content_length = file_size;
    // Get information about the file
    struct stat file_info;
    if (stat(file_path, &file_info) != 0)
    {
        printf("Could not get file info");
        return;
    }

    // Get the last modified time of the file
    time_t last_modified_time = file_info.st_mtime;
    if(last_modified_time<t2){
        printf("+---+");
        strcpy(response,"HTTP/1.1 404 FILE NOT FOUND(expired)\r\n");
        strcat(response,"Time : ");
        strcat(response,current);
        strcat(response,"\r\n\r\n");
        strcpy(rsp,response);
        return;
    }
    struct tm *lmt = gmtime(&last_modified_time);
    strftime(last_modified, MAX_HEADER_LEN, "%a, %d %b %Y %H:%M:%S GMT", lmt);

    char *ext = strchr(file_path, '.');
    strcpy(extn, ext);

    if (ext == NULL)
        strcpy(content_type, "text/*");
    else if (strcmp(ext, ".html") == 0)
        strcpy(content_type, "text/html");
    else if (strcmp(ext, ".pdf") == 0)
        strcpy(content_type, "application/pdf");
    else if (strcmp(ext, ".jpg") == 0)
        strcpy(content_type, "image/jpeg");
    else
        strcpy(content_type, "text/*");

    strcpy(response, "HTTP/1.1 200 OK\r\nExpires: ");
    strcat(response, expires);
    strcat(response, "\r\nCache-control: ");
    strcat(response, cache_control);
    strcat(response, "\r\nContent-language: ");
    strcat(response, content_language);
    strcat(response, "\r\nContent-length: ");
    char cnt_len[20];
    sprintf(cnt_len, "%d", content_length);
    strcat(response, cnt_len);
    strcat(response, "\r\nContent-type: ");
    strcat(response, content_type);
    strcat(response, "\r\nLast-Modified: ");
    strcat(response, last_modified);
    strcat(response, "\r\n\r\n");
    
    strcpy(entity, buffer);
    response[strlen(response)] = '\0';
    strcpy(rsp, response);

}

void PUTR(char *cmd, char *rsp, char *extn,char* f_path)
{

    char response[MAX_HEADER_LEN + MAX_CONTENT_LEN];
    char content_type[MAX_HEADER_LEN];
    char expires[MAX_HEADER_LEN];
    char cache_control[MAX_HEADER_LEN];
    char content_language[MAX_HEADER_LEN];
    int content_length;
    char last_modified[MAX_HEADER_LEN];
    char file_path[200];

    time_t now = time(NULL);
    time_t expires_time = now + 3 * 24 * 60 * 60;
    struct tm *expires_gmt = gmtime(&expires_time);

    strftime(expires, MAX_HEADER_LEN, "%a, %d %b %Y %H:%M:%S GMT", expires_gmt);
    strcpy(cache_control, "no-store");
    strcpy(content_language, "en-us");
    char* urls=strtok(cmd," ");
    char* to2=strtok(NULL," ");
    char* to3=strtok(NULL," ");
    strcpy(file_path,to2);
    strcat(file_path,to3);
    // char *urls = strchr(cmd, ' ')+1;
    
    // char *end2=strchr(urls,' ');
    // strncpy(file_path,urls,end2-urls);
    // file_path[end2-urls]='\0';

    // urls=strchr(urls,' ')+1;
    // char *end=strchr(urls,' ');
    // strncat(file_path,urls,end-urls+1);
    // file_path[end-urls+1]='\0';
    printf("|%s|\n",file_path);
    strcpy(f_path,file_path);
    char *ext = strchr(cmd, '.');
    strcpy(extn, ".*");
    if (ext == NULL)
        strcpy(content_type, "text/*");
    else if (strncmp(ext, ".html",5) == 0){
        strcpy(content_type, "text/html");
        strcpy(extn,".html");
    }

    else if (strncmp(ext, ".pdf",4) == 0){
        strcpy(content_type, "application/pdf");
        strcpy(extn,".pdf");
    }

    else if (strncmp(ext, ".jpg",4) == 0||strncmp(ext, ".jpeg",5) == 0){
        strcpy(content_type, "image/jpeg");
        strcpy(extn,".jpg");
    }

    else{
        strcpy(content_type, "text/*");
        char*tmp=malloc(30*sizeof(char));
        tmp=strtok(ext," ");
        strcpy(extn,tmp);

    }
    strcpy(response, "HTTP/1.1 200 OK\r\nExpires: ");
    strcat(response, expires);
    strcat(response, "\r\nCache-control: ");
    strcat(response, cache_control);
    strcat(response, "\r\nContent-language: ");
    strcat(response, content_language);
    strcat(response, "\r\nContent-type: ");
    strcat(response, content_type);
    strcat(response, "\r\n\r\n");
    response[strlen(response)] = '\0';
    strcpy(rsp, response);

}

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

void recievestr(int newsockfd, char *buffer)
{
    char buf[100];
    char comm[1000000];
    // comm=(char*)malloc((1000000*sizeof(char)));
    comm[0] = '\0';
    int sz;

    sz = recv(newsockfd, buf, 20, 0);
    strcpy(comm, buf);
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
    int sockfd; /* Socket descriptors */
//    int clilen;
    struct sockaddr_in serv_addr, google;

    int i;
//    char buf[400];
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Cannot create socket\n");
        exit(0);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(80);
    // serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Unable to bind local address\n");
        exit(0);
    }

    listen(sockfd, 5);
    while (1)
    {
        struct sockaddr_in cli_addr;
        int newsockfd, clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

        if (newsockfd < 0)
        {
            printf("Accept error\n");
            exit(0);
        }
        else
        {
            printf("Accepted\n");
        }
        int status = fork();
        if (status == 0)
        {
            close(sockfd);

            char *command;
            command = (char *)malloc(1000000 * sizeof(char));
            char *entity;
            entity = (char *)malloc(1000000 * sizeof(char));

            recievestr(newsockfd, command);
            printf("%s\n", command);
            FILE* log=fopen("AccessLog.txt","a");
            char date[200];
            char logs[1000];
            time_t now = time(NULL);
            struct tm *gmt = gmtime(&now);
            strftime(date, 200," %d:%m:%y %H:%M:%S ", gmt);
            strcpy(logs,date);
            strcat(logs,", ");
            strcat(logs,inet_ntoa(cli_addr.sin_addr));
            strcat(logs,", ");
            char port[20];
            sprintf(port, "%d", ntohs(cli_addr.sin_port));
            strcat(logs,(port));
            strcat(logs,", ");
            char *t;
            t= strtok(command,"\r\n");
            strcat(logs,t);
            fprintf(log,"%s\n",logs);
            fclose(log);
            char *rsp;
            rsp = (char *)malloc(1000000 * sizeof(char));
            char *ext;
            ext = (char *)malloc(100 * sizeof(char));
            char *file_path= (char *)malloc(200 * sizeof(char));
            if(strncmp(command,"GET",3)==0){
                GETR(command, rsp, entity, ext);
                if(strncmp(rsp+9,"200",3)!=0){
                    sendstr(newsockfd, rsp);
                }
                else{
                    sendstr(newsockfd, rsp);
                    char*pmt;
                    pmt = (char *)malloc(100 * sizeof(char));
                    recievestr(newsockfd,pmt);

                    char *file_path;
                    file_path = (char *)malloc(200 * sizeof(char));
                    char *token = strtok(command, " ");

                    if (strcmp(token, "GET") == 0)
                    {
                        token = strtok(command+4, " ");
                        // printf("%s\n",token);
                        strcpy(file_path, token);

                    }
                    file_path += 1;
                    FILE *fp = fopen(file_path, "rb");
                    if (fp == NULL) {
                        perror("fopen");
                        close(newsockfd);
                        continue;
                    }

                    size_t bytes_sent;
                    char response[1000];
                    do {
                        bytes_sent = fread(response, 1, 1000, fp);
                        if (ferror(fp)) {
                            perror("fread");
                            break;
                        }
                        if (send(newsockfd, response, bytes_sent, 0) < 0) {
                            perror("send");
                            break;
                        }

                    } while (bytes_sent > 0);

                    // Clean up
                    fclose(fp);
                }


            }
            else if(strncmp(command,"PUT",3)==0){
                PUTR(command, rsp, ext,file_path);
                    //sendstr(newsockfd,"Entity:");
                    
                    file_path+=1;
                    //printf("%s",file_path);
                    FILE *fp = fopen(file_path, "wb");
                    if (fp == NULL) {
                        perror("fopen");
                        sendstr(newsockfd,"HTTP/1.1 404 FILEPATH NOT FOUND\r\n\r\n");
                        close(newsockfd);
                        exit(0);
                    }
                    sendstr(newsockfd,rsp);
                    char response[1000];
                    // Receive the file data
                    int bytes_received;
                    do {
                        bytes_received = recv(newsockfd, response, 1000, 0);
                        if (bytes_received < 0) {
                            perror("recv");
                            break;
                        }
                        if (fwrite(response, 1, bytes_received, fp) < bytes_received) {
                            perror("fwrite");
                            break;
                        }

                    } while (bytes_received > 0);

                    // Clean up
                    fclose(fp);
                    char*fl=malloc(200*sizeof(char));
                    if(strcmp(ext,".pdf")==0)strcpy(fl,"open -a \"Adobe Acrobat Reader\" ");
                    else if(strcmp(ext,".html")==0 ||strcmp(ext,".jpg") ||strcmp(ext,".jpeg") )strcpy(fl,"open -a \"Google Chrome\" ");
                    else strcpy(fl,"open ");
                    strcat(fl,file_path);
                    system(fl);
                
            }
            else{
                strcpy(rsp,"HTTP/1.1 400 BAD REQUEST\r\n\r\n");
                sendstr(newsockfd,rsp);
            }

            exit(0);
        }
        else
        {
            wait(&status);
        }

        close(newsockfd);
    }
    return 0;
}