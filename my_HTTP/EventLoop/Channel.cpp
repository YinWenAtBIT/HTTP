/*************************************************************************
	> File Name: Channel.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Wed 11 May 2016 09:17:48 PM CST
 ************************************************************************/

#include "Channel.h"

using namespace std;

Channel::Channel(int connfd):
    _fd(connfd), _read_flag(false), _write_flag(false)
{}

int Channel::fd() const
{
    return _fd;
}

void Channel::set_read_flag(bool flag)
{
    _read_flag = flag;
}

void Channel::set_write_flag(bool flag)
{
    _write_flag = flag;
}

void Channel::set_read_callback(callback &read_call)
{
    _read_callable = read_call;
}

void Channel::set_write_callback(callback &write_call)
{
    _write_callable = write_call;
}

void Channel::handle()
{
    if(_read_flag)
        _read_callable();
    if(_write_flag)
        _write_callable();

}
