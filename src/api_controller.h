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
    template <typename Callable,typename Req,typename...Arguments>
    bool dispatch(Callable&& cb,Req& request, Arguments&&...params)
    {
      auto ident = get_controller_identifier(request);
      auto controller = sub_controllers.find(ident);
      bool found = (controller != sub_controllers.end());
      if( found )
      {
        auto new_req = request;
        new_req.uri = get_controller_uri(request,"/api");
        ((controller->second.get())->*cb)(new_req,std::forward<Arguments>(params)...);
      }
      return found;      
    }
    std::string get_controller_identifier(const HTTPP::HTTP::Request& request) const;
    typedef std::unordered_map<std::string,std::unique_ptr<http_controller>> controllers_map;
    controllers_map sub_controllers;
};

#endif // API_CONTROLLER_H
