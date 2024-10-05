#include <opencv2/opencv.hpp>
#include <vector>
#include <cstring> // memcpy
#include <memory>  // std::unique_ptr

extern "C"
{
	__declspec( dllexport ) uint8_t* applyBlur( const int32_t width, 
												const int32_t height, 
												const int32_t kernel_width, 
												const int32_t kernel_height, 
												const uint8_t* buffer )
	{
		std::vector<uint8_t> copied_buffer( buffer, buffer + ( width * height ) );

		cv::Mat inputImage( height, width, CV_8UC1, copied_buffer.data() );
		cv::Mat outputImage;
		/// default border type : BORDER_REFLECT_101
		cv::blur( inputImage, outputImage, cv::Size( kernel_width, kernel_height ) );

		auto result = new uint8_t[ height * width ];
		std::memcpy( result, outputImage.data, outputImage.total() * outputImage.elemSize() );

		return result;
	}

	__declspec( dllexport ) void freeBlurResult( uint8_t* buffer )
	{
		delete[] buffer;
	}
}
