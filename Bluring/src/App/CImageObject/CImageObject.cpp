#include "CImageObject.h"

CImageObject::CImageObject( const cv::Mat& mat )
	: m_width(mat.cols),
	m_height(mat.rows),
	m_image_type(mat.type())
{
	m_buffer.assign( mat.data, mat.data + mat.total() * mat.elemSize() );
}

void CImageObject::showImage()
{
	cv::imshow( "Processed Image", getImage() );
	cv::waitKey( 0 );
}

cv::Mat CImageObject::getImage() 
{
	if ( m_buffer.empty() )
	{
		return cv::Mat();
	}
	// TODO: buffer data 추가화깅ㄴ 필요. cv::Mat 내부 타입 조사
	cv::Mat mat( m_height, m_width, m_image_type, m_buffer.data() );
	return mat.clone(); 
}

void CImageObject::setImage(const cv::Mat& data)
{
	m_width = data.rows;
	m_height = data.cols;
	m_image_type = data.type();
	m_buffer.assign( data.data, data.data + data.total() * data.elemSize() );
}

bool CImageObject::isEmpty() const
{
	return m_buffer.empty();
}
