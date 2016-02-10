#ifndef API_CONTROLLER_H
#define API_CONTROLLER_H

#include "http_controller.h"
#include "http_config.h"
#include <unordered_map>

class api_controller :  public http_controller
{
public:
    api_controller(const http_config& config);
    ~api_controller();
    virtual void get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
    virtual void head(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
    virtual void post(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector< char >& body);
    virtual void put(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector< char >& body);
    virtual void delete_(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
private:
    template <typename Callable>
    bool call_sub_controller(Callable&& cb,const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
    template <typename Callable>
    bool call_sub_controller(Callable&& cb,const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector< char >& body);
    /*template <typename Callable,typename Req,typename...Arguments>
    bool call_sub_controller(Callable&& cb,Req& request, Arguments&&...params);    */
    std::string get_controller_identifier(const HTTPP::HTTP::Request& request) const;
    typedef std::unordered_map<std::string,std::unique_ptr<http_controller>> controllers_map;
    controllers_map sub_controllers;
};

#endif // API_CONTROLLER_H
