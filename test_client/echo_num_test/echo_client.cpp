/*************************************************************************
	> File Name: echo_client.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: 2016年08月05日 星期六 20时33分14秒
 ************************************************************************/

#include "../Base/csapp.h"
const int total_connect = 100000;
const int every_thread_connect =100;
int fds[total_connect];

char * words = "nihao";


void * start_connect(void * num);

void set_fd_non_blocking(int fd)
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

int main(int argc, char *argv[])
{
    char *host ="127.0.0.1";
    int port=8080;

    for(int i=0; i<total_connect; i++)
    {
        fds[i] = Open_clientfd(host, port);
        //set_fd_non_blocking(fds[i]);
        printf("%d connect complete\n", i);
        if(fds[i] <0)
        {
            fprintf(stderr, "cannot connect");
        }
    }


    char buf[20];
    int read_count =0;
    int fail_count =0;

    for(int i =0; i<total_connect; i++)
    {
        write(fds[i], words, 6);
        int n = read(fds[i], buf, 20);
        if(n<0)
        {
            fail_count +=1;
            continue;
        }
        buf[n] ='\0';
        //printf("%s\n", buf);
        read_count+=1;
    }

    printf("total read %d and total fail %d\n", read_count, fail_count);
    exit(0);
}

/*
void * start_connect(void * num)
{
    int start = (int) num;
    char buf[20];
    for(int i =0; i<every_thread_connect; i++)
    {
        write(fds[start*every_thread_connect+i], words, 6);
        int n = read(fds[start*every_thread_connect+i], buf, 20);
        if(n<0)
        {
            fails[start] +=1;
            continue;
        }
        buf[n] ='\0';
        printf("%s\n", buf);
        reads[start]+=1;
    }
}
*/
