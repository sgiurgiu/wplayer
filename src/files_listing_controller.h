#ifndef FILES_LISTING_CONTROLLER_H
#define FILES_LISTING_CONTROLLER_H

#include "http_config.h"
#include <memory>
#include <string>
#include <log4cplus/logger.h>

namespace crow {
    struct response;
}

class magic_handler;
class files_listing_controller
{
public:
    files_listing_controller(const http_config& config);
    ~files_listing_controller();
    crow::response get(const std::string& set,const std::string& path) const;
    crow::response get_sets() const;
private:
  folders multimedia_folders;
  std::unique_ptr<magic_handler> magic;
  static log4cplus::Logger logger;
};

#endif // FILES_LISTING_CONTROLLER_H
