CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -O3

# usage example : make neural_network
# we suppose that the files exists : test/neural_network_test.cpp src/neural_network.cpp src/neural_network.hpp

%: test/%_test.cpp src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) -o bin/$@ $^
