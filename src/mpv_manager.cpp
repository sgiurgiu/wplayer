#include "mpv_manager.h"

#include <log4cplus/loggingmacros.h>
#include <iostream>
#include <thread>


mpv_manager::mpv_manager()
    :handle(mpv_create(),mpv_handle_deleter()),logger(log4cplus::Logger::getInstance("mpv_manager"))
{
    const char* env = "DISPLAY=:0";
    putenv((char*)env);
    mpv_set_option_string(handle.get(), "input-default-bindings", "yes");
    mpv_set_option_string(handle.get(), "input-vo-keyboard", "yes");
    mpv_set_option_string(handle.get(), "vo", "opengl-hq,opengl,vdpau,xv,");
    mpv_set_option_string(handle.get(), "ao", "pulse,alsa");
    mpv_set_option_string(handle.get(), "hwdec", "auto");
    mpv_set_option_string(handle.get(), "terminal", "yes");
    //mpv_set_option_string(handle.get(), "msg-level", "all=v");
    //mpv_set_option_string(handle.get(), "fullscreen","");
    //mpv_set_option_string(handle.get(), "ontop","");
    //mpv_set_option_string(handle.get(), "no-border","");
    //mpv_set_option_string(handle.get(), "cursor-autohide","always");

    // mpv_set_option(handle.get(), "wid", MPV_FORMAT_INT64, &win);
    int val = 1;
    mpv_set_option(handle.get(), "osc", MPV_FORMAT_FLAG, &val);
    mpv_request_event(handle.get(),MPV_EVENT_LOG_MESSAGE,0);
    mpv_request_event(handle.get(),MPV_EVENT_GET_PROPERTY_REPLY,0);
    mpv_request_event(handle.get(),MPV_EVENT_SET_PROPERTY_REPLY,0);
    mpv_request_event(handle.get(),MPV_EVENT_COMMAND_REPLY,0);
    mpv_request_event(handle.get(),MPV_EVENT_START_FILE,0);
    mpv_request_event(handle.get(),MPV_EVENT_END_FILE,0);
    mpv_request_event(handle.get(),MPV_EVENT_FILE_LOADED,0);
    mpv_request_event(handle.get(),MPV_EVENT_TRACKS_CHANGED,0);
    mpv_request_event(handle.get(),MPV_EVENT_TRACK_SWITCHED,0);
    mpv_request_event(handle.get(),MPV_EVENT_IDLE,0);
    mpv_request_event(handle.get(),MPV_EVENT_PAUSE,0);
    mpv_request_event(handle.get(),MPV_EVENT_UNPAUSE,0);
    mpv_request_event(handle.get(),MPV_EVENT_TICK,0);
    mpv_request_event(handle.get(),MPV_EVENT_SCRIPT_INPUT_DISPATCH,0);
    mpv_request_event(handle.get(),MPV_EVENT_CLIENT_MESSAGE,0);
    mpv_request_event(handle.get(),MPV_EVENT_VIDEO_RECONFIG,0);
    mpv_request_event(handle.get(),MPV_EVENT_AUDIO_RECONFIG,0);
    mpv_request_event(handle.get(),MPV_EVENT_METADATA_UPDATE,0);
    mpv_request_event(handle.get(),MPV_EVENT_SEEK,0);
    mpv_request_event(handle.get(),MPV_EVENT_PLAYBACK_RESTART,0);
    mpv_request_event(handle.get(),MPV_EVENT_PROPERTY_CHANGE,0);
    mpv_request_event(handle.get(),MPV_EVENT_CHAPTER_CHANGE,0);
    mpv_request_event(handle.get(),MPV_EVENT_QUEUE_OVERFLOW,0);
    //mpv_observe_property(handle.get(),0,"",MPV_FORMAT_STRING);
    
    mpv_initialize(handle.get());
    std::thread t([this]()
    {
        while (true)
        {
            mpv_event *mp_event = mpv_wait_event(handle.get(), -1);
            if (mp_event->event_id == MPV_EVENT_SHUTDOWN)
            {
                break;
            }
            std::string ev_name(mpv_event_name(mp_event->event_id));
            LOG4CPLUS_DEBUG(logger, "event: "<< ev_name);                        
        }
        mpv_command_string(handle.get(),"stop");    
    });
   t.detach();
}
mpv_manager::~mpv_manager() = default;

std::string mpv_manager::create_metadata_object(mpv_node* metadata)
{
        if(metadata->format != MPV_FORMAT_NODE_MAP)
        {
            return "";
        }
        picojson::object metadata_object;
        mpv_node_list* list = metadata->u.list;
        LOG4CPLUS_DEBUG(logger, "Metadata count objects : "<< list->num);
        for(int i=0;i<list->num;i++)
        {
            picojson::value v = create_node_json_value(list->values[i]);
            metadata_object.insert({list->keys[i],v});
        }
        picojson::value val(metadata_object);
        return val.serialize();
}
picojson::value mpv_manager::create_node_json_value(const mpv_node& node)
{    
    LOG4CPLUS_DEBUG(logger, "Metadata node format : "<< node.format);
    switch(node.format)
    {
        case MPV_FORMAT_STRING:
            return picojson::value(node.u.string);
        case MPV_FORMAT_FLAG:
            return picojson::value((int64_t)node.u.flag);
        case MPV_FORMAT_DOUBLE:
            return picojson::value(node.u.double_);
        case MPV_FORMAT_INT64:
            return picojson::value(node.u.int64);
        case MPV_FORMAT_NODE_ARRAY:
            return picojson::value(create_node_json_array(node));
        case MPV_FORMAT_NODE_MAP:
            return picojson::value(create_node_json_map(node));
        default:
            break;
    }
    return picojson::value();
}
picojson::array mpv_manager::create_node_json_array(const mpv_node& node)
{
    picojson::array array;
    mpv_node_list* list = node.u.list;
    for(int i=0;i<list->num;i++)
    {
        array.push_back(create_node_json_value(list->values[i]));
    }    
    return array;
}
picojson::object mpv_manager::create_node_json_map(const mpv_node& node)
{
    picojson::object obj;
    mpv_node_list* list = node.u.list;
    for(int i=0;i<list->num;i++)
    {
        picojson::value v = create_node_json_value(list->values[i]);
        obj.insert({list->keys[i],v});
    }
    return obj;
}

void mpv_manager::play(const std::string& path)
{
    const char* cmd[] = {"loadfile", path.c_str(), "replace",nullptr};
    mpv_command(handle.get(),cmd);
}
void mpv_manager::stop()
{
    mpv_command_string(handle.get(),"stop");    
}

void mpv_manager::quit()
{
    mpv_command_string(handle.get(),"quit");    
}

mpv_status mpv_manager::get_mpv_status() const
{
    mpv_status status;
    mpv_get_property(handle.get(),"idle-active",MPV_FORMAT_FLAG,&status.idle);
    if(status.idle) return status;
    char* filename = mpv_get_property_string(handle.get(),"filename");
    if(filename) status.loaded_filename = filename;    
    mpv_get_property(handle.get(),"file-size",MPV_FORMAT_INT64,&status.file_size);
    mpv_get_property(handle.get(),"duration",MPV_FORMAT_DOUBLE,&status.total_duration);
    mpv_get_property(handle.get(),"percent-pos",MPV_FORMAT_INT64,&status.percent_complete);
    mpv_get_property(handle.get(),"time-pos",MPV_FORMAT_DOUBLE,&status.time_position);
    mpv_get_property(handle.get(),"ao-volume",MPV_FORMAT_INT64,&status.audio_volume);        
    
    return status;
}
