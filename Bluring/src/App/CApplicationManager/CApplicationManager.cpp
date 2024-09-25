#include "CApplicationManager.h"
#include "CLogger.h"

void CApplicationManager::run()
{
	/// application ����.
	BLUR_LOG( "[APP] Application Start" );
	cv::namedWindow( "OpenCV Blurred Image", cv::WINDOW_AUTOSIZE );
	cv::namedWindow( "Custom Blurred Image", cv::WINDOW_AUTOSIZE );
	cv::namedWindow( "Difference Image", cv::WINDOW_AUTOSIZE );
	cv::createTrackbar( "Kernel Size", "OpenCV Blurred Image", &m_kernelSize, m_maxKernel );

	if ( m_imageObj->getImage().channels() != 1 )
	{
		BLUR_LOG( "[APP] Error: Image is not 1 channel. Conversion is not possible." );
		return;
	}

	/// �ʱ� ���� ����
	onTrackbarChange();
	
	/// TODO: ��� Ȯ�ο� �ڵ��̸�, delete �ʿ�.
	cv::imshow( "Difference Image", m_imageObj->getImage() );  // ���� �̹����� ǥ��

	/// TODO: �Լ��� �и�.
	while ( true )
	{
		int key = cv::waitKey( 1 );
		if ( key == 27 )	/// esc = break
		{  
			BLUR_LOG( "[APP] Program exiting." );
			break;
		}

		if ( key == 's' && !m_savePath.empty() ) 
		{  
			cv::imwrite( m_savePath + "_opencv.jpg", m_openCVBlurredImage );
			cv::imwrite( m_savePath + "_custom.jpg", m_customBlurredImage );
			BLUR_LOG( "[APP] Blurred images saved." );
		}
	}
}

void CApplicationManager::setCustomDLL()
{
	m_hCustomDll = LoadLibrary( TEXT( "CustomBlur.dll" ) ); // ����� ���� DLL �ε�
	if ( m_hCustomDll )
	{
		// TODO: m_customBlurFunc �Լ� ������ ��ũ

		if ( m_customBlurFunc ) 
		{
			BLUR_LOG( "[APP] Custom Blur function loaded successfully." );
		}
		else 
		{
			BLUR_LOG( "[APP] Failed to load custom Blur function." );
		}
	}
	else
	{
		BLUR_LOG( "[APP] Failed to load custom DLL." );
	}
}

void CApplicationManager::setOpenCVDLL()
{
	m_hOpenCVDll = LoadLibrary( TEXT( "OpenCVBlur.dll" ) ); // OpenCV ��� DLL �ε�
	if ( m_hOpenCVDll )
	{
		// TODO: m_opencvBlurFunc �Լ� ������ ��ũ

		if ( m_opencvBlurFunc ) 
		{
			BLUR_LOG( "[APP] OpenCV Blur function loaded successfully." );
		}
		else 
		{
			BLUR_LOG( "[APP] Failed to load OpenCV Blur function." );
		}
	}
	else
	{
		BLUR_LOG( "[APP] Failed to load OpenCV DLL." );
	}
}

void CApplicationManager::onTrackbarChange()
{
	int32_t kernelSize = ( m_kernelSize % 2 == 1 ) ? m_kernelSize : m_kernelSize + 1;

	if ( kernelSize > m_maxKernel ) 
	{
		kernelSize = m_maxKernel;
	}

	if ( m_imageObj->isEmpty() ) 
	{
		BLUR_LOG( "[APP] Error: Image is empty." );
		return;
	}

	if ( m_opencvBlurFunc ) 
	{
		m_openCVBlurredImage = m_opencvBlurFunc( m_imageObj->getImage(), kernelSize );
		BLUR_LOG( "[APP] OpenCV Blur applied." );
	}
	else 
	{
		BLUR_LOG( "[APP] Error: OpenCV Blur function not loaded." );
		return;
	}

	if ( m_customBlurFunc ) 
	{
		m_customBlurredImage = m_customBlurFunc( m_imageObj->getImage(), kernelSize );
		BLUR_LOG( "[APP] Custom Blur applied." );
	}
	else 
	{
		BLUR_LOG( "[APP] Error: Custom Blur function not loaded." );
		return;
	}

	/// TODO: Blurred �̹��� ���� ����Լ� ���� ��, compare �̹��� ���� �ʿ�

	cv::imshow( "OpenCV Blurred Image", m_openCVBlurredImage );
	cv::imshow( "Custom Blurred Image", m_customBlurredImage );
//	cv::imshow( "Difference Image", compare );  // ���� �̹����� ǥ��
}
