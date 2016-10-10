#include "movie_controller.h"
#include "utils.h"
#include <boost/filesystem.hpp>
#include "crow/crow_all.h"

log4cxx::LoggerPtr movie_controller::logger(log4cxx::Logger::getLogger("movie_controller"));

movie_controller::movie_controller(const http_config &config):multimedia_folders(config.multimedia_folders)
{

}
crow::response movie_controller::play(const std::string &set, const std::string& path)
{
    std::string working_set = set;
    std::string working_path = path;
    LOG4CXX_DEBUG(logger, "Playing path "<<path<<" from set "<<set);
    utils::unescape(working_set);
    utils::unescape(working_path);

    crow::response rsp;

    if(multimedia_folders.find(working_set) != multimedia_folders.end())
    {
        boost::filesystem::path working_path_folder(working_path);
        boost::filesystem::path file_to_play(multimedia_folders.at(working_set));
        file_to_play /= working_path_folder;
        LOG4CXX_DEBUG(logger, "Playing "<<file_to_play);
        if(exists(file_to_play))
        {
            mpv.play(file_to_play.string());
        }
        rsp.code = 200;
    }
    else
    {
        rsp.code = 404;
    }

    return rsp;
}
