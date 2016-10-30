#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include "http_config.h"
#include "ws/server_ws.hpp"
#include "messages_sync_queue.h"

#include <mutex>
#include <memory>
#include <string>
#include <functional>
#include <map>
#include <log4cplus/logger.h>
namespace picojson 
{
    class value;
}    
class mpv_manager;

class player_service
{
public:
    using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;    
    
    player_service(const http_config& config);
    ~player_service();
    void start();
    void stop();
private:
    void handle_message(const std::string& msg);
    void play_command(const picojson::value& val);
    void stop_command(const picojson::value&);
private:
    MessageSyncQueue msg_queue;
    std::mutex mu;
    std::unique_ptr<mpv_manager> mpv;
    WsServer wsServer;
    folders multimedia_folders;
    log4cplus::Logger logger;
    using message_handler = std::function<void(const picojson::value& val)>;
    std::map<std::string,message_handler> handlers_map;    
    
};

#endif // PLAYER_SERVICE_H
