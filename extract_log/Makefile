CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++23

# Makefile
all : ftrace_extract
ftrace_extract : ftrace_extract.o
	$(CXX) $(CXXFLAGS) -o $@ $^
ftrace_extract.o : ftrace_extract.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
clean :
	rm -f ftrace_extract *.o
