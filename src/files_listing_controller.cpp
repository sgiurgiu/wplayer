#include "files_listing_controller.h"
#include "utils.h"

#include <boost/filesystem.hpp>
#include <algorithm>

#include "magic_handler.h"
#include "rapidjson/prettywriter.h"
#include "crow/crow_all.h"

#include <iostream>

using namespace boost::filesystem;
using namespace rapidjson;

log4cxx::LoggerPtr files_listing_controller::logger(log4cxx::Logger::getLogger("files_listing_controller"));

files_listing_controller::files_listing_controller(const http_config& config):multimedia_folders(config.multimedia_folders)
{
}
files_listing_controller::~files_listing_controller() = default;


crow::response files_listing_controller::get_sets() const
{
    LOG4CXX_DEBUG(logger, "Serving sets");
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    writer.StartArray();
    for(const auto& entry: multimedia_folders)
    {
        std::string name = entry.first;
        try
        {
            writer.StartObject();
            writer.String("name",4);
            writer.String(name.c_str(),name.length());
            writer.String("link",4);
            writer.String(name.c_str(),name.length());
            writer.String("size",4);
            writer.Uint64(0U);
            writer.String("type",4);
            writer.String("dir");
            writer.String("parent");
            writer.Bool(false);
            writer.EndObject();
        }
        catch(const filesystem_error& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    writer.EndArray();
    crow::response rsp;
    rsp.code = 200;
    rsp.set_header("Content-Type","application/json; charset=UTF-8");
    rsp.write(sb.GetString());
    return rsp;    
}

crow::response files_listing_controller::get(const std::string& set,const std::string& path) const
{
    std::string working_set = set;
    std::string working_path = path;
    LOG4CXX_DEBUG(logger, "Serving path "<<path<<" from set "<<set);
    utils::unescape(working_set);
    utils::unescape(working_path);
    boost::filesystem::path working_path_folder(working_path);
    boost::filesystem::path working_folder(working_set);
    working_folder /= working_path_folder;

    LOG4CXX_DEBUG(logger, "After unescape serving path "<<working_path<<" from set "<<working_set);
    std::vector<boost::filesystem::path> dir_entries;    
    try
    {
        if(multimedia_folders.find(working_set) != multimedia_folders.end())
        {
            boost::filesystem::path base_folder(multimedia_folders.at(working_set));
            base_folder /= working_path_folder;
            LOG4CXX_DEBUG(logger, "List folder of  "<<base_folder);
            if(exists(base_folder) || is_directory(base_folder))
            {
                copy(directory_iterator(base_folder), directory_iterator(),std::back_inserter(dir_entries));
            }
        }
    }
    catch(const filesystem_error& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    std::sort(dir_entries.begin(), dir_entries.end(),[](const boost::filesystem::path& p1,const boost::filesystem::path& p2){
        if(is_directory(p1) && !is_directory(p2)) return true;
        if(!is_directory(p1) && is_directory(p2)) return false;

        return p1 < p2;
    });


    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);



    writer.StartArray();
    writer.StartObject();
    writer.String("name");
    writer.String("..");
    std::string link = working_folder.has_parent_path() ? working_folder.parent_path().string() : "";
    writer.String("link");
    writer.String(link.c_str(),link.length());

    writer.String("size",4);
    writer.Uint64(0U);
    writer.String("type",4);
    writer.String("dir",3);
    writer.String("parent");
    writer.Bool(true);
    writer.EndObject();

    for(const auto& entry: dir_entries)
    {
        try
        {
            std::string name =entry.filename().string();
            if(name.length() > 0 && name[0]=='.') continue;
            if(is_other(entry)) continue;
            bool is_file = is_regular_file(entry);
            writer.StartObject();
            writer.String("name");
            writer.String(name.c_str(),name.length());
            std::string link = (working_folder / name).string();
            LOG4CXX_DEBUG(logger, "Creating link "<<link<<" from working path "<<working_path<<" and set "<<set<<" and name "<<name);
            writer.String("link");
            writer.String(link.c_str(),link.length());

            writer.String("size",4);
            writer.Uint64(is_file?file_size(entry):0U);
            writer.String("type",4);
            writer.String(is_file?"file":"dir");
            writer.String("parent");
            writer.Bool(false);
            writer.EndObject();
        }
        catch(const filesystem_error& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    writer.EndArray();
    crow::response rsp;
    rsp.code = 200;
    rsp.set_header("Content-Type","application/json; charset=UTF-8");
    rsp.write(sb.GetString());
    return rsp;
}
