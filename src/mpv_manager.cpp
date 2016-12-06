#include "mpv_manager.h"
#include <log4cplus/loggingmacros.h>
#include <iostream>
#include <thread>

#include <boost/variant.hpp>

class mpv_option_visitor
    : public boost::static_visitor<>
{
private:
    std::string key;
    mpv_handle* handle;    
public:
    mpv_option_visitor(const std::string& key,mpv_handle* handle):key(key),handle(handle)
    {
    }    
    void operator()(int64_t i) const
    {
        mpv_set_option(handle, key.c_str(), MPV_FORMAT_INT64, &i);
    }
    void operator()(const std::string & str) const
    {
        mpv_set_option_string(handle, key.c_str(), str.c_str());
    }
    void operator()(bool b) const
    {
        mpv_set_option(handle, key.c_str(), MPV_FORMAT_FLAG, &b);
    }
    void operator()(double d) const
    {
        mpv_set_option(handle, key.c_str(), MPV_FORMAT_DOUBLE, &d);
    }
};
#include <string.h>
mpv_manager::mpv_manager(database* db)
    :handle(mpv_create(),mpv_handle_deleter()),logger(log4cplus::Logger::getInstance("mpv_manager"))
{
    //const char* env = "DISPLAY=:0";
    for(const auto& env : db->get_environment())
    {
        putenv((char*)env.c_str());
    }
        
    for(const auto& pair : db->get_player_properties())
    {
        mpv_set_option_string(handle.get(), pair.first.c_str(), pair.second.c_str());        
    }

    //mpv_set_option_string(handle.get(),"no-input-default-bindings","");
    //mpv_set_option_string(handle.get(), "msg-level", "all=v");
    //mpv_set_option_string(handle.get(), "fullscreen","");
    //mpv_set_option_string(handle.get(), "ontop","");
    //mpv_set_option_string(handle.get(), "no-border","");
    //mpv_set_option_string(handle.get(), "cursor-autohide","always");
    
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

std::string mpv_manager::create_metadata_object(mpv_node* metadata) const
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
picojson::value mpv_manager::create_node_json_value(const mpv_node& node)  const
{    
    //LOG4CPLUS_DEBUG(logger, "Metadata node format : "<< node.format);
    switch(node.format)
    {
        case MPV_FORMAT_STRING:
            return picojson::value(node.u.string);
        case MPV_FORMAT_FLAG:
            return picojson::value((bool)(node.u.flag==1));
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
picojson::array mpv_manager::create_node_json_array(const mpv_node& node) const
{
    picojson::array array;
    mpv_node_list* list = node.u.list;
    for(int i=0;i<list->num;i++)
    {
        array.push_back(create_node_json_value(list->values[i]));
    }    
    return array;
}
picojson::object mpv_manager::create_node_json_map(const mpv_node& node) const
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

void mpv_manager::play_youtube(const std::string& youtube_url)
{
    const char* cmd[] = {"loadfile", youtube_url.c_str(), "replace",nullptr};
    mpv_command(handle.get(),cmd);
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
void mpv_manager::pause()
{
    int val = 1;
    mpv_set_property(handle.get(),"pause",MPV_FORMAT_FLAG,&val);
}
void mpv_manager::resume()
{
    int val = 0;
    mpv_set_property(handle.get(),"pause",MPV_FORMAT_FLAG,&val);
}
void mpv_manager::set_volume(double vol)
{
    int res = mpv_set_property(handle.get(),"volume",MPV_FORMAT_DOUBLE,&vol);
    if(res)
    {
        const char* ss = mpv_error_string(res);
        LOG4CPLUS_ERROR(logger, "Cannot set volume : "<< ss);
    }
}
void mpv_manager::backward()
{
    const char* cmd[] = {"seek", "-5", "relative",nullptr};
    mpv_command(handle.get(),cmd);
}
void mpv_manager::fast_backward()
{
    const char* cmd[] = {"seek", "-60", "relative",nullptr};
    mpv_command(handle.get(),cmd);
}
void mpv_manager::fast_forward()
{
    const char* cmd[] = {"seek", "60", "relative",nullptr};
    mpv_command(handle.get(),cmd);
}
void mpv_manager::forward()
{
    const char* cmd[] = {"seek", "5", "relative",nullptr};
    mpv_command(handle.get(),cmd);
}
void mpv_manager::seek_percent(double percent)
{
    const char* cmd[] = {"seek", std::to_string(percent).c_str(), "absolute-percent",nullptr};
    mpv_command(handle.get(),cmd);    
}
void mpv_manager::remove_sub(int64_t id)
{
    const char* cmd[] = {"sub-remove", std::to_string(id).c_str(), nullptr};
    mpv_command(handle.get(),cmd);    
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
    mpv_get_property(handle.get(),"volume",MPV_FORMAT_DOUBLE,&status.audio_volume);   
    mpv_get_property(handle.get(),"seekable",MPV_FORMAT_FLAG,&status.seekable);
    mpv_get_property(handle.get(),"pause",MPV_FORMAT_FLAG,&status.paused);
    mpv_node tracks;
    mpv_get_property(handle.get(),"track-list",MPV_FORMAT_NODE,&tracks);
    status.tracks = create_node_json_value(tracks);
    LOG4CPLUS_DEBUG(logger,"tracks list:"<<status.tracks.serialize());
    /*for(int64_t i = 0;i<tracks_count;++i)
    {
        int64_t id = 0;
        mpv_get_property(handle.get(),"track-list/count",MPV_FORMAT_INT64,&tracks_count);
        track-list/N/id
    }
    */
    //seekable
    //pause
    return status;
}
