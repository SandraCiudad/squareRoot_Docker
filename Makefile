all:
	cd build && valgrind ./executeTests --gtest_output=xml

	g++ sqrt_test.cpp -o output -fprofile-arcs -ftest-coverage  -I/usr/include/ -L/usr/lib/ -lgtest -lgtest_main  -pthread --coverage -g -O0
	gcovr

