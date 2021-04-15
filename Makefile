CXX=g++

CXXFLAGS=-std=c++11 -g

mizzo : io.o producer.o consumer.o main.o
	$(CXX) $(CXXFLAGS) io.o producer.o consumer.o main.o -o mizzo -lrt -lpthread

producer.o : producer.h producer.cpp
	$(CXX) $(CXXFLAGS) -c producer.cpp

consumer.o : consumer.h consumer.cpp
	$(CXX) $(CXXFLAGS) -c consumer.cpp

io.o : production.h io.h io.c
	$(CXX) $(CXXFLAGS) -c io.c

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean :
	rm *.o