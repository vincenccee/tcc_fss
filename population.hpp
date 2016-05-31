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
    double stepInd;
    double stepVol;
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

    double getStepInd();
    double getStepVol();
    double getMinWeight();
    double getMaxWeight();
    void setStepInd(double stepInd);
    void setStepVol(double stepVol);
    void setMinWeight(double minWeight);
    void setMaxWeight(double maxWeight);
};

#endif