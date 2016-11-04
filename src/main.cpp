#include "http_config.h"

#include "file_controller.h"
#include "files_listing_controller.h"
#include "crow/crow_all.h"
#include "player_service.h"
#include "options_parser.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>



int main(int argc, char** argv)
{ 
    options_parser parser;
    if(argc==1) {
        parser.showHelp(std::cout);
        return 1;
    }

    auto commandLineOptions = parser.parseArguments(argc,argv);

    if(commandLineOptions.help) {
        parser.showHelp(std::cout);
        return 1;
    }

    if(commandLineOptions.version) {
        parser.showVersion(std::cout);
        return 1;
    }        


    http_config config(commandLineOptions.config_file);
    
    log4cplus::BasicConfigurator logConfig;
    logConfig.configure();
    log4cplus::Logger logger = log4cplus::Logger::getRoot();
    logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);

    if(opt.daemon)
    {
        daemon(0,0);
    }

    file_controller files(config.http_server.files_folder);
    files_listing_controller files_listing(config);


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
    app.port(config.http_server.port).bindaddr(config.http_server.bind_address).multithreaded().run();
    LOG4CPLUS_DEBUG(logger, "Stopping WS Server");
    ps.stop();
    LOG4CPLUS_DEBUG(logger, "Stopped WS Server");
    player_service_thread.join();
    return 0;
}
