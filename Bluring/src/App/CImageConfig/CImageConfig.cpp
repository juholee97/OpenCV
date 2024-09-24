#include "CImageConfig.h"

CImageConfig::CImageConfig( const nlohmann::json& configData )
	:m_loadImagePath(configData[ "load_image_path" ].get<std::string>()),
	m_saveImagePath(configData[ "save_image_path" ].get<std::string>()),
	m_kernelSize(configData[ "kernel_size" ].get<int32_t>())
{
}

const std::string& CImageConfig::getLoadImagePath() const
{
	return m_loadImagePath;
}

const std::string& CImageConfig::getSaveImagePath() const
{
	return m_saveImagePath;
}

int32_t CImageConfig::getKernelSize() const
{
	return m_kernelSize;
}
