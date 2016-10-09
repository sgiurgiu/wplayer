#include "http_config.h"

#include "file_controller.h"
#include "files_listing_controller.h"

#include "crow/crow_all.h"

int main(int argc, char **argv) 
{ 
    if(false)
    {
        daemon(0,0);
    }
    http_config config;
    config.files_folder = "web";
    config.multimedia_folders["All"]="/mnt/homebackup/Downloads/complete/";
    
    file_controller files(config.files_folder);
    files_listing_controller files_listing(config);
    
    crow::SimpleApp app;
        

    CROW_ROUTE(app,"/api/files/<string>/<path>")        
    ([&files_listing](const std::string& set,const std::string& path){
        return files_listing.get(set,path);        
    });    
    CROW_ROUTE(app,"/api/files/<string>")        
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

    app.port(8080).multithreaded().run();

    return 0;
}
