#ifndef MOVIE_CONTROLLER_H
#define MOVIE_CONTROLLER_H

#include "http_config.h"
#include "mpv_manager.h"
#include <string>
#include <log4cplus/logger.h>


namespace crow {
    struct response;
}

class movie_controller
{
public:
    movie_controller(const http_config& config);
    crow::response play(const std::string& set,const std::string& path);
private:
    folders multimedia_folders;
    mpv_manager mpv;
    static log4cplus::Logger logger;
};

#endif // MOVIE_CONTROLLER_H
