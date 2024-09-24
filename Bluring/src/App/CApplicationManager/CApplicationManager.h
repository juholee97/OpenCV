#ifndef C_APPLICATION_MANAGER_H
#define C_APPLICATION_MANAGER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include "CImageObject.h"
#include "CImageConfig.h"

class CApplicationManager
{
public:
	using BlurFunction = std::function<cv::Mat( const cv::Mat&, int32_t )>;
	
	static std::unique_ptr<CApplicationManager> makeInstance( const CImageConfig& conf )
	{	
		return std::unique_ptr<CApplicationManager>( new CApplicationManager( conf ) );
	}

	CApplicationManager( const CApplicationManager& ) = delete;
	CApplicationManager( CApplicationManager&& ) = delete;
	CApplicationManager& operator=( const CApplicationManager& ) = delete;
	CApplicationManager& operator=( CApplicationManager&& ) = delete;

	void run();

private:
	explicit CApplicationManager( const CImageConfig& conf )
		: m_kernelSize( conf.getKernelSize() )
		, m_savePath( conf.getSaveImagePath() )
	{
		m_imageObj = std::make_shared<CImageObject>( cv::imread( conf.getLoadImagePath(), cv::IMREAD_GRAYSCALE ) );
		if ( m_imageObj->isEmpty() )
		{
			std::cerr << "Error: Could not open or find the image. ( " << MY_CONFIG_DIR << " )" << std::endl;
		}
	}

	void setOpenCVDLL();
	void setCustomDLL();
	void onTrackbarChange();

	const int32_t m_maxKernel { 31 };
	const std::string m_windowName { "CApplicationManager" };
	// DLL °ü·Ã ¸â¹ö
	HINSTANCE m_hCustomDll { nullptr };
	HINSTANCE m_hOpenCVDll { nullptr };

	std::shared_ptr<CImageObject> m_imageObj;
	int32_t m_kernelSize;
	const std::string m_savePath;

	cv::Mat m_openCVBlurredImage;
	cv::Mat m_customBlurredImage;
	BlurFunction m_customBlurFunc;
	BlurFunction m_opencvBlurFunc;
};
#endif /// C_APPLICATION_MANAGER_H