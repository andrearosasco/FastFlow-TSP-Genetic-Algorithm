CXX 		= g++ -std=c++17
INCLUDES 	= -iquote ./common
LDFLAGS 	= -lpthread -pthread
OPTFLAGS	= -O3

.SUFFIXES: .cpp 

%: %.cpp
	$(CXX) $(INCLUDES) $(LDDIRS) $(LDFLAGS) -o $@ $< $(OPTFLAGS)