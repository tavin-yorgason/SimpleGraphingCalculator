# compile lol

cppFiles := $(wildcard *.cpp)
objs := $(cppFiles:.cpp=.o)

calculator.out: $(objs)
	g++ $(objs) -o calculator.out

%.o: %.cpp %.hpp
	g++ -c $<

clean:
	rm *.o

