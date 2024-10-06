#ifndef C_IMAGE_OBJECT_H
#define C_IMAGE_OBJECT_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>

class CImageObject
{
public:
	CImageObject() = delete;
	explicit CImageObject( const std::string&, const cv::Mat& mat );
	~CImageObject() = default;
	CImageObject( const CImageObject& other ) = delete;
	CImageObject( CImageObject&& other ) = delete;
	CImageObject& operator=( const CImageObject& other ) = delete;
	CImageObject& operator=( CImageObject&& other ) = delete;

	void showImage();

	void setImage( const cv::Mat& data );

	const std::string& getImageName() const;
	cv::Mat getImage();
	const int32_t& getWidth() const;
	const int32_t& getHeight() const;
	const int32_t& getImageType() const;
	const std::vector<uint8_t>& getBuffer() const;

	bool isEmpty() const;


private:
	// Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);
	const std::string m_imageName;
	int32_t m_width;
	int32_t m_height;
	int32_t m_imageType;
	std::vector<uint8_t> m_buffer;
};

#endif ///C_IMAGE_OBJECT_H