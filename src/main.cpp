#include "http_config.h"

#include "file_controller.h"
#include "files_listing_controller.h"
#include "crow/crow_all.h"
#include "player_service.h"
#include "options_parser.h"
#include "database.h"
#include "sets_controller.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <boost/filesystem.hpp>


void setup_routes(crow::SimpleApp& app,file_controller& files,files_listing_controller& files_listing,player_service& ps,sets_controller& sets);

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
    
    crow::SimpleApp app;
    database db;
    file_controller files(config.http_server.files_folder);
    files_listing_controller files_listing(&db);
    player_service ps(config,&db);    
    sets_controller sets(&db);
   
    setup_routes(app,files,files_listing,ps,sets);

    if(commandLineOptions.daemon)
    {
        daemon(0,0);
    }
    
    app.port(config.http_server.port).bindaddr(config.http_server.bind_address).multithreaded().run();

    
    return 0;
}

void setup_routes(crow::SimpleApp& app,file_controller& files,files_listing_controller& files_listing,player_service& ps,sets_controller& sets)
{
    CROW_ROUTE(app,"/player")
    .websocket()
    .onopen([&ps](crow::websocket::connection& conn) {
        ps.add_connection(&conn);
    })
    .onclose([&ps](crow::websocket::connection& conn, const std::string& /*reason*/){
        ps.remove_connection(&conn);
    })
    .onmessage([&ps](crow::websocket::connection& conn, const std::string& data, bool /*is_binary*/){
        ps.handle_message(data,conn);
    });    
    
    CROW_ROUTE(app,"/api/files/<path>")
    ([&files_listing](const std::string& path){
        return std::move(*files_listing.get(path));
    });      
    CROW_ROUTE(app,"/api/files")        
    ([&files_listing](){
        return std::move(*files_listing.get_sets());
    });    
    CROW_ROUTE(app,"/api/sets")
    .methods("GET"_method,"POST"_method)
    ([&sets](const crow::request& req) {
        if(req.method == crow::HTTPMethod::Post)
        {
            return std::move(*sets.add_set(req));
        }
        return std::move(*sets.get_sets());
    });
    CROW_ROUTE(app,"/api/sets/<string>")
    .methods("DELETE"_method)
    ([&sets](const std::string& name) {
        return std::move(*sets.delete_set(name));
    });
    
    CROW_ROUTE(app,"/current")
    .methods("GET"_method)
    ([&files]() {
        return std::move(*files.get_file_contents("index.html"));
    });
    CROW_ROUTE(app,"/movies")
    .methods("GET"_method)
    ([&files]() {
        return std::move(*files.get_file_contents("index.html"));
    });
    CROW_ROUTE(app,"/custom")
    .methods("GET"_method)
    ([&files]() {
        return std::move(*files.get_file_contents("index.html"));
    });
    CROW_ROUTE(app,"/settings")
    .methods("GET"_method)
    ([&files]() {
        return std::move(*files.get_file_contents("index.html"));
    });
    CROW_ROUTE(app,"/playlist")
    .methods("GET"_method)
    ([&files]() {
        return std::move(*files.get_file_contents("index.html"));
    });

    CROW_ROUTE(app,"/<path>")
    .methods("GET"_method)
    ([&files](const std::string& path) {
        return std::move(*files.get_file_contents(path));
    });
    CROW_ROUTE(app,"/")
    .methods("GET"_method)
    ([&files]() {
        return std::move(*files.get_file_contents("index.html"));
    });    
}
