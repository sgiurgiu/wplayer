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

files_listing_controller::files_listing_controller(database* db):db(db)
{
}
files_listing_controller::~files_listing_controller() = default;


crow::response files_listing_controller::get_sets() const
{
    LOG4CPLUS_DEBUG(logger, "Serving sets");
    picojson::value::object json_sets;
    json_sets.insert({"cur_dir",picojson::value()});
    picojson::value::array files_list;
    auto multimedia_folders = db->get_multimedia_sets();
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

crow::response files_listing_controller::get(const std::string& path) const
{    
    LOG4CPLUS_DEBUG(logger, "Serving path "<<path);    
    std::string decoded_path = utils::decode(path);        
    boost::filesystem::path working_folder(decoded_path);
    LOG4CPLUS_DEBUG(logger, "After unescape serving path "<<decoded_path);
    working_folder.remove_trailing_separator();
    LOG4CPLUS_DEBUG(logger, "working_folder.parent_path().string() "<<working_folder.parent_path());
    std::vector<boost::filesystem::path> dir_entries;    
    try
    {
        boost::filesystem::path base_folder;
        for(const auto& path : working_folder)
        {            
            if(base_folder.empty())
            {
                auto folder = db->get_target_multimedia_folder(path.string());
                if(!folder.empty())
                {
                    base_folder /= folder;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                base_folder /= path;
            }
            LOG4CPLUS_DEBUG(logger, "path "<<path.string()<<" and base folder:"<<base_folder);    
        }
        if(boost::filesystem::exists(base_folder) && boost::filesystem::is_directory(base_folder))
        {
            base_folder = canonical(base_folder);
            LOG4CPLUS_DEBUG(logger, "List folder of  "<<base_folder);
            if(exists(base_folder) || is_directory(base_folder))
            {
                copy(directory_iterator(base_folder), directory_iterator(),std::back_inserter(dir_entries));
            }
        }
        else
        {
            crow::response rsp;
            rsp.code = 404;
            rsp.set_header("Content-Type","text/plain");
            rsp.write("Not found");
            return rsp;            
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
    json_parent_object.insert({"type",picojson::value("updir")});            
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
            LOG4CPLUS_DEBUG(logger, "Creating link "<<link<<" from working path "<<working_folder<<" and name "<<name);
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
