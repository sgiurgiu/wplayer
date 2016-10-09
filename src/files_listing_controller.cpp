#include "files_listing_controller.h"
#include <boost/filesystem.hpp>
#include <algorithm>

#include "magic_handler.h"
#include "rapidjson/prettywriter.h"
#include "crow/crow_all.h"

#include <iostream>

using namespace boost::filesystem;
using namespace rapidjson;

files_listing_controller::files_listing_controller(const http_config& config):multimedia_folders(config.multimedia_folders)
{
}
files_listing_controller::~files_listing_controller() = default;


crow::response files_listing_controller::get_sets() const
{
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
    std::vector<boost::filesystem::path> dir_entries;
    try
    {

        if(multimedia_folders.find(set) != multimedia_folders.end())
        {
            boost::filesystem::path base_folder(multimedia_folders.at(set));
            base_folder /= path;
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

    std::sort(dir_entries.begin(), dir_entries.end());
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    writer.StartArray();
    writer.StartObject();
    writer.String("name");
    writer.String("..");
    std::string link = set;
    if(!path.empty())
    {
       boost::filesystem::path parent_folder  = boost::filesystem::path(path).parent_path();
       link += parent_folder.filename().string();
    }
    writer.String("link");
    writer.String(link.c_str(),link.length());

    writer.String("size",4);
    writer.Uint64(0U);
    writer.String("type",4);
    writer.String("dir",3);
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
            std::string link = set + "/" + path + "/" + name;
            writer.String("link");
            writer.String(link.c_str(),link.length());

            writer.String("size",4);
            writer.Uint64(is_file?file_size(entry):0U);
            writer.String("type",4);
            writer.String(is_file?"file":"dir");
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
