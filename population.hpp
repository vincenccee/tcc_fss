#ifndef _POPULATION_H_
#define _POPULATION_H_
#include <iostream>
#include <vector>
#include <algorithm>

class Population {
  private:
  	std::vector<Fish> pop;
    double stepInd;
    double stepVol;
    double minWeight;
    double maxWeight;
    int generation;
    int tamPopulation

  public:
  	Population(int tamPopulation, int generetions);
  	Population();
    ~Population();

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