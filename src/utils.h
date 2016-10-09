#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <boost/algorithm/string.hpp>
class utils
{
private:
    static std::map<std::string,std::string> escape_map;
public:
    static void unescape(std::string& str)
    {
        if(str.empty()) return;

        for(const auto& entry : escape_map)
        {
            boost::algorithm::replace_all(str,entry.first,entry.second);
        }
    }
};

#endif // UTILS_H
