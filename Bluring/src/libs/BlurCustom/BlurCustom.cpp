#include <optional>
#include <cstdint>
#include <vector>
#include <array>

extern "C" __declspec( dllexport )
void BlurCustom( int32_t width, int32_t height, int32_t kernelSize, std::vector<uint8_t> imageData, [[maybe_unused]]std::optional<int32_t> image_type )
{
	std::vector<uint8_t> srcData ( imageData );
	
	int32_t padding = kernelSize / 2; 
	int32_t kernelArea = kernelSize * kernelSize; 
	// TODO: ±¸Çö
}