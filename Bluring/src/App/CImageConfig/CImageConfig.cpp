#include "CImageConfig.h"


CImageConfig::CImageConfig( const nlohmann::json& configData )
	:m_loadImagePath( configData[ "load_image_path" ].get<std::string>() ),
	m_saveImagePath( configData[ "save_image_path" ].get<std::string>() ),
	m_kernel_width( configData[ "kernel_width" ].get<int32_t>() ),
	m_kernel_height( configData[ "kernel_height" ].get<int32_t>() )
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

const int32_t& CImageConfig::getKernelWidth() const
{
	return m_kernel_width;
}

const int32_t& CImageConfig::getKernelHeight() const
{
	return m_kernel_height;
}
