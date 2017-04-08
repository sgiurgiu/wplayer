#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include "database.h"
#include "http_config.h"
#include <memory>
#include <string>
#include <functional>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_set>
#include <log4cplus/logger.h>

namespace picojson 
{
    class value;
}    
namespace crow
{
    namespace websocket
    {
        class connection;
    }
}
class mpv_manager;
struct mpv_status;
class player_service
{
public:   
    player_service(const http_config& config,database* db);
    ~player_service();
    void start();
    void stop();
    void add_connection(crow::websocket::connection* connection);
    void remove_connection(crow::websocket::connection* connection);
    void handle_message(const std::string& msg,crow::websocket::connection& conn);
private:    
    void play_command(const picojson::value& val);
    void play_youtube_command(const picojson::value& val);
    void add_to_playlist_command(const picojson::value& val);
    void add_to_playlist_play_command(const picojson::value& val);
    void playlist_next_command(const picojson::value&);
    void playlist_previous_command(const picojson::value&);
    void playlist_clear_command(const picojson::value&);
    void playlist_remove_command(const picojson::value& val);
    void playlist_move_command(const picojson::value& val);
    void playlist_shuffle_command(const picojson::value&);
    void get_playlist_command(const picojson::value&,crow::websocket::connection& conn);
    
    void stop_command(const picojson::value&);
    void pause_command(const picojson::value&);
    void resume_command(const picojson::value&);
    void volume_command(const picojson::value& val);    
    void seek_percent_command(const picojson::value& val);
    void forward_command(const picojson::value&);
    void fast_forward_command(const picojson::value&);
    void backward_command(const picojson::value&);
    void remove_sub_command(const picojson::value& val);
    void fast_backward_command(const picojson::value&);
    void setup_ws_server();
    void setup_polling_thread();
    void report_status(crow::websocket::connection* connection,mpv_status& status);
private:
    int64_t get_int64_val(const picojson::value& val,int64_t default_val = 0) const;
    std::string get_file_to_play(const picojson::value& val) const;
private:
    std::atomic_bool done_polling;
    std::unique_ptr<mpv_manager> mpv;
    database* db;
    log4cplus::Logger logger;
    using message_handler = std::function<void(const picojson::value& val)>;
    std::map<std::string,message_handler> handlers_map;    
    std::thread mpv_status_polling_thread;    
    std::mutex mtx;
    std::unordered_set<crow::websocket::connection*> connections;
};

#endif // PLAYER_SERVICE_H
