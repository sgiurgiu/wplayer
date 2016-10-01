#include "file_controller.h"
#include "magic_handler.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "crow/crow_all.h"
#include "not_found_exception.h"

using namespace boost::filesystem;


file_controller::file_controller(const std::string& dir):base_dir(dir),magic(std::make_unique<magic_handler>())
{
}
file_controller::~file_controller() = default;  

crow::response file_controller::get_file_contents(const std::string& file)
{
    
    crow::response rsp;
    try
    {
        std::string file_name = get_file(file).string();
        std::ifstream in (file_name);   
        rsp.code = 200;
        std::string contents = (static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());
        rsp.set_header("Content-Length",std::to_string(contents.length()));        
        std::string mime = get_mime_type(file_name);        
        rsp.set_header("Content-Type",mime);
        rsp.write(contents);
    }catch(const not_found_exception& ex) 
    {
        rsp.code = 404;
        rsp.write(ex.what());
    }
    //rsp.end();
    return rsp;

}
boost::filesystem::path file_controller::get_file(const std::string& file)
{
    boost::filesystem::path base_dir(this->base_dir);
    boost::filesystem::path file_(file); 
    boost::filesystem::path file_to_read = base_dir / file;        
    boost::filesystem::file_status status = boost::filesystem::status(file_to_read);
    if(status.type() != boost::filesystem::regular_file)
    {        
        throw not_found_exception("Cannot find "+file);
    }
    else
    {
        return file_to_read;
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
