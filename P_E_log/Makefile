CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++23

# Makefile
all : extract_remote_log
privilege_escalation : privilege_escalation.o
	$(CXX) $(CXXFLAGS) -o $@ $^
privilege_escalation.o : privilege_escalation.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
clean :
	rm -f privilege_escalation *.o