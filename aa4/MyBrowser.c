/*
Name : Budda Mohan Chandu
Roll No: 20CS10017
Name : Azad
Roll No: 20CS10032
Assignment-4

QUIT

Bad request
DELETE http://127.0.0.1/sn.jpeg

PUT http://127.0.0.1/ lisa.jpg
PUT http://127.0.0.1/ network.pdf
PUT http://127.0.0.1/ network.html

GET http://127.0.0.1/sn.pdf
GET http://127.0.0.1/sn.html
GET http://127.0.0.1/sn.jpeg
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

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
    char buf[40000];
    char comm[1000000];
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

void GETH(char *buffer, int *port, char *reqq, char *host, char *extension,char * f_path)
{

    char req[1000000];
    char url[1000], hos[1000], usr[100], acc[100], lang[50], ims[200],dat[200],file_path[200];

    char *hoss = strstr(buffer, "://") + 3;
    char *hose = strchr(hoss, ':');
    if (hose == NULL)
        hose = strchr(hoss, '/');
    strncpy(hos, hoss, hose - hoss);
    hos[hose - hoss] = '\0';
    char *urls = strchr(hoss, '/');
    strcpy(url, urls);
    while(urls!=NULL){
        strcpy(file_path,urls+1);
        urls = strchr(urls+1, '/');     
    }
    strcpy(f_path,file_path);
    if (hose != strchr(hoss, '/'))
        *port = atoi(hose + 1);
    else
        *port = 80;
    char *ext = strchr(url, '.');
    strcpy(extension, ext);
    if (ext == NULL)
        strcpy(acc, "text/*");
    else if (strcmp(ext, ".html") == 0)
        strcpy(acc, "text/html");
    else if (strcmp(ext, ".pdf") == 0)
        strcpy(acc, "application/pdf");
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
        strcpy(acc, "image/jpeg");
    else
        strcpy(acc, "text/*");

    strcpy(req, "GET ");
    strcat(req, url);
    strcat(req, " HTTP/1.1\r\nHost: ");
    strcat(req, hos);
    strcat(req, "\r\nAccept: ");
    strcat(req, acc);
    strcat(req,"\r\nDate: ");
    time_t now = time(NULL);
    struct tm *gmt = gmtime(&now);
    strftime(dat, 200, "%a, %d %b %Y %H:%M:%S GMT", gmt);
    strcat(req, dat);
    strcat(req, "\r\nIf-Modified-Since: ");
    time_t ims_time = now - 2 * 24 * 60 * 60;
    struct tm *ims_gmt = gmtime(&ims_time);
    strftime(ims, 200, "%a, %d %b %Y %H:%M:%S GMT", ims_gmt);
    strcat(req, ims);
    strcat(req, "\r\nContent-Type: ");
    strcat(req, ext);
    strcat(req, "\r\nAccept-Language: en-us, en;q=0.8\r\nContent-Language: en-us\r\nConnection: close\r\n\r\n\0");

    strcpy(reqq, req);

    strcpy(host, hos);
}

void PUTH(char *buffer, int *port, char *reqq, char *host, char *extension,char*ent,char*f_path) {
    
    char req[1000000];
    char url[1000], hos[1000], usr[100], acc[100], ims[200],dat[200];
    int content_length;

    char *hoss = strstr(buffer, "://") + 3;
    char *hose = strchr(hoss, ':');
    if (hose == NULL)
        hose = strchr(hoss, '/');
    strncpy(hos, hoss, hose - hoss);
    hos[hose - hoss] = '\0';
    char *urls = strchr(hoss, '/');
    strcpy(url, urls);

    if (hose != strchr(hoss, '/'))
        *port = atoi(hose + 1);
    else
        *port = 80;
    char *ext = strchr(url, '.');
    strcpy(extension, ext);

    char *file_path;
    file_path = (char *)malloc(100 * sizeof(char));
    file_path=strchr(url,' ');

    file_path += 1;
    strcpy(f_path,file_path);
    
    FILE *file = fopen(file_path, "rb");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    char *buf = (char *)malloc(1000000 * sizeof(char));

    // Read the contents of the file into the buffer
    size_t bytes_read = fread(buf, 1, file_size, file);

    // Null-terminate the buffer
    buf[file_size] = '\0';
    strcpy(ent,buf);
    content_length = file_size;
    fclose(file);
       strcpy(req, "PUT ");
    strcat(req, url);
    strcat(req, " HTTP/1.1\r\nHost: ");
    strcat(req, hos);
    strcat(req,"\r\nDate: ");
    time_t now = time(NULL);
    struct tm *gmt = gmtime(&now);
    strftime(dat, 200, "%a, %d %b %Y %H:%M:%S GMT", gmt);
    strcat(req, dat);
    strcat(req, "\r\nContent-Type: ");
    strcat(req, ext);
    strcat(req, "\r\nContent-length: ");
    char cnt_len[20];
    sprintf(cnt_len, "%d", content_length);
    strcat(req, cnt_len);
    strcat(req, "\r\nContent-Language: en-us\r\nConnection: close\r\n\r\n\0");

    strcpy(reqq, req);

    strcpy(host, hos);
}


int main(int argc, char *argv[])
{

    

    while (1)
    {
        char *command;
        command= (char*)malloc((1000*sizeof(char*)));
         printf("MyOwnBrowser>");
         scanf("\n");
         scanf("%[^\n]s",command);
         int sockfd;
        struct sockaddr_in server_addr;
        char buffer[500];
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror(":( socket not created!\n");
            exit(0);
        }
        else
        {
            printf(":) socket created successfully!\n");
        }

        int p = 80;
        char *http_req;
        char *http_res;
        char *hos;
        char *ext;
        char*ent;
        char *file_path;
        file_path = (char *)malloc(200 * sizeof(char));
        http_req = (char *)malloc(10000 * sizeof(char));
        http_res = (char *)malloc(10000 * sizeof(char));
        hos = (char *)malloc(1000 * sizeof(char));
        ext = (char *)malloc(100 * sizeof(char));
        ent = (char *)malloc(100000* sizeof(char));

        if(strncmp(command,"GET",3)==0){
            GETH(command, &p, http_req, hos, ext,file_path);
            server_addr.sin_family = AF_INET;
            inet_aton(hos, &server_addr.sin_addr);
            server_addr.sin_port = htons(p);
            if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            {
                perror(":( Unable to connect to the server!\n");
                exit(0);
            }
            else
            {
                printf(":) connected to the server!\n");
            }
            sendstr(sockfd, http_req);
            struct pollfd timer;
            timer.fd=sockfd;
            timer.events=POLLIN;
            int rt=poll(&timer,1,3000);
            if(rt==0){
                printf("Time OUT\n");
                close(sockfd);
                continue;
            }
            recievestr(sockfd, http_res);
            printf("%s\n", http_res);
            if(strncmp(http_res+9,"200",3)==0){
                sendstr(sockfd,"Entity:");
                // Create a new file to store the file data
                FILE *fp = fopen(file_path, "wb");
                if (fp == NULL) {
                    perror("fopen");
                    exit(EXIT_FAILURE);
                }
                char response[1000];
                // Receive the JPG file data
                int bytes_received;
                do {
                    bytes_received = recv(sockfd, response, 1000, 0);
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
                if(fork()==0){
                    if(strcmp(ext,".pdf")==0)strcpy(fl,"open -a \"Adobe Acrobat Reader\" ");
                    else if(strcmp(ext,".html")==0 ||strcmp(ext,".jpg") ||strcmp(ext,".jpeg") )strcpy(fl,"open -a \"Google Chrome\" ");
                    else strcpy(fl,"open ");
                    strcat(fl,file_path);
                    system(fl);
                    //system("open -a \"Safari\" ans.jpg");
                    exit(1);
                }
            }
        }
        else if(strncmp(command,"PUT",3)==0) {
            PUTH(command, &p, http_req, hos, ext, ent, file_path);
            server_addr.sin_family = AF_INET;
            inet_aton(hos, &server_addr.sin_addr);
            server_addr.sin_port = htons(p);
            if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            {
                perror(":( Unable to connect to the server!\n");
                exit(0);
            }
            else
            {
                printf(":) connected to the server!\n");
            }

            sendstr(sockfd, http_req);
            char *pmt;
            pmt = (char *) malloc(100 * sizeof(char));
            //recievestr(sockfd, pmt);
            recievestr(sockfd,http_res);
            printf("%s\n",http_res);
            if(strcmp(http_res,"HTTP/1.1 404 FILEPATH NOT FOUND\r\n\r\n")==0)continue;
            FILE *fp = fopen(file_path, "rb");
            if (fp == NULL) {
                perror("fopen");
                close(sockfd);
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
                if (send(sockfd, response, bytes_sent, 0) < 0) {
                    perror("send");
                    break;
                }

            } while (bytes_sent > 0);

            // Clean up
            fclose(fp);

        }
        else if(strncmp(command,"QUIT",4)==0){
            break;
        }
        else{
            server_addr.sin_family = AF_INET;
            inet_aton("127.0.0.1", &server_addr.sin_addr);
            server_addr.sin_port = htons(p);
            if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            {
                perror(":( Unable to connect to the server!\n");
                exit(0);
            }
            else
            {
                printf(":) connected to the server!\n");
            }
            strcpy(http_req,"BAD REQUEST HTTP/1.1");
            sendstr(sockfd,http_req);
            recievestr(sockfd,http_res);
            printf("%s\n",http_res);
        }
        close(sockfd);
    }

    return 0;
}