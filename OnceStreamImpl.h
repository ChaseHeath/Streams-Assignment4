#pragma once

#include "StreamImpl.h"

namespace stream
{

template <typename T>
class OnceStreamImpl: public StreamImpl<T> 
{
public:
	Optional<T> opt;
	bool first_time = true;
	OnceStreamImpl(T val)
	{
		opt = val;
	}

	Optional<T> next() 
	{
		if (first_time)
		{
			first_time = false;
			return Optional<T>(opt);
		}
		else
			return Optional<T>();
	}
};

}
