CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++23

# Makefile
all : extract_remote_log
extract_remote_log : extract_remote_log.o
	$(CXX) $(CXXFLAGS) -o $@ $^
extract_remote_log.o : extract.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
clean :
	rm -f extract_remote_log *.o
