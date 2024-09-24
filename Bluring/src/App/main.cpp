#include <iostream>
#include <fstream>
#include "CImageConfig.h"
#include "CLogger.h"
#include "CApplicationManager.h"
#include "json.hpp"

int main()
{
	BLUR_LOG( "[main] Start" );
	BLUR_LOG( "[main] Load Config Data" );
	std::ifstream config_file( MY_CONFIG_DIR );

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