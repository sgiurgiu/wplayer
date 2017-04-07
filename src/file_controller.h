#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <string>
#include <memory>
namespace boost {
    namespace filesystem {
        class path;
    }
}
namespace crow {
    struct response;
}

class magic_handler;
class file_controller
{
public:
    file_controller(const std::string& dir);
    ~file_controller();
    std::unique_ptr<crow::response> get_file_contents(const std::string& file);
    
private:
    std::string get_mime_type(const std::string& file);
    boost::filesystem::path get_file(const std::string& file);

private:
    std::string base_dir = "";
    std::unique_ptr<magic_handler> magic;
};

#endif // FILE_CONTROLLER_H
