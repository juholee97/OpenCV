#include <vector>
#include <memory>  // std::unique_ptr
#include <algorithm>
#include <cstring> // memcpy

extern "C"
{
	__declspec( dllexport ) uint8_t* applyBlur( int32_t width, int32_t height, int32_t kernel_width, int32_t kernel_height, const uint8_t* buffer )
	{
		// default anhor x, y 
		int32_t anchorX = kernel_width / 2 - ( kernel_width % 2 == 0 ? 1 : 0 );
		int32_t anchorY = kernel_height / 2 - ( kernel_height % 2 == 0 ? 1 : 0 );

		// 2d matrix
		std::vector<std::vector<uint8_t>> inputImage( height, std::vector<uint8_t>( width ) );
		for ( int32_t i = 0; i < height; ++i )
		{
			std::copy( buffer + i * width, buffer + ( i + 1 ) * width, inputImage[ i ].begin() );
		}

		std::vector<std::vector<uint8_t>> outputImage( height, std::vector<uint8_t>( width, 0 ) );

		/// default bolder type
		auto reflect101 = [] ( int p, int len )
		{
			if ( len == 1 )
			{
				return 0;
			}
			if ( p < 0 )
			{
				return -p;
			}
			if ( p >= len )
			{
				return 2 * len - p - 2;
			}
			return p;
		};

		for ( int32_t i = 0; i < height; i++ )
		{
			for ( int32_t j = 0; j < width; j++ )
			{
				double sum = 0.0;
				for ( int32_t m = 0; m < kernel_height; m++ )
				{
					for ( int32_t n = 0; n < kernel_width; n++ )
					{
						int32_t y = i + m - anchorY;
						int32_t x = j + n - anchorX;

						y = reflect101( y, height );
						x = reflect101( x, width );

						sum += static_cast< double >( inputImage[ y ][ x ] );
					}
				}

				double avg = sum / ( kernel_width * kernel_height );
				auto pixelValue = static_cast< int32_t >( avg + 0.5 );
				pixelValue = std::min( std::max( pixelValue, 0 ), 255 );

				outputImage[ i ][ j ] = static_cast< uint8_t >( pixelValue );
			}
		}

		// Convert output image to buffer
		size_t bufferSize = width * height;
		uint8_t* result = new uint8_t[ bufferSize ];
		for ( size_t i = 0; i < height; ++i )
		{
			std::memcpy( result + i * width, outputImage[ i ].data(), width );
		}


		return result;
	}

	__declspec( dllexport ) void freeBlurResult( uint8_t* buffer )
	{
		delete[] buffer;
	}
}
