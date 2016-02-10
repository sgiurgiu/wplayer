#include "http_controller.h"

http_controller::http_controller()
{

}
void http_controller::get(const HTTPP::HTTP::Request& /*request*/, HTTPP::HTTP::Response& response)
{
  response.setCode(HTTPP::HTTP::HttpCode::NotImplemented);
  response.setBody("Not Implemented");
}
void http_controller::head(const HTTPP::HTTP::Request& /*request*/, HTTPP::HTTP::Response& response)
{
  response.setCode(HTTPP::HTTP::HttpCode::NotImplemented);
  response.setBody("Not Implemented");
}
void http_controller::post(const HTTPP::HTTP::Request& /*request*/, HTTPP::HTTP::Response& response, const std::vector< char >& /*body*/)
{
  response.setCode(HTTPP::HTTP::HttpCode::NotImplemented);
  response.setBody("Not Implemented");
}
void http_controller::put(const HTTPP::HTTP::Request& /*request*/, HTTPP::HTTP::Response& response, const std::vector< char >& /*body*/)
{
  response.setCode(HTTPP::HTTP::HttpCode::NotImplemented);
  response.setBody("Not Implemented");
}
void http_controller::delete_(const HTTPP::HTTP::Request& /*request*/, HTTPP::HTTP::Response& response)
{
  response.setCode(HTTPP::HTTP::HttpCode::NotImplemented);
  response.setBody("Not Implemented");
}
std::string http_controller::get_controller_uri(const HTTPP::HTTP::Request& request,const std::string& base) const
{
  auto uri = request.uri;
  if(!uri.starts_with(base)) return "";
  return uri.substr(base.length()).to_string();
}