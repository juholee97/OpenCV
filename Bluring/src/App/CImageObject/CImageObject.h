#ifndef C_IMAGE_OBJECT_H
#define C_IMAGE_OBJECT_H

#include <opencv2/opencv.hpp>
#include <vector>

class CImageObject
{
public:
	CImageObject() = delete;
	explicit CImageObject( const cv::Mat& mat );
	~CImageObject() = default;
	CImageObject( const CImageObject& other ) = delete;
	CImageObject( CImageObject&& other ) = delete;
	CImageObject& operator=( const CImageObject& other ) = delete;
	CImageObject& operator=( CImageObject&& other ) = delete;

	void showImage();
	
	void setImage( const cv::Mat& data );
	cv::Mat getImage();
	bool isEmpty() const;

private:
	// Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);
	int32_t m_width;
	int32_t m_height;
	int32_t m_image_type;
	std::vector<uint8_t> m_buffer;
};

#endif ///C_IMAGE_OBJECT_H