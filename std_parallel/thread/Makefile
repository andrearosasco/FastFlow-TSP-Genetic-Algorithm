CXX 		= g++ -std=c++17
INCLUDES 	= -iquote ../../common
LDFLAGS 	= -lpthread -pthread
OPTFLAGS	= -O3
DEBUG 		= -Wall -g3 -fsanitize=address

.SUFFIXES: .cpp 

%: %.cpp
	$(CXX) $(INCLUDES) $(LDDIRS) $(LDFLAGS) -o $@ $< $(OPTFLAGS)