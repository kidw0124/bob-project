CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++23

# Makefile
all : ldpreload.so test
test : test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
ldpreload.so : ld_preload_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -shared -fPIC -ldl
clean :
	rm -f test *.so *.o