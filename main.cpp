#include "Stream.h"
#include <assert.h>
#include <iostream>

using namespace stream;

int main() {
	// Put your "unit test" codes here
	auto s = stream::empty<int>(); // Creates an empty stream of integers
	for (int i = 0; i < 100; i++)
		assert(!s.next()); // Calling next() always yields an empty value
	// Example:
	auto e = stream::once(2);
	auto opInt = e.next();
	// The first element pulled from s0 holds 2
	assert(opInt && *opInt == 2);
	// Subsequent next() invocation always yield empty value
	for (int i = 0; i < 100; i++)
		assert(!e.next());	// auto testStream = take(hamming(), 20);

	//	std::cout << *elem << std::endl;
	auto c = chain(once(2), once(3));
	auto first = c.next();
	assert(first && *first == 2);
	auto second = c.next();
	assert(second && *second == 3);
	// All elements from both streams are exhausted
	assert(!c.next());
	auto s0 = counter(1);   // s0 = { 1, 2, 3, ... }
	auto s1 = take(s0, 2);  // s1 = { 1, 2 }
	auto s12 = s1.next();
	auto s13 = s1.next();
	auto s14 = s1.next();
	assert(s12 && *s12 == 1);
	assert(s13 && *s13 == 2);
	assert(!s14);

	auto t0 = counter(1);     // s0 = { 1, 2, 3, ... }
// filter out all odd numbers
//std::function<bool(int)> 
auto f = [] (int num) { return num % 2 == 0; };
auto t1 = take(filter(t0, f),4); 
int i = 2;
int n = 0;
while (n < 4)
{
	auto opInt = t1.next();
	assert(opInt && *opInt == i);
	printf(":");
	i += 2;
	n++;
}
auto t5 = counter(1);
auto t6 = take(filter(t5, f), 4);
auto ff = [] (int num) { return num * 3; };
auto t4 = map(t6, ff);
n = 0;
i = 2;
while (n < 4)
{
	auto op = t4.next();
	assert(op && *op == (i * 3));
	i += 2;
	n++;
}
auto p = take(prime(), 20);
while (auto elem = p.next())
std::cout << *elem << '\n';

auto r1 = map(take(counter(1), 5),ff);
auto r2 = take(counter(4), 5);
auto r3 = mergeUnique(r1, r2);
int r = 0;
for (;r<8;r++)
	printf("%d", r3.next());
printf("\n");
auto h = take(hamming(), 20);
auto elem1 = h.next();
while (4 < 5)
{
	std::cout << *elem1 << "\n";
	elem1 = h.next();
}
}


