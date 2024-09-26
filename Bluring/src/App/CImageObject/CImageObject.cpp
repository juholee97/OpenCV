#include "CImageObject.h"

CImageObject::CImageObject( const cv::Mat& mat )
	: m_width( mat.cols )
	, m_height( mat.rows )
	, m_image_type( mat.type() )
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
	cv::Mat mat( m_height, m_width, m_image_type, static_cast< void* >( m_buffer.data() ) );
	return mat.clone();
}

const int32_t& CImageObject::getWidth() const
{
	return m_width;
}

const int32_t& CImageObject::getHeight() const
{
	return m_height;
}

const int32_t& CImageObject::getImageType() const
{
	return m_image_type;
}

const std::vector<uint8_t>& CImageObject::getBuffer() const
{
	return m_buffer;
}

void CImageObject::setImage( const cv::Mat& data )
{
	m_width = data.cols;
	m_height = data.rows;
	m_image_type = data.type();
	m_buffer.assign( data.data, data.data + data.total() * data.elemSize() );
}

bool CImageObject::isEmpty() const
{
	return m_buffer.empty();
}