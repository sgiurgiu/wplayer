#include "file_controller.h"
#include "magic_handler.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <magic.h>

using namespace boost::filesystem;
using namespace HTTPP;
using namespace HTTPP::HTTP;


file_controller::file_controller(const std::string& dir):base_dir(dir),magic(std::make_unique<magic_handler>())
{
}
file_controller::~file_controller() = default;  

void file_controller::get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response)
{
  path file(base_dir);  
  auto uri = request.uri;
  if(uri == "/" || uri == "/current" || uri == "/movies") uri = "index.html";
  file /= uri.to_string();
  if(exists(file))
  { 
    std::stringstream ss;
    ss << std::ifstream( file.string() ).rdbuf();    
    response.setCode(HttpCode::Ok);
    response.addHeader("Content-Type",get_mime_type(file.string()));
    response.setBody(ss.str());    
  }
  else 
  {
    response.setCode(HttpCode::NotFound);
    response.setBody("Not Found");
  }
}
std::string file_controller::get_mime_type(const std::string& file)
{
  if(file.rfind(".css") == (file.length() - 4))
  {
    return "text/css";
  }
  if(file.rfind(".js") == (file.length() - 3))
  {
    return "application/javascript; charset=UTF-8";
  }
  
  return magic->get_mime(file);
}
