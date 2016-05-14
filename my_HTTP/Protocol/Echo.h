/*************************************************************************
	> File Name: Echo.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sat 14 May 2016 05:16:43 PM CST
 ************************************************************************/

#ifndef _ECHO_H
#define _ECHO_H

class Echo
{
public:
    Echo(int fd);
    void handle();

private:
    int connfd;
};


#endif
