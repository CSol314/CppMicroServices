
#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"




int main(int argc, char * argv[]) {
	
	
	std::cout << "Hello, World!\n";
	::testing::InitGoogleTest(&argc, argv);
	
	
    return RUN_ALL_TESTS();
}
