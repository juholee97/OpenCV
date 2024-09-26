#ifndef C_IMAGE_CONFIG_H
#define C_IMAGE_CONFIG_H

#include <string>
#include <cstdint>
#include "json.hpp"

class CImageConfig 
{
public:
	CImageConfig() = delete;
	explicit CImageConfig( const nlohmann::json& configData );
	~CImageConfig() = default;
	CImageConfig( const CImageConfig& other ) = delete;
	CImageConfig( CImageConfig&& other ) noexcept = delete;
	CImageConfig& operator=( const CImageConfig& other ) = delete;
	CImageConfig& operator=( CImageConfig&& other ) noexcept = delete;
	
	const std::string& getLoadImagePath() const;
	const std::string& getSaveImagePath() const;

	const int32_t& getKernelWidth() const;
	const int32_t& getKernelHeight() const;

private:
	const std::string m_loadImagePath;
	const std::string m_saveImagePath;
	const int32_t m_kernel_width;
	const int32_t m_kernel_height;
};


#endif /// C_IMAGE_CONFIG_H