/*************************************************************************	> File Name: HTTP.cpp
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Mon 16 May 2016 08:24:14 PM CST
 ************************************************************************/

#include "HTTP.h"
#include "../Base/helper.h"
#include "../Base/csapp.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <ctype.h>

using std::cin;
using std::cout;
using std::endl;

void HTTP::handle()
{
    accept_request();
    handle_request();
    send_response();
}

void HTTP::set_request_flag(bool flag)
{
    _request_flag =flag;
}

void HTTP::set_response_flag(bool flag)
{
    _response_flag =flag;
}

void HTTP::accept_request()
{
    char buf[MAXLINE];
    rio_t rio;
    rio_readinitb(&rio, _connfd);
    
    int nread;
    nread = rio_readlineb(&rio, buf, MAXLINE);

    string first_line(buf, buf+nread);
    //cout<<"first line:\n"<<first_line<<endl;
    //cout<<"end of first_line"<<endl;


    char *start = buf;
    char *finish = start +nread;
    char *cur = start;
    char *end;

    while(isspace(*cur))
        cur++;
    end = cur;
    while(!isspace(*end))
        end++;
    

    //get method
    _request.set_method(cur, end);

    cur = end;
    while(isspace(*cur))
        cur++;
    end = cur;
    while(!isspace(*end))
        end++;
    

    //get uri and query
    string raw_uri(cur, end);
    //cout<<"raw_uri: "<<raw_uri<<endl;
    string query, uri;
    uri = parse_uri(raw_uri, query);
    //cout<<"query: "<<query<<endl<<"uri: "<<uri<<endl;

    _request.set_uri(uri);
    _request.set_query(query);
    
    
    cur = end;
    while(isspace(*cur))
        cur++;
    end = cur;
    while(!isspace(*end) && end<finish)
        end++;

    //get version
    _request.set_version(cur, end);
    
    
    //get headers
    while(nread = rio_readlineb(&rio, buf, MAXLINE))
    {
        start = buf;
        if(*start == '\r' && *(start+1) == '\n')
            break;
        
        finish = start +nread;

        cur = start;
        while(isspace(*cur))
            cur++;
        end = cur;
        while(!isspace(*end))
            end++;
        
        _request.add_header(cur, end);

    }

    //get body
    string len_str = _request.get_header("Content-Length");

    if(!len_str.empty()) 
    {
        int len = atoi(len_str.data());
        while(len>0)
        {
            nread = rio_readnb(&rio, buf, MAXLINE);
            if(nread <=0)
                break;
            _request.append_to_body(buf, buf+nread);
            len -= nread;
        }

    }
    set_request_flag(true);

}

void HTTP::handle_request()
{
    //accept_request first
    assert(_request_flag);

    set_response_flag(true);
    if(!supported())
    {
        noimplement();
        return;
    }

    string uri = _request.get_uri();
    string path = get_absolute_path();
    
    if(!is_path_exist(path) && _request.get_method()!=HTTP_Method::POST)
    {
        not_found();
        return;
    }
    
    if(is_dir(path))
    {
        if(!serve_index())
            serve_hello();
        return;
    }

    if(!is_access(path))
    {
        cout<<"outcome is cannot access: "<<path<<endl;
        serve_forbidden();
        return;
    }


    bool success = false;
    switch(_request.get_method())
    {
        case HTTP_Method::GET:
        case HTTP_Method::HEAD:
            if(_request.get_query().empty())
                success = serve_file(path);
            else
                success = execute_cgi(path);
            break;
        case HTTP_Method::PUT:
            success = put_file(path);
            break;

        case HTTP_Method::POST:
            success = execute_cgi(path);
            break;

    }
    if(!success)
        internal_server_error();

}


void HTTP::send_response()
{
    string time_now = get_now_time();
    _response.add_header("Date", time_now);
    string body = _response.get_body();
    int len = body.size();
    _response.add_header("Content-Length", std::to_string(len));

    string send_data;
    if(_request.get_method() == HTTP_Method::HEAD)
        send_data = _response.get_response_without_body();
    else
        send_data = _response.get_response();

    //rio_writen(_connfd, (void *)send_data.c_str(), send_data.size());
    int n = write(_connfd, send_data.c_str(), send_data.size());
    if(n == -1 && errno == SIGPIPE)
        close(_connfd);
}


