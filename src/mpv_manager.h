#ifndef MPV_MANAGER_H
#define MPV_MANAGER_H

#include <memory>
#include <mpv/client.h>
#include <log4cplus/logger.h>

#include <mutex>


struct mpv_handle_deleter
{
  void operator()(mpv_handle* handle)
  {
     mpv_terminate_destroy(handle);
  }
};

class mpv_manager
{
public:
    mpv_manager();
    ~mpv_manager();
    void play(const std::string& path);
    
private:    
  std::unique_ptr< mpv_handle, mpv_handle_deleter > handle;
  std::mutex mu;
  log4cplus::Logger logger;
};

#endif // MPV_MANAGER_H
