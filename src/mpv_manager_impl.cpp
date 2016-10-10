#include "mpv_manager_impl.h"
#include <mpv/client.h>

log4cxx::LoggerPtr mpv_manager_impl::logger(log4cxx::Logger::getLogger("mpv_manager_impl"));

mpv_manager_impl::mpv_manager_impl():handle(nullptr,mpv_handle_deleter())
{
}
mpv_manager_impl::~mpv_manager_impl()
{
}

void mpv_manager_impl::play(const std::string& path)
{
  std::lock_guard<std::mutex> lock(mu);

  handle.reset(mpv_create());
  const char* env = "DISPLAY=:0";
  putenv((char*)env);
  mpv_set_option_string(handle.get(), "input-default-bindings", "yes");
  mpv_set_option_string(handle.get(), "input-vo-keyboard", "yes");
  mpv_set_option_string(handle.get(), "vo", "opengl-hq,opengl,vdpau,xv,");
  mpv_set_option_string(handle.get(), "ao", "pulse,alsa");
  mpv_set_option_string(handle.get(), "hwdec", "auto");
  mpv_set_option_string(handle.get(), "terminal", "yes");
  mpv_set_option_string(handle.get(), "msg-level", "all=v");
  mpv_set_option_string(handle.get(), "fullscreen","");
  mpv_set_option_string(handle.get(), "ontop","");
  mpv_set_option_string(handle.get(), "no-border","");
  //mpv_set_option_string(handle.get(), "cursor-autohide","always");

 // mpv_set_option(handle.get(), "wid", MPV_FORMAT_INT64, &win);
  int val = 1;
  mpv_set_option(handle.get(), "osc", MPV_FORMAT_FLAG, &val);
  mpv_initialize(handle.get());

  const char* cmd[] = {"loadfile", path.c_str(), "replace",nullptr};
  mpv_command(handle.get(),cmd);
  while (1)
  {
    mpv_event *mp_event = mpv_wait_event(handle.get(), 10000);
    if (mp_event->event_id == MPV_EVENT_SHUTDOWN)
        break;
    LOG4CXX_DEBUG(logger, "event: "<< mpv_event_name(mp_event->event_id));
  }
}
