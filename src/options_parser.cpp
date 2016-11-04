#include "options_parser.h"

options_parser::options_parser()
{
visibleDescription.add_options()
    ("help,h","Get help using wplayer")
    ("quiet,q","be quiet")
    ("verbose,v","be verbose")
    ("version","show wplayer version and exit")    
    ("config,c",boost::program_options::value< std::string >()->default_value(""),"Configuration file")
    ("log,l",boost::program_options::value< std::string >()->default_value(""),"Log file")        
    ("daemon,d","Daemonize program")    
    ;
    optionsGroup.add(visibleDescription);
}
options options_parser::parseArguments(int argc, char ** argv)
{
    options opt;
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc,argv).
                                  options(optionsGroup).positional(positionalOd).run(),vm);
    boost::program_options::notify(vm);

    opt.help=vm.count("help");
    opt.quiet=vm.count("quiet");
    opt.verbose=vm.count("verbose");
    opt.version=vm.count("version");
    opt.daemon=vm.count("daemon");
    if(vm.count("config")) {
        opt.config_file=vm["config"].as<std::string>();
    }
    if(vm.count("log")) {
        opt.log_file=vm["log"].as<std::string>();
    }
    return opt;

}
void options_parser::showHelp(std::ostream& os)
{
  os<<"Usage: wplayer <options>\n";
  os<<"Options:\n";
  visibleDescription.print(os);

}
void options_parser::showVersion(std::ostream& os)
{
    os<<"wplayer version 0.1\n";
}
