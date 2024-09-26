#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include "CImageConfig.h"
#include "CLogger.h"
#include "CApplicationManager.h"
#include "json.hpp"

std::filesystem::path getExecutableDirectory()
{
	wchar_t buffer[ MAX_PATH ];
	GetModuleFileNameW( NULL, buffer, MAX_PATH );
	std::filesystem::path exePath( buffer );
	return exePath.parent_path();
}

int main()
{
	BLUR_LOG( "[main] Start" );
	BLUR_LOG( "[main] Load Config Data" );
	//	MY_CONFIG_DIR is set in CMakeList.txt
	//	std::ifstream config_file( MY_CONFIG_DIR );

	// config.json의 경로 설정
	std::filesystem::path exeDir = getExecutableDirectory();
	std::filesystem::path configPath = exeDir / "config.json";

	BLUR_LOG( "[main] Load Config Data from " + configPath.string() );

	std::ifstream config_file( configPath );

	if ( !config_file.is_open() )
	{
		BLUR_LOG( "Error: Could not open config file." );
		return -1;
	}

	BLUR_LOG( "[main] Parse Config Data" );
	const auto configData = nlohmann::json::parse( config_file );
	const CImageConfig conf( configData );
	BLUR_LOG( "[main] Config data ready." );


	const auto applicationManager = CApplicationManager::makeInstance( conf );
	applicationManager->run();


	BLUR_LOG( "[main] End" );
	return 0;
}