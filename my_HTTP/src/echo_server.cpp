/*************************************************************************
	> File Name: echo_server.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sat 14 May 2016 08:09:14 PM CST
 ************************************************************************/

#include "../TCPServer/TCPServer.h"
#include "../EventLoop/Epoll.h"
#include "../Protocol/Echo.h"

int main(int argc, char * argv[])
{
    TCPServer<Echo> EchoServer(8080, 8);
    EchoServer.run();
}

