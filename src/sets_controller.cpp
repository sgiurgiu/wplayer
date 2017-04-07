#include "sets_controller.h"
#include "crow/crow_all.h"
#include "picojson.h"
#include "utils.h"

#include <log4cplus/loggingmacros.h>

log4cplus::Logger sets_controller::logger(log4cplus::Logger::getInstance("sets_controller"));
sets_controller::sets_controller(database* db):db(db)
{
}
sets_controller::~sets_controller() = default;
std::unique_ptr<crow::response> sets_controller::get_sets() const
{
    LOG4CPLUS_DEBUG(logger, "Serving sets");
    picojson::value::array files_list;
    auto multimedia_folders = db->get_multimedia_sets();
    for(const auto& entry: multimedia_folders)
    {
        picojson::value::object json_set;
        json_set.insert({"name",picojson::value(entry.first)});
        json_set.insert({"target",picojson::value(entry.second)});
        files_list.push_back(picojson::value(json_set));
    }
    
    std::unique_ptr<crow::response> rsp = std::make_unique<crow::response>();
    rsp->code = 200;
    rsp->set_header("Content-Type","application/json; charset=UTF-8");
    picojson::value sets_value(files_list);
    rsp->write(sets_value.serialize());
    return rsp;        
}
std::unique_ptr<crow::response> sets_controller::add_set(const crow::request& req)
{
    picojson::value val;
    picojson::parse(val,req.body);
    std::string name = val.get("name").to_str();
    std::string target = val.get("target").to_str();
    std::unique_ptr<crow::response> rsp = std::make_unique<crow::response>();
    db->add_multimedia_set(name,target);
    rsp->code = 201;    
    return rsp;        
}
std::unique_ptr<crow::response> sets_controller::delete_set(const std::string& name)
{
    auto decoded_name = utils::decode(name);
    db->delete_multimedia_set(decoded_name);
    std::unique_ptr<crow::response> rsp = std::make_unique<crow::response>();
    rsp->code = 200;    
    return rsp;        
}
