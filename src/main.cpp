#include "http_config.h"

#include "file_controller.h"
#include "files_listing_controller.h"
#include "crow/crow_all.h"
#include "player_service.h"
#include "options_parser.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>


#include <boost/filesystem.hpp>

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
    boost::filesystem::path logFile(commandLineOptions.log_file);
    boost::filesystem::file_status logFileStatus = boost::filesystem::status(logFile);
    if(logFileStatus.type() != boost::filesystem::regular_file)
    {        
        log4cplus::BasicConfigurator logConfig;
        logConfig.configure();        
    }
    else
    {
        log4cplus::PropertyConfigurator logConfig(commandLineOptions.log_file);
        logConfig.configure();
    }    
    log4cplus::Logger logger = log4cplus::Logger::getRoot();    

    if(commandLineOptions.daemon)
    {
        daemon(0,0);
    }

    file_controller files(config.http_server.files_folder);
    files_listing_controller files_listing(config);
    player_service ps(config);    

    crow::SimpleApp app;
        
   
    CROW_ROUTE(app,"/player")
    .websocket()
    .onopen([&ps](crow::websocket::connection& conn) {
        ps.add_connection(&conn);
    })
    .onclose([&ps](crow::websocket::connection& conn, const std::string& /*reason*/){
        ps.remove_connection(&conn);
    })
    .onmessage([&ps](crow::websocket::connection& /*conn*/, const std::string& data, bool /*is_binary*/){
        ps.handle_message(data);
    });    
    
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
 
    app.port(config.http_server.port).bindaddr(config.http_server.bind_address).multithreaded().run();
       
    return 0;
}
