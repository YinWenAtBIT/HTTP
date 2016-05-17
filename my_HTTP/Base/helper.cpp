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


bool issapce(char c)
{
    return c==' ';
}
