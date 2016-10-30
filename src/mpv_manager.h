#ifndef MPV_MANAGER_H
#define MPV_MANAGER_H

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

class mpv_manager
{
public:
    mpv_manager();
    ~mpv_manager();
    void play(const std::string& path);
    void stop();
    void quit();
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
