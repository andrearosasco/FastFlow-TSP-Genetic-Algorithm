FF_ROOT		= /usr/local/fastflow

CXX 		= g++ -std=c++17
INCLUDES 	= -I $(FF_ROOT) -iquote ../../common
OPTFLAGS	= -O3
LDFLAGS 	= -pthread
DEBUG 		= -Wall -g3 -fsanitize=address

.SUFFIXES: .cpp 

%: %.cpp
	$(CXX) $(INCLUDES) -o $@ $< $(OPTFLAGS) $(LDFLAGS)