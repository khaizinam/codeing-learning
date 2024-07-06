# example make file
all : build run
# all : build run clean

build :
	g++ main.cpp -o main

run :
	./main

clean :
	rm main