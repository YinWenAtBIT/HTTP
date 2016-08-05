/*************************************************************************
	> File Name: Epoll.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Thu 12 May 2016 07:56:00 PM CST
 ************************************************************************/

#include "Epoll.h"
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static const int MAX_EVENT = 50;

Epoll::Epoll(int listen, pthread_mutex_t * mutex, pthread_cond_t * cond):
    _listenfd(listen), _mutex_ptr(mutex), _cond_ptr(cond)
{

}

void Epoll::loop(ChannelList & work_list)
{
    epfd = http_epoll_create(1024);
    struct epoll_event events[MAX_EVENT];
    epoll_add_event(epfd, _listenfd, EPOLLIN | EPOLLET);

    int nfds;
    while(1)
    {
        nfds = epoll_wait(epfd, events, MAX_EVENT, -1);
        for(int i=0; i<nfds; i++)
        {
            if(events[i].data.fd == _listenfd)
            {
                handle_accept(epfd, events[i].data.fd);
            }
            else
            {
                add_to_channel(events[i].data.fd, work_list);
            }
        }
    }

}

void Epoll::add_to_channel(int fd, ChannelList & work_list)
{
    Channel * new_one = new Channel(fd);
    new_one->set_read_flag(true);
    std::function<void()> fct = [=](){read_callback(fd);};
    new_one->set_read_callback(fct);
    std::function<void(int)> fct_int = std::bind(Epoll::epoll_delete_event, this, std::placeholders::_1);
    new_one->set_complete_callback(fct_int);

    pthread_mutex_lock(_mutex_ptr);
    work_list.push_back(new_one);
    pthread_cond_signal(_cond_ptr);
    pthread_mutex_unlock(_mutex_ptr);

}

int Epoll::http_epoll_create(int size)
{
	int epoll_fd;
	epoll_fd = epoll_create(size);

	if (-1 == epoll_fd)
	{
		perror("epoll_create");
		exit(-1);
	}

	return epoll_fd; 
}


void Epoll::epoll_add_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;

	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		perror("EPOLL_CTL_ADD");
		exit(-1);
	}	
}

void Epoll::epoll_delete_event(int fd_to_delete)
{
	struct epoll_event ev;
	ev.events = EPOLL_CTL_DEL;
	ev.data.fd = fd_to_delete;
    
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd_to_delete, &ev) == -1)
	{
		perror("EPOLL_CTL_DEL");
		exit(-1);
	}	
    close(fd_to_delete);
}

void Epoll::handle_accept(int epfd, int listenfd)
{
	int conn_sock;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;

	while ((conn_sock = accept(listenfd, (sockaddr *)&clientaddr, &addrlen)) > 0) 
	{
		printf("Client from %s connected!\n", inet_ntoa(((struct sockaddr_in *)&clientaddr)->sin_addr));
		set_fd_nonblocking(conn_sock);
    	epoll_add_event(epfd, conn_sock, EPOLLIN|EPOLLET);
	}
	
	if (conn_sock == -1) 
	{
    	if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
    	{
    		perror("accept");
    	}
	}

	return;
}


void Epoll::set_callback(std::function<void(int)> &fcn)
{
    read_callback = fcn;
}

void Epoll::set_fd_nonblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		perror("fcntl: F_GETFL");
		exit(-1);
	}
	flags |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flags);
	if(ret < 0)
	{
		perror("fcntl");
		exit(-1);
	}
}
