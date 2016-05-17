/*************************************************************************
	> File Name: HTTP_Response.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 15 May 2016 05:13:53 PM CST
 ************************************************************************/


#include "HTTP_Response.h"

void HTTP_Response::set_status_code(int code)
{
    _status_code = static_cast<HTTP_Status_Code>(code);
}

string HTTP_Response::version_str()
{
    return version_to_str_dict[_version];
}

void HTTP_Response::append_to_body(const string& b)
{
    _body.append(b);
}

void HTTP_Response::append_to_body(const char* b, const char* e)
{
    _body.append(b, e);
}

void HTTP_Response::set_body(const string& body)
{
    _body = body;
}

const string& HTTP_Response::get_body()
{
    return _body;
}

void HTTP_Response::add_header(const string& k, const string& v)
{
    _headers[k] = v;
}

// 支持HTTP的HEAD方法
string HTTP_Response::get_response_without_body()
{
    string response;
    response += version_to_str_dict[_version];
    response += " ";
    response += statuscode_to_str_dict[_status_code];
    response += "\r\n";
    
    for (auto& it : _headers)
    {
        response += it.first;
        response += ": ";
        response += it.second;
        response += "\r\n";
    }
    response += "\r\n";
    return response;
}

string HTTP_Response::get_response()
{
    string response = get_response_without_body();
    response += _body;
    return response;
}

void HTTP_Response::print(std::ostream& os)
{
    //string response = get_response();
    string response = get_response_without_body();
    
    auto beg = response.begin();
    auto cur = beg;
    auto end = response.end();
    
    for (; cur != end; ++cur)
    {
        if (*cur == '\r' && *(cur+1) == '\n')
        {
            os << "<";
            os << string(beg, cur+2);
            
            // "\r\n\r\n" meaning headers end.
            if (beg == cur)
                break;
            else
                beg = cur+2;
        }
    }

    //os << "<" << string(cur+2, end);
    os << std::endl;
}
