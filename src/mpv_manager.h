#ifndef MPV_MANAGER_H
#define MPV_MANAGER_H

#include "picojson.h"
#include "http_config.h"
#include <string>
#include <vector>
#include <memory>
#include <mpv/client.h>
#include <log4cplus/logger.h>

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
    picojson::value tracks;
};
struct mpv_playlist_entry
{
    std::string filename = "";
    bool playing = false;
};

class mpv_manager
{
public:
    mpv_manager(const http_config& config);
    ~mpv_manager();
    void play(const std::string& path);
    void play_youtube(const std::string& youtube_url);
    void add_to_playlist(const std::string& path);
    void add_to_playlist_play(const std::string& path);
    void playlist_next();
    void playlist_previous();
    void playlist_clear();
    void playlist_remove(int index = -1);
    void playlist_move(int index1, int index2);
    void playlist_shuffle();
    std::vector<mpv_playlist_entry> get_playlist() const;
    void stop();
    void quit();
    void pause();
    void resume();
    void forward();
    void fast_forward();
    void backward();
    void fast_backward();
    void set_volume(double vol);
    void remove_sub(int64_t id);
    void seek_percent(double percent);
    mpv_status get_mpv_status() const;
private:
    std::string create_metadata_object(mpv_node* metadata) const;
    picojson::value create_node_json_value(const mpv_node& node) const;
    picojson::array create_node_json_array(const mpv_node& node) const;
    picojson::object create_node_json_map(const mpv_node& node) const;
private:    
  std::unique_ptr< mpv_handle, mpv_handle_deleter > handle;
  log4cplus::Logger logger;
};

#endif // MPV_MANAGER_H
