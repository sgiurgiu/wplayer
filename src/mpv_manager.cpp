#include "mpv_manager.h"

#include <log4cplus/loggingmacros.h>
#include <iostream>


mpv_manager::mpv_manager()
    :handle(nullptr,mpv_handle_deleter()), logger(log4cplus::Logger::getInstance("mpv_manager"))
{

}
mpv_manager::~mpv_manager() = default;

void mpv_manager::play(const std::string& path)
{
    {
        std::lock_guard<std::mutex> lock(mu);  
        if(!handle)
        {
            handle.reset(mpv_create());
        }    
        const char* env = "DISPLAY=:0";
        putenv((char*)env);
        mpv_set_option_string(handle.get(), "input-default-bindings", "yes");
        mpv_set_option_string(handle.get(), "input-vo-keyboard", "yes");
        mpv_set_option_string(handle.get(), "vo", "opengl-hq,opengl,vdpau,xv,");
        mpv_set_option_string(handle.get(), "ao", "pulse,alsa");
        mpv_set_option_string(handle.get(), "hwdec", "auto");
        mpv_set_option_string(handle.get(), "terminal", "yes");
        mpv_set_option_string(handle.get(), "msg-level", "all=v");
        //mpv_set_option_string(handle.get(), "fullscreen","");
        //mpv_set_option_string(handle.get(), "ontop","");
        //mpv_set_option_string(handle.get(), "no-border","");
        //mpv_set_option_string(handle.get(), "cursor-autohide","always");

        // mpv_set_option(handle.get(), "wid", MPV_FORMAT_INT64, &win);
        int val = 1;
        mpv_set_option(handle.get(), "osc", MPV_FORMAT_FLAG, &val);
        mpv_initialize(handle.get());

        const char* cmd[] = {"loadfile", path.c_str(), "replace",nullptr};
        mpv_command(handle.get(),cmd);
    }
    while (true)
    {
        mpv_event *mp_event = mpv_wait_event(handle.get(), 10000);
        if (mp_event->event_id == MPV_EVENT_SHUTDOWN)
            break;
        if (mp_event->event_id == MPV_EVENT_END_FILE)
            break;

        if (mp_event->event_id == MPV_EVENT_FILE_LOADED)
        {
            char* file_name = mpv_get_property_string(handle.get(),"media-title");
            LOG4CPLUS_DEBUG(logger, "LOADED FILE : "<< file_name);
            mpv_free(file_name);
        }

        LOG4CPLUS_DEBUG(logger, "event: "<< mpv_event_name(mp_event->event_id));
    }
    const char* quit_cmd[] = {"quit",nullptr};
    mpv_command(handle.get(),quit_cmd);
}
void mpv_manager::stop()
{
    std::lock_guard<std::mutex> lock(mu);  
    if(!handle) return;
    const char* quit_cmd[] = {"quit",nullptr};
    mpv_command(handle.get(),quit_cmd);    
}

