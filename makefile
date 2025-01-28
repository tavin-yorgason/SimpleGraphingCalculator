# compile lol

cppFiles := $(wildcard *.cpp)
objs := $(cppFiles:.cpp=.o)

calculator.out: $(objs)
	g++ -Wall $(objs) -o calculator.out

%.o: %.cpp %.hpp
	g++ -Wall -c $<

clean:
	rm *.o

