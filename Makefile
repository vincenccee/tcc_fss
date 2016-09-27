PARAMS=-g -c -Wall -std=c++11
# -fopenmp

all: app

app: fish.o population.o main.o fish_school_search.o problem.o scenario.o ackley.o griewank.o rastring.o rosembrock.o schwefel.o moving_peaks.o
	g++ fish.o population.o main.o fish_school_search.o problem.o scenario.o ackley.o griewank.o rastring.o rosembrock.o schwefel.o moving_peaks.o -o app

main.o: main.cpp
	g++ $(PARAMS) main.cpp

fish.o: fish.cpp
	g++ $(PARAMS) fish.cpp

fish_school_search.o: algorithms/fish_school_search.cpp
	g++ $(PARAMS) algorithms/fish_school_search.cpp

population.o: population.cpp
	g++ $(PARAMS) population.cpp

problem.o: benchmarcks/problem.cpp
	g++ $(PARAMS) benchmarcks/problem.cpp

ackley.o: benchmarcks/statics/ackley.cpp
	g++ $(PARAMS) benchmarcks/statics/ackley.cpp

griewank.o: benchmarcks/statics/griewank.cpp
	g++ $(PARAMS) benchmarcks/statics/griewank.cpp

rastring.o: benchmarcks/statics/rastring.cpp
	g++ $(PARAMS) benchmarcks/statics/rastring.cpp

rosembrock.o: benchmarcks/statics/rosembrock.cpp
	g++ $(PARAMS) benchmarcks/statics/rosembrock.cpp

schwefel.o: benchmarcks/statics/schwefel.cpp
	g++ $(PARAMS) benchmarcks/statics/schwefel.cpp

moving_peaks.o: benchmarcks/dynamics/moving_peaks.cpp
	g++ $(PARAMS) benchmarcks/dynamics/moving_peaks.cpp

scenario.o: benchmarcks/scenario.cpp
	g++ $(PARAMS) benchmarcks/scenario.cpp

clean:
	rm *~ *.o *.eps *.dataset *.output app; clear;

god:
	make clean; make; ./app

try:
	make clean; make

# plot:
# 	gnuplot -e "file='saida.csv'" plot.plg

# to compile
# g++ -o test main.cpp population.cpp fish.cpp fish_school_search.cpp