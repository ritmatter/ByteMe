CXX = g++-4.8
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -Wshadow -Werror -pedantic

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g3 -DDEBUG
else
	CXXFLAGS += -DNDEBUG
endif

HEADERFILES = sstring.hpp

%.o: %.cpp $(HEADERFILES)
	@echo 'Compiling $<'
	@$(CXX) $(CXXFLAGS) -c $< -o $@

main: main.o sstring.o
	@echo 'Linking $<'
	@$(CXX) -o main $^ $(LDFLAGS)

all: main

.PHONY: clean
clean:
	@echo 'Removing object files and executable'
	@rm -rf *.o main
