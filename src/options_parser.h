#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <boost/program_options.hpp>

#include "options.h"

class options_parser
{
public:
    options_parser();
    options parseArguments(int argc,char** argv);
    void showHelp(std::ostream& os);
    void showVersion(std::ostream& os);
private:
    boost::program_options::options_description visibleDescription;
    boost::program_options::options_description optionsGroup;
    boost::program_options::positional_options_description positionalOd;
};

#endif // OPTIONS_PARSER_H
