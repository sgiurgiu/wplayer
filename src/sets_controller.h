#ifndef SETS_CONTROLLER_H
#define SETS_CONTROLLER_H

#include "database.h"
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
    crow::response get_sets() const;
    crow::response add_set(const crow::request& req);
    crow::response delete_set(const std::string& name);
private:
    database* db;
    static log4cplus::Logger logger;
};

#endif // SETS_CONTROLLER_H
