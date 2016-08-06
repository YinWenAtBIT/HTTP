/*************************************************************************
	> File Name: Echo.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sat 14 May 2016 05:18:23 PM CST
 ************************************************************************/

#include "Echo.h"
#include "../Base/csapp.h"


Echo::Echo(int fd)
:connfd(fd)
{}

void Echo::handle()
{
    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    int nread;
    while(nread = Rio_readlineb(&rio, buf, MAXLINE) >0)
        Rio_writen(connfd, buf, nread);
}
