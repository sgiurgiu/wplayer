#ifndef WPLAYER_OPTIONS_H
#define WPLAYER_OPTIONS_H

#include <string>

struct options
{  
  bool quiet=false;
  bool verbose=false;
  bool help = false;
  bool version = false;
  bool daemon = false;
  std::string config_file = "";    
  std::string log_file = "";
};



#endif // WPLAYER_OPTIONS_H
