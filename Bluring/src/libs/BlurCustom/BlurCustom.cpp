#include <vector>
#include <memory>  // std::unique_ptr
#include <algorithm>
#include <cstring> // memcpy
#include <future>
#include <thread>

extern "C"
{
	__declspec( dllexport ) uint8_t* applyBlur( const int32_t width,
												const int32_t height,
												const int32_t kernel_width,
												const int32_t kernel_height,
												const uint8_t* buffer )
	{
		// default anhor x, y (-1, -1)
		const int32_t anchorX = kernel_width / 2;
		const int32_t anchorY = kernel_height / 2;
		const size_t bufferSize = static_cast< size_t >( height ) * width;
		auto blurred_buffer = new uint8_t[ bufferSize ];

		/// default bolder type ( Reflect101 )
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

		// set thread
		const int32_t num_threads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		int32_t rowsPerThread = height / num_threads;

		auto blurFunction = [ & ] ( int32_t startRow, int32_t endRow ) {
			std::vector<float> kernelColumnSums( width, 0.0f );

			for ( int32_t y = startRow; y < endRow; ++y )
			{
				if ( y == startRow )
				{
					for ( int32_t x = 0; x < width; ++x )
					{
						float sum = 0.0f;
						for ( int32_t ky = -anchorY; ky <= kernel_height - anchorY - 1; ++ky )
						{
							int32_t yIndex = reflect101( y + ky, height );
							sum += buffer[ yIndex * width + x ];
						}
						kernelColumnSums[ x ] = sum;
					}
				}
				else
				{
					int32_t yOld = reflect101( y - anchorY - 1, height );
					int32_t yNew = reflect101( y + kernel_height - anchorY - 1, height );
					for ( int32_t x = 0; x < width; ++x )
					{
						kernelColumnSums[ x ] += buffer[ yNew * width + x ] - buffer[ yOld * width + x ];
					}
				}

				float sum = 0.0f;
				for ( int32_t kx = -anchorX; kx <= kernel_width - anchorX - 1; ++kx )
				{
					int32_t xIndex = reflect101( kx, width );
					sum += kernelColumnSums[ xIndex ];
				}

				for ( int32_t x = 0; x < width; ++x )
				{
					if ( x > 0 )
					{
						int32_t xOld = reflect101( x - anchorX - 1, width );
						int32_t xNew = reflect101( x + kernel_width - anchorX - 1, width );
						sum += kernelColumnSums[ xNew ] - kernelColumnSums[ xOld ];
					}

					float avg = sum / ( kernel_width * kernel_height );
					int32_t pixelValue = static_cast< int32_t >( avg + 0.5f );
					pixelValue = std::min( std::max( pixelValue, 0 ), 255 );
					blurred_buffer[ y * width + x ] = static_cast< uint8_t >( pixelValue );
				}
			}
		};

		for ( int32_t t = 0; t < num_threads; ++t )
		{
			int32_t startRow = t * rowsPerThread;
			int32_t endRow = ( t == num_threads - 1 ) ? height : startRow + rowsPerThread;
			threads.emplace_back( blurFunction, startRow, endRow );
		}

		for ( auto& thread : threads )
		{
			thread.join();
		}

		return blurred_buffer;
	}

	__declspec( dllexport ) void freeBlurResult( uint8_t* buffer )
	{
		delete[] buffer;
	}
}