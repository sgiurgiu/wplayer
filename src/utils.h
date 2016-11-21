#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

class utils
{
private:
    static unsigned char hexToChar(const std::string &str) {
        short c = 0;
         
        if(!str.empty()) {
            std::istringstream in(str);
             
            in >> std::hex >> c;
             
            if(in.fail()) {
                throw std::runtime_error("stream decode failure");
            }
        }
         
        return static_cast<unsigned char>(c);
    }    
public:
    static std::string decode(const std::string& str)
    {
        std::ostringstream out;
         
        for(std::string::size_type i=0; i < str.length(); ++i) {
            if(str.at(i) == '%') {
                std::string s(str.substr(i+1, 2));
                out << hexToChar(s);
                i += 2;
            } else {
                out << str.at(i);
            }
        }
         
        return out.str();
    }
};

#endif // UTILS_H
