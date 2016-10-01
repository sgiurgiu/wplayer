#ifndef NOT_FOUND_EXCEPTION_H
#define NOT_FOUND_EXCEPTION_H

#include <stdexcept>

class not_found_exception : public std::runtime_error
{
public:
    not_found_exception(const std::string& what):std::runtime_error(what){}
    not_found_exception(const char* what): std::runtime_error(what) {}

};

#endif // NOT_FOUND_EXCEPTION_H
