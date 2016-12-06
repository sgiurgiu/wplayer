#ifndef FILES_LISTING_CONTROLLER_H
#define FILES_LISTING_CONTROLLER_H

#include "database.h"
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
    files_listing_controller(database* db);
    ~files_listing_controller();
    crow::response get(const std::string& path) const;
    crow::response get_sets() const;
private:
  std::unique_ptr<magic_handler> magic;
  database* db;
  static log4cplus::Logger logger;
};

#endif // FILES_LISTING_CONTROLLER_H
