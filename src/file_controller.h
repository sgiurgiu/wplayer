#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "http_controller.h"
#include <string>
#include <memory>

class magic_handler;
class file_controller : public http_controller
{
public:
    file_controller(const std::string& dir);
    virtual ~file_controller();      
    virtual void get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response) override;
private:
    std::string get_mime_type(const std::string& file);
private:
    std::string base_dir = "";
    std::unique_ptr<magic_handler> magic;
};

#endif // FILE_CONTROLLER_H
