#ifndef _ACKLEY_H_
#define _ACKLEY_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "../problem.hpp"

using namespace std;

class Ackley: public Problem {
  public:
    Ackley(int dimension);
    Ackley();
    ~Ackley();

    double getUpperBound();
    double getLowerBound();
    double evaluateFitness(std::vector<double> solution);
    double getDimension();
    std::vector<double> validatePosition(std::vector<double> position);
    std::string getName();
    bool fitnesIsBetter(double newFit, double oldFit);
    bool isMinimization();

  private:
    double dimension;
}; 

#endif
