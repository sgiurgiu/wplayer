#include "mpv_manager.h"
#include "mpv_manager_impl.h"

#include <iostream>



mpv_manager::mpv_manager()
    : impl(std::make_unique<mpv_manager_impl>())
{

}
mpv_manager::~mpv_manager() = default;

void mpv_manager::play(const std::string& path)
{
  impl->play(path);
}
