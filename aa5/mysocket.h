#ifndef MYSOCKET_H
#define MYSOCKET_H


#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#define SOCK_MYTCP 100 // MyTCP socket type

typedef struct
{
    char *data;
    int size;
} message;

typedef struct
{
    int front;
    int back;
    int size;
    message msgs[10];
} table;


int my_socket(int domain, int type, int protocol);
int my_bind(int sockfd, struct sockaddr * saadr, socklen_t addrlen);
int my_listen(int sockfd, int number_of_connections);
int my_accept(int sockfd, struct sockaddr * saadr,socklen_t *addrlen);
int my_connect(int sockfd, struct sockaddr * saadr, socklen_t addrlen);
int my_send(int sockfd, char *buffer, int size,int flag); // my_send call behaves as follows: it puts the message in the Send_Message table if the table has a free entry and returns immediately. If the table does not have a free entry, it gets blocked until an entry is free.
int my_recv(int sockfd, char *buffer, int size,int flag);
int my_close(int sockfd); // The threads are killed and the data structures freed when the MyTCP socket is closed.
void* run_s();
void* run_r();


#endif
