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
    Rio_readinitb(&rio, _connfd);
    
    int nread;
    nread = Rio_readlineb(&rio, buf, MAXLINE);

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
    string query, uri;
    uri = parse_uri(raw_uri, query);

    _request.set_uri(uri);
    _request.set_query(query);
    
    
    cur = end;
    while(isspace(*cur))
        cur++;
    end = cur;
    while(!isspace(*end))
        end++;
    

    //get version
    _request.set_version(cur, end);


    //get headers
    while(nread = Rio_readlineb(&rio, buf, MAXLINE))
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
            nread = Rio_readnb(&rio, buf, MAXLINE);
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
    
    if(!is_path_exit(path) && _request.get_method()!=HTTP_Method::POST)
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
                success = execte_cgi(path);
            break;
        case HTTP_Method::PUT:
            success = put_file(path);
            break;

        case HTTP_Method::POST:
            success = execte_cgi(path);
            break;

    }
    if(!success)
        internel_server_error();

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
    
    std::ifstream f(path);
    string body;
    f>>body;
    
    _response.set_body(body);
}


string HTTP::get_filetype(string &path)
{
    int i = path.size()- 1;
    string type;
    while(path[i] !='.' ||path[i] != '/')
        i--;

    if(path[i] == '/')
        type = "/text/plain";
    else
    {
        string sub= path.substr(i);
        auto it = content_type_dict.find(sub);
        if(it != content_type_dict.end())
            type = it->second;
        else
            type = "/text/plain";
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
        if(is_path_exit(now))
        {
            serve_file(now);
            return true;

        }

    }
    return false;
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

    if(_request.get_method() == HTTP_Method::UNKNOWN || _request.get_version() == HTTP_Version::UNKNOWN)
        return false;

    return true;
}

void HTTP::noimplement()
{
    string body;
    body +="<HTML><HEAD><TITLE>method not implemented!</TITLE></HEAD>\r\n";
    body +="<BODY><p>http request method not implemented</p>\r\n";
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
