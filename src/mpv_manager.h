#ifndef MPV_MANAGER_H
#define MPV_MANAGER_H

#include <memory>

class mpv_manager_impl;
class mpv_manager
{
public:
    mpv_manager();
    ~mpv_manager();
    void show_window();
    
private:    
    std::unique_ptr<mpv_manager_impl> impl;
};

#endif // MPV_MANAGER_H
