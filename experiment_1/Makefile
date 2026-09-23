CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

all: t1 t2

t2: CXXFLAGS += -fwrapv

t1 t2: %: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	./t1
	./t2

clean:
	rm -f t1 t2

.PHONY: all run clean
