#include "mysocket.h"
pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex for send_buffer
pthread_mutex_t recv_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex for recv_buffer
pthread_t S;
pthread_t R;
int sock;
int newsock = -1;
int newsockvar = -1;
table *send_table;

table *recv_table;

void add_to_send_buffer(message *msg)
{
    pthread_mutex_lock(&send_mutex);
    // add to send buffer
    send_table->back %= 10;
    int x = send_table->back++;
    send_table->msgs[x].data=strdup( msg->data);
    send_table->msgs[x].size = msg->size;
    send_table->size++;
    pthread_mutex_unlock(&send_mutex);
}

char *delete_from_send_buffer()
{
    pthread_mutex_lock(&send_mutex);
    // delete from send buffer

    send_table->front %= 10;
    int x = send_table->front++;
    char *buf;
    buf=strdup(send_table->msgs[x].data);
    send_table->size--;
    pthread_mutex_unlock(&send_mutex);
    return buf;
}

void add_to_recv_buffer(message *msg)
{
    pthread_mutex_lock(&recv_mutex);
    // add to recv buffer
    recv_table->back %= 10;
    int x = recv_table->back++;
    recv_table->msgs[x].data=strdup( msg->data);
    recv_table->msgs[x].size = msg->size;
    recv_table->size++;
    pthread_mutex_unlock(&recv_mutex);
}
char *delete_from_recv_buffer()
{
    pthread_mutex_lock(&recv_mutex);
    // delete from recv buffer

    recv_table->front %= 10;
    int x = recv_table->front++;
    char *buf;
    buf=strdup( recv_table->msgs[x].data);
    recv_table->size--;
    pthread_mutex_unlock(&recv_mutex);

    return buf;
}

int my_socket(int domain, int type, int protocol)
{
    if(type!=SOCK_MYTCP)return -1;
    int sock1 = socket(domain, SOCK_STREAM, protocol);
    sock = sock1;
    send_table = (table *)malloc(sizeof(table));
    send_table->size = 0;
    send_table->front = 0;
    send_table->back = 0;
    recv_table = (table *)malloc(sizeof(table));
    recv_table->size = 0;
    recv_table->front = 0;
    recv_table->back = 0;
    
    for(int i=0;i<10;i++){
    	send_table->msgs[i].data=(char*)malloc(5001*sizeof(char));
    	recv_table->msgs[i].data=(char*)malloc(5001*sizeof(char));
    }

    // create thread r and s
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&R, &attr, run_r, NULL);
    pthread_create(&S, &attr, run_s, NULL);

    return sock1;
}

int my_bind(int sockfd, struct sockaddr *saadr, socklen_t addrlen)
{
    return bind(sockfd, saadr, addrlen);
}

int my_listen(int sockfd, int number_of_connections)
{
    return listen(sockfd, number_of_connections);
}

int my_accept(int sockfd, struct sockaddr *saadr, socklen_t *addrlen)
{
    newsock = accept(sockfd, saadr, addrlen);
    newsockvar = 1;
    return newsock;
}

int my_connect(int sockfd, struct sockaddr *saadr, socklen_t addrlen)
{
    int f = connect(sockfd, saadr, addrlen);
    newsock = sockfd;
    newsockvar = 1;
    return f;
}

int my_send(int sockfd, char *buffer, int size, int flag)
{
    message *msg = (message *)malloc(sizeof(message));
    msg->size = strlen(buffer);
    msg->data=strdup( buffer);
    while (1)
    {
        if (send_table->size < 10)
        {
            add_to_send_buffer(msg);
            break;
        }
    }
    return 1;
} // my_send call behaves as follows: it puts the message in the Send_Message table if the table has a free entry and returns immediately. If the table does not have a free entry, it gets blocked until an entry is free.

int my_recv(int sockfd, char *buffer, int size, int flag)
{
    // receive data
    char *buf;
    while (recv_table->size <= 0)
    {
    }
    if (recv_table->size > 0)
    {
        //printf("");
        buf=strdup( delete_from_recv_buffer()); // print the message
        strcpy(buffer, buf);
    }
    return strlen(buffer);
}
int my_close(int sockfd)
{

    // printf("a%d\n", recv_table->size);
    // printf("b%d\n", send_table->size);
    sleep(5);
    int x = close(sockfd);
    // printf("a%d\n", recv_table->size);
    // printf("b%d\n", send_table->size);
    if (sockfd == sock)
    {
        pthread_cancel(R);
        pthread_cancel(S);
    }
    if (sockfd == newsock)
    {
        free(send_table);
        free(recv_table);
    }

    return x;
}
void send_string(int sockfd, char *str)
{
    int len = 0;
    int temp = 0;
    char buf[11];
    memset(buf, '\0', 11);

    while (!temp)
    {
        for (int i = 0; i <= 10; i++)
            buf[i] = '\0';
        for (int i = 0; i < 10; i++)
        {
            if (str[len] == '\0')
            {
                buf[i] = str[len];
                temp = 1;
                break;
            }
            buf[i] = str[len++];
        }
        if (temp == 1)
            send(sockfd, buf, strlen(buf) + 1, 0);
        else
            send(sockfd, buf, 10, 0);
    }
    return;
}
void *run_s()
{
    char *buf;
    while (1)
    {
        // send data
        //printf("");
        // fflush(stdout);
        while (send_table->size == 0)
        {
        }
        while(newsockvar==-1);
        if (send_table->size > 0)
        {

            buf=strdup( delete_from_send_buffer());
            send_string(newsock, buf);
        }
    }
    // printf("run_s closed");
    // return NULL;
}
void recieve_string(int newsockfd)
{
    char buf[11];
    char comm[5000];
    comm[0] = '\0';
    int sz;
    sz = recv(newsockfd, buf, 1, 0);
    
    if (sz == 0 || buf[0] == '\0')
    {
        newsockvar = -1;
        return;
    }
    strcat(comm, buf);
    if (buf[0] == '\0')
    {
        // printf("empty string");
    }
    while (buf[sz - 1] != '\0')
    {
        memset(buf, '\0', 1);
        sz = recv(newsockfd, buf, 1, 0);
        strcat(comm, buf);
    }

    message *msg = (message *)malloc(sizeof(message));
    msg->size = strlen(comm);
    msg->data=strdup( comm);
    while (1)
    {
        if (recv_table->size < 10)
        {
            add_to_recv_buffer(msg);
            break;
        }
    }
    return;
}
void *run_r()
{

    char *str;

    str = (char *)malloc(5000 * sizeof(char));
    while (1)
    {
        while (newsockvar == -1)
            ;
        recieve_string(newsock);
    }
    return NULL;
}
