#ifndef MOVIE_CONTROLLER_H
#define MOVIE_CONTROLLER_H

#include "http_controller.h"

class movie_controller : public http_controller
{
public:
    movie_controller();
    virtual void get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response);
};

#endif // MOVIE_CONTROLLER_H
