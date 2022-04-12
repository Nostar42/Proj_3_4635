CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -lpthread

SOBJECTS = User.o
COBJECTS =
HEADERS := $(shell find . -path ./test -prune -o -name "*.hpp" -print)

.PHONY: main test clean git run

server:  server.o $(SOBJECTS)
	$(CXX) $(CXXFLAGS) -o server $^

client:  client.o $(COBJECTS)
	$(CXX) $(CXXFLAGS) -o client $^

$(OBJECTS): $(HEADERS)

clean:
	rm -rf *.dSYM
	$(RM) *.o *.gc* *.s *.exe myshell *.zip server client SloanServer SloanClient

#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
