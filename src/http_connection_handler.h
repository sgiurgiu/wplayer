#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "http_controller.h"
#include "http_config.h"

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>

#include <memory>

class http_connection_handler
{
public:
  http_connection_handler(const http_config& config);
  void operator()(HTTPP::HTTP::Connection* connection);
private:
  typedef std::unique_ptr<http_controller> controller_ptr;
  controller_ptr files;
  controller_ptr apis;
};

#endif // HTTP_CONNECTION_HANDLER_H
