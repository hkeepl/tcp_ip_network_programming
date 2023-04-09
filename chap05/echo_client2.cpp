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
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_addr;

    if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
    {
        error_handling("WSAStartup() error.");
    }

    unsigned short port = 4242;
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock)
    {
        error_handling("socket() error.");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);

    if (-1 == connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        error_handling("connect() error");
    }
    else
    {
        puts("Connected......");
    }

    while (true)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
        {
            break;
        }

        str_len = send(sock, message, strlen(message), 0);

        recv_len = 0;
        while (recv_len < str_len)
        {
            recv_cnt = recv(sock, message, BUF_SIZE - 1, 0);
            if (recv_cnt == -1)
            {
                error_handling("recv() error.");
            }
            recv_len += recv_cnt;
        }
        message[recv_len] = 0;
        printf("Message from server: %s", message);
    }
    closesocket(sock);
    WSACleanup();

    return 0;
}

#else

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_addr;

    unsigned short port = 4242;
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock)
    {
        error_handling("socket() error.");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);

    if (-1 == connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        error_handling("connect() error");
    }
    else
    {
        puts("Connected......");
    }

    while (true)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
        {
            break;
        }

        str_len = write(sock, message, strlen(message));

        recv_len = 0;
        //! 注意：这里我们使用小于符号，避免程序发生错误时进入死循环。
        while (recv_len < str_len)
        {
            recv_cnt = read(sock, message, BUF_SIZE - 1);
            if (recv_cnt == -1)
            {
                error_handling("read() error.");
            }
            recv_len += recv_cnt;
        }
        message[recv_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);

    return 0;
}

#endif