#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "serializer.h"

int main() {
	Serializer serializer = Serializer();
	int test_i = 10;
	serializer << test_i;
	std::cout << serializer.str() << std::endl;

	double test_d = 10.4324;
	serializer << test_d;
	std::cout << serializer.str() << std::endl;

	
	DeSerializer deserializer = DeSerializer(serializer.str());
	int test_i_r;
	double test_d_r;
	deserializer >> test_i_r;
	deserializer >> test_d_r;

	std::cout << test_i_r << ", " << test_d_r << std::endl;

	return 0;
}

