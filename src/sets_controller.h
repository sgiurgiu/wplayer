#ifndef SETS_CONTROLLER_H
#define SETS_CONTROLLER_H

#include "database.h"
#include <memory>
#include <log4cplus/logger.h>

namespace crow {
    struct response;
    struct request;
}

class sets_controller
{
public:
    sets_controller(database* db);
    ~sets_controller();
    std::unique_ptr<crow::response> get_sets() const;
    std::unique_ptr<crow::response> add_set(const crow::request& req);
    std::unique_ptr<crow::response> delete_set(const std::string& name);
private:
    database* db;
    static log4cplus::Logger logger;
};

#endif // SETS_CONTROLLER_H
