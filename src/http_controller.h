#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include <httpp/http/Request.hpp>
#include <httpp/http/Response.hpp>

class http_controller
{
public:
  http_controller();
  http_controller(http_controller&&) = default;                 
  http_controller& operator=(http_controller&&) = default;      
  http_controller(const http_controller&) = default;             
  http_controller& operator=(const http_controller&) = default;    
  virtual ~http_controller() = default;  
  virtual void get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
  virtual void head(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
  virtual void post(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector<char>& body);
  virtual void put(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector<char>& body);
  virtual void delete_(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
  
};

#endif // HTTP_CONTROLLER_H
