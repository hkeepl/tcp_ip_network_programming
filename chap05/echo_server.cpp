#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}

#ifdef WIN32

#include <WinSock2.h>

int main(int argc, char *argv[])
{
    WSAData wsa_data;
    char message[BUF_SIZE];
    int str_len;
    int clnt_sock, serv_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    if(0 != WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
        error_handling("WSAStartup() error.");
    }

    unsigned short port = 4242;
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == serv_sock)
    {
        error_handling("socket() error.");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        error_handling("bind() error");
    }

    if (-1 == listen(serv_sock, 5))
    {
        error_handling("listen() error");
    }

    int clnt_addr_len = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        if (-1 == clnt_sock)
        {
            error_handling("accept() error.");
        }
        else
        {
            printf("Connected client %d \n", i + 1);
        }

        while ((str_len = recv(clnt_sock, message, BUF_SIZE, 0)) != 0)
        {
            send(clnt_sock, message, str_len, 0);
        }

        closesocket(clnt_sock);
    }

    closesocket(serv_sock);
    WSACleanup();

    return 0;
}


#else

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    char message[BUF_SIZE];
    int str_len;
    int clnt_sock, serv_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    unsigned short port = 4242;
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == serv_sock)
    {
        error_handling("socket() error.");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        error_handling("bind() error");
    }

    if (-1 == listen(serv_sock, 5))
    {
        error_handling("listen() error");
    }

    int clnt_addr_len = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        if (-1 == clnt_sock)
        {
            error_handling("accept() error.");
        }
        else
        {
            printf("Connected client %d \n", i + 1);
        }

        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
        {
            write(clnt_sock, message, str_len);
        }

        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}

#endif