#include "file_controller.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <magic.h>

using namespace boost::filesystem;
using namespace HTTPP;
using namespace HTTPP::HTTP;

file_controller::file_controller(const std::string& dir):base_dir(dir)
{

}
void file_controller::get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response)
{
  path file(base_dir);  
  auto uri = request.uri;
  std::cout << uri <<std::endl;
  if(uri == "/") uri = "index.html";
  std::cout << uri <<std::endl;
  file /= uri.to_string();
  std::cout << file <<std::endl;
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
  magic_t myt = magic_open(MAGIC_CONTINUE|MAGIC_ERROR/*|MAGIC_DEBUG*/|MAGIC_MIME_TYPE|MAGIC_MIME_ENCODING);
  magic_load(myt,NULL);
  std::string mime = magic_file(myt, file.c_str());
  std::cout << mime <<std::endl;
  magic_close(myt);
  return mime;
}
