#ifndef MPV_MANAGER_IMPL_H
#define MPV_MANAGER_IMPL_H

#include <memory>
#include <mpv/client.h>

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
  void show_window();
private:
  std::unique_ptr< mpv_handle, mpv_handle_deleter > handle;
};

#endif // MPV_MANAGER_IMPL_H
