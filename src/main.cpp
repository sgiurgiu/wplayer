#include "http_connection_handler.h"
#include "http_config.h"
#include <iostream>
#include <memory>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <httpp/HttpServer.hpp>

static bool shouldExit = false;

void handle_term(int /*signo*/)
{
  std::cout << "exiting "<<std::endl;
  shouldExit = true;
}

int main(int argc, char **argv) {
 
  if(false)
  {
    daemon(0,0);
  }  
  signal(SIGTERM, handle_term);  
  signal(SIGINT, handle_term);
  HTTPP::HttpServer server;
  server.start();
  http_config config;
  config.files_folder = "web";
  config.multimedia_folders.emplace_back("/mnt/homebackup/Downloads/complete/");
  http_connection_handler handler(config);
  server.setSink(std::ref(handler));
  server.bind("localhost", "8080");
  while(!shouldExit)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0;
}
