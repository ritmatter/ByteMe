CXX = g++-4.8
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -Wshadow -Werror -pedantic

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g3 -DDEBUG
else
	CXXFLAGS += -DNDEBUG
endif

HEADERFILES = sstring.hpp \
			  word_maps.hpp

%.o: %.cpp $(HEADERFILES)
	@echo 'Compiling $<'
	@$(CXX) $(CXXFLAGS) -c $< -o $@

compressor: compressor.o sstring.o word_maps.o
	@echo 'Linking $<'
	@$(CXX) -o compressor $^ $(LDFLAGS)

all: main

.PHONY: clean
clean:
	@echo 'Removing object files and executable'
	@rm -rf *.o main
