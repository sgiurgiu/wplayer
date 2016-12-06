#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <log4cplus/logger.h>
#include <map>
#include <set>
#include <string>
#include <stdexcept>
#include <memory>

class database_exception : public std::runtime_error
{
public:
    database_exception(const std::string& what):std::runtime_error(what){}
    database_exception(const char* what): std::runtime_error(what) {}
};
struct sqlitedb_deleter
{
    void operator()(sqlite3* db)
    {
        if(db)
        {
            sqlite3_close(db);
        }        
    }
};
class database
{
public:
    database();
    
    std::map<std::string,std::string> get_multimedia_sets() const;
    std::string get_target_multimedia_folder(const std::string& set) const;
    std::set<std::string> get_environment() const;
    std::map<std::string,std::string> get_player_properties() const;
    void add_multimedia_set(const std::string& name,const std::string& target);
    void delete_multimedia_set(const std::string& name);
private:
    void create_tables();
    static void errorLogCallback(void*, int iErrCode, const char* zMsg);
private:
    std::unique_ptr<sqlite3,sqlitedb_deleter> db;
    static log4cplus::Logger logger;
};

#endif // DATABASE_H
