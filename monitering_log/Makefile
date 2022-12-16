CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++23

# Makefile
all : monitoring_log
monitoring_log : monitoring_log.o
	$(CXX) $(CXXFLAGS) -o $@ $^
monitoring_log.o : monitoring_log.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
clean :
	rm -f monitoring_log *.o