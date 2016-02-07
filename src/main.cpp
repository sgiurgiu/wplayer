#include <iostream>
#include <thread>
#include <memory>
#include <string>

#include "mpv_manager.h"

#include <microhttpd.h>
#include <signal.h>
#include <unistd.h>

static bool shouldExit = false;

void handle_term(int /*signo*/)
{
    shouldExit = true;
}

int url_handler (void *cls,
    MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data, size_t *upload_data_size, void **ptr)
{
  std::cout << "url:"<<url<<std::endl;
  std::cout << "method:"<<method<<std::endl;
  std::string buf = "Hello there";
  MHD_Response* response = MHD_create_response_from_buffer (buf.length(), (void*)buf.c_str(), MHD_RESPMEM_MUST_COPY);  
  mpv_manager mpv;
  mpv.show_window();
  MHD_add_response_header(response, "Content-Type", "text");
  int ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}
 
void run_server()
{
  auto mhd_deleter = [](MHD_Daemon* d){
    MHD_stop_daemon(d);
  };
  std::unique_ptr<MHD_Daemon,decltype(mhd_deleter)> d ( MHD_start_daemon(MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,8080,
    nullptr,nullptr,&url_handler,nullptr,MHD_OPTION_END
  ),mhd_deleter);
  
  while(!shouldExit) {
    sleep(1);
  }
}

int main(int argc, char **argv) {
 
  if(false)
  {
    //daemon(0,0);
  }  
  signal(SIGTERM, handle_term);  
  std::thread main_thread([](){
    run_server();
  });
  main_thread.join();
  return 0;
}
