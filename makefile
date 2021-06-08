all: main.out client.out router.out
CXXFLAGS = -std=c++11

main.out: main.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) main.cpp functions.cpp -o main.out

client.out: client.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) client.cpp functions.cpp -o client.out

router.out: router.cpp functions.cpp functions.h
	g++ $(CXXFLAGS) router.cpp functions.cpp -o router.out

.PHONY: clean
clean:
	rm *.out
	rm -r pipes

