/*************************************************************************
	> File Name: HTTP_Request.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 15 May 2016 04:43:36 PM CST
 ************************************************************************/

#include "HTTP_Request.h"

#include <algorithm>
#include <iostream>

void HTTP_Request::set_method(const char* start, const char* end)
{
    string m(start, end);
    auto it = str_to_method_dict.find(m);
    if (it != str_to_method_dict.end())
        _method = it->second;
    else
        _method = HTTP_Method::UNKNOWN;
}

HTTP_base::HTTP_Method HTTP_Request::get_method()
{
    return _method;
}

void HTTP_Request::set_version(const char* start, const char* end)
{
    string v(start, end);
    auto it = str_to_version_dict.find(v);
    if (it != str_to_version_dict.end())
        _version = it->second;
    else
        _version = HTTP_Version::UNKNOWN;
}

HTTP_base::HTTP_Version HTTP_Request::get_version()
{
    return _version;
}

void HTTP_Request::set_uri(const char* start, const char* end)
{
    _uri.assign(start, end);
}

void HTTP_Request::set_uri(const string & uri)
{
    _uri = uri;
}

string HTTP_Request::get_uri()
{
    return _uri;
}

void HTTP_Request::set_query(const char* start, const char* end)
{
    _query.assign(start, end);
}

void HTTP_Request::set_query(const string &query)
{
    _query = query;
}

string HTTP_Request::get_query()
{
    return _query;
}

void HTTP_Request::add_header(const char* start, const char* end)
{
    const char* p = std::find(start, end, ':');
    string key(start, p);
    
    p++;
    while (p < end && isspace(*p))
        p++;
    const char* e = end;
    while (e > p && isspace(*e))
        e--;
    
    string value(p, e+1);
    _headers[key] = value;
}

string HTTP_Request::get_header(const string& k)
{
    string result;
    auto it = _headers.find(k);
    if (it != _headers.end())
        result = it->second;
    return result;
}

void HTTP_Request::append_to_body(const string& b)
{
    _body.append(b);
}

void HTTP_Request::append_to_body(const char* b, const char* e)
{
    _body.append(b, e);
}

const string& HTTP_Request::body()
{
    return _body;
}

string HTTP_Request::method_str()
{
    return method_to_str_dict[_method];
}

string HTTP_Request::version_str()
{
    return version_to_str_dict[_version];
}

void HTTP_Request::print(std::ostream& os)
{
    os << ">" << method_to_str_dict[_method];
    os << " " << _uri;
    if (!_query.empty())
        os << "?" << _query;
    os << " ";
    
    os << version_str();
    os << "\r\n";
    
    for (auto& header : _headers)
    {
        os << ">";
        os << header.first << ": " << header.second << "\r\n";
    }
    os << "\r\n";
    os << ">" << _body << std::endl;
}
