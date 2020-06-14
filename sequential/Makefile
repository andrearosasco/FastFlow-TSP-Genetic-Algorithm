CXX 		= g++ -std=c++17
INCLUDES 	= -iquote ../common
OPTFLAGS	= -O3

.SUFFIXES: .cpp 

%: %.cpp
	$(CXX) $(INCLUDES) -o $@ $< $(OPTFLAGS)