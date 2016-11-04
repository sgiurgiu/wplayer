#include "http_config.h"
#include "picojson.h"

#include <fstream>
#include <exception>

http_config::http_config(const std::string& config_file)
{
    std::ifstream file(config_file);
    picojson::value config;
    std::string res = picojson::parse(config,file);
    if (!res.empty()) {
        throw std::runtime_error("Cannot parse "+config_file+". Error: "+res);
    }
    display = config.get("display").to_str();
    picojson::value multimedia_sets = config.get("multimedia_sets");
    if(multimedia_sets.is<picojson::null>() || !multimedia_sets.is<picojson::object>())
    {
        throw std::runtime_error("No usable multimedia_sets defined");
    }
    picojson::object multimedia_sets_obj = multimedia_sets.get<picojson::object>();
    for(const auto& pair : multimedia_sets_obj)
    {
        multimedia_folders.insert({pair.first,pair.second.to_str()});
    }
    
    picojson::value http_server = config.get("http_server");
    if(http_server.is<picojson::null>() || !http_server.is<picojson::object>())
    {
        throw std::runtime_error("No usable http_server defined");
    }
    this->http_server.bind_address = http_server.get("address").to_str();
    this->http_server.files_folder = http_server.get("web_files_folder").to_str();    
    this->http_server.port = http_server.get("port").get<int64_t>();
    
    picojson::value player_properties = config.get("player_properties");
    if(player_properties.is<picojson::object>())
    {
        picojson::object player_properties_obj = player_properties.get<picojson::object>();
        for(const auto& pair : player_properties_obj)
        {
            picojson::value val = pair.second;
            int type = val.get_type();
            switch(type)
            {
                case picojson::string_type:
                    this->player_properties.insert({pair.first,val.get<std::string>()});
                    break;
                case picojson::boolean_type:
                    this->player_properties.insert({pair.first,val.get<bool>()});                    
                    break;                    
                case picojson::number_type:
                    this->player_properties.insert({pair.first,val.get<double>()});                    
                    break;                    
                case picojson::int64_type:
                    this->player_properties.insert({pair.first,val.get<int64_t>()});                    
                    break;                                    
            }
        }        
    }
}
