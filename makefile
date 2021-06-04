all: main_client.out client.out main_server.out server.out main_groupserver.out groupserver.out main_router.out router.out
CXXFLAGS = -std=c++11

main_client.out: main_client.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) main_client.cpp functions.cpp -o main_client.out

client.out: client.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) client.cpp functions.cpp -o client.out
	
main_server.out: main_server.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) main_server.cpp functions.cpp -o main_server.out

server.out: server.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) server.cpp functions.cpp -o server.out

main_groupserver.out: main_groupserver.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) main_groupserver.cpp functions.cpp -o main_groupserver.out

groupserver.out: groupserver.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) groupserver.cpp functions.cpp -o groupserver.out
	
main_router.out: main_router.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) main_router.cpp functions.cpp -o main_router.out

router.out: router.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) router.cpp functions.cpp -o router.out

.PHONY: clean
clean:
	rm *.out
	rm -r pipes

