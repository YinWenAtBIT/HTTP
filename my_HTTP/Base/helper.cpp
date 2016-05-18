/*************************************************************************
	> File Name: helper.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Mon 16 May 2016 08:48:09 PM CST
 ************************************************************************/

#include "helper.h"


string parse_uri(string &path, string &query)
{
    string uri;
    auto it = path.find('?');
    if(it != string::npos)
    {
        uri.assign(path.begin(), path.begin()+it);
        it++;
        query.assign(path.begin()+it, path.end());
    }
    else
        uri = path;

    return uri;
    
}

string get_now_time()
{
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    char buf[64];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
    string res(buf);
    return res;
}

