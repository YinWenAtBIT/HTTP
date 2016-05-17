/*************************************************************************
	> File Name: HTTP_Response.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 15 May 2016 05:09:47 PM CST
 ************************************************************************/

#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H
#include "HTTP_base.h"
#include <iostream>


class HTTP_Response: public HTTP_base
{
public:
    HTTP_Response(HTTP_Version v=HTTP_Version::v11, HTTP_Status_Code s= HTTP_Status_Code::UNKNOWN)
    :_version(v), _status_code(s)
    {}

    void set_status_code(int code);
    string version_str();
    void add_header(const string &key, const string &value);
    void set_body(const string &b);

    void append_to_body(const string &b);
    void append_to_body(const char *s, const char *e);
    const string & get_body();

    string get_response();
    string get_response_without_body();

    void print(std::ostream &os);

private:
    HTTP_Version _version;
    HTTP_Status_Code _status_code;
    map<string, string> _headers;
    string _body;
};



#endif
