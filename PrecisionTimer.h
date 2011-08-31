#ifndef _PRECISIONTIMER_H_
#define _PRECISIONTIMER_H_

#include <windows.h>

class PrecisionTimer
{
	private:
		LARGE_INTEGER lFreq, lStart;

	public:
		PrecisionTimer( void )
		{
			QueryPerformanceFrequency( &lFreq );
		}

		inline void Start( void )
		{
			QueryPerformanceCounter( &lStart );
		}

		inline double Stop( void )
		{
			// Return duration in seconds...
			LARGE_INTEGER lEnd;
			QueryPerformanceCounter( &lEnd );

			return ( double( lEnd.QuadPart - lStart.QuadPart ) / lFreq.QuadPart );
		}
};

#endif // _PRECISIONTIMER_H_
