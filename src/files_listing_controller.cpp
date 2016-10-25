#include "files_listing_controller.h"
#include "utils.h"

#include <boost/filesystem.hpp>
#include <algorithm>

#include "magic_handler.h"
#include "picojson.h"
#include "crow/crow_all.h"

#include <iostream>
#include <log4cplus/loggingmacros.h>

using namespace boost::filesystem;

log4cplus::Logger files_listing_controller::logger(log4cplus::Logger::getInstance("files_listing_controller"));

files_listing_controller::files_listing_controller(const http_config& config):multimedia_folders(config.multimedia_folders)
{
}
files_listing_controller::~files_listing_controller() = default;


crow::response files_listing_controller::get_sets() const
{
    LOG4CPLUS_DEBUG(logger, "Serving sets");
    picojson::value::object json_sets;
    json_sets.insert({"cur_dir",picojson::value()});
    picojson::value::array files_list;
    for(const auto& entry: multimedia_folders)
    {
        std::string name = entry.first;
        try
        {
            picojson::value::object json_set;
            json_set.insert({"name",picojson::value(name)});
            json_set.insert({"link",picojson::value(name)});
            json_set.insert({"size",picojson::value((int64_t)0)});
            json_set.insert({"type",picojson::value("dir")});            
            json_set.insert({"parent",picojson::value(false)});
            files_list.push_back(picojson::value(json_set));
        }
        catch(const filesystem_error& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    json_sets.insert({"files",picojson::value(files_list)});
    
    crow::response rsp;
    rsp.code = 200;
    rsp.set_header("Content-Type","application/json; charset=UTF-8");
    picojson::value sets_value(json_sets);
    rsp.write(sets_value.serialize());
    return rsp;    
}

crow::response files_listing_controller::get(const std::string& set,const std::string& path) const
{
    std::string working_set = set;
    std::string working_path = path;
    LOG4CPLUS_DEBUG(logger, "Serving path "<<path<<" from set "<<set);
    utils::unescape(working_set);
    utils::unescape(working_path);
    boost::filesystem::path working_path_folder(working_path);
    boost::filesystem::path working_folder(working_set);
    working_folder /= working_path_folder;

    LOG4CPLUS_DEBUG(logger, "After unescape serving path "<<working_path<<" from set "<<working_set);
    std::vector<boost::filesystem::path> dir_entries;    
    try
    {
        if(multimedia_folders.find(working_set) != multimedia_folders.end())
        {
            boost::filesystem::path base_folder(multimedia_folders.at(working_set));
            base_folder /= working_path_folder;
            LOG4CPLUS_DEBUG(logger, "List folder of  "<<base_folder);
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

    picojson::value::object json_files;
    json_files.insert({"cur_dir",picojson::value(working_folder.string())});
    picojson::value::array files_list;

    std::string link = working_folder.has_parent_path() ? working_folder.parent_path().string() : "";
    picojson::value::object json_parent_object;
    json_parent_object.insert({"name",picojson::value("..")});
    json_parent_object.insert({"link",picojson::value(link)});
    json_parent_object.insert({"size",picojson::value((int64_t)0)});
    json_parent_object.insert({"type",picojson::value("dir")});            
    json_parent_object.insert({"parent",picojson::value(true)});
    files_list.push_back(picojson::value(json_parent_object));
        
    for(const auto& entry: dir_entries)
    {
        try
        {
            std::string name =entry.filename().string();
            if(name.length() > 0 && name[0]=='.') continue;
            if(is_other(entry)) continue;
            bool is_file = is_regular_file(entry);
            std::string link = (working_folder / name).string();
            LOG4CPLUS_DEBUG(logger, "Creating link "<<link<<" from working path "<<working_path<<" and set "<<set<<" and name "<<name);
            picojson::value::object json_object;
            json_object.insert({"name",picojson::value(name)});
            json_object.insert({"link",picojson::value(link)});
            json_object.insert({"size",picojson::value((int64_t)(is_file?file_size(entry):0))});
            json_object.insert({"type",picojson::value(is_file?"file":"dir")});            
            json_object.insert({"parent",picojson::value(false)});
            files_list.push_back(picojson::value(json_object));
        }
        catch(const filesystem_error& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    json_files.insert({"files",picojson::value(files_list)});
    crow::response rsp;
    rsp.code = 200;
    rsp.set_header("Content-Type","application/json; charset=UTF-8");
    rsp.write(picojson::value(json_files).serialize());
    return rsp;
}
