#ifndef FILES_LISTING_CONTROLLER_H
#define FILES_LISTING_CONTROLLER_H

#include "http_config.h"
#include <memory>
#include <string>

namespace crow {
    struct response;
}

class magic_handler;
class files_listing_controller
{
public:
    files_listing_controller(const http_config& config);
    ~files_listing_controller();
    crow::response get(const std::string& path);
private:
  std::vector<std::string> folders;
  std::unique_ptr<magic_handler> magic;
};

#endif // FILES_LISTING_CONTROLLER_H