void HTTP::internal_server_error()
{
    string body;
    body +="<HTML><HEAD><TITLE>Internel Server Error</TITLE></HEAD>\r\n";
    body +="<BODY><p>!</p>\r\n";
    body +="</p></HTML>\r\n";

    _response.set_status_code(403);
    _response.add_header("Content-Type", "text/html");
    _response.set_body(body);

}


bool HTTP::execute_cgi(string &path)
{
    pid_t pid;
    int input_cgi[2];
    int output_cgi[2];

    struct stat sbuf;
    stat(path.c_str(), &sbuf);

    if(!(S_IXUSR & sbuf.st_mode))
        return false;
    
    if(pipe(input_cgi) ==-1)
        return -1;

    if(pipe(output_cgi) == -1)
    {
        close(input_cgi[0]);
        close(input_cgi[1]);
        return false;
    }

    if((pid = fork())== 0)
    {
        //child
        close(input_cgi[1]);
        close(output_cgi[0]);

        dup2(input_cgi[0], 0);
        dup2(output_cgi[1],1);

        set_child_env();
        execl(path.c_str(), path.c_str(), 0);

    }
    else
    {
        //parent
        close(input_cgi[0]);
        close(output_cgi[1]);
        string body = _request.body();
        if(!body.empty())
        {
            int nwrite;
            nwrite = rio_writen(input_cgi[1], (void *)body.c_str(), body.size());
            if(nwrite ==-1)
                return false;
        }
        close(input_cgi[1]);

        //read header
        rio_t rio;
        rio_readinitb(&rio, output_cgi[0]);
        
        char buf[MAXLINE];
        char *start, *end;
        char *finish, *rcur;

        int nread;
        while(nread = rio_readlineb(&rio, buf, MAXLINE))
        {
            start = buf;
            finish = buf+nread;

            if(*start =='\r' && *(start+1) == '\n')
                break;

            while(isspace(*start))
                start++;
            
            end = start;
            while(*end !=':')
                end++;

            string key(start, end);
            

            start = end+1;
            while(isspace(*start))
                start++;
            rcur =finish-1;
            while(isspace(*rcur) && rcur >start)
                rcur--;
            if(!isspace(*rcur))
                rcur++;

            string value(start, rcur);
            _response.add_header(key, value);
        }

        //add body
        while(nread = rio_readnb(&rio, buf, MAXLINE))
        {
            _response.append_to_body(buf, buf+nread);
        }

        close(output_cgi[0]);
        int status;
        waitpid(pid, &status, 0);
        if(status == -1)
            return false;
    }
    _response.set_status_code(200);
    return true;

}

void HTTP::set_child_env()
{
    string server_protocol(_request.version_str());
    string request_method(_request.method_str());
    string http_accept(_request.get_header("Accept"));
    string http_user_agent(_request.get_header("User-Agent"));
    string http_referer(_request.get_header("Referer"));
    string script_name(_request.get_uri());
    string query_string(_request.get_query());
    string content_length(_request.get_header("Content-Length"));
    
    string host(_request.get_header("Host"));
    string server_name;
    string server_port;
    
    auto it = host.begin();
    for (; it != host.end(); ++it)
        if (*it == ':')
            break;
    server_name = string(host.begin(), it);
    
    // 默认为80端口
    if (it == host.end())
        server_port = "80";
    else
        server_port = std::move(string(it+1, host.end()));
    
    if (!server_name.empty())
        setenv("SERVER_NAME", server_name.data(), 0);
    
    if (!server_port.empty())
        setenv("SERVER_PORT", server_port.data(), 0);
    
    if (!server_protocol.empty())
        setenv("SERVER_PROTOCOL", server_protocol.data(), 0);
    
    if (!request_method.empty())
        setenv("REQUEST_METHOD", request_method.data(), 0);
    
    if (!http_accept.empty())
        setenv("HTTP_ACCEPT", http_accept.data(), 0);
    
    if (!http_user_agent.empty())
        setenv("HTTP_USER_AGENT", http_user_agent.data(), 0);
    
    if (!http_referer.empty())
        setenv("HTTP_REFERER", http_referer.data(), 0);
    
    if (!script_name.empty())
        setenv("SCRIPT_NAME", script_name.data(), 0);
    
    if (!query_string.empty())
        setenv("QUERY_STRING", query_string.data(), 0);
    
    if (!content_length.empty())
        setenv("CONTENT_LENGTH", content_length.data(), 0);    
}


bool HTTP::put_file(string& path)
{
    std::ofstream f;
    f.open(path);
    if(!f)
        return false;

    string body = _request.body();
    f<<body;
    f.close();

    string url ="HTTP://";
    url += _request.get_header("Host");
    url += _request.get_uri();
    _response.set_body(url);
    return true;
    
}

