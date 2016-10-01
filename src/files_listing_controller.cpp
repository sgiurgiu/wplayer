#include "files_listing_controller.h"
#include <boost/filesystem.hpp>
#include <algorithm>

#include "magic_handler.h"
#include "rapidjson/prettywriter.h"
#include "crow/crow_all.h"

#include <iostream>

using namespace boost::filesystem;
using namespace rapidjson;

files_listing_controller::files_listing_controller(const http_config& config):folders(config.multimedia_folders)
{
}
files_listing_controller::~files_listing_controller() = default;

crow::response files_listing_controller::get(const std::string& path)
{
    std::string browse_folder = "/";
    if(!path.empty())
    {
        browse_folder.append(path);
    }
    std::vector<boost::filesystem::path> dir_entries;
    for(const auto& folder:folders)
    {
        try
        {
            boost::filesystem::path base_folder(folder);
            base_folder /= browse_folder;
            if(!exists(base_folder) || !is_directory(base_folder)) continue;
            copy(directory_iterator(base_folder), directory_iterator(),std::back_inserter(dir_entries));
        }
        catch(const filesystem_error& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    std::sort(dir_entries.begin(), dir_entries.end());
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    writer.StartArray();
    if(browse_folder != "/")
    {
        writer.StartObject();
        writer.String("name",4);
        writer.String("..");
        writer.String("size",4);
        writer.Uint64(0U);
        writer.String("type",4);
        writer.String("dir",3);
        writer.EndObject();
    }

    for(const auto& entry: dir_entries)
    {
        try
        {
            std::string name = entry.filename().string();
            if(name.length() > 0 && name[0]=='.') continue;
            if(is_other(entry)) continue;
            bool is_file = is_regular_file(entry);
            writer.StartObject();
            writer.String("name",4);
            writer.String(name.c_str(),name.length());
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
