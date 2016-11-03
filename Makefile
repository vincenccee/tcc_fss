PARAMS=-g -c -Wall -std=c++11
# -fopenmp

all: app

app: main.o fish.o population.o general_tools.o fish_school_search.o fish_school_search_2.o problem.o scenario.o ackley.o griewank.o rastring.o rosembrock.o schwefel.o sphere.o moving_peaks.o
	g++ main.o fish.o population.o general_tools.o fish_school_search.o fish_school_search_2.o problem.o scenario.o ackley.o griewank.o rastring.o rosembrock.o schwefel.o sphere.o moving_peaks.o -o app

main.o: main.cpp
	g++ $(PARAMS) main.cpp

fish.o: fish.cpp
	g++ $(PARAMS) fish.cpp

population.o: population.cpp
	g++ $(PARAMS) population.cpp

general_tools.o: general_tools.cpp
	g++ $(PARAMS) general_tools.cpp

fish_school_search.o: algorithms/fish_school_search.cpp
	g++ $(PARAMS) algorithms/fish_school_search.cpp

fish_school_search_2.o: algorithms/fish_school_search_2.cpp
	g++ $(PARAMS) algorithms/fish_school_search_2.cpp

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

sphere.o: benchmarcks/statics/sphere.cpp
	g++ $(PARAMS) benchmarcks/statics/sphere.cpp

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