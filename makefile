all: main.out client.out server.out groupserver.out router.out
CXXFLAGS = -std=c++11

main.out: main.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) main.cpp functions.cpp -o main.out

client.out: client.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) client.cpp functions.cpp -o client.out

server.out: server.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) server.cpp functions.cpp -o server.out

groupserver.out: groupserver.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) groupserver.cpp functions.cpp -o groupserver.out

router.out: router.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) router.cpp functions.cpp -o router.out

.PHONY: clean
clean:
	rm *.out
	rm -r pipes

