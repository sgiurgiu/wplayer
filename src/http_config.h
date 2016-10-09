#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H

#include <string>
#include <map>

typedef std::map<std::string,std::string> folders;
struct http_config
{
    std::string files_folder = "";
    folders multimedia_folders;
};

#endif // HTTP_CONFIG_H
