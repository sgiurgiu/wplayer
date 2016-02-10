#include "movie_controller.h"

movie_controller::movie_controller()
{

}
void movie_controller::get(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Response& response)
{
  response.setCode(HTTPP::HTTP::HttpCode::Ok);
  response.setBody("from movie_controller");
}
