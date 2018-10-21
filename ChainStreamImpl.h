#pragma once

#include "StreamImpl.h"

namespace stream
{

	template <typename T>
	class ChainStreamImpl : public StreamImpl<T> {
	public:
		bool first1 = true;
		Optional<T> opt;
		Stream<T> firstStream;
		Stream<T> secondStream;
		ChainStreamImpl(const Stream<T> &first, const Stream<T> &second)
		{
			firstStream = first;
			secondStream = second;
		}

		Optional<T> next() {
			opt = firstStream.next();
			if (opt)
				return opt;
			else
			{
				opt = secondStream.next();
				if(opt)
					return opt;
				else
					return Optional<T>();
			}

			return opt;
		}
	};

}
