all: parser.cpp
	c++ --std=c++11 parser.cpp -o parser
clean:
	rm parser
