#include "CApplicationManager.h"

CApplicationManager::CApplicationManager( const CImageConfig& conf )
	: m_kernel_width( conf.getKernelWidth() )
	, m_kernel_height( conf.getKernelHeight() )
	, m_savePath( conf.getSaveImagePath() )
{
	m_imageObj = std::make_shared<CImageObject>( cv::imread( conf.getLoadImagePath(), cv::IMREAD_GRAYSCALE ) );
	if ( m_imageObj->isEmpty() )
	{
		BLUR_LOG( "Error: Could not open or find the image." );
	}
}


void CApplicationManager::run()
{
	/// application ½ÃÀÛ.
	BLUR_LOG( "[APP] Application Start" );
	cv::namedWindow( "OpenCV Blurred Image", cv::WINDOW_AUTOSIZE );
	cv::namedWindow( "Custom Blurred Image", cv::WINDOW_AUTOSIZE );
	cv::namedWindow( "Diff Compared Image(diff = RED pixel)", cv::WINDOW_AUTOSIZE );

	if ( m_imageObj->getImage().channels() != 1 )
	{
		BLUR_LOG( "[APP] Error: Image is not 1 channel. Conversion is not possible." );
		return;
	}

	setOpenCVDLL();
	setCustomDLL();

	blurringImage();
	cv::waitKey( 0 );
}

void CApplicationManager::setCustomDLL()
{
	m_hCustomDll = LoadLibrary( TEXT( "BlurCustom" ) );
	if ( m_hCustomDll )
	{
		m_customBlurFunc = ( BlurDLLFunc_t )GetProcAddress( m_hCustomDll, "applyBlur" );
		m_customFreeFunc = ( FreeBlurDLLMemFunc_t )GetProcAddress( m_hCustomDll, "freeBlurResult" );


		if ( m_customBlurFunc )
		{
			BLUR_LOG( "[APP] Custom Blur function loaded successfully." );
		}
		else
		{
			DWORD errorCode = GetLastError();
			std::ostringstream oss;
			oss << "[APP] Failed to load CustomBlur.dll's function Error code: " << errorCode;
			BLUR_LOG( oss.str() );
		}
	}
	else
	{
		DWORD errorCode = GetLastError();
		std::ostringstream oss;
		oss << "[APP] Failed to load custom DLL. Error code: " << errorCode;
		BLUR_LOG( oss.str() );
	}
}

void CApplicationManager::setOpenCVDLL()
{
	m_hOpenCVDll = LoadLibrary( TEXT( "BlurOpenCV" ) ); 
	if ( m_hOpenCVDll )
	{
		m_opencvBlurFunc = ( BlurDLLFunc_t )GetProcAddress( m_hOpenCVDll, "applyBlur" );
		m_opencvFreeFunc = ( FreeBlurDLLMemFunc_t )GetProcAddress( m_hOpenCVDll, "freeBlurResult" );

		if ( m_opencvBlurFunc )
		{
			BLUR_LOG( "[APP] OpenCV Blur function loaded successfully." );
		}
		else
		{
			DWORD errorCode = GetLastError();
			std::ostringstream oss;
			oss << "[APP] Failed to load OpenCVBlur.dll's function . Error code: " << errorCode;
			BLUR_LOG( oss.str() );
		}
	}
	else
	{
		DWORD errorCode = GetLastError();
		std::ostringstream oss;
		oss << "[APP] Failed to load OpenCV DLL. Error code: " << errorCode;
		BLUR_LOG( oss.str() );
	}
}

void CApplicationManager::blurringImage()
{
	if ( m_imageObj->isEmpty() )
	{
		BLUR_LOG( "[APP] Error: Image is empty." );
		return;
	}

	const int32_t width = m_imageObj->getWidth();
	const int32_t height = m_imageObj->getHeight();
	const std::vector<uint8_t>& buffer = m_imageObj->getBuffer();

	if ( m_opencvBlurFunc )
	{
		uint8_t* opencvResult = m_opencvBlurFunc( width, height, m_kernel_width, m_kernel_height, buffer.data() );

		if ( opencvResult == nullptr )
		{
			BLUR_LOG( "[APP] Error: OpenCV Blur function returned null." );
			return;
		}
		cv::Mat tempOpenCVImage( height, width, CV_8UC1, opencvResult );
		m_openCVBlurredImage = tempOpenCVImage.clone();

		m_opencvFreeFunc( opencvResult );
		BLUR_LOG( "[APP] OpenCV Blur DLL function complete." );
	}
	else
	{
		BLUR_LOG( "[APP] Error: OpenCV Blur function not loaded." );
		return;
	}

	if ( m_customBlurFunc )
	{
		uint8_t* customResult = m_customBlurFunc( width, height, m_kernel_width, m_kernel_height, buffer.data() );

		if ( customResult == nullptr )
		{
			BLUR_LOG( "[APP] Error: Custom Blur function returned null." );
			return;
		}
		cv::Mat tempCustomImage( height, width, CV_8UC1, customResult );
		m_customBlurredImage = tempCustomImage.clone();

		m_customFreeFunc( customResult );

		BLUR_LOG( "[APP] Custom Blur DLL function complete." );
	}
	else
	{
		BLUR_LOG( "[APP] Error: Custom Blur function not loaded." );
		return;
	}

	// compare
	cv::Mat compareImage;
	cv::Mat mask;
	cv::absdiff( m_openCVBlurredImage, m_customBlurredImage, compareImage );
	cv::threshold( compareImage, mask, 0, 255, cv::THRESH_BINARY );
	cv::cvtColor( m_openCVBlurredImage, compareImage, cv::COLOR_GRAY2BGR );
	compareImage.setTo( cv::Scalar( 0, 0, 255 ), mask );


	cv::imshow( "OpenCV Blurred Image", m_openCVBlurredImage );
	cv::imshow( "Custom Blurred Image", m_customBlurredImage );
	cv::imshow( "Diff Compared Image(diff = RED pixel)", compareImage ); 

	// save images
	if ( !m_savePath.empty() )
	{
		cv::imwrite( m_savePath + "/OpenCVBlurredImage.png", m_openCVBlurredImage );
		cv::imwrite( m_savePath + "/CustomBlurredImage.png", m_customBlurredImage );
		cv::imwrite( m_savePath + "/DiffImage.png", compareImage );
		BLUR_LOG( "[APP] Blurred images saved." );
	}
}
