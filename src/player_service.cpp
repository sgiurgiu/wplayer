#include "player_service.h"
#include "mpv_manager.h"
#include "picojson.h"

#include <log4cplus/loggingmacros.h>
#include <boost/filesystem.hpp>
#include <chrono>
#include "crow/crow_all.h"

player_service::player_service(const http_config &config):done_polling(false),mpv(new mpv_manager(config)),multimedia_folders(config.multimedia_folders),
                                                    logger(log4cplus::Logger::getInstance("player_service"))
{
    using namespace std::placeholders;   
    handlers_map["play"]=std::bind(&player_service::play_command,this,_1);
    handlers_map["stop"]=std::bind(&player_service::stop_command,this,_1);
    handlers_map["pause"]=std::bind(&player_service::pause_command,this,_1);      
    handlers_map["resume"]=std::bind(&player_service::resume_command,this,_1);
    handlers_map["volume"]=std::bind(&player_service::volume_command,this,_1);      
    handlers_map["forward"]=std::bind(&player_service::forward_command,this,_1);
    handlers_map["backward"]=std::bind(&player_service::backward_command,this,_1);
    handlers_map["fast-forward"]=std::bind(&player_service::fast_forward_command,this,_1);
    handlers_map["fast-backward"]=std::bind(&player_service::fast_backward_command,this,_1);
    handlers_map["seek-percent"]=std::bind(&player_service::seek_percent_command,this,_1);
    handlers_map["youtube"]=std::bind(&player_service::play_youtube_command,this,_1);  
    
    setup_polling_thread();
}
player_service::~player_service()
{
    mpv->quit();
    done_polling = true;
    mpv_status_polling_thread.join();
}

void player_service::setup_polling_thread()
{
    mpv_status_polling_thread = std::thread([this](){
        while(!done_polling)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2s);
            std::lock_guard<std::mutex> lock(mtx);
            for(auto con: connections)
            {
                report_status(con);
            }
        }
    });
}

void player_service::add_connection(crow::websocket::connection* connection)
{
    std::lock_guard<std::mutex> lock(mtx);
    connections.insert(connection);
    report_status(connection);
}
void player_service::remove_connection(crow::websocket::connection* connection)
{
    std::lock_guard<std::mutex> lock(mtx);
    connections.erase(connection);
}

void player_service::report_status(crow::websocket::connection* connection)
{
    mpv_status status = mpv->get_mpv_status();
    picojson::object status_obj;
    status_obj.insert({"file_size",picojson::value((int64_t)status.file_size)});
    status_obj.insert({"idle",picojson::value(status.idle)});
    status_obj.insert({"file_name",picojson::value(status.loaded_filename)});
    status_obj.insert({"percent_complete",picojson::value((int64_t)status.percent_complete)});
    status_obj.insert({"time_position",picojson::value(status.time_position)});
    status_obj.insert({"total_duration",picojson::value(status.total_duration)});
    status_obj.insert({"volume",picojson::value(status.audio_volume)});
    status_obj.insert({"paused",picojson::value(status.paused)});
    status_obj.insert({"seekable",picojson::value(status.seekable)});
    
    connection->send_text(picojson::value(status_obj).serialize());    
}

void player_service::handle_message(const std::string& msg)
{
    LOG4CPLUS_DEBUG(logger, "Server: Message received: \"" << msg);  
    picojson::value msg_val;
    std::string err = picojson::parse(msg_val, msg);
    if(!err.empty())
    {
        LOG4CPLUS_ERROR(logger, "Message not json "<<msg<<". Error:"<<err);                  
        return;
    }
    auto name_obj = msg_val.get("name");
    if(name_obj.is<picojson::null>())
    {
        LOG4CPLUS_ERROR(logger, msg<<" has no name");                  
        return;        
    }
    const std::string& command_name = name_obj.get<std::string>();
    auto it = handlers_map.find(command_name);
    if(it == handlers_map.end())
    {
        LOG4CPLUS_ERROR(logger, "Unknown command "<<command_name);                       
    }
    else
    {
        //execute it
        it->second(msg_val);        
    }
}
void player_service::volume_command(const picojson::value& val)
{
    auto vol_obj = val.get("value");
    double vol = 0;
    if(vol_obj.is<double>())
    {
        vol = vol_obj.get<double>();
    }
    else
    {
        vol = std::stod(vol_obj.get<std::string>());
    }
    mpv->set_volume(vol);
}

void player_service::play_command(const picojson::value& val)
{
    auto link_obj = val.get("link");
    if(link_obj.is<picojson::null>())
    {
        LOG4CPLUS_ERROR(logger, val<<" has no link");                  
        return;        
    }
    const std::string& link = link_obj.get<std::string>();    
    boost::filesystem::path link_path(link);
    auto path_it = link_path.begin();
    boost::filesystem::path set_path = *path_it;
    ++path_it;
    boost::filesystem::path path_to_file;
    for(;path_it != link_path.end();++path_it)
    {
        path_to_file /= *path_it;
    }
    
    if(multimedia_folders.find(set_path.string()) != multimedia_folders.end())
    {        
        boost::filesystem::path file_to_play(multimedia_folders.at(set_path.string()));
        file_to_play /= path_to_file;
        LOG4CPLUS_DEBUG(logger, "Playing "<<file_to_play);
        if(exists(file_to_play))
        {
            mpv->play(file_to_play.string());
        }        
    }
}

void player_service::stop_command(const picojson::value&)
{
    LOG4CPLUS_DEBUG(logger, " Calling STOP");         
    mpv->stop();
}
void player_service::pause_command(const picojson::value&)
{
    mpv->pause();
}
void player_service::resume_command(const picojson::value&)
{
    mpv->resume();
}
void player_service::forward_command(const picojson::value&)
{
    mpv->forward();
}
void player_service::backward_command(const picojson::value&)
{
    mpv->backward();
}
void player_service::fast_backward_command(const picojson::value&)
{
    mpv->fast_backward();
}
void player_service::fast_forward_command(const picojson::value&)
{
    mpv->fast_forward();
}
void player_service::seek_percent_command(const picojson::value& val)
{
    auto percent_obj = val.get("value");
    double percent = -1;
    if(percent_obj.is<double>())
    {
        percent = percent_obj.get<double>();
    }
    else
    {
        percent = std::stod(percent_obj.get<std::string>());
    }    
    mpv->seek_percent(percent);
}
void player_service::play_youtube_command(const picojson::value& val)
{
    auto youtube_id_obj = val.get("value");
    mpv->play_youtube(youtube_id_obj.get<std::string>());
}
