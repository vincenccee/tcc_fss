#ifndef _POPULATION_H_
#define _POPULATION_H_
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "fish.hpp"

class Population {
  private:
  	std::vector<Fish> population;
    double minWeight;
    double maxWeight;
    double upperBound;
    double lowerBound;
    int dimension;
    int tamPopulation;

  public:
  	Population(int tamPopulation, int dimension, double lowerBound, double upperBound);
  	Population();
    ~Population();

    void showPopulation();
    void initializePipulation();
    void evaluatePopulation();
    std::vector<double> randonPosition();
    std::vector<Fish> getPopulation();

    int getTamPopulation();
    double getMinWeight();
    double getMaxWeight();
    void setMinWeight(double minWeight);
    void setMaxWeight(double maxWeight);

    double fRand(double fMin, double fMax);
};

#endif