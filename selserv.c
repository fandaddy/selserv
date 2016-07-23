/*************************************************************************
	> File Name: selserv.c
	> Author: 
	> Mail: 
	> Created Time: Fri 22 Jul 2016 08:25:19 PM CST
 ************************************************************************/

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

/************************************************************************/
/* External Definition (Constant and Marco)                             */
/************************************************************************/
/* 通讯端口定义 */
#define PORTNUM 15001
/* 允许并发客户端个数 */
//#define FD_SETSIZE 10 已经被定义过了，真搞笑

int make_socket_connect(int);

int main(int ac, char *av[])
{
    int sock_id;
    int retval;
    fd_set rfds;
    struct timeval tv;
    int conn;
    struct sockaddr_in peeraddr;
    socklen_t peeraddrlen = sizeof(peeraddr);
    int client[FD_SETSIZE];
    int i;
    int maxi = 0;
    int maxfd;

    for(i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
    }

    /* 建立连接 */
    sock_id = make_socket_connect(PORTNUM);
    if(sock_id == -1)
    {
        perror("make_socket_connect");
        exit(1);
    }
    maxfd = sock_id;

    while(1)
    {
        /* 每次调用select都要重新设置文件描述符和间隔时间 */
        FD_ZERO(&rfds);
        FD_SET(sock_id, &rfds);
        retval = select(maxfd+1, &rfds, NULL, NULL, NULL);
        if(retval == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        else if(retval == 0)
        {
            continue;
        }
        else if(FD_ISSET(sock_id, &rfds))
        {
            conn = accept(sock_id, (struct sockaddr *)&peeraddr, &peeraddrlen);
            if(conn == -1)
            {
                perror("connect");
                exit(EXIT_FAILURE);
            }
            for(i = 0; i < FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = conn;
                    if(i > maxi)
                    {
                        maxi = i;
                    }
                    break;
                }
            }
            if(i == FD_SETSIZE)
            {
                fprintf(stderr, "too many clients\n");
                exit(EXIT_FAILURE);
            }
            FD_SET(conn, &rfds);
            if(conn > maxfd)
            {
                maxfd = conn;
            }
        }
        for(i = 0; i <= maxi; i++)
        {
            conn = client[i];
            if(conn == -1)
            {
                continue;
            }
            if(FD_ISSET(conn, &rfds))
            {
                char recvbuf[1024] = {0};
                int ret = read(conn, recvbuf, 1024);
                if(ret <= 0)
                {
                    FD_CLR(conn, &rfds);
                    close(conn);
                    client[i] = -1;
                    continue;
                }
                fputs(recvbuf, stdout);
                write(conn, recvbuf, strlen(recvbuf)+1);
            }
        }
    }
    return 0;

}
