/*************************************************************************
	> File Name: HTTP.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 15 May 2016 05:45:28 PM CST
 ************************************************************************/

#ifndef _HTTP_H
#define _HTTP_H
#include "HTTP_base.h"
#include "HTTP_Request.h"
#include "HTTP_Response.h"

class HTTP: public HTTP_base
{
public:
    HTTP(int fd):
    _connfd(fd), _request_flag(false), _response_flag(false)
    {}
    void handle();
    void set_request_flag(bool flag);
    void set_response_flag(bool flag);

    void accept_request();
    void handle_request();
    void send_response();

    string get_absolute_path();
    string get_filetype(string &path);
    bool is_dir(string &path);
    bool is_path_exit(string &path);
    bool is_access(string &path);

    
    bool execte_cgi(string &path);
    bool serve_file(string &path);
    bool put_file(string &path);
    void internel_server_error();


    int get_file_size(string &path);

    bool supported();
    void noimplement();
    void not_found();
    void serve_forbidden();

    bool serve_index();
    void serve_hello();

private:
    int _connfd;
    bool _request_flag;
    bool _response_flag;
    HTTP_Request _request;
    HTTP_Response _response;


};


#endif
