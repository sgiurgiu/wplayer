#include "mpv_manager.h"
#include "mpv_manager_p.h"
#include <mpv/client.h>
#include <iostream>

mpv_manager_impl::mpv_manager_impl():handle(mpv_create(),mpv_handle_deleter())
{
}
mpv_manager_impl::~mpv_manager_impl()
{
}

void mpv_manager_impl::show_window()
{  
  putenv("DISPLAY=:0");
  //mpv_set_option_string(handle.get(), "input-default-bindings", "yes");
  //mpv_set_option_string(handle.get(), "input-vo-keyboard", "yes");  
  mpv_set_option_string(handle.get(), "vo", "opengl-hq,opengl,vdpau,xv,");  
  mpv_set_option_string(handle.get(), "hwdec", "auto");  
  mpv_set_option_string(handle.get(), "terminal", "yes");
  mpv_set_option_string(handle.get(), "msg-level", "all=v");  
  mpv_set_option_string(handle.get(), "fullscreen","");
  mpv_set_option_string(handle.get(), "ontop","");
  mpv_set_option_string(handle.get(), "no-border","");
  mpv_set_option_string(handle.get(), "cursor-autohide","always");
  
 // mpv_set_option(handle.get(), "wid", MPV_FORMAT_INT64, &win);
  int val = 1;
  mpv_set_option(handle.get(), "osc", MPV_FORMAT_FLAG, &val);  
  mpv_initialize(handle.get());

  const char* cmd[] = {"loadfile", "/home/sergiu/Downloads/simpsons.mp4", "replace",nullptr};
  mpv_command(handle.get(),cmd);
  while (1) {
    mpv_event *mp_event = mpv_wait_event(handle.get(), 10000);
    if (mp_event->event_id == MPV_EVENT_SHUTDOWN)
        break;
    std::cout << "event: "<< mpv_event_name(mp_event->event_id)<<std::endl;
  }
}

mpv_manager::mpv_manager()
    : impl(std::make_unique<mpv_manager_impl>())
{

}
mpv_manager::~mpv_manager() = default;

void mpv_manager::show_window()
{
  impl->show_window();
}
