#ifndef C_LOGGER_H
#define C_LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <chrono>

class CLogger
{
public:
	CLogger()
	{
		std::filesystem::path logFilePath = getExecutablePath() / "image_blur.log";
		m_logStream.open( logFilePath, std::ios::app );
	}

	~CLogger()
	{
		if ( m_logStream.is_open() )
		{
			try
			{
				m_logStream.close();
			}
			catch ( const std::exception& e )
			{
				std::cerr << "Exception during log file closure: " << e.what() << std::endl;
			}
		}
	}
	CLogger( const CLogger& other ) = delete;
	CLogger( CLogger&& other ) = delete;
	CLogger& operator=( const CLogger& other ) = delete;
	CLogger& operator=( CLogger&& other ) = delete;

	void log( const std::string& message )
	{
		m_logStream << getCurrentTime() << " " << getTickTime() << " " << message << std::endl;
	}

private:
	std::ofstream m_logStream;
	std::chrono::steady_clock::time_point m_startTime { std::chrono::steady_clock::now() };

	std::filesystem::path getExecutablePath() const
	{
		return std::filesystem::current_path();
	}
	std::string getCurrentTime() const
	{
		auto now = std::chrono::system_clock::now();

		auto now_time_t = std::chrono::system_clock::to_time_t( now );
		auto now_ms = std::chrono::duration_cast< std::chrono::milliseconds >( now.time_since_epoch() ) % 1000;

		std::tm now_tm;

		localtime_s( &now_tm, &now_time_t );

		std::ostringstream oss;
		oss << std::put_time( &now_tm, "%Y-%m-%d %H:%M:%S" ) << '.' << std::setfill( '0' ) << std::setw( 3 ) << now_ms.count();
		return oss.str();
	}

	int64_t getTickTime() const
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( now - m_startTime );
		return elapsed.count();
	}
};

// ofstream 로깅이기 때문에 전역변수에 대해 const 불가.
static CLogger g_logger;

#define BLUR_LOG(message) g_logger.log(message)

#endif /// C_LOGGER_H