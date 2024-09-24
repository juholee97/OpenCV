#include <opencv2/opencv.hpp>
#include <optional>
#include <cstdint>
#include <vector>
#include <array>

extern "C" __declspec( dllexport )
void BlurOpenCV( int32_t width, int32_t height, int32_t kernelSize, std::vector<uint8_t> imageData, std::optional<int32_t> input_type )
{
	int32_t image_type = input_type.value_or( CV_8UC3 );
	// Load the input image
	cv::Mat src { height, width, image_type, imageData.data() };
	cv::Mat dst;
	cv::blur( src, dst, cv::Size( kernelSize, kernelSize ) );
	// TODO: 결과 어떻게 반환시킬지 구현.
}