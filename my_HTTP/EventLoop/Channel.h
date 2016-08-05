/*************************************************************************
	> File Name: Channel.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Wed 11 May 2016 08:46:37 PM CST
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>

class Channel
{
public:
    typedef std::function<void()> callback;
    typedef std::function<void(int)> completeCallback;
    
    Channel(int connfd);
    int fd() const;
    void set_read_flag(bool flag);
    void set_write_flag(bool flag);
    void set_read_callback(callback &read_call);
    void set_write_callback(callback &write_call);
    void set_complete_callback(completeCallback & com_call);
    void handle();
    
private:
    int _fd;
    bool _read_flag;
    bool _write_flag;
    bool _com_flag;
    callback _read_callable;
    callback _write_callable;
    completeCallback _complete_callback;
};



#endif
