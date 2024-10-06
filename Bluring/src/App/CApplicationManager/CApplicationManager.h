#ifndef C_APPLICATION_MANAGER_H
#define C_APPLICATION_MANAGER_H

#include <memory>
#include <string>
#include <cstdint>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "CImageObject.h"
#include "CImageConfig.h"
#include "CLogger.h"

using Width_t = int32_t;
using Height_t = int32_t;
using Kernel_Width_t = int32_t;
using Kernel_Height_t = int32_t;
using Buffer_t = const std::vector<uint8_t>&;
using BlurDLLFunc_t = uint8_t * ( * )( int32_t, int32_t, int32_t, int32_t, const uint8_t* );
using FreeBlurDLLMemFunc_t = void ( * )( uint8_t* );

class CApplicationManager
{
public:
	static std::unique_ptr<CApplicationManager> makeInstance( const CImageConfig& conf )
	{
		return std::unique_ptr<CApplicationManager>( new CApplicationManager( conf ) );
	}

	CApplicationManager( const CApplicationManager& ) = delete;
	CApplicationManager( CApplicationManager&& ) = delete;
	CApplicationManager& operator=( const CApplicationManager& ) = delete;
	CApplicationManager& operator=( CApplicationManager&& ) = delete;

	~CApplicationManager() = default;

	void run();

private:
	explicit CApplicationManager( const CImageConfig& conf );

	void setOpenCVDLL();
	void setCustomDLL();

	void applyImageBlur( cv::Mat& lhs,
						 const std::shared_ptr<const CImageObject>& rhs_Object,
						 const std::pair<BlurDLLFunc_t, FreeBlurDLLMemFunc_t>& blurFunc ) const;

	void compareImages( cv::Mat& comp_image, const cv::Mat& image1, const cv::Mat& image2 ) const;

	const int32_t m_kernel_width;
	const int32_t m_kernel_height;
	const std::string m_savePath;
	const std::string m_windowName { "CApplicationManager" };

	// DLL
	HINSTANCE m_hCustomDll { nullptr };
	HINSTANCE m_hOpenCVDll { nullptr };
	BlurDLLFunc_t m_customBlurFunc { nullptr };
	BlurDLLFunc_t m_opencvBlurFunc { nullptr };
	FreeBlurDLLMemFunc_t m_customFreeFunc { nullptr };
	FreeBlurDLLMemFunc_t m_opencvFreeFunc { nullptr };

	std::vector<std::shared_ptr<CImageObject>> m_imageObjs;
};

#endif /// C_APPLICATION_MANAGER_H
