/*************************************************************************
	> File Name: helper.h
	> Author:  YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Mon 16 May 2016 08:44:15 PM CST
 ************************************************************************/

#ifndef _HELPER_H
#define _HELPER_H

#include <string>

using std::string;

/*return uri and save query_str in query*/
string parse_uri(string &path, string &query);

bool isspace(char c);


#endif
