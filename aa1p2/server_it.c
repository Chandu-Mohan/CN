#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<ctype.h>

double dnum[1024];
int tn = -1, to = -1;
char oper[1024];

void push_num(double n)
{
    dnum[++tn]=n;
}
void push_op(char ch)
{
    oper[++to]=ch;
}
double pop_num()
{
    return dnum[tn--];
}
char pop_op()
{
    return oper[to--];
}
double infix_eval(double dnum[1024], char oper[1024])
{
    double x, y;
    char ope;
    x = pop_num();
    y = pop_num();
    ope = pop_op();

    switch (ope)
    {
    case '+':
        return x + y;
    case '-':
        return y - x;
    case '*':
        return x * y;
    case '/':
        if (x == 0)
        {
            printf("\nCan not divide by 0");
            exit(0);
        }
        else
            return ((double)y / x);
    }
    return 0;
}

int precedence(char c)
{
    switch (c)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    case '^':
        return 3;
    }
    return -1;
}
int is_operator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

double eval(char exp[1024])
{
    int i;
    double output,num,r;
    char c;
    for (i = 0; exp[i] != '\0'; i++)
    {
        c = exp[i];
        if(c==' '){

        }
        else if (isdigit(c) != 0)
        {
            num = 0;
            while (isdigit(c))
            {
                num = num * 10 + (c - '0');
                i++;
                if (i < strlen(exp))
                    c = exp[i];
                else
                    break;
            }
            if(c=='.'){
                i++;
                c=exp[i];
                double temp=10;
                while(isdigit(c)){
                    c=exp[i];
                    i++;
                    if (i < strlen(exp))
                        c = exp[i];
                    else
                        break;
                    num=num+(c-'0')/temp;
                    temp=temp*10;
                }
            }
            i--;
            //printf("||%f",num);
            push_num(num);
        }
        else if (c == '(')
        {
            push_op(c);
        }
        else if (c == ')')
        {
            while (oper[to] != '(')
            {
                r = infix_eval(dnum, oper);
                push_num(r);
            }
            pop_op();
        }
        else if (is_operator(c))
        {
            
            while (to != -1 && precedence(c) <= precedence(oper[to]))
            {
                output = infix_eval(dnum, oper);
                
                push_num(output);
            }
            
            push_op(c);
        }
    }
    while (to != -1)
    {
        output = infix_eval(dnum, oper);
        push_num(output);
    }
    return pop_num();
}

int main()
{
    int sockfd, new_sockfd, client_len, server_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[100];
    char calc[1024];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(":( socket not created!\n");
        exit(0);
    }
    else
    {
        printf(":) socket created successfully!\n");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(20000);
    server_len = 0;
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror(":( unable to bind the local address!\n");
        exit(0);
    }
    else
    {
        printf(":) binding successful!\n");
    }
    listen(sockfd, 5);
    printf(":) Im listening!\n");

    while (1)
    {
        client_len = sizeof(client_addr);
        if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len)) < 0)
        {
            perror(":( unable to accept\n");
            exit(0);
        }
        else
        {
            printf(":) connection accepted!\n");
        }
        while (1)
        {
            memset(calc, '\0', 1024);
            memset(buffer, '\0', 100);
            while (recv(new_sockfd, buffer, 11, 0) > 0 && strlen(buffer) != 0)
            {
                
                strcat(calc, buffer);
                if(calc[strlen(calc)-1]=='?'){
                    calc[strlen(calc)-1]='\0';
                    break;
                }
                
                memset(buffer, '\0', 100);
            }
            if(strcmp(calc,"")==0)break;
            printf("%s ", calc);
            char result[100];
            double answer=eval(calc);
            sprintf(result, "%lf", answer);
            printf("= %s\n", result);
            send(new_sockfd, result, strlen(result)+1, 0);
            // recv(new_sockfd, buffer, 25, 0);
        }
        close(new_sockfd);
    }
}