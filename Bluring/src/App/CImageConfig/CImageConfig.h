#ifndef C_IMAGE_CONFIG_H
#define C_IMAGE_CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
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
	int32_t getKernelSize() const;

private:
	std::string m_loadImagePath;
	std::string m_saveImagePath;
	int32_t m_kernelSize;
};

#endif /// C_IMAGE_CONFIG_H