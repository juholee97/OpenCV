#include "CApplicationManager.h"
#include <chrono> 

CApplicationManager::CApplicationManager( const CImageConfig& conf )
	: m_kernel_width( conf.getKernelWidth() )
	, m_kernel_height( conf.getKernelHeight() )
	, m_savePath( conf.getSaveImagePath() )
{
	std::vector<std::string> imagePaths;

	for ( const auto& entry : std::filesystem::directory_iterator( conf.getLoadImagePath() ) )
	{
		if ( entry.is_regular_file() )
		{
			std::string extension = entry.path().extension().string();
			std::transform( extension.begin(), extension.end(), extension.begin(), ::tolower );

			if ( extension == ".jpg" || extension == ".png" || extension == ".bmp" )
			{
				imagePaths.push_back( entry.path().string() );
			}
		}
	}
	if ( imagePaths.empty() )
	{
		BLUR_LOG( "Error: No images found in the specified directory." );
		return;
	}

	for ( const auto& imagePath : imagePaths )
	{
		const std::string imageName = std::filesystem::path( imagePath ).filename().string();
		auto imageObj = std::make_shared<CImageObject>( imageName, cv::imread( imagePath, cv::IMREAD_GRAYSCALE ) );
		if ( imageObj->isEmpty() )
		{
			BLUR_LOG( "Error: Could not open or find the image: " + imagePath );
		}
		else
		{
			m_imageObjs.push_back( imageObj );
		}
	}
}

void CApplicationManager::run()
{
	/// application start.
	BLUR_LOG( "[APP] Application Start" );
	cv::namedWindow( "Diff Compared Image(diff = RED pixel)", cv::WINDOW_AUTOSIZE );

	setOpenCVDLL();
	setCustomDLL();

	for ( const auto& imageObj : m_imageObjs )
	{
		if ( imageObj->getImage().channels() != 1 )
		{
			BLUR_LOG( "[APP] Error: Image is not 1 channel. Conversion is not possible." );
			continue;
		}

		// OpenCV blur
		cv::Mat blurred_image1;
		auto start_opencv = std::chrono::high_resolution_clock::now();
		applyImageBlur( blurred_image1, imageObj, { m_opencvBlurFunc, m_opencvFreeFunc } );
		auto end_opencv = std::chrono::high_resolution_clock::now();

		// check processing time
		std::chrono::duration<double, std::milli> opencv_duration = end_opencv - start_opencv;
		BLUR_LOG( "[APP] OpenCV blur processing time: " + std::to_string( opencv_duration.count() ) + " ms" );

		// Custom blur
		cv::Mat blurred_image2;
		auto start_custom = std::chrono::high_resolution_clock::now();
		applyImageBlur( blurred_image2, imageObj, { m_customBlurFunc, m_customFreeFunc } );
		auto end_custom = std::chrono::high_resolution_clock::now();

		// check processing time
		std::chrono::duration<double, std::milli> custom_duration = end_custom - start_custom;
		BLUR_LOG( "[APP] Custom blur processing time: " + std::to_string( custom_duration.count() ) + " ms" );

		// compare
		cv::Mat comp_image;
		compareImages( comp_image, blurred_image1, blurred_image2 );

		// print compare
		cv::imshow( "Diff Compared Image(diff = RED pixel)", comp_image );

		// save images
		if ( !m_savePath.empty() )
		{
			const std::string openCVPath = m_savePath + "/OpenCV_" + imageObj->getImageName();
			const std::string customPath = m_savePath + "/Custom_" + imageObj->getImageName();
			cv::imwrite( openCVPath, blurred_image1 );
			cv::imwrite( customPath, blurred_image2 );
			BLUR_LOG( "[APP] Blurred image " + imageObj->getImageName() + " saved." );
		}

		cv::waitKey( 0 );
	}

	cv::destroyAllWindows();
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
			const DWORD errorCode = GetLastError();
			std::ostringstream oss;
			oss << "[APP] Failed to load CustomBlur.dll's function. Error code: " << errorCode;
			BLUR_LOG( oss.str() );
		}
	}
	else
	{
		const DWORD errorCode = GetLastError();
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
			const DWORD errorCode = GetLastError();
			std::ostringstream oss;
			oss << "[APP] Failed to load OpenCVBlur.dll's function. Error code: " << errorCode;
			BLUR_LOG( oss.str() );
		}
	}
	else
	{
		const DWORD errorCode = GetLastError();
		std::ostringstream oss;
		oss << "[APP] Failed to load OpenCV DLL. Error code: " << errorCode;
		BLUR_LOG( oss.str() );
	}
}

