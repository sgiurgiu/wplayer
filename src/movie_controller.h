#ifndef MOVIE_CONTROLLER_H
#define MOVIE_CONTROLLER_H

#include <string>

class movie_controller
{
public:
    movie_controller();
    virtual void get(const std::string& path);
};

#endif // MOVIE_CONTROLLER_H
