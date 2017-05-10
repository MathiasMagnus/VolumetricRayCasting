#include <iostream>
#include "Test.h"
int main()
{
	auto f1 = []() {std::cout << "a";};
	auto f2 = []() {std::cout << "b";};

	Test<decltype(f1), decltype(f2)> t(f1, f2, 4);

	t.print();

	return 0;

}