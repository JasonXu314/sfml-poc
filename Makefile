GXX=g++

default: run

.PHONY: run

run: bin/main
	./bin/main

bin/main: bin/.objects/main.o
	g++ $^ -o bin/main -lsfml-graphics -lsfml-window -lsfml-system

bin/.objects/main.o: main.cpp
	g++ -c $^ -o $@