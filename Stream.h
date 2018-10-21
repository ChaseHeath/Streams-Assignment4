#pragma once
#include <cstdio>
#include "DelayStreamImpl.h"
#include "EmptyStreamImpl.h"
#include "StreamImpl.h"
#include "OnceStreamImpl.h"
#include "ChainStreamImpl.h"
#include "TakeStreamImpl.h"
#include "FilterStreamImpl.h"
#include "MapStreamImpl.h"
#include <memory>

namespace stream
{

template <typename T>
class Stream {
private:
	Stream(const std::shared_ptr<StreamImpl<T>>& i) : impl(i) {}
	std::shared_ptr<StreamImpl<T>> impl;

	// Make those functions friend of Stream<T>.
	// They are't static members of Stream<T> since we want to avoid the users having to explicitly specify T
	template <typename U> friend Stream<U> empty();												//Making functions a friend of Stream
	template <typename F> friend decltype(std::declval<F>()()) delay(F&&);
	template <typename W> friend Stream<W> once(W);
    template <typename R> friend Stream<R> chain(const Stream<R> &, const Stream<R> &);
    template <typename Z> friend Stream<Z> take(const Stream<Z> &, size_t);
	template <typename Q, typename F> friend Stream<Q> filter(const Stream<Q> &, F&&);
	template <typename S, typename F> friend Stream<decltype(std::declval<F>()(std::declval<S>()))> map(const Stream<S> &, F&&);
	template <typename size_t> friend Stream<size_t> sieve(Stream<size_t>);
	template <typename q> friend Stream<q> mergeUnique(Stream<q>, Stream<q>);
public:
	using value_type = T;
	Stream() : impl(0) {}

	Optional<T> next() {
		return impl->next();
	}
};

template <typename T>
Stream<T> empty() {
	return Stream<T>(std::make_shared<EmptyStreamImpl<T>>());
}


template <typename F>
decltype(std::declval<F>()()) delay(F&& func) {
	using V = decltype(std::declval<F>()());
	using T = typename V::value_type;
	return V(std::make_shared<DelayStreamImpl<T, F>>(std::forward<F>(func)));
}

template <typename T>
Stream<T> once(T val) {
	// Fill in the implementation here												//creates a singleton stream
	Optional<T> opt = val;
	return Stream<T>(std::make_shared<OnceStreamImpl<T>>(val));
}

template <typename T>
Stream<T> chain(const Stream<T>& first, const Stream<T>& second) {					//create a chain of Streams
	// Fill in the implementation here
	return Stream<T>(std::make_shared<ChainStreamImpl<T>>(first, second));

}
template <typename T>
Stream<T> take(const Stream<T>& stream, size_t n) {									//take a size n of a stream and return it
	// Fill in the implementation here
	return Stream<T>(std::make_shared<TakeStreamImpl<T>>(stream,n));
}
template <typename T, typename F>
Stream<T> filter(const Stream<T>& stream, F&& f) {									//filter out unwanted items out of the stream
	// Fill in the implementation here
	return Stream<T>(std::make_shared<FilterStreamImpl<T>>(stream,std::forward<F>(f)));
}
template <typename S, typename F>
Stream<decltype(std::declval<F>()(std::declval<S>()))> map(const Stream<S>& stream, F&& f) {	//map each value of the stream to a new value
	// Fill in the implementation here
	return Stream<S>(std::make_shared<MapStreamImpl<S,F>>(stream, std::forward<F>(f)));
}

Stream<int> counter(int a) {
return chain(
once(a),
delay([a] () { return counter(a + 1); } )												//count upwards from a starting number
);
}
Stream<int> sieve(Stream<int> s)
{
	Stream<int> filtered;																//filter out unwanted numbers that aren't primes
	Optional<int> optInt = s.next();
	auto ff = [optInt](int num) { return !(num % *optInt == 0); };
	if (*optInt == 2)
	{
		s = filter(s, ff);
		return chain(once(*optInt), delay([s]() { return sieve(s); }));
	}
	else
	{
		s = filter(s, ff);
		return chain(once(*optInt), delay([s]() {return sieve(s); }));
	}
}
// The prime() function returns a stream of prime numbers, in increasing order
// A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself
// The first 20 prime numbers are { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71 }
Stream<int> prime() {
	// Fill in the implementation here
	return sieve(counter(2));
}
bool first0 = true, first1 = true;
Optional<int> op0;
Optional<int> op1;
Stream<int> mergeUnique(Stream<int> s0, Stream<int> s1)					//creating a merge sort out of streams
{
	Stream<int> j;
	if (first0)
		op0 = s0.next();
	if (first1)
		op1 = s1.next();
	if (!op0 && !op1)
		return j;
	if (!op1)
	{
		first0 = true;
		first1 = false;
		return chain(once(*op0), delay([s0, s1]() { return mergeUnique(s0, s1); }));
	}
	if (!op0)
	{
		first0 = false;
		first1 = true;
		return chain(once(*op1), delay([s0, s1]() { return mergeUnique(s0, s1); }));
	}
	if (*op0 == *op1)
	{
		first0 = true;
		first1 = true;
		return chain(once(*op1), delay([s0, s1]() { return mergeUnique(s0, s1); }));
	}
	if (*op0 > *op1)
	{
		first0 = false;
		first1 = true;
		return chain(once(*op1), delay([s0, s1]() { return mergeUnique(s0, s1); }));
	}
	if (*op0 < *op1)
	{
		first0 = true;
		first1 = false;
		return chain(once(*op0), delay([s0, s1]() { return mergeUnique(s0, s1); }));
	}
}
// The hamming() function returns a stream of hamming numbers, in increasing order
// A hamming sequence enumerate, in ascending order with no repetitions, all positive integers with no prime factors other than 2, 3, or 5
// The first 20 hamming numbers are { 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36 }
Stream<int> str0 = once(1);
Stream<int> str1 = once(1);
Stream<int> str2 = once(1);
Stream<int> hamming() {
	// Fill in the implementation here
	auto f = [](int num) { return num * 2; };
	auto ff = [](int num) { return num * 3; };
	auto fff = [](int num) { return num * 5; };
	return mergeUnique(once(1), delay([f, ff, fff]() {return chain(map(str0, f), delay([ff, fff]() {return chain(map(str1, ff), delay([fff]() {return chain(map(str2, fff), delay([]() {return hamming(); })); })); })); }));
}

// You need to implement all the funtions listed below. Read the handout for more information
// Here is a checklist: 
// - once() --------------------- done
// - chain() -------------------- done
// - take() --------------------- done
// - filter() ------------------- done
// - map() ---------------------- done
// - prime() -------------------- done
// - hamming() ------------------ done?
// - pi()


/*












// (THIS IS A HELPER FUNCTION FOR pi())
// The partialSum function takes a streams s of doubles (potentially infinite) and returns a new stream representing the partial sum of the input stream
// e.g. if s = { 1, 2, 3, 4, 5 }, the returning stream should be { 1, 3, 6, 10, 15 }
Stream<double> partialSum() {
	// Fill in the implementation here
}

// The pi() function returns a stream of increasingly accurate approximations of pi.
// Use the expansion series for pi/4 for the approximation: 
// pi/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...
Stream<double> pi() {
	// Fill in the implementation here
}
*/
}
