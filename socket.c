/*************************************************************************
	> File Name: socket.c
	> Author: Jiang Yuhang
	> Mail: jiangxiaobai1989@gmail.com
	> Created Time: Fri 22 Jul 2016 10:20:53 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 5

#define CADDR  "115.28.49.209"


int make_socket_connect(int portnum)
{
    int sd;
    struct sockaddr_in saddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1)
    {
        perror("socket");
        exit(1);
    }

    bzero((void *)&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(portnum);
    saddr.sin_addr.s_addr = inet_addr(CADDR);

    if(bind(sd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
        perror("bind");
        exit(2);
    }

    if(listen(sd, BACKLOG) == -1)
    {
        perror("listen");
        exit(3);
    }

    return sd;
}

