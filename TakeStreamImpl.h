#pragma once
#include <cstdio>
#include "StreamImpl.h"

namespace stream
{

template <typename T>
class TakeStreamImpl: public StreamImpl<T> {
public:
	Stream<T> partStream;
	Optional<T> opt;
	size_t count;
	TakeStreamImpl(const Stream<T> &stream, size_t n)
	{
		partStream = stream;
		count = n;
	}

	Optional<T> next() {
		opt = partStream.next();
		if(opt && count != 0)
		{
			count -=1;
			return opt;
		}
		else
			return Optional<T>();
	/*	for(;n != 0;i++)
		{
			if(opt)
				return opt;
			else
				Optional<T>();
			opt = partStream.next();
		}
		return Optional<T>();*/
	}
};

}
