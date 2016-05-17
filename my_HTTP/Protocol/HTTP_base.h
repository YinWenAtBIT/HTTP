/*************************************************************************
	> File Name: HTTP_base.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 15 May 2016 02:36:34 PM CST
 ************************************************************************/

#ifndef _HTTP_BASE_H
#define _HTTP_BASE_H

#include <map>
#include <string>

using std::string;
using std::map;

class HTTP_base
{
public:
    enum class HTTP_Method
    {
        GET,
        HEAD,
        POST,
        PUT,
        UNKNOWN
    };

    enum class HTTP_Version
    {
        UNKNOWN,
        v09,
        v10,
        v11,
        v20
    };

    enum class HTTP_Status_Code
    {
        UNKNOWN = 0,
        s200 = 200,
        s204 = 204,
        s206 = 206,
        s301 = 301,
        s302 = 302,
        s303 = 303,
        s304 = 304,
        s307 = 307,
        s400 = 400,
        s401 = 401,
        s403 = 403,
        s404 = 404,
        s500 = 500,
        s501 = 501,
        s503 = 503
    };

    static string ROOT_DIR;
    static map<HTTP_Method, string> method_to_str_dict;
    static map<string, HTTP_Method> str_to_method_dict;
    static map<HTTP_Version, string> version_to_str_dict;
    static map<string, HTTP_Version> str_to_version_dict;
    static map<HTTP_Status_Code, string> statuscode_to_str_dict;
    static map<string, string> content_type_dict;
    static void set_root_dir(string path)
    {
        ROOT_DIR = path;
    }

};



#endif
