/*************************************************************************
	> File Name: http_server.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Wed 18 May 2016 10:15:21 PM CST
 ************************************************************************/


#include "../TCPServer/TCPServer.h"
#include "../EventLoop/Epoll.h"
#include "../Protocol/HTTP.h"

int main(int argc, char * argv[])
{
    TCPServer<HTTP> HTTPServer(8080, 8);
    HTTPServer.run();
}