void HTTP::serve_forbidden()
{
    string body;
    body +="<HTML><HEAD><TITLE>FORBIDDEN</TITLE></HEAD>\r\n";
    body +="<BODY><p>cannot access this file!</p>\r\n";
    body +="</p></HTML>\r\n";

    _response.set_status_code(403);
    _response.add_header("Content-Type", "text/html");
    _response.set_body(body);
    
}


bool HTTP::is_access(string &path)
{
    struct stat sbuf;
    stat(path.c_str(), &sbuf);
    if(S_ISREG(sbuf.st_mode) || S_IRUSR & sbuf.st_mode)
        return true;

    return false;
}

int HTTP::get_file_size(string &path)
{
    struct stat sbuf;
    stat(path.c_str(), &sbuf);
    return sbuf.st_size;
}

bool HTTP::serve_file(string &path)
{
    string file_type = get_filetype(path);
    _response.add_header("Content-type", file_type);
    
    int file_size = get_file_size(path);
    int fd = open(path.c_str(), O_RDONLY, 0);
    if(fd<0)
        return false;
    
    char buf[MAXLINE];
    int nread;

    while(file_size >0)
    {
        nread = read(fd, buf, MAXLINE);
        _response.append_to_body(buf, buf+nread);
        file_size -= nread;
    }
    _response.set_status_code(200);
    return true;
    
}


string HTTP::get_filetype(string &path)
{
    int i = path.size()- 1;
    string type;
    while(path[i] !='.' && path[i] != '/')
        i--;

    if(path[i] == '/')
        type = "text/plain";
    else
    {
        string sub= path.substr(i);
        auto it = content_type_dict.find(sub);
        if(it != content_type_dict.end())
            type = it->second;
        else
            type = "text/plain";
    }

    return type;
}


bool HTTP::is_dir(string &path)
{
    struct stat sbuf;
    stat(path.c_str(), &sbuf);
    return S_ISDIR(sbuf.st_mode);
}


bool HTTP::serve_index()
{
    string index[] ={"index.html", "index.htm", "home.html"};
    string path = get_absolute_path();
    for(int i=0; i<3; i++)
    {
        string now = path+index[i];
        if(is_path_exist(now))
        {
            serve_file(now);
            return true;

        }

    }
    return false;
}

bool HTTP::is_path_exist(string &path)
{
    struct stat sbuf;
    int n = stat(path.c_str(), &sbuf);
    return n ==0;
}


void HTTP::serve_hello()
{
    
    string body;
    body +="<HTML><HEAD><TITLE>HELLO</TITLE></HEAD>\r\n";
    body +="<BODY><p>hello world!</p>\r\n";
    body +="</p></HTML>\r\n";

    _response.set_status_code(200);
    _response.add_header("Content-Type", "text/html");
    _response.set_body(body);
}

string HTTP::get_absolute_path()
{
    string path = _request.get_uri();
    auto it = path.begin();
    while(*it == '.' ||*it == '/')
        it++;
    string sub(it, path.end());
    return HTTP_base::ROOT_DIR+sub;
}

bool HTTP::supported()
{
    //accept_request first
    assert(_request_flag);

    if(_request.get_method() == HTTP_Method::UNKNOWN )
    {
        //cout<<"method UNKNOWN"<<endl;
        return false;
    }

    if(_request.get_version() == HTTP_Version::UNKNOWN)
    {
        //cout<<"Version UNKNOWN"<<endl;
        return false;
    }

    return true;
}

void HTTP::noimplement()
{
    string body;
    body +="<HTML><HEAD><TITLE>method not implemented!</TITLE></HEAD>\r\n";
    string method = method_to_str_dict[_request.get_method()];
    body +="<BODY><p>http request method ";
    body+= method;
    body += " not implemented</p>\r\n";
    body +="</p></HTML>\r\n";

    _response.set_status_code(501);
    _response.add_header("Content-Type", "text/html");
    _response.set_body(body);
}

void HTTP::not_found()
{
    string body;

    body +="<HTML><HEAD><TITLE>Not Found!</TITLE></HEAD>\r\n";
    body +="<BODY><p>404 not found</p>\r\n";
    body +="</p></HTML>\r\n";

    _response.set_status_code(404);
    _response.add_header("Content-Type", "text/html");
    _response.set_body(body);
}
