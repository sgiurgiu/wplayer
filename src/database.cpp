#include "database.h"
#include <log4cplus/loggingmacros.h>
#include <boost/filesystem.hpp>

log4cplus::Logger database::logger(log4cplus::Logger::getInstance("database"));
struct stm_deleter
{
    void operator()(sqlite3_stmt* stmt)
    {
            if(stmt)
            {
                sqlite3_finalize(stmt);
            }
    }
};


database::database() : db(nullptr,sqlitedb_deleter())
{
    sqlite3_config(SQLITE_CONFIG_LOG,database::errorLogCallback,NULL);
    boost::filesystem::path db_file(getenv("HOME"));
    db_file /= "/.config/wplayer/";
    if(!boost::filesystem::exists(db_file))
    {
        boost::filesystem::create_directories(db_file);
    }
    db_file /= "wplayer.db";   
    sqlite3* db_p;
    if(sqlite3_open(db_file.string().c_str(),&db_p) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db_p));
    }
    db.reset(db_p);
    create_tables();
}
std::string database::get_target_multimedia_folder(const std::string& set) const
{
    sqlite3_stmt* stmt;
    std::string sql("SELECT TARGET FROM MULTIMEDIA_SETS WHERE NAME=?1");
    if(sqlite3_prepare_v2(db.get(),sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt,stm_deleter> stmt_p(stmt,stm_deleter());
    if(sqlite3_bind_text(stmt_p.get(),1,set.c_str(),set.length(),SQLITE_TRANSIENT) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    if(sqlite3_step(stmt_p.get()) == SQLITE_ROW)
    {
        return std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt_p.get(),0)));
    }
    else
    {
        return "";
    }
}

std::map<std::string, std::string> database::get_multimedia_sets() const
{
    sqlite3_stmt* stmt;
    std::string sql("SELECT NAME,TARGET FROM MULTIMEDIA_SETS ORDER BY NAME");
    if(sqlite3_prepare_v2(db.get(),sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt,stm_deleter> stmt_p(stmt,stm_deleter());
    std::map<std::string, std::string> multimedia_sets;
    while(sqlite3_step(stmt_p.get()) == SQLITE_ROW)
    {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt_p.get(),0));
        std::string target = reinterpret_cast<const char*>(sqlite3_column_text(stmt_p.get(),1));
        multimedia_sets.insert({name,target});
    }
        
    return multimedia_sets;
}
std::set<std::string> database::get_environment() const
{
    sqlite3_stmt* stmt;
    std::string sql("SELECT ENV_VAR FROM PLAYER_ENVIRONMENT ORDER BY ENV_VAR");
    if(sqlite3_prepare_v2(db.get(),sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt,stm_deleter> stmt_p(stmt,stm_deleter());
    std::set<std::string> environment;
    while(sqlite3_step(stmt_p.get()) == SQLITE_ROW)
    {
        std::string env_var = reinterpret_cast<const char*>(sqlite3_column_text(stmt_p.get(),0));        
        environment.insert(env_var);
    }
        
    return environment;    
}
std::map<std::string, std::string> database::get_player_properties() const
{
    sqlite3_stmt* stmt;
    std::string sql("SELECT NAME,VALUE FROM PLAYER_OPTIONS ORDER BY NAME");
    if(sqlite3_prepare_v2(db.get(),sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt,stm_deleter> stmt_p(stmt,stm_deleter());
    std::map<std::string, std::string> player_properties;
    while(sqlite3_step(stmt_p.get()) == SQLITE_ROW)
    {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt_p.get(),0));
        std::string value = reinterpret_cast<const char*>(sqlite3_column_text(stmt_p.get(),1));
        player_properties.insert({name,value});
    }
        
    return player_properties;
}
void database::add_multimedia_set(const std::string& name, const std::string& target)
{
    sqlite3_stmt* stmt;
    std::string sql("INSERT INTO MULTIMEDIA_SETS (NAME,TARGET) VALUES (?1,?2)");
    if(sqlite3_prepare_v2(db.get(),sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt,stm_deleter> stmt_p(stmt,stm_deleter());
    if(sqlite3_bind_text(stmt_p.get(),1,name.c_str(),name.length(),SQLITE_TRANSIENT) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    if(sqlite3_bind_text(stmt_p.get(),2,target.c_str(),target.length(),SQLITE_TRANSIENT) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    
    if(sqlite3_step(stmt_p.get()) != SQLITE_DONE)
    {
            throw database_exception(sqlite3_errmsg(db.get()));
    }
}
void database::delete_multimedia_set(const std::string& name)
{
    sqlite3_stmt* stmt;
    std::string sql("DELETE FROM MULTIMEDIA_SETS WHERE NAME=?1");
    if(sqlite3_prepare_v2(db.get(),sql.c_str(),sql.length(),&stmt,NULL) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt,stm_deleter> stmt_p(stmt,stm_deleter());
    if(sqlite3_bind_text(stmt_p.get(),1,name.c_str(),name.length(),SQLITE_TRANSIENT) != SQLITE_OK)
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }
    
    if(sqlite3_step(stmt_p.get()) != SQLITE_DONE)
    {
            throw database_exception(sqlite3_errmsg(db.get()));
    }
}


void database::create_tables()
{    
    if( sqlite3_exec(db.get(),"CREATE TABLE IF NOT EXISTS MULTIMEDIA_SETS(NAME TEXT PRIMARY KEY,TARGET TEXT)",nullptr,nullptr,nullptr) != SQLITE_OK ||
        sqlite3_exec(db.get(),"CREATE TABLE IF NOT EXISTS PLAYER_OPTIONS(NAME TEXT PRIMARY KEY,VALUE TEXT)",nullptr,nullptr,nullptr) != SQLITE_OK   ||
        sqlite3_exec(db.get(),"CREATE TABLE IF NOT EXISTS PLAYER_ENVIRONMENT(ENV_VAR TEXT PRIMARY KEY)",nullptr,nullptr,nullptr) != SQLITE_OK
    )
    {
        throw database_exception(sqlite3_errmsg(db.get()));
    }    
}

void database::errorLogCallback(void* /*pArg*/, int iErrCode, const char* zMsg)
{
    LOG4CPLUS_ERROR(database::logger,"DB Err: "<<iErrCode<<", msg:"<<zMsg);
}