void CApplicationManager::applyImageBlur( cv::Mat& lhs,
										  const std::shared_ptr<const CImageObject>& rhs_Object,
										  const std::pair<BlurDLLFunc_t, FreeBlurDLLMemFunc_t>& blurFunc ) const
{
	if ( rhs_Object->isEmpty() )
	{
		BLUR_LOG( "[APP] Error: Image is empty." );
		return;
	}

	const int32_t width = rhs_Object->getWidth();
	const int32_t height = rhs_Object->getHeight();
	const std::vector<uint8_t>& buffer = rhs_Object->getBuffer();

	if ( blurFunc.first )
	{
		uint8_t* blurred_buffer = blurFunc.first( width, height, m_kernel_width, m_kernel_height, buffer.data() );

		if ( blurred_buffer == nullptr )
		{
			BLUR_LOG( "[APP] Error: Blur function returned null." );
			return;
		}

		lhs = cv::Mat( height, width, CV_8UC1, blurred_buffer ).clone();
		blurFunc.second( blurred_buffer );
	}
	else
	{
		BLUR_LOG( "[APP] Error: Blur function not loaded." );
	}

	return;
}

void CApplicationManager::compareImages( cv::Mat& comp_image, const cv::Mat& image1, const cv::Mat& image2 ) const
{
	// Check image is empty or have different sizes or type
	if ( image1.empty() || image2.empty() )
	{
		BLUR_LOG( "[APP] Error: One or both images are empty." );
		return;
	}
	if ( image1.size() != image2.size() || image1.type() != image2.type() )
	{
		BLUR_LOG( "[APP] Error: Images have different sizes or types." );
		return;
	}

	// Compare images
	cv::Mat compResult;
	cv::Mat mask;
	cv::absdiff( image1, image2, compResult );
	cv::threshold( compResult, mask, 0, 255, cv::THRESH_BINARY );

	const int32_t nonZeroCount = cv::countNonZero( mask );

	// Convert differences red
	cv::cvtColor( image1, compResult, cv::COLOR_GRAY2BGR );
	compResult.setTo( cv::Scalar( 0, 0, 255 ), mask );

	// Resize image size
	const int32_t maxPrintWidth = 800;
	const int32_t maxPrintHeight = 600;
	cv::Mat resizedCompareImage;
	cv::resize( compResult, resizedCompareImage, cv::Size( maxPrintWidth, maxPrintHeight ), 0, 0, cv::INTER_LINEAR );

	// Prepare result text
	const int32_t fontFace = cv::FONT_HERSHEY_SIMPLEX;
	const double fontScale = 1.0;
	const int32_t thickness = 2;
	int32_t baseline = 0;

	const std::string matchText = ( nonZeroCount == 0 ) ? "Match" : "Not Match (" + std::to_string( nonZeroCount ) + " pixels)";
	const cv::Size textSize = cv::getTextSize( matchText, fontFace, fontScale, thickness, &baseline );

	// Add additional space
	const int32_t txtBoxHeight = textSize.height + baseline + 10;
	comp_image = cv::Mat( resizedCompareImage.rows + txtBoxHeight, resizedCompareImage.cols, resizedCompareImage.type() );
	resizedCompareImage.copyTo( comp_image( cv::Rect( 0, 0, resizedCompareImage.cols, resizedCompareImage.rows ) ) );


	// Add background and add text
	cv::rectangle( comp_image,
				   cv::Point( 0, resizedCompareImage.rows ),
				   cv::Point( comp_image.cols, comp_image.rows ),
				   cv::Scalar( 0, 0, 0 ), cv::FILLED );

	const cv::Point textOrg( ( comp_image.cols - textSize.width ) / 2, resizedCompareImage.rows + textSize.height + 5 );
	cv::putText( comp_image, matchText, textOrg, fontFace, fontScale, cv::Scalar( 0, 255, 0 ), thickness );

	return;
}
