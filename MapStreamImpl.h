#pragma once
#include "StreamImpl.h"

namespace stream
{

	template <typename S, typename F>
	class MapStreamImpl : public StreamImpl<S> {
	public:
		Stream<S> filtered;
		Optional<S> opt;
		F func;
		MapStreamImpl(const Stream<S> &stream, F&& f) : filtered(stream), func(std::forward<F>(f)) {}


		Optional<S> next() {

			do
			{
				opt = filtered.next();
				*opt = func(*opt);
				return opt;
			}while (opt);
			return Optional<S>();
		}
	};

}
