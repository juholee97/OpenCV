#include <opencv2/opencv.hpp>
#include <vector>
#include <cstring> // memcpy
#include <memory>  // std::unique_ptr

extern "C"
{
	__declspec( dllexport ) uint8_t* applyBlur( int32_t width, int32_t height, int32_t kernel_width, int32_t kernel_height, const uint8_t* buffer )
	{
		std::vector<uint8_t> copied_buffer( buffer, buffer + ( width * height ) );

		cv::Mat inputImage( height, width, CV_8UC1, copied_buffer.data() );
		cv::Mat outputImage;
		/// default border type : BORDER_REFLECT_101
		cv::blur( inputImage, outputImage, cv::Size( kernel_width, kernel_height ) );
		std::vector<uint8_t> outputBuffer( outputImage.begin<uint8_t>(), outputImage.end<uint8_t>() );

		size_t bufferSize = outputBuffer.size();
		uint8_t* result = new uint8_t[ bufferSize ];
		std::memcpy( result, outputBuffer.data(), bufferSize );

		return result;
	}

	__declspec( dllexport ) void freeBlurResult( uint8_t* buffer )
	{
		delete[] buffer;
	}
}
