#ifndef MAGIC_HANDLER_H
#define MAGIC_HANDLER_H

#include <magic.h>
#include <memory>

struct magic_deleter
{
  typedef magic_t pointer;
  void operator()(magic_t magic)
  {
    magic_close(magic);
  }
};

class magic_handler
{
public:
  magic_handler():
    magic(magic_open(MAGIC_ERROR/*|MAGIC_DEBUG*/|MAGIC_MIME), magic_deleter())
  {
    magic_load(magic.get(),NULL);
  }
  magic_handler(magic_handler&&) = default;                 
  magic_handler& operator=(magic_handler&&) = default;      
  magic_handler(const magic_handler&) = delete;             
  magic_handler& operator=(const magic_handler&) = delete;    
  ~magic_handler() = default;    
  std::string get_mime(const std::string& file) const
  {
    return magic_file(magic.get(), file.c_str());
  }

private:
  std::unique_ptr<magic_t,magic_deleter> magic;  
};
#endif // MAGIC_HANDLER_H
