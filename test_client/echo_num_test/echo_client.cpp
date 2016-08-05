/*************************************************************************
	> File Name: echo_client.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: 2016年08月05日 星期六 20时33分14秒
 ************************************************************************/

#include "../Base/csapp.h"

int fds[1000];

char * words = "nihao";


void * start_connect(void * num);

int main(int argc, char *argv[])
{
    /*
    if(argc!= 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }
    */

    //host = argv[1];
    //port = atoi(argv[2]);

    char *host ="127.0.0.1";
    int port=8080;
   /* 
    int fd_test = Open_clientfd(host, port);
    write(fd_test, words, 6);
    char rece[20];
    int n= read(fd_test, rece, 20);
    rece[n] ='\n';
    printf("%s", rece);
*/

    for(int i=0; i<1000; i++)
    {
        fds[i] = Open_clientfd(host, port);
        printf("%d connect complete\n", i);
        if(fds[i] <0)
        {
            fprintf(stderr, "cannot connect");
        }
    }

    pthread_t pids[10];

    for(int i=0; i<10; i++)
        pthread_create(&pids[i], NULL, start_connect, (void *)i);

    exit(0);
}

void * start_connect(void * num)
{
    int start = (int) num;
    char buf[20];
    for(int i =0; i<100; i++)
    {
        write(fds[start*100+i], words, 6);
        int n = read(fds[start+i], buf, 20);
        buf[n] ='\0';
        printf("%s\n", buf);
        /*
        if(strcmp(buf, words) != 0)
        {
            fprintf(stderr, "wrong");
            exit(1);
        }
        */
    }
}
