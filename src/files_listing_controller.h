#ifndef FILES_LISTING_CONTROLLER_H
#define FILES_LISTING_CONTROLLER_H

#include "http_controller.h"
#include "http_config.h"
#include <memory>

class magic_handler;
class files_listing_controller : public http_controller
{
public:
    files_listing_controller(const http_config& config);
    virtual ~files_listing_controller();
    virtual void get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
private:
  std::vector<std::string> folders;
  std::unique_ptr<magic_handler> magic;
};

#endif // FILES_LISTING_CONTROLLER_H
