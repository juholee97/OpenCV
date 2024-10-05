#include <vector>
#include <memory>  // std::unique_ptr
#include <algorithm>
#include <cstring> // memcpy
#include <future>

extern "C"
{
	__declspec( dllexport ) uint8_t* applyBlur( const int32_t width, 
												const int32_t height, 
												const int32_t kernel_width, 
												const int32_t kernel_height, 
												const uint8_t* buffer )
	{
		// default anhor x, y 
		const int32_t anchorX = kernel_width / 2;
		const int32_t anchorY = kernel_height / 2;

		// 2d matrix
		std::vector<uint8_t> outputImage( height * width );
		const size_t bufferSize = height * width;
		auto result = new uint8_t[ bufferSize ];

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

		const int num_threads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads( num_threads );

		for ( int t = 0; t < num_threads; ++t ) 
		{
			threads[ t ] = std::thread( [ =, &buffer, &result ] () {
				for ( int32_t i = t * height / num_threads; i < ( t + 1 ) * height / num_threads; i++ )
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

								sum += static_cast< double >( buffer[ y * width + x ] );
							}
						}

						double avg = sum / static_cast< double >( kernel_width * kernel_height );
						auto pixelValue = static_cast< int32_t >( avg + 0.5 );
						pixelValue = std::min( std::max( pixelValue, 0 ), 255 );

						result[ i * width + j ] = static_cast< uint8_t >( pixelValue );
					}
				} } );
		}

		for ( auto& thread : threads ) {
			thread.join();
		}

		return result;
	}

	__declspec( dllexport ) void freeBlurResult( uint8_t* buffer )
	{
		delete[] buffer;
	}
}
