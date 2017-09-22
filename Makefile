# makefile

all: echoserver echoclient

echoserver: echoserver.cpp 
	g++ -std=c++11 -g -o echoserver echoserver.cpp -lpthread

echoclient: echoclient.cpp
	g++ -std=c++11 -g -o echoclient echoclient.cpp -lpthread

clean:
	rm echoserver echoclient
