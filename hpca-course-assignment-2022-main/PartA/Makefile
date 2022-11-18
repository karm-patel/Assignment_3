all: data/input_16.in data/input_512.in data/input_1024.in data/input_2048.in rmm

rmm: main.cpp header/single_thread.h header/multi_thread.h
	g++ -g -ggdb main.cpp -o rmm -I ./header -lpthread -mavx -mavx2

data/generate: data/generate.cpp
	g++ ./data/generate.cpp -o ./data/generate

data/input_16.in: data/generate
	./data/generate 16

data/input_512.in: data/generate
	./data/generate 512

data/input_1024.in: data/generate
	./data/generate 1024

data/input_2048.in: data/generate
	./data/generate 2048 

run: data/input_16.in rmm
	./rmm data/input_16.in
	./rmm data/input_512.in
	./rmm data/input_1024.in
	./rmm data/input_2048.in

clean:
	rm rmm
