#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H

#include <string>
#include <map>
#include <boost/variant.hpp>
#include <vector>

typedef std::map<std::string,std::string> folders;
typedef std::map<std::string,boost::variant<double,int64_t,bool,std::string>> variant_map;
struct server_config 
{
    std::string files_folder = "";
    std::string bind_address = "localhost";
    int port = 8080;
};
class http_config
{
public:
    http_config(const std::string& config_file);
public:    
    folders multimedia_folders;    
    server_config http_server;
    variant_map player_properties;
    std::vector<std::string> environment;
};

#endif // HTTP_CONFIG_H
