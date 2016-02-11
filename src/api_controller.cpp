#include "api_controller.h"
#include "files_listing_controller.h"
#include "movie_controller.h"

using namespace HTTPP::HTTP;

api_controller::api_controller(const http_config& config)
{
  sub_controllers.emplace("files", std::make_unique<files_listing_controller>(config));
  sub_controllers.emplace("movie", std::make_unique<movie_controller>());
}

api_controller::~api_controller() = default;


void api_controller::get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response)
{
  if(!dispatch(&http_controller::get,request,response))
  {
    http_controller::get(request,response);    
  }
}

void api_controller::head(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response)
{
  if(!dispatch(&http_controller::head,request,response))
  {
    http_controller::head(request,response);    
  }
}

void api_controller::post(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector< char >& body)
{
  if(!dispatch(&http_controller::post,request,response,body))
  {
    http_controller::post(request,response,body);
  }  
}

void api_controller::put(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response, const std::vector< char >& body)
{
  if(!dispatch(&http_controller::put,request,response,body))
  {
    http_controller::put(request,response,body);
  }  
}

void api_controller::delete_(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response)
{
  if(!dispatch(&http_controller::delete_,request,response))
  {
    http_controller::delete_(request,response);
  }    
}
std::string api_controller::get_controller_identifier(const HTTPP::HTTP::Request& request) const
{
  auto uri = request.uri;
  if(!uri.starts_with("/api")) return "";
  auto str_uri = uri.to_string();
  auto end = str_uri.find('/',5);  
  return str_uri.substr(5, end == std::string::npos ? std::string::npos : end - 5);
}

