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

    double getUpperBound(int pos);
    double getLowerBound(int pos);
    double evaluateFitness(std::vector<double> solution);
    std::string getName();
    bool fitnesIsBetter(double newFit, double oldFit);
    bool isMinimization();
}; 

#endif
