#include "log.h"

bool Logger::Initialize(const std::string& sfileName, const std::string& sLogDir, int max_file_bytes, int max_files,spdlog::level::level_enum lvl)
{
	std::string existLogdir = sLogDir;
	if(existLogdir.empty()) 
	{
		existLogdir=GLOBAL_DEFAULT_LOG_DIR;
	}
	else if(existLogdir[existLogdir.length()-1]!='/')
	{
		existLogdir+="/";
	}
	
	create_logdir(existLogdir);
	//if initialized
	if (m_logger) {
		return true;
	}
	std::vector<spdlog::sink_ptr> sinks;
	// rotating sink
	std::string slogfile = (existLogdir + sfileName + ".log");
	auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(slogfile, max_file_bytes, max_files);
	sinks.push_back(rotating_sink);

	spdlog::init_thread_pool(8192, 1);
	m_logger = std::make_shared<spdlog::async_logger>(sfileName, sinks.begin(), sinks.end(), spdlog::thread_pool(),
			spdlog::async_overflow_policy::block);
	//spdlog's default format is "%+", it means [%Y-%m-%d %T.%e] [%n] [%l] %v
	m_logger->set_pattern("%+"); // will take effects on all sinks
	m_lvl=lvl;
	m_logger->set_level(lvl);
	m_logger->flush_on(spdlog::level::err);
	//register into global static map
	spdlog::register_logger(m_logger);
	return true;
}
spdlog::level::level_enum Logger::get_level()
{
	return m_lvl;
}
//////////////////
Logger* Logger::Instance()
{
	static Logger s_log_instance;
	return (&s_log_instance);
}
void Logger::Log(spdlog::level::level_enum lvl,const std::string &s)
{
	m_logger->log(lvl,s);
}
/**
 * you can flush the log in the cache to disk,
 * when an error occurs or an exception signal is caught!
 */
void Logger::flush()
{
	m_logger->flush();
}

int Logger::create_logdir(const std::string& logdir)
{
	if (access(logdir.c_str(), 0) == -1) {
		int flag = mkdir(logdir.c_str(), 0755);
		return flag;
	}
	return 0;
}



