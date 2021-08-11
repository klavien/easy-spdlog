
#pragma once
#include <sstream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <unistd.h> //for access

#define LOGGER (*Logger::Instance())

#define GLOBAL_DEFAULT_LOG_DIR "/tmp/log/"

#define  trace_LOG   LogHelp()(spdlog::level::level_enum::trace)
#define  debug_LOG   LogHelp()(spdlog::level::level_enum::debug)
#define  info_LOG    LogHelp()(spdlog::level::level_enum::info)
#define  warn_LOG    LogHelp()(spdlog::level::level_enum::warn)
#define  error_LOG   LogHelp()(spdlog::level::level_enum::err)
#define  fatal_LOG   LogHelp()(spdlog::level::level_enum::critical)

/**   depend on spdlog v1.1.0 or later
 * usage:
 *  Logger::Instance()->Initialize("ModuleName");
 *  Logger::Instance()->info("info message");
 *  Logger::Instance()->error("error: {}", "this is an error message!");
 *  ...
 */
class Logger {
public:
    bool Initialize(const std::string& sfileName, const std::string& sLogDir, int max_file_bytes, int max_files,spdlog::level::level_enum lvl=spdlog::level::level_enum::debug);
	spdlog::level::level_enum get_level();
    //////////////////
    static Logger* Instance();
	void Log(spdlog::level::level_enum lvl,const std::string &s);
    /**
     * you can flush the log in the cache to disk,
     * when an error occurs or an exception signal is caught!
     */
    void flush();
private:
    Logger()=default;
    // if success create, return 0
    static int create_logdir(const std::string& logdir);
private:
	spdlog::level::level_enum m_lvl;
    std::shared_ptr<spdlog::logger> m_logger;
};

struct LogHelp{
    std::ostream& operator()(spdlog::level::level_enum lvl)
    {
		m_lvl=lvl;
		if(lvl<Logger::Instance()->get_level()) return ossNoUse;
        return oss;
    }
    ~LogHelp()
    {
        if(!(oss.str().empty()))
        {
			Logger::Instance()->Log(m_lvl,oss.str());
        }
    }
	spdlog::level::level_enum m_lvl;
    std::ostringstream ossNoUse;
    std::ostringstream oss;
};
