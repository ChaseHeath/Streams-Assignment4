#pragma once
#include <functional>
#include "StreamImpl.h"

namespace stream
{

	template <typename T>
	class PrimeStreamImpl : public StreamImpl<T> {
	public:
		Stream<T> filtered;
		Optional<T> opt;
		std::function<bool(T)> func;
		PrimeStreamImpl(const Stream<T> &stream, std::function<bool(T)> f)
		{
			filtered = stream;
			func = f;
		}

		Optional<T> next() {
			opt = filtered.next();

			while (opt)
			{
				if (func(*opt))
					return opt;
				opt = filtered.next();
			}
			return Optional<T>();
		}
	};

}