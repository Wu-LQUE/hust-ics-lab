CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2 -fwrapv

all: t1 t2

t1: t1.cpp
	$(CXX) $(CXXFLAGS) -fexec-charset=GBK $< -o $@

t2: t2.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	./t1 | iconv -f GBK -t UTF-8
	./t2

.PHONY: all run
