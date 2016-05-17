/*************************************************************************
	> File Name: HTTP_Request.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 15 May 2016 03:58:33 PM CST
 ************************************************************************/

#ifndef _HTTP_REQUEST_H
#define _HTTP_REQUEST_H

#include "HTTP_base.h"
#include <iostream>


class HTTP_Request: public HTTP_base
{
public:
    HTTP_Request(HTTP_Version v = HTTP_Version::UNKNOWN, HTTP_Method m = HTTP_Method::UNKNOWN):
    _version(v), _method(m)
    {}
    
    void set_version(const char *s, const char *e);
    HTTP_Version get_version();

    void set_method(const char *s, const char *e);
    HTTP_Method get_method();

    void set_uri(const char *s, const char *e);
    void set_uri(const string &);
    string get_uri();

    void set_query(const char *s, const char *e);
    void set_query(const string&);
    string get_query();

    void add_header(const char *s, const char* e);
    string get_header(const string &key);
    
    void append_to_body(const char *s, const char *e);
    void append_to_body(const string &);

    const string & body();

    string version_str();
    string method_str();

    void print(std::ostream &os);


private:
    HTTP_Version _version;
    HTTP_Method _method;
    string _uri;
    string _query;
    map<string, string> _headers;
    string _body;

};





#endif
