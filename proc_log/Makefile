CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++23

# Makefile
all : extract_remote_log
pr_stat : proc_stat.o
	$(CXX) $(CXXFLAGS) -o $@ $^
proc_stat.o : proc_stat.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
clean :
	rm -f pr_stat *.o
