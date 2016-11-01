#ifndef MPV_MANAGER_H
#define MPV_MANAGER_H

#include <string>
#include <memory>
#include <mpv/client.h>
#include <log4cplus/logger.h>
#include "picojson.h"

struct mpv_handle_deleter
{
  void operator()(mpv_handle* handle)
  {
     mpv_terminate_destroy(handle);
  }
};

struct mpv_status
{
    std::string loaded_filename = "";
    uint64_t file_size = 0;
    int percent_complete = 0; //0-100
    double total_duration = 0;
    double time_position = 0;
    double audio_volume = 0;
    bool idle = true;
    bool paused = false;
    bool seekable = false;
};
class mpv_manager
{
public:
    mpv_manager();
    ~mpv_manager();
    void play(const std::string& path);
    void play_youtube(const std::string& youtube_id);
    void stop();
    void quit();
    void pause();
    void resume();
    void forward();
    void fast_forward();
    void backward();
    void fast_backward();
    void set_volume(double vol);
    void seek_percent(double percent);
    mpv_status get_mpv_status() const;
private:
    std::string create_metadata_object(mpv_node* metadata);
    picojson::value create_node_json_value(const mpv_node& node);
    picojson::array create_node_json_array(const mpv_node& node);
    picojson::object create_node_json_map(const mpv_node& node);
private:    
  std::unique_ptr< mpv_handle, mpv_handle_deleter > handle;
  log4cplus::Logger logger;
};

#endif // MPV_MANAGER_H
