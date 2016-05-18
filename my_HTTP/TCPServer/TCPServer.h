/*************************************************************************
	> File Name: TCPServer.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sat 14 May 2016 01:05:27 PM CST
 ************************************************************************/

#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "../EventLoop/Epoll.h"
#include "../Protocol/Echo.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <string.h>
#include <signal.h>


template <typename Protocol>
class TCPServer
{
public:
    TCPServer(uint32_t port = 80, uint32_t threads = 5):
    _port(port), _pthread_num(threads), _listenfd(TCP_listen(_port)), epoll_loop(_listenfd, &worker_mutex, &worker_cond)
    {
        pthread_mutex_init(&worker_mutex, NULL);
        pthread_cond_init(&worker_cond, NULL);
        std::function<void(int)> fcn = std::bind(&TCPServer<Protocol>::handle_proc, this, std::placeholders::_1);
        epoll_loop.set_callback(fcn);
        
    }

    ~TCPServer()
    {
        pthread_mutex_destroy(&worker_mutex);
        pthread_cond_destroy(&worker_cond);
    }

    TCPServer(TCPServer & rhs) = delete;
    TCPServer & operator =(TCPServer &rhs) = delete;

    void run()
    {
        pthread_t tid;
        for(int i=0; i<_pthread_num; i++)
        {
            int ret = pthread_create(&tid, NULL, handle_worker_list, (void *) this);
            if(ret !=0)
            {
                perror("thread_pool init: ");
                exit(1);
            }

        }
        
        struct sigaction act;
        act.sa_handler = SIG_IGN;
        if(sigaction(SIGPIPE, &act, NULL) != 0)
            exit(1);
        epoll_loop.loop(worker_list);
    }

private:
    uint32_t _port;
    uint32_t _pthread_num;
    int _listenfd;
    Epoll epoll_loop;
    std::list<Channel *> worker_list;
    pthread_mutex_t worker_mutex;
    pthread_cond_t worker_cond;




    void handle_proc(int connfd)
    {
        Protocol pro(connfd);
        pro.handle();
    }

    int TCP_listen(uint32_t port)
    {
        int listenfd;
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        if(-1 == listenfd)
        {
            perror("socket create error: ");
            exit(1);
        }
        int flag = fcntl(listenfd, F_GETFL, 0);
        if(flag == -1)
        {
            perror("F_GETFL error:");
            exit(1);
        }
        flag |=O_NONBLOCK;
        flag = fcntl(listenfd, F_SETFL, flag);
        if(flag == -1)
        {
            perror("F_SETFL error:");
            exit(1);
        }
        int on=1;
        int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if(ret == -1)
        {
            perror("SO_REUSEADDR:");
            exit(1);
        }
        sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(-1 == bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr)))
        {
            perror("bind:");
            exit(1);
        }

        listen(listenfd, 5);
        return listenfd;

    }



    static void * handle_worker_list(void* arg)
    {
        pthread_detach(pthread_self());

        TCPServer<Protocol> * self = (TCPServer<Protocol> *) arg;

        while(1)
        {
            pthread_mutex_lock(&self->worker_mutex);
            while(self->worker_list.empty())
                pthread_cond_wait(&self->worker_cond, &self->worker_mutex);
            
            Channel * now_job = self->worker_list.front();
            self->worker_list.pop_front();
            pthread_mutex_unlock(&self->worker_mutex);

            now_job->handle();
            delete now_job;

        }
        return NULL;
    }
};




#endif
