#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H

#include <string>
#include <vector>

struct http_config
{
    std::string files_folder = "";
    std::vector<std::string> multimedia_folders;
};

#endif // HTTP_CONFIG_H
