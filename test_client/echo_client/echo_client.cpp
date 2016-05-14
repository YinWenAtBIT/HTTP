/*************************************************************************
	> File Name: echo_client.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: 2016年05月14日 星期六 23时57分14秒
 ************************************************************************/

#include "../Base/csapp.h"

int main(int argc, char *argv[])
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    /*
    if(argc!= 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }
    */

    //host = argv[1];
    //port = atoi(argv[2]);
    host ="127.0.0.1";
    port=8080;

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while(Fgets(buf, MAXLINE, stdin) != NULL)
    {
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    close(clientfd);
    exit(0);
}
