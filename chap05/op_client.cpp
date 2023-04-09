#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define OPERAND_SIZE 4
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
    char op_msg[BUF_SIZE] = {0};
    struct sockaddr_in serv_addr;
    int operand_count;
    int result;

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

    fputs("Operand count: ", stdout);
    scanf("%d", &operand_count);
    op_msg[0] = (char)operand_count;
    for (int i = 0; i < operand_count; ++i)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&op_msg[1 + i * OPERAND_SIZE]);
    }
    fgetc(stdin); //! 删掉缓冲中的字符\n
    fputs("Operator: ", stdout);
    scanf("%c", &op_msg[operand_count * OPERAND_SIZE + 1]);

    send(sock, op_msg, operand_count * OPERAND_SIZE + 2, 0);
    recv(sock, (char *)&result, sizeof(result), 0);

    printf("Operation result: %d\n", result);
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
    char op_msg[BUF_SIZE] = {0};
    struct sockaddr_in serv_addr;
    int operand_count;
    int result;

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

    fputs("Operand count: ", stdout);
    scanf("%d", &operand_count);
    op_msg[0] = (char)operand_count;
    for (int i = 0; i < operand_count; ++i)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&op_msg[1 + i * OPERAND_SIZE]);
    }
    fgetc(stdin); //! 删掉缓冲中的字符\n
    fputs("Operator: ", stdout);
    scanf("%c", &op_msg[operand_count * OPERAND_SIZE + 1]);

    write(sock, op_msg, operand_count * OPERAND_SIZE + 2);
    read(sock, &result, sizeof(result));

    printf("Operation result: %d\n", result);
    close(sock);
    return 0;
}

#endif