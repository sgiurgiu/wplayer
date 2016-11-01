#include "http_config.h"

#include "file_controller.h"
#include "files_listing_controller.h"
#include "crow/crow_all.h"
#include "player_service.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>



int main(int /*argc*/, char** /*argv*/)
{ 


    http_config config;
    config.files_folder = "web";
    config.multimedia_folders["All"]="/mnt/homebackup/Downloads/complete/";
    

    log4cplus::BasicConfigurator logConfig;
    logConfig.configure();
    log4cplus::Logger logger = log4cplus::Logger::getRoot();
    logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);


    file_controller files(config.files_folder);
    files_listing_controller files_listing(config);

    if(false)
    {
        daemon(0,0);
    }

    crow::SimpleApp app;
        

    CROW_ROUTE(app,"/api/files/<string>/<path>")        
    ([&files_listing](const std::string& set,const std::string& path){
        return files_listing.get(set,path);        
    });    
    CROW_ROUTE(app,"/api/files/<string>/")
    ([&files_listing](const std::string& set){
        return files_listing.get(set,"");        
    });      
    CROW_ROUTE(app,"/api/files")        
    ([&files_listing](){
        return files_listing.get_sets();
    });    
    
    CROW_ROUTE(app,"/current")
    .methods("GET"_method)
    ([&files]() {
        return files.get_file_contents("index.html");
    });
    CROW_ROUTE(app,"/movies")
    .methods("GET"_method)
    ([&files]() {
        return files.get_file_contents("index.html");
    });
    CROW_ROUTE(app,"/custom")
    .methods("GET"_method)
    ([&files]() {
        return files.get_file_contents("index.html");
    });

    CROW_ROUTE(app,"/<path>")
    .methods("GET"_method)
    ([&files](const std::string& path) {
        return files.get_file_contents(path);
    });
    CROW_ROUTE(app,"/")
    .methods("GET"_method)
    ([&files]() {
        return files.get_file_contents("index.html");
    });
        
    player_service ps(config);
    std::thread player_service_thread([&logger,&ps]() {
        LOG4CPLUS_DEBUG(logger, "Starting WS Server:");                          
        ps.start();
    });
    app.port(8080).multithreaded().run();
    LOG4CPLUS_DEBUG(logger, "Stopping WS Server");
    ps.stop();
    LOG4CPLUS_DEBUG(logger, "Stopped WS Server");
    player_service_thread.join();
    return 0;
}
