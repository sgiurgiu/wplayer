#include "http_connection_handler.h"
#include "file_controller.h"

using namespace HTTPP;
using namespace HTTPP::HTTP;


http_connection_handler::http_connection_handler(const http_config& config):files(std::make_unique<file_controller>(config.files_folder))
{

}

void http_connection_handler::operator()(Connection* connection)
{
  read_whole_request(connection, [this](std::unique_ptr<
                                          HTTPP::HTTP::helper::ReadWholeRequest> hndl,
                                      const boost::system::error_code& ec) {
        const auto& body = hndl->body;
        const auto& connection = hndl->connection;
        const Request& request  = connection->request();
        Response& response = connection->response();
        if (ec)
        {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
        }
        http_controller* controller = nullptr;
        if(request.uri.starts_with("/api"))
        {
          //api controller
        }
        else
        {
          controller = files.get();
        }
        if(controller != nullptr)
        {
          switch(request.method)
          {
            case Method::HEAD:
              controller->head(request,response);
              break;
            case Method::GET:
              controller->get(request,response);
              break;
            case Method::PUT:
              controller->put(request,response,body);
              break;
            case Method::POST:
              controller->post(request,response,body);
              break;
            case Method::DELETE_:
              controller->delete_(request,response);
              break;
            default:
              response.setCode(HttpCode::MethodNotAllowed);
              response.setBody("Method Not Allowed");
              break;
          }
        }
        else
        {
          response.setCode(HttpCode::BadRequest);
          response.setBody("Bad Request");          
        }
        HTTPP::HTTP::setShouldConnectionBeClosed(request, response);
        connection->sendResponse(); // connection pointer may become invalid
    });
}
