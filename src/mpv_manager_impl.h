#ifndef MPV_MANAGER_IMPL_H
#define MPV_MANAGER_IMPL_H

#include <memory>
#include <mpv/client.h>
#include <log4cxx/logger.h>
#include <mutex>

struct mpv_handle_deleter
{
  void operator()(mpv_handle* handle)
  {
     mpv_terminate_destroy(handle);
  }
};

class mpv_manager_impl
{
public:
  mpv_manager_impl();
  ~mpv_manager_impl();
  void play(const std::string& path);
private:
  std::unique_ptr< mpv_handle, mpv_handle_deleter > handle;
  std::mutex mu;
  static log4cxx::LoggerPtr logger;
};

#endif // MPV_MANAGER_IMPL_H
