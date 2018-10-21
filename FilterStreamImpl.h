#pragma once
#include <functional>
#include "StreamImpl.h"

namespace stream
{

template <typename T>
class FilterStreamImpl: public StreamImpl<T> {
public:
	Stream<T> filtered;
	Optional<T> opt;
	std::function<bool(T)> func;
	FilterStreamImpl(const Stream<T> &stream, std::function<bool(T)> f)
	{
		filtered = stream;
		func = f;
	}

	Optional<T> next() {
		opt = filtered.next();

		while(opt)
		{
			if(func(*opt))
				return opt;
			opt = filtered.next();
		}
		return Optional<T>();
	}
};

}
